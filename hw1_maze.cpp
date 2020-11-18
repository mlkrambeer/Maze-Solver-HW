//CS215 Homework #1 - Recursive maze solver, due by Wed Sept 16 at 8:40 am
//Fall 2020
//Matt Krambeer

#include <iostream>
#include <fstream>
using namespace std;

const int MAX_ROWS = 10;
const int MAX_COLS = 10;
typedef char MazeType[MAX_ROWS][MAX_COLS];
typedef bool TrackType[MAX_ROWS][MAX_COLS];
typedef int DistType[MAX_ROWS][MAX_COLS];

bool findPath(int, int, int, int, int, MazeType, TrackType, DistType);
char convertStep(int);
void printMaze(int, int, MazeType);
void erasePath(int, int, MazeType);


int main(){
	ifstream inFile;
	int rows, cols;
	MazeType maze;  //holds the maze
	TrackType visited;  //used to track whether or not a cell has been visited already
	DistType shortPaths; //used to track shortest distance to cell (x,y) which has been found so far

	inFile.open("maze.in");
	inFile >> rows;
	inFile >> cols;

	//initialize all the arrays
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			inFile >> maze[i][j];  
			visited[i][j] = false; 
			shortPaths[i][j] = INT_MAX;
		}
	}
	inFile.close();

	//findPath will only return false if there is no possible solution to the maze
	if (!findPath(rows, cols, 0, 0, 0, maze, visited, shortPaths))
		cout << "No solution found!\n";
	
	//print the maze whether or not a path is found
	printMaze(rows, cols, maze);

	return 0;
}


/*rows and cols are the size of the maze; x and y are the coordinates of the current cell being checked.
* step keeps track of how many steps it took to get to the current cell.
* maze is a 2D character array which defines the maze. 
* visited is a 2D boolean array which keeps track of which cells have already been visited.
* shorthPaths is an int array which keeps track of the length of the shortest path found to each cell in the maze.
* findPath returns false if no solution to the maze is found; returns true otherwise.
* if a path is found, this function modifies the given maze array by drawing the path using lower-case letters.
*/
bool findPath(int rows, int cols, int x, int y, int step, MazeType maze, TrackType visited, DistType shortPaths) {

	//if new shortest path to this cell is found, store its length
	//if not, we don't need to keep checking this path
	if (step < shortPaths[x][y])
		shortPaths[x][y] = step;
	else
		return false;

	//base case - hit a wall, or path looped back on itself; this is not a valid path
	if (maze[x][y] == '*' || visited[x][y])
		return false;

	//base case - the exit
	if(x == rows - 1 && y == cols - 1) {
		erasePath(rows, cols, maze);  //any previously found paths are longer, so we can erase them and then draw the newly found path
		maze[x][y] = convertStep(step);
		return true;
	}

	
	visited[x][y] = true; //Mark this cell as visited while making the recursive calls

	bool pathFound = false; //keeps track of whether a path is found among the 4 subsequent recursive calls


	//check down, if it's a valid move
	if (x < rows - 1 && findPath(rows, cols, x + 1, y, step + 1, maze, visited, shortPaths)) {
		
		//a new shortest path was found, so draw this step
		maze[x][y] = convertStep(step);
		pathFound = true;

		//if shortest POSSIBLE path was found, we can stop checking other paths
		if (shortPaths[rows - 1][cols - 1] == rows + cols - 2)
			return true;
	}

	//check right
	if (y < cols - 1 && findPath(rows, cols, x, y + 1, step + 1, maze, visited, shortPaths)) {
		maze[x][y] = convertStep(step);
		pathFound = true;
		if (shortPaths[rows - 1][cols - 1] == rows + cols - 2)
			return true;
	}

	//check up
	if (x > 0 && findPath(rows, cols, x - 1, y, step + 1, maze, visited, shortPaths)) {
		maze[x][y] = convertStep(step);
		pathFound = true;
		if (shortPaths[rows - 1][cols - 1] == rows + cols - 2)
			return true;
	}

	//check left
	if (y > 0 && findPath(rows, cols, x, y - 1, step + 1, maze, visited, shortPaths)) {
		maze[x][y] = convertStep(step);
		pathFound = true;
		if (shortPaths[rows - 1][cols - 1] == rows + cols - 2)
			return true;
	}

	/*Change visited status back to false before popping off of the stack.
	* This will allow for recursive calls with lower step count to check for
	* other paths through this cell.
	*/
	visited[x][y] = false;

	return pathFound;  //this will only be false if no valid paths are found in any of the 4 directions from this cell
}


/*This function converts the step variable into a character to be printed in the maze path.
* The remainder of the step variable mod 26 ensures that 'z' loops back around to 'a'.
*/
char convertStep(int step) {
	step = step % 26;
	return 'a' + step;
}


/*Prints the contents of the given maze array to the screen.
* Requires the # of rows and columns so that it doesn't print too much.
*/
void printMaze(int rows, int cols, MazeType maze) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << maze[i][j];
		}
		cout << "\n";
	}
}

/*This function should be used to erase a previous path.
* It finds cells which are not '.' or '*' and are therefore letters drawn for a path,
* and changes them back to '.'
*/
void erasePath(int rows, int cols, MazeType maze) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (!(maze[i][j] == '.' || maze[i][j] == '*')) {
				maze[i][j] = '.';
			}
		}
	}
}
