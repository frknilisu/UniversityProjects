package q1;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class CellThread extends Thread {

	int x, y;
	int cell, next_cell;

	Population population;

	BinarySemaphore mutex;
	BinarySemaphore barier;

	// offsets of 8 neighbors
	int[][] neighbors = { { -1, 0 }, { +1, 0 }, { 0, -1 }, { 0, +1 }, { -1, -1 }, { -1, +1 }, { +1, -1 }, { +1, +1 } };

	public CellThread(Population population, int x, int y, BinarySemaphore mutex, BinarySemaphore barier) {
		this.population = population;
		this.x = x;
		this.y = y;
		this.mutex = mutex;
		this.barier = barier;
	}

	/*
	 * A 1 cell value stays 1 if exactly two or three neighbors are 1 valued. A 1
	 * cell value goes to 0 if less than two or greater than three neighbors are 1
	 * valued. A 0 cell value goes to 1 if exactly three neighbors are 1 valued. A 0
	 * cell value stays 0 if less than three or greater than three neighbors are 1
	 * valued.
	 * 
	 */
	public int checkRules(int one) {
		int new_cell = 0;
		if (cell == 1) {
			if (one == 2 | one == 3) // exactly two or three neighbors are 1 valued
				new_cell = 1;
			else
				new_cell = 0;
		} else {
			if (one == 3) // exactly three neighbors are 1 valued
				new_cell = 1;
			else
				new_cell = 0;
		}
		return new_cell;
	}

	// Count how many neighbors are 1 valued for given x and y coordinate on grid
	public int countOnes() {
		int ret = 0;
		for (int i = 0; i < 8; i++) {
			int xx = x + neighbors[i][0];
			int yy = y + neighbors[i][1];

			// check coordinates of the possible neighbor is in grid boundaries
			if (xx >= 0 && xx < population.M && yy >= 0 && yy < population.N) {
				ret += population.grid[xx][yy];
			}
		}
		return ret;

	}

	@Override
	public void run() {

		for (int i = 1; i <= Game.numberOfGeneration; i++) {
			
			this.cell = this.population.grid[x][y];

			int ones = countOnes();
			next_cell = checkRules(ones);

			mutex.P();
			population.count += 1;

			if (population.count == population.M * population.N) {
				// System.out.println("all thread done");
				population.count = 0;
				
				for(int c=0; c<population.M*population.N; c++) {
					CellThread cth = Game.cellThreads[c];
					population.grid[cth.x][cth.y] = cth.next_cell;
				}
				
				population.printGrid(population.grid);
				
				barier.V();
			}
			mutex.V();
			
			barier.P();
			
			barier.V();

		}

	}

}
