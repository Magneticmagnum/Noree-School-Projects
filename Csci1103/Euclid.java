/*
 * Chaichana Noree
 * 
 * This program should implement Euclid's algorithm for finding the greatest common divisor.
 */

import java.util.Scanner;

public class Euclid {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);

		int a = -1;
		int b = -1;
		int gcd;

		while (a < 0) {
			System.out.println("Enter a positive number for a:");
			a = in.nextInt();
			if (a < 0) {
				System.out.println("You entered a negative number, try again.");
			}
		}

		while (b < 0) {
			System.out.println("Enter a positive number for b:");
			b = in.nextInt();
			if (b < 0) {
				System.out.println("You entered a negative number, try again.");
			}
		}
		if (a > b) {
			int temp;
			temp = a;
			a = b;
			b = temp;
		}
		
		gcd = euclid(a, b);
		
		System.out.println("The greatest common devisor is " + gcd);
		

	}
	public static int euclid(int x, int y){
        if (y == 0) return x;
        else return euclid(y, x % y);
	}
}
