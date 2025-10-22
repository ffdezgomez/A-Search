#pragma once
#include "CellType.h"
#include <functional>

/**
 * @brief Represents a cell in a grid with coordinates and type.
 * 
 */
struct Cell {
  int x;
  int y;
  CellType type;

  bool operator==(const Cell& other) const {
    return x == other.x && y == other.y && type == other.type;
  }

  bool operator!=(const Cell& other) const {
    return !(*this == other);
  }
};

/**
 * @brief Hash function specialization for Cell to be used in unordered containers.
 * 
 */
namespace std {
  template <>
  struct hash<Cell> {
    std::size_t operator()(const Cell& c) const {
      return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1) ^ (std::hash<int>()(c.type) << 2);
    }
  };
};