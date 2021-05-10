package q1;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class Population {

	public int M, N;
	//public List<List<Integer>> grid;
	//public List<List<Integer>> next_grid;
	public static int[][] grid, arr2;
	

	public int count = 0;
	public int count2 = 0;

	// Generate a Grid by random numbers with given M(rows) and N(columns)
	public Population(int _M, int _N) {
		this.M = _M;
		this.N = _N;
		grid = new int[M][N];
		generateRandom();
		//this.next_grid = new ArrayList<>(Collections.nCopies(M, new ArrayList<Integer>(Collections.nCopies(N, 0))));
	}

	// Generate a Grid with given M(rows), N(columns) and Grid from input file
	public Population(int _M, int _N, int[][] _grid) {
		this.M = _M;
		this.N = _N;
		this.grid = _grid;
		//this.next_grid = new ArrayList<>(Collections.nCopies(M, new ArrayList<Integer>(Collections.nCopies(N, 0))));
	}

	public String checkErrors() {
		// TODO Auto-generated method stub
		String msg = "";

		if (this.M <= 0 || this.N <= 0) {
			msg += "the size of the rectangular grid must be positive";
		}

		for (int i = 0; i < this.M; i++) {
			for (int j = 0; j < this.N; j++) {
				if (!(this.grid[i][j] == 0 | this.grid[i][j] == 1)) {
					msg += "cell values are not 0 or 1";
					break;
				}
			}
		}
		return msg;
	}

	// Generate random Grid
	public void generateRandom() {
		grid = new int[M][N];

		Random rand = new Random();

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				grid[i][j] = rand.nextInt(2);
			}
		}
	}

	// Print given grid to console
	public void printGrid(int[][] _grid) {
		if (_grid == null)
			_grid = grid;

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				System.out.print(_grid[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println("-----------------");
	}

}
