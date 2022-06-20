#include <stdio.h>
#include <stdlib.h>
#define INITIAL_TERM_SIZE 8

// For recording non-zero terms in sparse matrix.
typedef struct {
    int row;
    int col;
    int numAliveNeighbors;
} term;

const int numDirection = 8;

typedef struct {
    short int vert;
    short int horiz;
} neighbors;

neighbors neighbor[8] = {
    [0].vert = -1, [0].horiz = 0,   // Top: 0
    [1].vert = -1, [1].horiz = 1,    // Top right: 1
    [2].vert = 0, [2].horiz = 1,    // Right: 2
    [3].vert = 1, [3].horiz = 1,    // Bottom right: 3
    [4].vert = 1, [4].horiz = 0,    // Bottom: 4
    [5].vert = 1, [5].horiz = -1,   // Bottom left: 5
    [6].vert = 0, [6].horiz = -1,   // Left: 6
    [7].vert = -1, [7].horiz = -1,  // Top left: 7
};


void countAliveNeighbor(int * numAliveNeighbors, int * ptrOldCells, int sizeArray, int currentRow, int currentCol, int dir) {

    int (*oldCells)[sizeArray][sizeArray] = (int (*)[sizeArray][sizeArray]) ptrOldCells;

    // Get the row index and colomn index of the current adjacent cell we attempt to find.
    int nextRow = currentRow + neighbor[dir].vert;
    int nextCol = currentCol + neighbor[dir].horiz;

    // When the neighbor exist, determine whether the adjacent is alive.
    if (nextRow >= 0 && nextRow < sizeArray && nextCol >= 0 && nextCol < sizeArray) {
        // Get the value of the adjacent cell.
        int adjacentCell = (*oldCells)[nextRow][nextCol];

        if (adjacentCell == 1) {
            ++(*numAliveNeighbors);
        }
    }
}


term * evolve(int * ptrCells, int sizeArray, int numGeneration) {

    // Get the old cell matrix.
    int (*oldCells)[sizeArray][sizeArray] = (int (*)[sizeArray][sizeArray]) ptrCells;

    // Declare the new cell matrix and its pointer.
    int newCells[sizeArray][sizeArray];

    // Declare the initial `newCellTerms` as 8 times the size of type `term` for 
    // returning the sparse new cell matrix in terms' form.
    int termSize = INITIAL_TERM_SIZE;
    // indexTerm 0 for recording the term info.
    int indexTerm = 1;
    term * newCellTerms = (term *) malloc(INITIAL_TERM_SIZE * sizeof(term));

    // Initialize the number of final alive cells to zero.
    int numAliveCells = 0;
       
    for (int i = 0; i <= numGeneration; i++) {
        
        // Declare `ptrOldCells` as an argument of `countAliveNeighbor()`
        int * ptrOldCells = (int *) oldCells;

        // For each cell, we calculate the number of alive cells at the neighbor of each cell.
        for (int row = 0; row < sizeArray; row++) {
            for (int col = 0; col < sizeArray; col++) {

                int oldCell = (*oldCells)[row][col];
                int numAliveNeighbors = 0;

                // Determine if the current cell is alive.
                if (oldCell == 1) {

                    // Count the number of alive adjacent cells.
                    for (int dir = 0; dir < numDirection; dir++) {
                        countAliveNeighbor(&numAliveNeighbors, ptrOldCells, sizeArray, row, col, dir);
                    }

                    // After `numGeneration` evolution, we record the number of alive adjacent cells of the current alive cell
                    // to build the new cell term for the sparse matrix.
                    if (i == numGeneration) {
                        // If the array of type `term` is full, then reallocate 2 times the previous size of term.
                        if (indexTerm >= termSize) {
                            newCellTerms = (term *) realloc(newCellTerms, 2 * termSize * sizeof(term));
                            termSize = 2 * termSize;
                        }

                        (newCellTerms + indexTerm)->row = row;
                        (newCellTerms + indexTerm)->col = col;
                        (newCellTerms + (indexTerm++))->numAliveNeighbors = numAliveNeighbors;
                        ++numAliveCells;
                        continue;
                    }

                    // Die
                    if (numAliveNeighbors == 0 || numAliveNeighbors == 1) {
                        newCells[row][col] = 0;
                    }
                    // Survive
                    else if (numAliveNeighbors == 2 || numAliveNeighbors == 3) {
                        newCells[row][col] = 1;
                    }
                    // Die
                    else {
                        newCells[row][col] = 0;
                    }
                }
                // The current cell is dead.
                else {

                    // Count the number of alive adjacent cells.
                    for (int dir = 0; dir < numDirection; dir++) {
                        countAliveNeighbor(&numAliveNeighbors, ptrOldCells, sizeArray, row, col, dir);
                    }

                    // Revive
                    if (numAliveNeighbors == 3) {
                        newCells[row][col] = 1;
                    }
                    // Still dead
                    else {
                        newCells[row][col] = 0;
                    }
                }
            }
        }

        // Befor `numGeneration` evolutions, copy the newCells matrix to the oldCells 
        // for the next round determination.
        if (i != numGeneration) {
            for (int row = 0; row < sizeArray; row++) {
                for (int col = 0; col < sizeArray; col++) {
                    (*oldCells)[row][col] = newCells[row][col];
                }
            }
        }
    }
    
    // Assign the number of rows, the number of columns and the number of alive cells to the first term.
    (newCellTerms)->row = sizeArray;
    (newCellTerms)->col = sizeArray;
    (newCellTerms)->numAliveNeighbors = numAliveCells;
    
    return newCellTerms;
}



int main() {

    FILE * fp = fopen("./test data/input_2.txt", "r");
    FILE * output_2_fp = fopen("output_2.txt", "w");

    int numQuestion;
    char buffer[256];
    int sizeArray;
    int numGeneration;
    
    printf("===================== HW1 Q2 =====================\n");
    // Find the number of questions.
    numQuestion = fscanf(fp, "%d", &numQuestion);
    printf("numQuestion: %d\n", numQuestion);
    // Finish reading the first line.
    fgets(buffer, 256, fp);
    
    for (int i = 0; i < 3; i++) {
        // Get the size of an array, N, and the number of generations, G.
        fscanf(fp, "%d %d", &sizeArray, &numGeneration);
        printf("Size of Array: %d\tNumber of Generation: %d\n", sizeArray, numGeneration);
        fgets(buffer, 256, fp);

        // Declare the cell matrix.
        int cells[sizeArray][sizeArray];
        int * ptrCells;
        term * newCellTerms;

        // Build the cell matrix.
        for (int row = 0; row < sizeArray; row++) {
            for (int col = 0; col < sizeArray; col++) {
                int num;
                fscanf(fp, "%d", &num);
                cells[row][col] = num;
            }
        }

        ptrCells = (int *) cells;
        newCellTerms = evolve(ptrCells, sizeArray, numGeneration);

        printf("row\tcol\tvalue\n");
        int numAliveCells = newCellTerms->numAliveNeighbors;
        for (int j = 0; j <= numAliveCells; j++) {
            int currentRow = (newCellTerms + j)->row;
            int currentCol = (newCellTerms + j)->col;
            int numAliveNeighbors = (newCellTerms + j)->numAliveNeighbors;
            fprintf(output_2_fp, "%d %d %d\n", currentRow, currentCol, numAliveNeighbors);
            printf("%d\t%d\t%d\n", currentRow, currentCol, numAliveNeighbors);
        }
        
        printf("--------------------------------------------------\n");
        free(newCellTerms);
    }
    
    return 0;
}