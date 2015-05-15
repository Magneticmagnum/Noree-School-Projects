/*
 * Chaichana Noree
 * Csci 1103 - Intro to java
 * Prof: Dovolis
 * Subj: Lab #5 - a class that will represent an interval of numbers. 
 * An interval is defined by 2 endpoints, a left and a right endpoint, 
 * and contains all the numbers between these two endpoints. Implement
 * the required functions for Interval.
 */

public class Interval {

	private int leftEndpoint;
	private int rightEndpoint;

	public Interval(int left, int right) {
		if (left > right) {
			int temp = left;
			left = right;
			right = temp;
		}
		leftEndpoint = left;
		rightEndpoint = right;
	}

	public Interval() {
		leftEndpoint = 0;
		rightEndpoint = 0;
	}

	public int getLeftEndpoint() {
		return leftEndpoint;
	}

	public void setLeftEndpoint(int left) {
		leftEndpoint = left;
	}

	public int getRightEndpoint() {
		return rightEndpoint;
	}

	public void setRightEndpoint(int right) {
		rightEndpoint = right;
	}

	public String toString() {
		String s = "[" + leftEndpoint + ", " + rightEndpoint + "]";
		return s;
	}

	public boolean equals(Interval otherInterval) {
		return (this == otherInterval);
	}

	public int length() {
		return this.rightEndpoint - this.leftEndpoint;
	}

	public boolean contains(Interval otherInterval) {
		return (this.rightEndpoint > otherInterval.rightEndpoint && this.leftEndpoint < otherInterval.leftEndpoint);
	}

	public boolean overlap(Interval otherInterval) {
		if (this.contains(otherInterval)) {
			return true;
		} else {
			return ((this.leftEndpoint > otherInterval.leftEndpoint) && (this.leftEndpoint < otherInterval.rightEndpoint))
					|| ((this.rightEndpoint > otherInterval.leftEndpoint) && (this.rightEndpoint < otherInterval.rightEndpoint));
		}
	}

	public Interval union(Interval otherInterval) {
		int low, high;
		if (this.leftEndpoint < otherInterval.leftEndpoint) {
			low = this.leftEndpoint;
		} else {
			low = otherInterval.leftEndpoint;
		}
		if (this.rightEndpoint > otherInterval.rightEndpoint) {
			high = this.rightEndpoint;
		} else {
			high = otherInterval.rightEndpoint;
		}

		Interval temp = new Interval(low, high);

		return temp;
	}

	public Interval intersection(Interval otherInterval) {
		int left, right;
		if (this.overlap(otherInterval)) {
			if (this.leftEndpoint < otherInterval.leftEndpoint) {
				left = otherInterval.leftEndpoint;
			} else {
				left = this.leftEndpoint;
			}
			if (this.rightEndpoint < otherInterval.rightEndpoint) {
				right = this.rightEndpoint;
			} else {
				right = otherInterval.rightEndpoint;
			}
			Interval temp = new Interval(left, right);
			return temp;
		} else {
			Interval temp = new Interval();
			return temp;
		}

	}

	public static void main(String[] args) {

		Interval i1 = new Interval(5, 2);
		Interval i2 = new Interval(3, 4);
		Interval i3 = new Interval(0, 0);
		Interval i4 = new Interval(-5, 5);

		// tests for equals method
		System.out.println("Does " + i1 + " equal " + i2 + " ?");
		System.out.println("expected: False");
		System.out.println("Actual: " + i1.equals(i2));
		System.out.println();

		System.out.println("Does " + i3 + " equal " + i3 + " ?");
		System.out.println("expected: True");
		System.out.println("Actual: " + i3.equals(i3));
		System.out.println();

		// tests for length method

		System.out.println("What s the length of " + i1 + " ?");
		System.out.println("Expected: 3");
		System.out.println("Actual: " + i1.length());
		System.out.println();

		System.out.println("What s the length of " + i2 + " ?");
		System.out.println("Expected: 1");
		System.out.println("Actual: " + i2.length());
		System.out.println();

		// tests for contains method

		System.out.println("Does " + i1 + " contain " + i2 + " ? ");
		System.out.println("Expected: True");
		System.out.println("Actual: " + i1.contains(i2));
		System.out.println();

		System.out.println("Does " + i3 + " contain " + i4 + " ? ");
		System.out.println("Expected: False");
		System.out.println("Actual: " + i3.contains(i4));
		System.out.println();

		// tests for overlap method
		System.out.println("Does " + i1 + " overlap " + i2 + " ? ");
		System.out.println("Expected: true");
		System.out.println("Acutal: " + i1.overlap(i2));
		System.out.println();

		System.out.println("Does " + i3 + " overlap " + i2 + " ? ");
		System.out.println("Expected: false");
		System.out.println("Acutal: " + i3.overlap(i2));
		System.out.println();
		
		//tests for union method
		System.out.println("What is the union of " + i1 + " and " + i2 + " ? ");
		System.out.println("Expected: [2, 5]");
		System.out.println("Acutal: " + i1.union(i2));
		System.out.println();
		
		System.out.println("What is the union of " + i3 + " and " + i1 + " ? ");
		System.out.println("Expected: [0, 5]");
		System.out.println("Acutal: " + i3.union(i1));
		System.out.println();
		
		//tests for intersection method
		System.out.println("What is the intersection of " + i1 + " and " + i2 + " ? ");
		System.out.println("Expected: [3, 4]");
		System.out.println("Acutal: " + i1.intersection(i2));
		System.out.println();
		
		System.out.println("What is the intersection of " + i3 + " and " + i2 + " ? ");
		System.out.println("Expected: [0, 0] *meaning no intersection exists*");
		System.out.println("Acutal: " + i3.intersection(i2));
		System.out.println();
		
	}
}