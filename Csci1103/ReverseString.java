/*
 * 	name: Chaichana Noree 
 *
 * 	Reverses a string.
 */

import java.util.Scanner;

public class ReverseString {

	private static String reverser(String s){
		int length = s.length();
		int last = length-1;
		char[] c = s.toCharArray();
		for(int i = 0; i < length/2; i++){
			char temp = c[i];
			c[i] = c[last-i];
			c[last-i] = temp;
		}
		
		return new String(c);
	}
	
	public static void main(String[] args){
		Scanner in = new Scanner(System.in);
		System.out.println("Enter a string to reverse:");
		
		String input = in.nextLine();
		
		System.out.println(reverser(input));
	}
}
