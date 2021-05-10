/**
 * 
 */
package q1;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * @author frkn
 *
 * Abdullah Furkan Ilisu
 * 2013400201
 * abdullahfurkan71@gmail.com
 * CMPE436-Assignment 1
 *
 */
public class MatrixMultiplication {

	static Scanner scanner = null;

	static int R1, C1, R2, C2;
	static List<List<Integer>> matrix1, matrix2;

	static int Rr, Cr;
	static List<List<Integer>> result;

	/*
	 * Read file with scanner
	 * according to param no, read related file to related matrix
	 * */ 
	public static void readFile(String path, int no) throws FileNotFoundException {

		File file = new File(path);
		scanner = new Scanner(file);

		int R = scanner.nextInt();
		int C = scanner.nextInt();

		List<List<Integer>> matrix = new ArrayList<>();
		List<Integer> list = new ArrayList<>();

		for (int i = 0; i < R; i++) {
			list.clear();
			for (int j = 0; j < C; j++) {
				list.add(scanner.nextInt());
			}
			matrix.add(new ArrayList<>(list));
		}

		switch (no) {
		case 1:
			R1 = R;
			C1 = C;
			matrix1 = new ArrayList<>(matrix);
			break;
		case 2:
			R2 = R;
			C2 = C;
			matrix2 = new ArrayList<>(matrix);
		default:
			break;
		}

	}

	public static void writeFile(String path) throws IOException {
		File file = new File(path);

		// creates the file
		file.createNewFile();

		// creates a FileWriter Object
		FileWriter writer = new FileWriter(file);

		// Writes the content to the file
		writer.write(Rr + " " + Cr + "\n");

		for (int i = 0; i < Rr; i++) {
			for (int j = 0; j < Cr; j++) {
				writer.write(result.get(i).get(j) + "\t");
			}
			writer.write("\n");
		}
		writer.flush();
		writer.close();
	}

	public static void printMatrix(List<List<Integer>> matrix, int R, int C) {
		System.out.println(R + " " + C);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				System.out.print(matrix.get(i).get(j) + " ");
			}
			System.out.println();
		}
		System.out.println();
	}

	public static String checkErrors(int R1, int C1, int R2, int C2) {

		String message = "";

		if (R1 <= 0 | C1 <= 0 | R2 <= 0 | C2 <= 0)
			message += "dimensions cannot be less than 0 or equal to 0 => undefined matrix";

		if (C1 != R2)
			message += "dimensions are not compatible";

		return message;
	}

	public static void multiplyMatrices() {
		Rr = R1;
		Cr = C2;

		result = new ArrayList<>();

		int v = 0;
		List<Integer> list = new ArrayList<>();

		for (int i = 0; i < R1; i++) {
			list.clear();
			for (int j = 0; j < C2; j++) {
				v = 0;
				for (int k = 0; k < C1; k++) {
					v += matrix1.get(i).get(k) * matrix2.get(k).get(j);
				}
				list.add(v);
			}
			result.add(new ArrayList<>(list));
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			readFile(args[0], 1);
			readFile(args[1], 2);

			// printMatrix(matrix1, R1, C1);
			// printMatrix(matrix2, R2, C2);

			String msg = checkErrors(R1, C1, R2, C2);

			if (msg != "") {
				System.out.println(msg);
			} else {
				multiplyMatrices();
				printMatrix(result, Rr, Cr);
				writeFile(args[2]);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
