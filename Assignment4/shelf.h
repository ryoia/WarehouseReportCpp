//--------------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
//
// shelf.h
//
// header containing class function declarations for shelf class
// a shelf is a modified linked list with added functionality
// a shelf also contains references to the head and tail node of the linked list
// for optimization
// a shelf has functions for receiving items and sending out items and removing
// expired items
//
// a shelf is a linked list of different shipments of the same product to a warehouse
// each node in the shelf represents the same products, but the quantities received
// on different days
// for example, the head node in the list might represent the 8 units of peaches received
// on january 15, and the tail node might represent the 5 units of peaches received on 
// january 22
// the shelf is set up this way so older products are sent out first and products can be
// removed easily if they have expired
//--------------------------------------------

#ifndef SHELF_H
#define SHELF_H

#include "node.h"

namespace reports
{
  // forward declaration for node class
  class node;

  class shelf
  {
    // declaring warehouse as a friend class so a warehouse has access to the private members
    // of a shelf
    friend class warehouse;

  private:
    // constructor - builds a new shelf
    // parameter - life - the shelf life of all products on the shelf
    shelf(int life);

    // deconstructor - destroys a shelf and frees memory
    ~shelf();

    // receive - handles incoming shipment of the item this shelf contains
    // this method will add the quantity to the tail node of the shelf, since the tail
    // node represents the shipment of goods on the given date
    // parameter - qty - quantity of items received
    // parameter - currentDate - int representing days since start date, used to calculate
    // the expiration date of a given product received on the current date
    void receive(int qty, int currentDate);

    // request - handles requests to send out a shipment of the item this shelf contains
    // this method will step through as many nodes as it can from head to tail until
    // either the order is satisfied or the shelf has been emptied
    // parameter - qty - quantity of items to send out
    void request(int qty);

    // advanceDay - checks to see if the goods in the head node have expired. If so, 
    // moves the head node pointer to the head node's next and deletes the old
    // head node
    // parameter - currentDate - int representing days since start date which will be
    // check against the node's expiration date
    void removeExpired(int currentDate);

    // clean - helper method for deconstructor
    void clean();

    // DIAGNOSTICS - tester
    void DIAGNOSTICS();

    // pointers to head and tail node
    node* head;
    node* tail;
    
    // int representing the shelfLife of the product this shelf contains
    int shelfLife;

  public:
    static long long constructor_count ();
    static long long destructor_count ();

  private:
    static long long constructor_calls;
    static long long destructor_calls;
  };
}

#endif
