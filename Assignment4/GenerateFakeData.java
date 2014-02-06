//This is a java program that generates random data files for testing
//The user can indicate the #s of days, transactions/day, cities, products, random seeds in the program
/**
 * 
 */
package assignment03;

import java.io.*;
import java.util.*;

/**
 * @author pajensen
 *
 */
public class GenerateFakeData
{

    /**
     * The following is quick-n-dirty code.  No guarantees.
     * 
     * @param args
     */
    public static void main (String[] args) throws IOException
    {
        Scanner keyboard = new Scanner (System.in);
        
        // Prompt for test data variables.
        
        System.out.print ("Enter a start date: ");
        String startDate = keyboard.next();
        
        System.out.print ("How many cities? ");
        int citysCount = keyboard.nextInt();
        
        System.out.print ("How many products? ");
        int productsCount = keyboard.nextInt();
        
        System.out.print ("Max shelf life? ");
        int maxShelfLife = keyboard.nextInt();
        
        System.out.print ("Days of simulation? ");
        int daysCount = keyboard.nextInt();
        
        System.out.print ("Max actions per day? ");
        int maxActions = keyboard.nextInt();
        
        System.out.print ("Output filename: ");
        String outputFilename = keyboard.next();
        
        System.out.print ("Random seed: ");
        long seed = keyboard.nextLong();
        
        Random rand = new Random(seed);
        
        // Read in the list of cities.
        
        List<String> cities = new ArrayList<String>();
        Map<String, Integer> population = new HashMap<String, Integer>();
        
        File cityTxt = new File("src/assignment03/cities.txt");
        Scanner s = new Scanner(cityTxt);
        
        while (s.hasNext())
        {
            String line = s.nextLine().trim();
            if (line.length() == 0)
                continue;
            
            String name = line;
            int pop = s.nextInt();
            
            cities.add(name);
            population.put(name, pop);
        }
        
        while (cities.size() > citysCount)
            cities.remove(rand.nextInt(cities.size()));
        
        // Build the list of products.
        
        List<FoodItem> foods = new ArrayList<FoodItem>();
        Map<String, FoodItem> UPCLookup = new HashMap<String, FoodItem>();
        
        File productsTxt = new File("src/assignment03/products.txt");
        s = new Scanner(productsTxt);
        
        while (s.hasNext())
        {
            String line = s.nextLine().trim();
            if (line.length() == 0)
                continue;
            
            String upc;
            do
            {
                upc = String.format("%010d", rand.nextInt(1000000000));
            } while (UPCLookup.get(upc) != null);
            
            int life = rand.nextInt(maxShelfLife) + 1;
            FoodItem f = new FoodItem(line, upc, life);
            foods.add(f);
            UPCLookup.put(f.getUpcCode(), f);
        }
        
        while (foods.size() > productsCount)
            foods.remove(rand.nextInt(foods.size()));
        
        // Open the output file.
        
        File outputFile = new File("src/assignment03/" + outputFilename);
        PrintWriter output = new PrintWriter(outputFile);

        // Output a list of items.
        
        for (FoodItem f : foods)
            output.println(f);
        
        // Output a list of warehouses.
        
        for (String city : cities)
            output.println("Warehouse - " + city);
        
        // Output the start date.
        
        output.println ("Start date: " + startDate);
        
        // Generate random transactions.
        
        for (int day = 0; day < daysCount; day++)
        {
            int transactionCount = rand.nextInt(maxActions)+1;
            for (int count = 0; count < transactionCount; count++)
            {
                // Pick a transaction type, warehouse, item, and quantity.
                
                int type = rand.nextInt(2);
                String warehouse = cities.get(rand.nextInt(cities.size()));
                FoodItem product = foods.get(rand.nextInt(foods.size()));
                int quantity = rand.nextInt(10)+1;
                
                // Output the transaction.
                
                if (type == 0)
                    output.print("Receive: ");
                else
                    output.print("Request: ");
                
                output.println (product.getUpcCode() + " " + quantity + " " + warehouse);
            }
            
            if (day < daysCount - 1)
                output.println ("Next day:");
        }
        output.println ("End");
        
        // Close the output file.
        
        output.close();
    }

}
