#pragma once

/**
 * @brief Enumeration of different types a cell can have in the grid.
 * 
 */
enum CellType {
  EMPTY,
  WALL,
  START,
  GOAL,
  AGENT,
  PATH,
  WALKED
};