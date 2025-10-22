#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

#include "../include/Grid.h"
#include "../include/Search.h"
#include "../include/Utils.h"
#include "../include/Agent.h"

int main (int argc, char* argv[]) {
  Arguments arguments = Utils::ParseArgs(argc, argv);

  /// Opening input file
  std::ifstream input_file(arguments.input_filename);
  if (!input_file) {
    std::cerr << "Error opening input file." << std::endl;
    return 1;
  }
  std::string instance = std::filesystem::path(arguments.input_filename).filename().string();

  /// Change start and goal if required
  Grid grid(input_file);
  if (arguments.change_start_goal) {
    Cell new_start, new_goal;
    std::cout << "Enter new start coordinates (x y): ";
    std::cin >> new_start.x >> new_start.y;
    new_start.type = START;
    std::cout << "Enter new goal coordinates (x y): ";
    std::cin >> new_goal.x >> new_goal.y;
    new_goal.type = GOAL;
    grid.NewStartAndGoal(new_start, new_goal);
  }
  
  /// Initializations
  unsigned iteration = 0;
  Cell start = grid.GetStart();
  Cell goal = grid.GetGoal();
  Agent agent(grid);
  SearchAStar search(grid, arguments.input_filename);
  std::vector<Node*> path {};
  Metrics metrics;
  metrics.walls_percentage = grid.CalculateWallsPercentage();
  metrics.path.push_back(start);
  
  /// Prepare output file
  std::filesystem::path dir = "output";
  if (arguments.output_filename.empty()) {
    std::filesystem::create_directories(dir);
    arguments.output_filename = std::to_string(start.x) + "-" + std::to_string(start.y) 
                                + "_to_" + std::to_string(goal.x) + "-" + std::to_string(goal.y)
                                + "_" + instance;
  }
  std::ofstream output_file(dir / arguments.output_filename);
  if (!output_file) {
    std::cerr << "Error opening output file." << std::endl;
    return 1;
  }

  std::stringstream iteration_ss;
  while (agent.GetPosition() != goal) {
    iteration_ss << "----- Iteration " << ++iteration << " -----" << std::endl;
    iteration_ss << separator;
    /// Run A* Search
    search.Run(start, metrics, iteration_ss);
    path = search.GetPath();
    /// Try 5 times to find a path if none is found
    if (path.empty() && metrics.retrys_count <= 5) {
      metrics.retrys_count++;
      agent.GetEnvironment().UpdateGrid();
      search.Reset();
      search.SetGrid(agent.GetEnvironment());
      metrics.walls_percentage = agent.GetEnvironment().CalculateWallsPercentage();
      metrics.Reset();
      continue;
    } else if (metrics.retrys_count > 5) {
      std::cerr << "No path found after 5 retries. Terminating." << std::endl;
      return 0;
    } else {
      /// Move the agent one step 
      agent.Step(path);
      metrics.steps++;
      metrics.cost_of_path = path.back()->GetG();
      metrics.cost_of_path_walked += path[1]->GetG();
      metrics.path.push_back(path[1]->GetCell());
      /// Print the current state
      Utils::PrintGridWithPath(path, agent.GetEnvironment(), iteration_ss);
      iteration_ss << separator;
      /// Print iteration metrics
      Utils::PrintIteration(metrics, search, iteration_ss);
      iteration_ss << separator;
      /// Update grid for next iteration
      start = agent.GetPosition();
      agent.GetEnvironment().UpdateGrid();
      metrics.walls_percentage = agent.GetEnvironment().CalculateWallsPercentage();
      metrics.Reset();
      metrics.retrys_count = 0;
      search.Reset();
      search.SetGrid(agent.GetEnvironment());
    }
  }
  output_file << "----- Final Metrics Table -----" << std::endl;
  search.PrintResultsTable(metrics, output_file);
  output_file << std::endl;
  output_file << "----- Iteration Details -----" << std::endl;
  output_file << iteration_ss.str();
  return 0;
}