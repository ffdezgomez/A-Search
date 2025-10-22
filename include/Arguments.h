#pragma once
#include <string>

/**
 * @brief Struct to hold command line arguments.
 * 
 */
struct Arguments {
  std::string input_filename;
  bool change_start_goal = false;
  std::string output_filename = "";
};
