#include "../include/Grid.h"

/**
 * @brief Construct a new Grid:: Grid object
 * 
 * @param input_file 
 */
Grid::Grid(std::ifstream& input_file) {
  input_file >> rows_ >> columns_;
  grid_.resize(rows_, std::vector<CellType>(columns_, EMPTY));
  
  char cell;
  for (int i = 0; i < grid_.size(); i++) {
    for (int j = 0; j < grid_[i].size(); j++) {
      input_file >> cell;
      cell_count_++;
      switch (cell) {
        case '0':
          grid_[i][j] = EMPTY;
          break;
        case '1':
          grid_[i][j] = WALL;
          wall_count_++;
          break;
        case '3':
          grid_[i][j] = START;
          start_ = {i, j, START};
          break;
        case '4':
          grid_[i][j] = GOAL;
          goal_ = {i, j, GOAL};
          break;
        default:
          throw std::runtime_error("Invalid cell type in input file");
          break;
      }
    }
  }
}

/**
 * @brief Get the Cell Type at a given position
 * 
 * @param x 
 * @param y 
 * @return CellType 
 */
CellType Grid::GetCellType(int x, int y) const {
  if (x < 0 || y < 0 || y >= grid_.size() || x >= grid_[0].size()) {
    return WALL; // Out of bounds treated as WALL
  }
  return grid_[x][y];
}

/**
 * @brief Get the neighbours of a given cell and the action to reach them
 * 
 * @param cell
 * @return std::vector<Action> 
 */
std::vector<std::pair<Cell, Action>> Grid::GetNeighbours(Cell cell) const {
  std::vector<std::pair<Cell, Action>> neighbours;
  std::pair<Cell, Action> neighbour_action;
  if (cell.x < rows_ && cell.y - 1 < columns_ && cell.x >= 0 && cell.y - 1 >= 0) {
    if (GetCellType(cell.x, cell.y - 1) != WALL) {
      neighbour_action = {Cell{cell.x, cell.y - 1, GetCellType(cell.x, cell.y - 1)}, NORTH};
      neighbours.push_back(neighbour_action);
    }
  } 
  if (cell.x < rows_ && cell.y + 1 < columns_ && cell.x >= 0 && cell.y + 1 >= 0) {
    if (GetCellType(cell.x, cell.y + 1) != WALL) {
      neighbour_action = {Cell{cell.x, cell.y + 1, GetCellType(cell.x, cell.y + 1)}, SOUTH};
      neighbours.push_back(neighbour_action);
    }
  } 
  if (cell.x - 1 < rows_ && cell.y < columns_ && cell.x - 1 >= 0 && cell.y >= 0) {
    if (GetCellType(cell.x - 1, cell.y ) != WALL) {
      neighbour_action = {Cell{cell.x - 1, cell.y, GetCellType(cell.x - 1, cell.y)}, WEST};
      neighbours.push_back(neighbour_action);
    }
  } 
  if (cell.x + 1 < rows_ && cell.y < columns_ && cell.x + 1 >= 0 && cell.y >= 0) {
    if (GetCellType(cell.x + 1, cell.y) != WALL) {
      neighbour_action = {Cell{cell.x + 1, cell.y, GetCellType(cell.x + 1, cell.y)}, EAST};
      neighbours.push_back(neighbour_action);
    }
  }
  if (cell.x - 1 < rows_  && cell.y - 1 < columns_ && cell.x - 1 >= 0 && cell.y - 1 >= 0) {
    if (GetCellType(cell.x - 1, cell.y - 1) != WALL) {
      neighbour_action = {Cell{cell.x - 1, cell.y - 1, GetCellType(cell.x - 1, cell.y - 1)}, NORTH_WEST};
      neighbours.push_back(neighbour_action);
    }
  }
  if (cell.x + 1 < rows_ && cell.y - 1 < columns_ && cell.x + 1 >= 0 && cell.y - 1 >= 0) {
    if (GetCellType(cell.x + 1, cell.y - 1) != WALL) {
      neighbour_action = {Cell{cell.x + 1, cell.y - 1, GetCellType(cell.x + 1, cell.y - 1)}, NORTH_EAST};
      neighbours.push_back(neighbour_action);
    }
  }
  if (cell.x - 1 < rows_ && cell.y + 1 < columns_ && cell.x - 1 >= 0 && cell.y + 1 >= 0) {
    if (GetCellType(cell.x - 1, cell.y + 1) != WALL) {
      neighbour_action = {Cell{cell.x - 1, cell.y + 1, GetCellType(cell.x - 1, cell.y + 1)}, SOUTH_WEST};
      neighbours.push_back(neighbour_action);
    }
  }
  if (cell.x + 1 < rows_ && cell.y + 1 < columns_ && cell.x + 1 >= 0 && cell.y + 1 >= 0) {
    if (GetCellType(cell.x + 1, cell.y + 1) != WALL) {
      neighbour_action = {Cell{cell.x + 1, cell.y + 1, GetCellType(cell.x + 1, cell.y + 1)}, SOUTH_EAST};
      neighbours.push_back(neighbour_action);
    }
  }

  return neighbours;
} 

/**
 * @brief Update the grid based on probabilities
 * 
 * @param p_in 
 * @param p_out 
 */
void Grid::UpdateGrid(float p_in, float p_out) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  
  for (int i = 0; i < grid_.size(); i++) {
    for (int j = 0; j < grid_[i].size(); j++) {
      if (grid_[i][j] == EMPTY) {
        if (dis(gen) >= (1 - p_in)) {
          grid_[i][j] = WALL;
          wall_count_++;
        }
      } else if (grid_[i][j] == WALL) {
        if (dis(gen) >= (1 - p_out)) {
          grid_[i][j] = EMPTY;
          wall_count_--;
        }
      }
    }
  }
}

/**
 * @brief Calculate the percentage of walls in the grid
 * 
 * @return float 
 */
float Grid::CalculateWallsPercentage() const {
  if (cell_count_ == 0) return 0.0f;
  return (static_cast<float>(wall_count_) / static_cast<float>(cell_count_));
}

/**
 * @brief Set new start and goal positions in the grid
 * 
 * @param new_start 
 * @param new_goal 
 */
void Grid::NewStartAndGoal(const Cell& new_start, const Cell& new_goal) {
  grid_[start_.x][start_.y] = EMPTY;
  grid_[goal_.x][goal_.y] = EMPTY;
  grid_[new_start.x][new_start.y] = START;
  grid_[new_goal.x][new_goal.y] = GOAL;
  start_ = new_start;
  goal_ = new_goal;
}