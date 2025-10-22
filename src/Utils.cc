#include "../include/Utils.h"

/**
 * @brief Prints the iteration metrics.
 * 
 * @param metrics 
 * @param search 
 * @param os 
 */
void Utils::PrintIteration(Metrics& metrics, const SearchAStar& search, std::ostream& os) {
  os << "Generated Nodes: ";
  search.PrintOpenList(os) << std::endl;
  os << "Number of Generated Nodes: " << metrics.generated_nodes << std::endl;
  os << "Inspected Nodes: ";
  search.PrintClosedList(os) << std::endl;
  os << "Number of Inspected Nodes: " << metrics.inspected_nodes << std::endl;
  os << "Steps Taken: " << metrics.steps << std::endl;
  os << "Retries Count: " << metrics.retrys_count << std::endl;
  os << "Walls Percentage: " << std::fixed << std::setprecision(2) << metrics.walls_percentage * 100 << "%" << std::endl;
  os << "Cost of Path: " << metrics.cost_of_path << std::endl;
  os << "Cost of Path Walked: " << metrics.cost_of_path_walked << std::endl;
}

/**
 * @brief Prints the grid with the path marked.
 * 
 * @param path 
 * @param grid 
 * @param os 
 */
void Utils::PrintGridWithPath(const std::vector<Node*>& path, Grid grid, std::ostream& os) {
  if(!path.empty()) {
    for (int i = 1; i < path.size() - 1; i++) { 
      if (grid.GetCellType(path[i]->GetCell().x, path[i]->GetCell().y) != AGENT)
      grid.SetCellType(path[i]->GetCell().x, path[i]->GetCell().y, PATH);
    }
  }

  for (int i = 0; i < grid.GetGrid().size(); i++) {
    for (int j = 0; j < grid.GetGrid()[0].size(); j++) {
      switch (grid.GetCellType(i, j)) {
        case EMPTY:
          os << "0 ";
          break;
        case WALL:
          os << "1 ";
          break;
        case START:
          os << "S ";
          break;
        case GOAL:
          os << "E ";
          break;
        case PATH:
          os << "* ";
          break;
        case WALKED:
          os << "+ ";
          break;
        case AGENT:
          os << "A ";
          break;
        default:
          os << "? "; // Unknown cell type
          break;
      }
    }
    os << std::endl;
  }
}

/**
 * @brief Parses command-line arguments.
 * 
 * @param argc 
 * @param argv 
 * @return Arguments 
 */
Arguments Utils::ParseArgs(int argc, char* argv[]) {
  Arguments args;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--input" && i + 1 < argc) {
      args.input_filename = argv[++i];
    } else if (arg == "--change") {
      args.change_start_goal = true;
    } else if (arg == "--output" && i + 1 < argc) {
      args.output_filename = argv[++i];
    } else {
      std::cerr << "Unknown argument: " << arg << std::endl;
      PrintHelp();
      exit(1);
    }
  }
  if (args.input_filename.empty()) {
    std::cerr << "Input filename is required." << std::endl;
    PrintHelp();
    exit(1);
  }
  return args;
}

/**
 * @brief Prints help message.
 * 
 */
void Utils::PrintHelp() {
  std::cout << "Usage: ./bin/search --input <input_filename> [--change] [--output <output_filename>]" << std::endl;
  std::cout << "--input <input_filename>   Specify the input file containing the grid." << std::endl;
  std::cout << "--change                   Change the start and goal positions." << std::endl;
  std::cout << "--output <output_filename> Specify the output file to save results." << std::endl;
}