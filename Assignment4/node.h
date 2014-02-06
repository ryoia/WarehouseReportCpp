//------------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
//
// node.h
// The node class header and function declarations
// The node acts as a data holder for the shelf
// which is a modified linked list
//
// Each node contains an effective expiration date and quantity of items that node holds
// each stored as ints
// Essentially a node represents the quantity of items received on a certain date
// Nodes received earlier have an earlier expiration date and as such need to be
// used up sooner
//------------------------------------------

#ifndef NODE_H
#define NODE_H

namespace reports
{
  class node
  {

    // Declaring shelf as a friend class so the shelf has access to the node's private data
    friend class shelf;

  private:
    // Constructor, creates a new node
    // Each node is instantiated with a quantity of 0
    // Parameters: currentDate - int representing days since start of data read
    // Parameters: shelfLife - int representing amount of days to take that item to expire
    node(int currentDate, int shelfLife);

    // Deconstructor, destroys a node and opens up memory
    ~node();

    // Quantity: amount of food stock in the node
    int quantity;

    // expireDate: int representing days from start date in which the node will expire
    int expireDate;

    // next: pointer to a next node. A next node represents items received on a later date
    node* next;


    // Auditing data to check for memory leaks and testing, copied from homework 3
  public:
    static long long constructor_count ();
    static long long destructor_count ();

  private:
    static long long constructor_calls;
    static long long destructor_calls;
  };
}

#endif
