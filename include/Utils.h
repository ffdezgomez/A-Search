#pragma once

#include <algorithm>
#include <iostream>
#include <filesystem>

#include "Grid.h"
#include "Node.h"
#include "Metrics.h"
#include "Search.h"
#include "Arguments.h"

/**
 * @brief Namespace Utils containing utility functions.
 * 
 */
namespace Utils {
  Arguments ParseArgs(int argc, char* argv[]);
  void PrintGridWithPath(const std::vector<Node*>& path, Grid grid, std::ostream& os = std::cout);
  void PrintIteration(Metrics& metrics, const SearchAStar& search, std::ostream& os = std::cout);
  void PrintHelp();
}

