public class Matrix {
	private double[][] arr;

	public Matrix(int row, int col) {
		arr = new double[row][col];

	}

	public double getIndex(int i, int j) {
		return arr[i][j];
	}

	public void setIndex(int i, int j, double val) {
		arr[i][j] = val;
	}

	public int numberRows() {
		return arr.length;
	}

	public int numberColumns() {
		return arr[0].length;
	}

	public void print() {
		System.out.print("[");
		for (int i = 0; i < this.numberRows(); i++) {
			for (int j = 0; j < this.numberColumns(); j++) {
				System.out.print(arr[i][j]);
				if(j == this.numberColumns()-1){
					System.out.print("; ");
				}
				else{
					System.out.print(", "); 
				}
			}
		}
		System.out.print("]");
		System.out.println();
	}

	public Matrix multiplyByConstant(double c) {
		/*
		 * This method should multiply every element of the matrix by a constant
		 * double that is passed in. It should then return a new Matrix of the
		 * result.
		 */
		Matrix a = new Matrix(this.numberRows(), this.numberColumns());
		for (int i = 0; i < a.numberRows(); i++) {
			for (int j = 0; j < a.numberColumns(); j++) {
				a.setIndex(i, j, a.getIndex(i, j) * c);
			}
		}
		return a;
	}
	
/*	public boolean sameDimensions(Matrix a, Matrix b){
		if(a.numberRows() != b.numberRows() || a.numberColumns() != b.numberColumns()){
			return false;
		}
		return true;
	}
*/
	public Matrix add(Matrix m) {
		/*
		 * This method should add two matrices and return a new Matrix. Note
		 * that for an addition to be valid, the dimensions must agree (same
		 * number of rows and columns). Use the methods written in the previous
		 * step to test if the dimensions agree, else return null.
		 */
		if(this.numberRows() != m.numberRows() || this.numberColumns() != m.numberColumns()){
			return null;
		}
		Matrix a = new Matrix(this.numberRows(), this.numberColumns());
		for (int i = 0; i < a.numberRows(); i++) {
			for (int j = 0; j < a.numberColumns(); j++) {
				a.setIndex(i, j, this.getIndex(i, j) + m.getIndex(i, j));
			}
		}
		return a;
	}

	public Matrix subtract(Matrix m) {
		/*
		 * This method should subtract two matrices and return a new Matrix.
		 * Note that for a subtraction to be valid, the dimensions must agree
		 * (same number of rows and columns). Use the methods written in the
		 * previous step to test if the dimensions agree, else return null.
		 */
		if(this.numberRows() != m.numberRows() || this.numberColumns() != m.numberColumns()){
			return null;
		}
		Matrix a = new Matrix(this.numberRows(), this.numberColumns());
		for (int i = 0; i < a.numberRows(); i++) {
			for (int j = 0; j < a.numberColumns(); j++) {
				a.setIndex(i, j, this.getIndex(i, j) - m.getIndex(i, j));
			}
		}
		return a;
	}

	public Matrix multiply(Matrix m) {
		/*
		 * This method should multiply two matrices and return a new Matrix.
		 * Note that for the multplication to be valid, the number of columns of
		 * the first Matrix (the one calling multiply) must equal the rows of
		 * the Matrix being passed in (Matrix m). Use the methods written in the
		 * previous step to test if the dimensions agree, else return null.
		 */
		
		Matrix a = new Matrix(this.numberRows(), this.numberColumns());
		if(a.numberRows() != m.numberColumns() || a.numberColumns() != m.numberRows()){
			return null;
		}
		for (int i = 0; i < a.numberRows(); i++) {
			for (int j = 0; j < m.numberColumns(); j++) {
				for(int k = 0; k < a.numberColumns(); k++){
					 a.setIndex(i,j, a.getIndex(i,j) + this.getIndex(i,k) * m.getIndex(k, j));
				}
			}
		}
		return a;
	
	}
	
	public static void main(String[] args){
		Matrix a = new Matrix(2,2);
		Matrix b = new Matrix(2,2);
		Matrix tempA, tempB;	//temp copies of matrix a and b
		
		a.setIndex(0, 0, 2.0);
		a.setIndex(0, 1, 2.3);
		a.setIndex(1, 0, 5.2);
		a.setIndex(1, 1, 1.0);
		
		b.setIndex(0, 0, 0.0);
		b.setIndex(0, 1, 10.0);
		b.setIndex(1, 0, 13.3);
		b.setIndex(1, 1, 2.1);
		
		System.out.println("Matrix a is: ");
		a.print();
		System.out.println("Matrix b is: ");
		b.print();
		
		System.out.println("Testing Marix a * constant c, where c = 2.0 ");
		System.out.println("Expected: [4.0, 4.6; 10.4, 2.0;]");
		System.out.println("Actual: ");
		tempA = a.multiplyByConstant(2.0);
		tempA.print();
		tempA = a;			//reset tempA back to A
		
		System.out.println("Testing a + b: ");
		System.out.println("Expected: [2.0, 12.3; 18.5, 3.1;]");
		System.out.println("Actual: ");
		tempA = a.add(b);
		tempA.print();
		tempA = a;			
		
		System.out.println("Testing a - b: ");
		System.out.println("Expected: [-2.0, -8.7; -8.1, -1.1;]");
		System.out.println("Actual: ");
		tempA = a.subtract(b);
		tempA.print();
		tempA = a;			
		
		System.out.println("Testing a * b: ");
		System.out.println("Expected: [30.59, 24.83; 13.3, 54.1;]");
		System.out.println("Actual: ");
		tempA = a.multiply(b);
		tempA.print();
		tempA = a;			
	}
}
