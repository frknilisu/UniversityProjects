package q2;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * @author frkn
 *
 *         Abdullah Furkan Ilisu 
 *         2013400201 
 *         abdullahfurkan71@gmail.com
 *         CMPE436-Assignment 1
 *
 */
public class Game {

	static Scanner scanner = null;

	static int M, N, numberOfGeneration;
	static List<List<Integer>> grid;

	static Population population = null;

	// Read file with scanner
	public static void readFile(String path) throws FileNotFoundException {

		File file = new File(path);
		scanner = new Scanner(file);

		grid = new ArrayList<>();
		List<Integer> list = new ArrayList<>();

		for (int i = 0; i < M; i++) {
			list.clear();
			for (int j = 0; j < N; j++) {
				list.add(scanner.nextInt());
			}
			grid.add(new ArrayList<>(list));
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		M = Integer.parseInt(args[0]);
		N = Integer.parseInt(args[1]);
		numberOfGeneration = Integer.parseInt(args[2]);
		if (args.length == 3) { // input data is M, N and maxGenerations
			population = new Population(M, N); // then initialize the grid to random values (0 or 1)
		} else if (args.length == 4) { // input data is M, N, maxGenerations and a file name (input.txt)
			try {
				readFile(args[3]); // then you may read initial values of the cells from the file
				population = new Population(M, N, grid);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		String errorMsg = population.checkErrors(); // check for illegal input values
		if (errorMsg == "") {
			System.out.println("Initial Population:");
			population.printGrid(null);
			for (int i = 1; i <= numberOfGeneration; i++) {
				System.out.println((i + 1) + ". Generation:");
				population.nextGeneration(); // go to next generation
			}
		} else {
			System.out.println("Error: " + errorMsg);
		}
	}

}
