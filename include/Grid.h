#pragma once

#include <vector>
#include <fstream>
#include <random>

#include "Cell.h"
#include "CellType.h"
#include "Actions.h"

#define P_IN 0.0 
#define P_OUT 0.5

/**
 * @brief Class Grid that represents the environment grid.
 * 
 */
class Grid {
  public:
    /// Constructors
    Grid() = default;
    Grid(std::ifstream& input_file);

    /// Public Methods
    CellType GetCellType(int x, int y) const;
    std::vector<std::pair<Cell, Action>> GetNeighbours(Cell cell) const;
    void UpdateGrid(float p_in = P_IN, float p_out = P_OUT);
    float CalculateWallsPercentage() const;
    void NewStartAndGoal(const Cell& new_start, const Cell& new_goal);

    /// Getters
    Cell GetStart() const { return start_; }
    Cell GetGoal() const { return goal_; }   
    std::vector<std::vector<CellType>> GetGrid() const { return grid_; }
    int GetRows() const { return rows_; }
    int GetColumns() const { return columns_; }

    /// Setters
    void SetCellType(int x, int y, CellType type) { grid_[x][y] = type; }
    void SetStart(Cell start) { start_ = start; }
    void SetGoal(Cell goal) { goal_ = goal; }
  private:
    std::vector<std::vector<CellType>> grid_; /// 2D vector representing the grid
    int rows_;                                /// Number of rows in the grid
    int columns_;                             /// Number of columns in the grid
    Cell start_;                              /// Start cell
    Cell goal_;                               /// Goal cell
    int cell_count_ = 0;                      /// Total number of cells
    int wall_count_ = 0;                      /// Total number of wall cells
};