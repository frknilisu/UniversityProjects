package q1;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Game {
	
	private static Scanner scanner = null;

	private static int M, N;
	public static int numberOfGeneration;

	private static int[][] grid;

	private static Population population = null;
	public static CellThread[] cellThreads = null;

	// Read file with scanner
	public static void readFile(String path) throws FileNotFoundException {

		File file = new File(path);
		scanner = new Scanner(file);

		grid = new int[M][N];

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				grid[i][j] = scanner.nextInt();
			}
		}
	}
	
	public static void main(String[] args) {
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

			BinarySemaphore mutex = new BinarySemaphore(true);
			BinarySemaphore barier = new BinarySemaphore(false);

			cellThreads = new CellThread[M*N];
			
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					cellThreads[j+i*N] = new CellThread(population, i, j, mutex, barier);
					cellThreads[j+i*N].start();
				}
			}

		} else {
			System.out.println("Error: " + errorMsg);
		}

	}

}
