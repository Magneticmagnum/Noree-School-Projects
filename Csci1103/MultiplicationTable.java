/*
 * name:  Chaichana Noree
 * 
 * 	Given two ints x and y, output the multiplication table of x columns, and y rows.
 */

import java.util.Scanner;

public class MultiplicationTable {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int width = -1;
		int height = -1;

		while (width < 0) {
			System.out.println("Enter width for Multiplication table: ");
			width = in.nextInt();
			if(width < 0){
				System.out.println("Please enter a positive width, try again");
			}
		}
		while(height < 0){
			System.out.println("Enter height for Multiplication table: ");
			height = in.nextInt();
			if(height < 0){
				System.out.println("Please enter a positive height, try again");
			}
		}
		
		System.out.print("x ");
		
		for(int z = 1; z <= width; z++){	//prints initial line
			System.out.printf("%3d", z);
		}
		System.out.println();
		
	      for (int i = 1; i <= height; i++) {
	         System.out.print(i + " ");
	         for (int j = 1; j <= width; j++) {
	            //System.out.print(i*j + "    ");
	        	 int timed = i * j;
	        	 System.out.printf("%3d", timed);
	         }
	         System.out.println();
	      }
	}

}
