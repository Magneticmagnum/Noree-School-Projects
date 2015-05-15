/*
 * Name: Chaichana Noree
 * 
 * This program should implement the Collatz Conjecture. 
 * Take a number n from the user, and test the Collatz Conjecture from 1 to n. 
 * Check to make sure the user input is greater than or equal to 1. 
 * Prompt the user for a new n if the number is less than 1. 
 * Count the number of loops it takes for the number to get back to one. 
 * Display this for each number from 1 to n.
 * 
 */

import java.util.Scanner;

public class Collatz {

	public static void main(String [] args){
		Scanner in = new Scanner(System.in);
		
		int steps = 0;
		int number;
		int input;
		
		System.out.println("Enter a number to Collaz it: ");
		input = in.nextInt();
		
		for(int i = 1; i < input; i++){
			number = i;
			while(number != 1){
				if(number %2 == 0){ //its even
					number = number / 2;
				}
				else {  //its odd
					number = (number * 3) + 1;
				}
				steps++;
			}
			
			System.out.println("The number " + input + " takes " + steps + "steps to reach 1");
			steps = 0;
		}
	}
}
