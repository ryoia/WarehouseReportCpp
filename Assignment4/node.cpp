//----------------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
// 
// node.cpp
// The node class definitions for the shelf
// which is a modified linked list
//----------------------------------------------

#include <iostream>
#include "node.h"

namespace reports
{

  // Constructor, creates a new node
  // Parameter: currentDate - int representing days since start date
  // Parameter: shelfLife - int representing amount of days to take the items in the node to expire
  node::node(int currentDate, int shelfLife)
  {
    // Quantity is instantiated to 0
    this->quantity = 0;

    // Expiration date is calculated by adding the current date to the shelf life
    this->expireDate = currentDate + shelfLife;

    // next node by default is null
    this->next = NULL;

    // modify constructor calls for auditing purposes
    node::constructor_calls++;
  }

  // Destructor, destroys a node and opens of memory
  node::~node()
  {
    // modify destructor calls for auditing purposes
    node::destructor_calls++;
  }

  // Methods and data for auditing purposes, copied from homework 3
  long long node::constructor_calls = 0;
  long long node::destructor_calls = 0;

  // Returns value of constructor_calls
  long long node::constructor_count()
  {
    return node::constructor_calls;
  }

  // Returns value of destructor_calls
  long long node::destructor_count()
  {
    return node::destructor_calls;
  }
}
