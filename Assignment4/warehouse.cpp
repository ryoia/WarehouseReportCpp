//------------------------------------------------
// Colin Probasco
// Minwen Gao
// 28 January 2014
//
// warehouse.h
//
// Function definitions for warehouse class
// Full description found in warehouse.h
// Contains map for fast access of shelves in the warehouse
// Contains functions for handling various requests
// Contains historical data for the warehouse regarding busiest day
//------------------------------------------------

#include <iostream>
#include "warehouse.h"

namespace reports
{
  // Constructor - builds a new warehouse object
  warehouse::warehouse()
    {
      // Set initial values to 0
      busiestDay = 0;
      highestTransactionsToDate = 0;
      currentDayTransactions = 0;

      // Instantiate shelfMap for quick lookup of shelves based on upc codes
      shelfMap = new std::map<std::string, shelf*>();

      // Increment constructor calls
      warehouse::constructor_calls++;
    }

  // Destructor - destroys warehouse object and frees memory
  warehouse::~warehouse()
    {
      warehouse::destructor_calls++;
      clean();
    }
    
  // receiveToShelf - handles incoming receive of a certain product
  // parameter - upc_code - upc code of the product to use to look up on the shelf map
  // parameter - qty - quantity of incoming product
  // parameter - currentDate - int for calculating expiration date of incoming product
  // parameter - shelfLife - int for calculating expiration date of incoming product (only
  // used if shelf doesn't exist yet)
  void warehouse::receiveToShelf(std::string upc_code, int qty, int currentDate, int shelfLife)
    {
      // map->at throws an exception if the key doesn't exist, so if found, do the following:
      try
	{
	  // acquire the appropriate shelf from the shelf table
	  shelf *curr = shelfMap->at(upc_code);

	  // pass the receive command to the shelf
	  curr->receive(qty, currentDate);

	  // in addition, add the quantity to current day's transactions
	  currentDayTransactions += qty;
	}
      // otherwise, the key doesn't exist yet, so a new shelf must be made
      catch(std::exception e)
	{
	  // make a new shelf
	  shelf *curr = new shelf(shelfLife);

	  // add the shelf to the shelf map
	  shelfMap->insert(std::pair<std::string, shelf*>(upc_code, curr));

	  // pass the receive command to the shelf
	  curr->receive(qty, currentDate);

	  // in addition, add the quantity to the current day's transactions
	  currentDayTransactions += qty;
	}
    }

  // requestToShelf - handles incoming requests for a certain product
  // parameter - upc_code - upc code of the product to look up on the shelf map
  // parameter - qty - quantity of product requested
  void warehouse::requestToShelf(std::string upc_code, int qty)
  {
    // map->at throws an exception if the key doesn't exist, so if found, do the following:
    try
      {
	// find the appropriate shelf associated with the upc code
        shelf *curr = shelfMap->at(upc_code);

	// pass the request to the shelf
        curr->request(qty);

	// add the quantity to the day's transactions
        currentDayTransactions += qty;
	
	// if there is no more contents in the shelf, remove it from the map and delete the shelf
	if (!isStocked(upc_code))
	  {
	    delete curr;
	    // Note to test
	    shelfMap->erase(upc_code);
	  }
      }
    // no shelf is found in this case, so just add to the transactions
    catch(std::exception e)
      {
        currentDayTransactions += qty;
      }
  }

  // advanceDay - handles removal of all expired products and calculates if the current
  // day's transactions exceed the previous maximum
  void warehouse::advanceDay(int dayVal)
  {
    // define an iterator to iterate through all of the shelves in the map
    typedef std::map<std::string, shelf*>::iterator walkThrough;
    // iterate through all keys, checking to for expired goods on all shelves
    for(walkThrough iterator = shelfMap->begin(); iterator != shelfMap->end(); ++iterator)
      {	 
	// remove expired products from the shelf if any
	shelf *curr = iterator->second;
        curr->removeExpired(dayVal);

	// if removal of expired objects from the shelf resulted in it being empty
	// delete the shelf and remove it from the shelf map
	// note: map.erase does in fact destroy the item in the value
	// but since this is a map of pointers, only the pointer is removed,
	// so the object must also be deleted
	/*if (!isStocked(iterator->first))
	  {
	    //--INCOMPLETE--//
	    delete iterator->second;
	    }*/
      }
    // now check if the total transactions on the current day exceed or is equivalent
    // to the previous highest transactions, if so update the busiest day and
    // highest transaction numbers appropriately
    if ( currentDayTransactions >= highestTransactionsToDate)
      {
        busiestDay = dayVal;
        highestTransactionsToDate = currentDayTransactions;
      }

    // and reset the transactions to 0
    currentDayTransactions = 0;
  }

  // isStocked - checks if a certain product is stocked in the warehouse
  // returns - true if stocked, false if not
  bool warehouse::isStocked(std::string upc_code)
  {
    // since map.at throws an exception if the key is not found, do the following if found:
    try
      {
        shelf *curr = shelfMap->at(upc_code);

	// note, this part should probably always be reached if the shelf exists
        if(curr->head != NULL)
          return true;

	// this should probably never be reached
        else
          return false;
      }
    // not finding the key implies the shelf doesn't exist, so return false
    catch(std::exception e)
      {
        return false;
      }
  }

  // getBusiestDay - returns an int representing the busiest day as days since the start date
  int warehouse::getBusiestDay()
  {
    return busiestDay;
  }

  // getHighestTransactions - returns an int representing highest transactions
  int warehouse::getHighestTransactions()
  {
    return highestTransactionsToDate;
  }

  void warehouse::clean()
  {
    typedef std::map<std::string, shelf*>::iterator walkThrough;
    for(walkThrough iterator = shelfMap->begin(); iterator != shelfMap->end(); ++iterator)
      {	 
	delete iterator->second;
      }
    shelfMap->clear();
  }

  //--- Auditing ---///

  // Methods and data for auditing purposes, copied from homework 3
  long long warehouse::constructor_calls = 0;
  long long warehouse::destructor_calls = 0;

  // Returns value of constructor_calls
  long long warehouse::constructor_count()
  {
    return warehouse::constructor_calls;
  }

  // Returns value of destructor_calls
  long long warehouse::destructor_count()
  {
    return warehouse::destructor_calls;
  }
  
}
