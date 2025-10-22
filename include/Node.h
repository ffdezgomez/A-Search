#ifndef NODE_H
#define NODE_H

#include "Cell.h"

/**
 * @brief Class Node that represents a node in the search tree.
 * 
 */
class Node {
  public:
    /// Constructors
    Node(Cell cell, Node* father = nullptr, float cost_g = 0, float cost_h = 0) : 
        cell_(cell), fatherptr_(father), cost_g_(cost_g), cost_h_(cost_h), cost_f_(cost_g_ + cost_h_) {}
  
    /// Getters
    Node* GetFatherptr() const { return fatherptr_; }
    float GetG() const { return cost_g_; }
    float GetH() const { return cost_h_; }
    float GetF() const { return cost_f_; }
    Cell GetCell() const { return cell_; }

  /// Setters
    void SetFatherptr(Node* father) { fatherptr_ = father; }
    void SetG(float cost_g) { cost_g_ = cost_g; cost_f_ = cost_g_ + cost_h_; }
    void SetH(float cost_h) { cost_h_ = cost_h; cost_f_ = cost_g_ + cost_h_; }
    void SetF(float cost_f) { cost_f_ = cost_f; }

  private:
    Node* fatherptr_;   /// Pointer to the father node
    float cost_g_;      /// Cost to reach this node from the start node
    float cost_h_;      /// Heuristic cost to reach the goal from this node
    float cost_f_;      /// Total cost (g + h)
    Cell cell_;         /// State represented by this node
};


#endif