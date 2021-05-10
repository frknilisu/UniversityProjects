package q2;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Population {

	int M, N;
	List<List<Integer>> grid, new_grid;
	
	// offsets of 8 neighbors
	int[][] neighbors = { { -1, 0 }, { +1, 0 }, { 0, -1 }, { 0, +1 }, { -1, -1 }, { -1, +1 }, { +1, -1 }, { +1, +1 } };

	// Generate a Grid by random numbers with given M(rows) and N(columns) 
	public Population(int _M, int _N) {
		this.M = _M;
		this.N = _N;
		generateRandom();
	}


	// Generate a Grid with given M(rows), N(columns) and Grid from input file
	public Population(int _M, int _N, List<List<Integer>> _grid) {
		this.M = _M;
		this.N = _N;
		this.grid = new ArrayList<>(_grid);
	}
	
	public String checkErrors() {
		// TODO Auto-generated method stub
		String msg = "";
		
		if(this.M <= 0 || this.N <= 0) {
			msg += "the size of the rectangular grid must be positive";
		}
		
		for(int i = 0;i<this.M;i++) {
			for(int j=0;j<this.N;j++) {
				if(!(this.grid.get(i).get(j) == 0 | this.grid.get(i).get(j) == 1)) {
					msg += "cell values are not 0 or 1";
					break;
				}
			}
		}
		return msg;
	}

	// Generate random Grid
	public void generateRandom() {
		grid = new ArrayList<>();
		List<Integer> list = new ArrayList<>();

		Random rand = new Random();

		for (int i = 0; i < M; i++) {
			list.clear();
			for (int j = 0; j < N; j++) {
				list.add(rand.nextInt(2));
			}
			grid.add(new ArrayList<>(list));
		}
	}

	// Print given grid to console
	public void printGrid(List<List<Integer>> _grid) {
		if (_grid == null)
			_grid = grid;

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				System.out.print(_grid.get(i).get(j) + " ");
			}
			System.out.println();
		}
		System.out.println("-----------------");
	}

	
	/*
	 * A 1 cell value stays 1 if exactly two or three neighbors are 1 valued.
	 * A 1 cell value goes to 0 if less than two or greater than three neighbors are 1 valued.
	 * A 0 cell value goes to 1 if exactly three neighbors are 1 valued.
	 * A 0 cell value stays 0 if less than three or greater than three neighbors are 1 valued. 
	 * 
	 * */
	public int checkRules(int cell, int one) {
		int new_cell = 0;
		if (cell == 1) {
			if (one == 2 | one == 3)	// exactly two or three neighbors are 1 valued
				new_cell = 1;
			else
				new_cell = 0;
		} else {
			if (one == 3)	// exactly three neighbors are 1 valued
				new_cell = 1;
			else
				new_cell = 0;
		}
		return new_cell;
	}

	/*
	 *  calculate new generation values
	 *  write it to new_grid in order to make the changes simultaneously
	 *  then copy new_grid to grid 
	 */
	public void nextGeneration() {
		new_grid = new ArrayList<>();
		List<Integer> list = new ArrayList<>();

		for (int i = 0; i < M; i++) {
			list.clear();
			for (int j = 0; j < N; j++) {
				int ones = countOnes(i, j);
				list.add(checkRules(grid.get(i).get(j), ones));
				// System.out.println(i + " " + j + " -> " + ones);
			}
			new_grid.add(new ArrayList<>(list));
		}
		
		// printGrid(grid);
		grid = new ArrayList<>(new_grid);
		printGrid(grid);
	}

	// Count how many neighbors are 1 valued for given x and y coordinate on grid
	public int countOnes(int x, int y) {
		int ret = 0;
		for (int i = 0; i < 8; i++) {
			int xx = x + neighbors[i][0];
			int yy = y + neighbors[i][1];

			// check coordinates of the possible neighbor is in grid boundaries
			if (xx >= 0 && xx < M && yy >= 0 && yy < N) {
				ret += grid.get(xx).get(yy);
			}
		}
		return ret;
	}
}
