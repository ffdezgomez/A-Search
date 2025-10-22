#pragma once

#include <vector>
#include "Cell.h"

/**
 * @brief Struct to hold various metrics related to the agent's performance.
 * 
 */
struct Metrics {
  int generated_nodes = 0;
  int inspected_nodes = 0;
  int steps = 0;
  int retrys_count = 0;
  float walls_percentage = 0;
  float cost_of_path = 0;
  float cost_of_path_walked = 0;
  std::vector<Cell> path = {};

  void Reset() {
    generated_nodes = 0;
    inspected_nodes = 0;
    retrys_count = 0; 
    cost_of_path = 0;
  }
};