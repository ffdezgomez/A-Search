#pragma once

#include <vector>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Node.h"
#include "Grid.h"
#include "Cell.h"
#include "Metrics.h"

#define newline std::cout << std::endl;
#define separator "------------------------------------------------------------------------------------------------------------------\n";

/**
 * @brief Class implementing the A* search algorithm.
 * 
 */
class SearchAStar {
  public:
    /// Constructors
    SearchAStar() = default;
    SearchAStar(Grid& grid, std::string instance) : grid_(grid), instance_(instance) {}
    
    /// Public Methods
    void Run(const Cell& start, Metrics& metrics, std::ostream& os = std::cout);
    void PrintResultsTable(const Metrics& metrics, std::ostream& os = std::cout) const;
    std::ostream& PrintOpenList(std::ostream& os) const;
    std::ostream& PrintClosedList(std::ostream& os) const;
    void Reset();

    /// Getters
    std::vector<Node*> GetPath() const { return path_; }
    Grid GetGrid() const { return grid_; }
    
    /// Setters
    void SetGrid(const Grid& grid) { grid_ = grid; }
  private:
    /// Attributes
    std::unordered_set<Node*> open_list_;
    std::vector<Node*> open_list_copy_; 
    std::unordered_set<Cell> closed_list_;
    std::vector<Cell> closed_list_copy_;
    unsigned iteration_ = 0;
    Grid grid_;
    std::vector<Node*> path_;
    std::string instance_;

    /// Private Methods
    int ManhattanDistance(const Cell& node, const Cell& end, int weight = 3);
    Node* SearchNodeWithLowestF(const std::unordered_set<Node*>& node_set);
    Node* WhoIsBetterNode(Node* node1);
    bool IsInOpenList(const Cell& node, const std::unordered_set<Node*>& open_list);
    std::ostream& PrintPath(const std::vector<Cell>& path, std::ostream& os) const;
    std::vector<Node*> ReconstructPath(Node* current_node);
};