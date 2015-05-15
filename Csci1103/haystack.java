/*
 * name:  Chaichana Noree
 * 
 * Needle in a haystack problem:  Given a char needle, find the needles in a given string.
 * Return the number of needles found.
 */

import java.util.Scanner;

public class haystack {

	public static void main(String [] args){
		char needle;
		String haystack, temp;
		int counter = 0;
		Scanner in = new Scanner(System.in);
		System.out.println("Enter a string");
		haystack = in.next();
		System.out.println("Enter a char to find in that string");
		temp = in.next();
		
		char haystack2[] = new char[haystack.length()];
		haystack2 = haystack.toCharArray();
		needle = temp.charAt(0);
		
		for(int i = 0; i < haystack.length(); i++){
			if(needle == haystack2[i]){
				counter++;
			}
		}
		System.out.println(counter);
	}
}
