//----------------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
//
// shelf.cpp
//
// class function definitions for shelf class
// a more detailed description of the shelf class can be found in shelf.h
// the shelf is a modified linked list representing the shipments of a certain
// products received on certain days
// the shelf contains references to head and tail node as well as functions
// for modifying the quantities of products on the shelf
//----------------------------------------------

#include "shelf.h"
#include "node.h"
#include <iostream>

namespace reports
{
  // constructor - builds a new shelf
  // parameter - life - the shelf life of the product on the shelf
  shelf::shelf(int life)
  {
    // set private shelfLife value to the parameter
    this->shelfLife = life;

    // head and tail nodes initially set to null
    this->head = NULL;
    this->tail = NULL;

    shelf::constructor_calls++;
  }

  // deconstructor - destroys a shelf and frees memory
  shelf::~shelf()
  {
    // call clean helper
    this->clean();
    shelf::destructor_calls++;
  }

  // receive - handles incoming shipment of the item this shelf contains
  // adds the quantity of the shipment to the tail node if it exists
  // the tail node is added to because the tail node represents the current day's
  // shipment
  // if the tail node does not exist, that means nothing has been put on the shelf yet
  // if the tail node does not exist, builds a new node which both head and tail point to
  void shelf::receive(int qty, int currentDate)
  {
    // if the tail exists and the current tail node does not represent the current date
    // make a new node as tail's next node, and point tail to it
    if (tail != NULL && currentDate != tail->expireDate - this->shelfLife)
      {
	tail->next = new node(currentDate, this->shelfLife);
	tail = tail->next;
      }
    // otherwise, if the tail does not exist, build a new node and point the head and tail
    // to it. No tail means there are no nodes in the linked list yet
    else if (tail == NULL)
      {
	head = new node(currentDate, this->shelfLife);
	tail = head;
      }

    // add the qty to tail's quantity
    tail->quantity += qty;
  }

  // request - handles requests to send out a shipment of the item this shelf contains
  // steps through the linked list head to tail subtracting from the node's quantities
  // until the requested quantity has been satisfied or the entire shelf has been emptied
  // out
  // parameter - qty - quantity of items to sent out
  void shelf::request(int qty)
  {
    // remain_qty represents how much left of the order needs to be filled
    int remain_qty = qty;

    // while there are still nodes to step through, step through them
    while(head != NULL)
      {
	// amountSubt represents the quantity to subtract from remain_qty and the current
	// node's quantity. amountSubt is the smaller of the two values
	int amountSubt = (remain_qty > head->quantity) ? head->quantity : remain_qty;

	// subtract amountSub from the remain_qty and current node's quantity
	remain_qty -= amountSubt;
	head->quantity -= amountSubt;

	// if the current node's quantity was exhausted, check the next node and delete
	// the current node as it is no longer needed
	if (head->quantity == 0)
	  {

	    // if the tail and the head are equal, the shelf needs to be effectively reset
	    // this implies the last node was consumed
	    // when this happens, return as there is nothing left to do
	    if (tail == head)
	      {
		delete head;

		head = tail;
		tail = NULL;
		head = NULL;

		return;
	      }
	    // point head to head's next node and delete the old head
	    else
	      {
		node* temp = head;
		head = head->next;
		delete temp;
	      }
	  }

        // otherwise, a non-emptied shelf implies the order was fulfilled and there's nothing
        // else to do
	else
          {
	    return;
	  }
      }
  }

  // removeExpired - checks to see if the goods in the current head node have expired, if so,
  // moves the head node pointer to the head node's next and deletes the old
  // head node
  // parameter - currentDate - int representing days since start date
  void shelf::removeExpired(int currentDate)
  {
    // Assuming the head exists and it's expiration date is equal to the current date...
    if (head != NULL && head->expireDate == currentDate)
      {
	// if the tail and head point to the same node, need to reset the shelf
	// this implies there was only one node remaining in the list, so tail
	// must be modified as well
	if (tail == head)
	  {
	    delete head;
	    head = tail;
	    tail = NULL;
	    head = NULL;
	  }
	// point head to head's next node and delete the old head
	else
	  {
	    node* temp = head;
	    head = head->next;
	    delete temp;
	  }
      }
  }

  // clean - helper for deconstructor
  // steps through the list and deletes every node, then sets head and tail to null
  void shelf::clean()
  {
    node *curr = head;
    while(curr != NULL)
      {
	node *temp = curr;
	curr = curr->next;
	delete temp;
      }
    head = NULL;
    tail = NULL;
  }
  /*
  // tester which displays the whole contents of the shelf
  void shelf::DIAGNOSTICS()
  {
    node *current = head;
    std::cout << "Begin scan" << std::endl;
    while (current != NULL)
      {
	std::cout << current->quantity << std::endl;
	current = current->next;
      }
    std::cout << "End scan" << std::endl;
  }
*/
  // Methods and data for auditing purposes, copied from homework 3
  long long shelf::constructor_calls = 0;
  long long shelf::destructor_calls = 0;

  // Returns value of constructor_calls
  long long shelf::constructor_count()
  {
    return shelf::constructor_calls;
  }

  // Returns value of destructor_calls()
  long long shelf::destructor_count()
  {
    return shelf::destructor_calls;
  }
}

