/*
 * Name:  Chaichana Noree
 * ID:    4295362
 * HW:    #3
 * Prof:  Dovolis
 * Class: Csci1103
 * 
 * Implements various vector functions using the vector class.
 */

public class vector {

	public static void print_vector(double[] d) {
		/*
		 * This method prints every element in the given array d. If d is null,
		 * print "null". Example output when d = {1.5, 10.86, 3.45, -2.7}: [1.5,
		 * 10.86, 3.45, -2.7]
		 */
		if (d == null) {
			System.out.print("null");
		} else {
			System.out.print("[");
			for (int i = 0; i < d.length; i++) {
				System.out.print(d[i] + ", ");
			}
			System.out.print("]");
		}
	}

	public static double[] add_vectors(double[] v1, double[] v2) {
		/*
		 * This method should check if the number of elements in v1 is equal to
		 * the number of elements in v2. If so, create and return a new double[]
		 * v3 where v3[i] = v1[i] + v2[i]. If not, return null.
		 */
		if (v1.length == v2.length) {
			double[] v3 = new double[v1.length];
			for (int i = 0; i < v1.length; i++) {
				v3[i] = v1[i] + v2[i];
			}
			return v3;
		}
		return null;
	}

	public static double[] multiply_by_const(double[] v1, double c) {
		/*
		 * This method should create and return a new double[] v2 where v2[i] =
		 * v1[i] * c.
		 */
		double[] v2 = new double[v1.length];
		for (int i = 0; i < v1.length; i++) {
			v2[i] = v1[i] * c;
		}
		return v2;
	}

	public static double norm_length(double[] v1) {
		/*
		 * This method should return a double that is equal to the square root
		 * of the sum of the squares of the elements of v1. For example, if v1 =
		 * {3.0, 4.0, 5.0} then return a double named length such that length =
		 * square root of ((3.0)^2 + (4.0)^2 + (5.0)^2).
		 * 
		 * Hint: square root is Math.sqrt(double x).
		 */
		double length = 0;
		for (int i = 0; i < v1.length; i++) {
			length += Math.pow(v1[i], 2);
		}
		return Math.sqrt(length);

	}

	public static double dot_product(double[] v1, double[] v2) {
		/*
		 * This method should check if the number of elements in v1 is equal to
		 * the number of elements in v2. If so, it should return a double named
		 * dp that is equal to the sum of the pairwise products of v1 and v2.
		 * For example, if v1 = {1.0, 2.0, 3.0} and v2 = {3.0, 4.0, 1.0} then dp
		 * = (1.0 * 3.0) + (2.0 * 4.0) + (3.0 * 1.0). If not, return Double.NaN
		 * (this stands for Not a Number in java).
		 */
		if (v1.length == v2.length) {
			double dp = 0;
			;
			for (int i = 0; i < v1.length; i++) {
				dp += (v1[i] * v2[i]);
			}
			return dp;
		}
		return Double.NaN;
	}

	public static void main(String[] args) {
		double[] a = { 1.78, 2.34, 1.61, 42.0 };
		double[] b = { 2.34, 5.0, 6.0 };
		double[] c = { 9.0, 4.0, 6.0, 13.0 };
		double[] d = { 7.6, -34.5, 18.0, 3.33 };

		System.out.println("Test 1:");
		System.out.println("Add a and b:");
		System.out.print("a = ");
		print_vector(a);
		System.out.print("b = ");
		print_vector(b);
		System.out.println("Expected : null");
		System.out.print("Actual   : ");
		print_vector(add_vectors(a, b));
		System.out.println();

		System.out.println("Test 2:");
		System.out.println("Add a and c:");
		System.out.print("a = ");
		print_vector(a);
		System.out.print("c = ");
		print_vector(c);
		System.out.println("Expected : {10.78, 6.34, 7.61, 55.0}");
		System.out.print("Actual   : ");
		print_vector(add_vectors(a, c));
		System.out.println();

		System.out.println("Test 3:");
		System.out.println("Multiply a by 2.0:");
		System.out.print("a = ");
		print_vector(a);
		System.out.println("Expected : {3.56, 4.68, 3.22, 84.0}");
		System.out.print("Actual   : ");
		print_vector(multiply_by_const(a, 2.0));
		System.out.println();

		System.out.println("Test 4:");
		System.out.println("Norm length of c:");
		System.out.print("c = ");
		print_vector(c);
		System.out.println("Expected : 17.3781");
		System.out.print("Actual   : " + norm_length(c));
		System.out.println();

		System.out.println("Test 5:");
		System.out.println("Dot product of c and d:");
		System.out.print("c = ");
		print_vector(c);
		System.out.print("d = ");
		print_vector(d);
		System.out.println("Expected : 81.69");
		System.out.print("Actual   : " + dot_product(c, d));
		System.out.println();

		System.out.println("Test 6:");
		System.out.println("Dot product of c and b:");
		System.out.print("c = ");
		print_vector(c);
		System.out.print("b = ");
		print_vector(b);
		System.out.println("Expected : NaN");
		System.out.print("Actual   : " + dot_product(c, b));
		System.out.println();
	}
}
