//--------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
//
// warehouse.h
//
// Header and declarations for warehouse class
// A warehouse contains a key value map mapping UPC codes to shelf pointers
// A warehouse also contains values which hold data regarding the warehouse's busiest day
// the warehouse class also has functions for handling incoming recieves and outgoing
// requests and for removing expired goods
//--------------------------------------

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <iostream>
#include "shelf.h"
#include <string>
#include <map>

namespace reports
{
  // Forward declaration of shelf class
  class shelf;

  class warehouse
  {
  public:

    // Constructor - builds a new warehouse object
    warehouse();

    // Destructor - destroys warehouse object and frees memory
    ~warehouse();

    // receiveToShelf - handles incoming recieve of a certain product
    // parameter - upc_code - upc_code of the product to look up on the shelf map
    // parameter - qty - quantity of incoming product
    // parameter - currentDate - int for calculating expiration date of incoming product
    // parameter - shelfLife - int for calculating expiration date of incoming product
    void receiveToShelf(std::string upc_code, int qty, int currentDate, int shelfLife);

    // requestToShelf - handles incoming requests for a certain product
    // parameter - upc_code - upc_code of the product to look up on the shelf map
    // parameter - qty - amount of product requested
    void requestToShelf(std::string upc_code, int qty);

    // advanceDay - handles removal of all expired products and calculates if the current
    // day's transactions exceed the previous maximum
    void advanceDay(int dayVal);

    // isStocked - checks if a certain product is stocked in the warehouse
    // returns - true if stocked, false if not
    bool isStocked(std::string upc_code);

    // getBusiestDay - returns an int representing the busiest day as days since start date
    int getBusiestDay();

    // getHighestTransactions - returns an int representing max transactions since start date
    int getHighestTransactions();
  private:
    // map object which will map upc_codes to shelf pointers for fast access to certain
    // product shelves
    std::map<std::string, shelf*> *shelfMap;

    void clean();

    // int representing the busiest day for the warehouse as days since start date
    int busiestDay;

    // int representing the historical highest transactions on the busiest day
    int highestTransactionsToDate;

    // int representing the current day's total transactions
    int currentDayTransactions;

    //--- Auditing ---//

    // methods and data for auditing purposes, copied from homework 3
  public:
    static long long constructor_count ();
    static long long destructor_count ();

  private:
    static long long constructor_calls;
    static long long destructor_calls;
  };
}

#endif
