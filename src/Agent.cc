#include "../include/Agent.h"

/**
 * @brief Construct a new Agent:: Agent object
 * 
 * @param environment 
 */
Agent::Agent(const Grid& environment) 
  : environment_(environment), position_(environment.GetStart()), path_(std::vector<Cell> {position_}) {}

/**
 * @brief Perform a step for the agent along the given path.
 * 
 * @param path 
 */
void Agent::Step(const std::vector<Node*>& path) {
  if (path.empty()) {
    return; // No path to follow
  }
  if (position_.type != START)
    environment_.SetCellType(position_.x, position_.y, WALKED);
  position_ = path[1]->GetCell();
  environment_.SetCellType(position_.x, position_.y, AGENT);
  path_.push_back(position_);
} 