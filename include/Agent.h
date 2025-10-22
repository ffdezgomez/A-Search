#pragma once

#include "Cell.h"
#include "Grid.h"
#include "Node.h"

/**
 * @brief Class representing an agent that can navigate through a grid environment.
 * 
 */
class Agent {
  public:
    /// Constructors
    Agent() = default;
    Agent(const Grid& environment);

    /// Public Methods
    void Step(const std::vector<Node*>& path);

    /// Getters and Setters
    Cell GetPosition() const { return position_; }
    Grid& GetEnvironment() { return environment_; }
    const Grid& GetEnvironment() const { return environment_; }
    void SetEnvironment(const Grid& environment) { environment_ = environment; }
    
  private:
    Cell position_; /// Current position of the agent in the grid
    Grid environment_; /// The grid environment the agent navigates
    std::vector<Cell> path_; /// The path taken by the agent
};