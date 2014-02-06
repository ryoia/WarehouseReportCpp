//--------------------------------------------
// Colin Probasco
// Minwen Gao
// 29 January 2014
//
// report.cpp
//
// this file contains the main method and is the default entry point
// for the application
//
// main first reads from the data file supplied as a command line argument
// with each line read, main will modify either of the two maps
// main will modify the food index if the line begins with a food item
// main will modify the warehouse map if the line begins with anything else
// --initially warehouses are just added to the map, but requests will modify
// --the contents of each warehouse
//
// based on the data read from the first part of main, main will then generate
// a report consisting of data related to products that aren't stocked at all
// products that are fully stocked, and each warehouse's busiest day
//--------------------------------------------

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

#include "boost/date_time/gregorian/gregorian.hpp"

#include "warehouse.h"
#include "shelf.h"
#include "node.h"

//Food struct that contains name and shelf life. 
struct food
{
  int shelfLife;
  std::string name;
  food(std::string i_name, int i_shelfLife)
  {
    shelfLife = i_shelfLife;
    name = i_name;
  }
};

int main(int argc, char* argv[])
{
  //termninates if the argument length isn't 1
  if(argc != 2)
    {
      std::cout << "Terminates due to wrong #s of arguments being passed, please try again and only pass 1 text file." << std::endl;
      return 0;
    }
  else
    {
      //using boost::gregorian date library
      boost::gregorian::date startDate;
      
      std::string fileName = argv[1];
      int daysSinceStart = 0;

      std::map<std::string, reports::warehouse*>* warehouseMap;
      std::map<std::string, food> foodIndex;

      warehouseMap = new std::map<std::string, reports::warehouse*>();
      foodIndex = std::map<std::string, food>();
      //

      std::string pFood = "Foo";
      std::string pWarehouse = "War";
      std::string pStart = "Sta";
      std::string pReceive = "Rec";
      std::string pRequest = "Req";
      std::string pNext = "Nex";
      std::string pEnd = "End";
      try
	{
	  //start reading file. 
	  std::ifstream readFile;
	  readFile.open(fileName.c_str());
	  std::string line;
	  while(std::getline(readFile, line))
	    {	      
	      std::string id = line.substr(0, 3);

	      // If it's food
	      if(id == pFood)
		{
		  std::string upcCode = line.substr(21, 10);
		  int indexOfWhiteSpace = 0;
	 	  //trace to the whitespace
		  for(int i = 45; i < line.length(); i++)
		    {
		      if(line[i] == ' ')
			{
			  indexOfWhiteSpace = i;
			  break;
			}
		    }
		  int life = atoi(line.substr(45, indexOfWhiteSpace - 45).c_str());
		  int sizeOfName = line.length() - (indexOfWhiteSpace + 8) - 1;
		  std::string foodName = line.substr(indexOfWhiteSpace + 8, sizeOfName);
		  
		  food foodToInsert = food(foodName, life);

		  foodIndex.insert(std::pair<std::string, food>(upcCode, foodToInsert));
		  
		}
	      
	      // It's a warehouse
	      else if(id == pWarehouse)
		{
		  std::string wName = line.substr(12, line.length() - 13);
		  if (warehouseMap->find(wName) == warehouseMap->end())
		    {
       		      reports::warehouse *houseToInsert = new reports::warehouse();
		      warehouseMap->insert(std::pair<std::string, reports::warehouse*>(wName, houseToInsert));
		    }
		}
		
	      // It's the start date
	      else if (id == pStart)
	        {
		  std::string month = line.substr(12, 2);
		  std::string day = line.substr(15, 2);
		  std::string year = line.substr(18, 4);

		  std::string dateString = year+day+year;

		  startDate = boost::gregorian::date(boost::gregorian::from_undelimited_string(dateString));
		}

	      // It's receive
	      else if (id == pReceive)
		{
		  std::string upcCode = line.substr(9, 10);
		  int indexOfWhiteSpace = 0;
		  for(int i = 20; i < line.length(); i++)
		    {
		      if(line[i] == ' ')
			{
			  indexOfWhiteSpace = i;
			  break;
			}
		    }
		  
		  int qty = atoi(line.substr(20, indexOfWhiteSpace - 20).c_str());
		  int nameLength = line.length() - (indexOfWhiteSpace + 2) ;
		  std::string name = line.substr(indexOfWhiteSpace + 1, nameLength);
		  //checks if food's name already exists
		  try 
		    {
		      food foodLookup = foodIndex.at(upcCode);
		      reports::warehouse* curr = warehouseMap->at(name);
		      curr->receiveToShelf(upcCode, qty, daysSinceStart, foodLookup.shelfLife);
		    }
		  catch (std::exception& e)
		    {
		      std::cout << e.what() << std::endl;
		      std::cout << "caught something in receive. " << std::endl;
		    }
		}

	      // It's request
	      else if (id == pRequest)
		{
		  std::string upcCode = line.substr(9, 10);
		  int indexOfWhiteSpace = 0;
		  for(int i = 20; i < line.length(); i++)
		    {
		      if(line[i] == ' ')
			{
			  indexOfWhiteSpace = i;
			  break;
			}
		    }

		  int qty = atoi(line.substr(20, indexOfWhiteSpace - 20).c_str());
		  int nameLength = line.length() - (indexOfWhiteSpace + 2);
		  std::string name = line.substr(indexOfWhiteSpace + 1, nameLength);

		  try 
		    {
		      reports::warehouse* curr = warehouseMap->at(name);
		      curr->requestToShelf(upcCode, qty);
		    }
		  catch (std::exception& e)
		    {
		      std::cout << e.what() << std::endl;
		      std::cout << "caught something in request. " << std::endl;
		    }
		}

	      // It's next day
	      else if (id == pNext)
		{
		  //goes through each warehouse and increments the day. 
		  typedef std::map<std::string, reports::warehouse*>::iterator walkThrough;
		  for(walkThrough iterator = warehouseMap->begin(); iterator != warehouseMap->end(); ++iterator)
		    {
		      reports::warehouse *curr = iterator->second;
		      curr->advanceDay(daysSinceStart);
		    }
		  daysSinceStart++;
		}

	      // It's the end
	      else if (id == pEnd)
		{
		  readFile.close();
		  break;
		}

	    }
	}
      catch (std::exception& e)
	{
	  std::cout << e.what() << std::endl;
	  std::cout << "caught exceptions when trying to read data. " << std::endl;
	}
      typedef std::map<std::string, reports::warehouse*>::iterator walkThrough;
      typedef std::map<std::string, food>::iterator foodWalk;

      // Now generate the report
      std::cout << "Report by Colin & Minwen" << std::endl;
      std::cout << std::endl;
      //iterates through for unStocked and fully-stocked items.
      std::cout << "Unstocked Products:" << std::endl;
      for(foodWalk fIterator = foodIndex.begin(); fIterator != foodIndex.end(); ++fIterator)
	{
	  bool isStocked = true;
	  for(walkThrough iterator = warehouseMap->begin(); iterator != warehouseMap->end(); ++iterator)
	    {
	      reports::warehouse* curr = iterator->second;
	      isStocked = curr->isStocked(fIterator->first);

	      if (isStocked) break;
	    }
	  if (!isStocked)
	    {
	      std::cout << fIterator->first << " " << fIterator->second.name << std::endl;
	    }
	}

      std::cout << std::endl;
      std::cout << "Fully-Stocked Products: " << std::endl;
      for(foodWalk fIterator = foodIndex.begin(); fIterator != foodIndex.end(); ++fIterator)
	{
	  bool isStocked = false;
	  for(walkThrough iterator = warehouseMap->begin(); iterator != warehouseMap->end(); ++iterator)
	    {
	      reports::warehouse* curr = iterator->second;
	      isStocked = curr->isStocked(fIterator->first);

	      if (!isStocked) break;
	    }
	  if (isStocked)
	    {
	      std::cout << fIterator->first << " " << fIterator->second.name << std::endl;
	    }
	}

      //busiest days
      std::cout << std::endl;
      std::cout << "Busiest Days:" << std::endl;
      for(walkThrough iterator = warehouseMap->begin(); iterator != warehouseMap->end(); ++iterator)
	{
	  int busiestDay = iterator->second->getBusiestDay();
	  boost::gregorian::date busiest = boost::gregorian::date(startDate);
	  boost::gregorian::date_duration since = boost::gregorian::date_duration((long)daysSinceStart);
	  busiest = busiest + since;
	  std::cout << iterator->first << " " << busiest.month() << "/" << busiest.day() << "/" << busiest.year() << " " << iterator->second->getHighestTransactions() << std::endl;
	}

      //--- Clear memory ---//
      for(walkThrough iterator = warehouseMap->begin(); iterator != warehouseMap->end(); ++iterator)
	{
	  delete iterator->second;
	}
      warehouseMap->clear();
      delete warehouseMap;

    }

  /*std::cout << "Node C : " << reports::node::constructor_count() << std::endl;
  std::cout << "Node D : " << reports::node::destructor_count() << std::endl;

  std::cout << "" << std::endl;

  std::cout << "Shelf C : " << reports::shelf::constructor_count() << std::endl;
  std::cout << "Shelf D : " << reports::shelf::destructor_count() << std::endl;

  std::cout << "" << std::endl;

  std::cout << "Warehouse C : " << reports::warehouse::constructor_count() << std::endl;
  std::cout << "warehouse D : " << reports::warehouse::destructor_count() << std::endl;
*/
}
