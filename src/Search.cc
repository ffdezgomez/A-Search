#include "../include/Search.h"

/**
 * @brief Get the neighbours of a given cell and the action to reach them
 * 
 * @param start 
 * @param metrics 
 * @param os 
 */
void SearchAStar::Run(const Cell& start, Metrics& metrics, std::ostream& os) {
  /// 1. Calcular f(n), g(n) y h(n) para el punto de entrada al laberinto, S, que se inserta
  ///   en la lista de nodos abiertos A.
  Cell end_cell = grid_.GetGoal();
  Node* start_node = new Node(start, nullptr, 0, ManhattanDistance(start, end_cell));
  open_list_.insert(start_node);
  open_list_copy_.push_back(start_node);
  metrics.generated_nodes++;
  /// 2. Repetir mientras A no esté vacía.
  while (!open_list_.empty()) {
    /// (a) Seleccionar el nodo de menor coste f(n), e insertarlo en la lista de nodos
    ///   cerrados C.
    Node* current_node = SearchNodeWithLowestF(open_list_);
    open_list_.erase(current_node);
    // open_list_copy_.erase(std::remove(open_list_copy_.begin(), open_list_copy_.end(), current_node), open_list_copy_.end());
    closed_list_.insert(current_node->GetCell());
    closed_list_copy_.push_back(current_node->GetCell());
    metrics.inspected_nodes++;
    if (current_node->GetCell() == end_cell) {
      path_ = ReconstructPath(current_node);
      return; 
    }
    
    /// (b) Para cada nodo vecino, realizar las siguientes acciones:
    std::vector<std::pair<Cell, Action>> neighbours = grid_.GetNeighbours(current_node->GetCell());
    for (int i = 0; i < neighbours.size(); i++) {
      ///   i. Si el nodo no está ni en A, ni en C, su nodo padre será el nodo analizado
      ///     y será insertado en A. Realizar las acciones que corresponda.
      if (closed_list_.find(neighbours[i].first) != closed_list_.end()) {
        continue;
      }
      
      float new_g = current_node->GetG() + ((neighbours[i].second == NORTH || neighbours[i].second == SOUTH || 
      neighbours[i].second == EAST || neighbours[i].second == WEST) ? 5 : 7);
      float new_h = ManhattanDistance(neighbours[i].first, end_cell);
      Node* neighbour_node = new Node(neighbours[i].first, current_node, new_g, new_h);
        
      ///   ii. Si el nodo está en A, tener en cuenta que quizás sea necesario actualizar
      ///     su coste g(n) y, por lo tanto, su padre en el camino. Nótese que se deben
      ///     recalcular los costes necesarios.
      if (IsInOpenList(neighbours[i].first, open_list_)) {
        Node* better_node = WhoIsBetterNode(neighbour_node);
      } else {
        open_list_.insert(neighbour_node);
        open_list_copy_.push_back(neighbour_node);
        metrics.generated_nodes++;
      }
    }
  }
  /// 3. Si A = ∅ y no se ha llegado a la salida del laberinto, E, no existe camino y se
  ///   deberá mostrar en pantalla un mensaje que así lo indique.
  return; // No path found
}

/**
 * @brief Calculates the Manhattan distance between two cells.
 * 
 * @param node 
 * @param end 
 * @param weight 
 * @return int 
 */
int SearchAStar::ManhattanDistance(const Cell& node, const Cell& end, int weight) {
  int distance = (std::abs(end.x - node.x) + std::abs(end.y - node.y));
  return weight * distance;
}

/**
 * @brief Searches for the node with the lowest f(n) in a set of nodes.
 * 
 * @param node_set 
 * @return Node* 
 */
Node* SearchAStar::SearchNodeWithLowestF(const std::unordered_set<Node*>& node_set) {
  if (node_set.empty()) return nullptr;
  Node* min_node = nullptr;
  for (const auto& node : node_set) {
    if (min_node == nullptr || node->GetF() < min_node->GetF()) {
      min_node = node;
    }
  }
  return min_node;
}

/**
 * @brief Checks if a cell is in the open list.
 * 
 * @param node 
 * @param open_list 
 * @return true 
 * @return false 
 */
bool SearchAStar::IsInOpenList(const Cell& node, const std::unordered_set<Node*>& open_list) {
  for (const auto& open_node : open_list) {
    if (node == open_node->GetCell()) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Compares two nodes and returns the better one based on f(n) cost. 
 *        Also updates the open list node if necessary.
 * 
 * @param node1 
 * @return Node* 
 */
Node* SearchAStar::WhoIsBetterNode(Node* node1) {
  for (const auto& open_node : open_list_) {
    if (node1->GetCell() == open_node->GetCell()) {
      if (node1->GetF() < open_node->GetF()) {
        open_node->SetG(node1->GetG());
        open_node->SetH(node1->GetH());
        open_node->SetFatherptr(node1->GetFatherptr());
        open_node->SetF(node1->GetF());
        return node1;
      } else {
        return open_node;
      }
    }
  }
  return nullptr;
}

/**
 * @brief Prints the open list of nodes.
 * 
 * @param list 
 */
std::ostream& SearchAStar::PrintOpenList(std::ostream& os) const {
  for (auto node : open_list_copy_) {
    os << "(" << node->GetCell().x << ", " << node->GetCell().y << ")" << " ";
  }
  return os;
}

/**
 * @brief Prints the closed list of states.
 * 
 * @param list 
 */
std::ostream& SearchAStar::PrintClosedList(std::ostream& os) const {
  for (auto cell : closed_list_copy_) {
    os << "(" << cell.x << ", " << cell.y << ")" << " ";
  }
  return os;
}

/**
 * @brief Prints the results table. 
 * 
 * @param metrics 
 * @param os 
 */
void SearchAStar::PrintResultsTable(const Metrics& metrics, std::ostream& os) const {
  os << std::left << std::setw(15) << "Instancia" 
     << std::left << std::setw(10) << "n" 
     << std::left << std::setw(10) << "m" 
     << std::left << std::setw(10) << " S" 
     << std::left << std::setw(10) << " E" 
     << std::left << std::setw(10) << "Coste" 
     << std::endl;
  os << separator;
  os << std::left << std::setw(15) << instance_
     << std::left << std::setw(10) << grid_.GetRows() 
     << std::left << std::setw(10) << grid_.GetColumns() 
     << std::left << std::setw(10) << "(" + std::to_string(grid_.GetStart().x) + "," + std::to_string(grid_.GetStart().y) + ")" 
     << std::left << std::setw(10) << "(" + std::to_string(grid_.GetGoal().x) + "," + std::to_string(grid_.GetGoal().y) + ")" 
     << std::left << std::setw(10) << metrics.cost_of_path_walked 
     << std::endl;
  os << separator;
  os << "Camino: ";
  PrintPath(metrics.path, os);
  os << std::endl;
  os << separator;
}

/**
 * @brief Prints the path taken.
 * 
 * @param path 
 * @param os 
 * @return std::ostream& 
 */
std::ostream& SearchAStar::PrintPath(const std::vector<Cell>& path, std::ostream& os) const {
  for (int i = 0; i < path.size() - 1; i++) {
    os << "(" << path[i].x << ", " << path[i].y << ")" << " -> ";
  }
  os << "(" << path.back().x << ", " << path.back().y << ")"; 
  return os;
}

/**
 * @brief Resets the search state.
 * 
 */
void SearchAStar::Reset() {
  open_list_.clear();
  closed_list_.clear();
  iteration_ = 0;
  open_list_copy_.clear();
  closed_list_copy_.clear();
  path_.clear();
}

/**
 * @brief Reconstructs the path from the current node to the start node.
 * 
 * @param current_node 
 * @return std::vector<Node*> 
 */
std::vector<Node*> SearchAStar::ReconstructPath(Node* current_node) {
  std::vector<Node*> path;
  while (current_node != nullptr) {
    path.push_back(current_node);
    current_node = current_node->GetFatherptr();
  }
  std::reverse(path.begin(), path.end());
  return path;
}