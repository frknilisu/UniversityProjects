Abdullah Furkan Ilisu
2013400201
CmpE436 - Assignment 1

Project contains 2 packages under the src:
	q1 package for Question 1 - Matrix Multiplication
	q2 package for Question 2 - Game of Life

q1:
	readFile() -> read input matrices from files via scanner
	writeFile() -> write multiplication result to file via FileWriter
	printMatrix() -> print given matrix to console
	checkErrors() -> check possible errors and return error message
	multiplyMatrices() -> multiply matrix1 and matrix2 then write to result
	
	give 3 arguments as inputs -> inputFile1 inputFile2 outputFile

q2:
	Game.java:
		this is main class
		readFile() -> read input grid from file via scanner

		give 3 or 4 arguments as inputs -> M(rows) N(columns) numberOfGeneration inputFile
		if 3 arguments given, grid is created with randoms numbers
		if 4 arguments given, grid is read from file
	
	Population.java:
		int[][] neighbors is offsets of 8 neighbors
		checkErrors() -> check cell values and size of grid
		generateRandom() -> generate grid with random numbers
		printGrid() -> print given grid to console
		checkRules() -> calculate new value of cell in next generation according to game rules
		nextGeneration() -> make next generation changes simultaneously
		countOnes() -> Count how many neighbors are 1 valued for given x and y coordinate on grid

	prints all generations to console
