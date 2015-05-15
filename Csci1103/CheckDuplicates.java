/*
 * Name:  Chaichana Noree
 * 
 * Checks for duplicates in a string and returns the index of the locations found
 */


import java.util.Scanner;

public class CheckDuplicates {

	public static void main(String[] args) {
		
		Scanner in = new Scanner(System.in);
		
		System.out.println("Enter a string to check: ");
		String str = in.nextLine();
		
		System.out.println("Enter a char to check: ");
		String c = in.nextLine();
		char checker = c.charAt(0);

		for (int i = 0; i < str.length(); i++) {
			if (checker == str.charAt(i)) {
				System.out.println("Found char " + checker + " at " + i);
			}
		}

	}
}
