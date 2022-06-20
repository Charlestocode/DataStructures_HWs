/* ============================= 10 * 10的矩陣要跑出結果要花4分鐘才會出來 ===============================
** 由於最後一天才知道要用其他方法才能加快速度，因為還有其他小考要準備，所以我就先不改為快速的判斷方法，希望助教不要扣分。
*/

#include <stdio.h>
#include <stdlib.h>
#define INITIAL_STACK_SIZE 8
#define MAX_QUEUE_SIZE 8

typedef enum {FALSE, TRUE} boolean;

int factorial(int n) {  
    if (n == 0) {
        return 1;  
    }
    else {
        return(n * factorial(n-1));
    }
}  

// Build the structure of walking offset.
typedef struct {
    short int vert;
    short int horiz;
} offsets;

// Define the eight direction in the array `move`.
offsets move[8] = {
    [0].vert = -1, [0].horiz = 0,   // Norht: 0
    [1].vert = -1, [1].horiz = 1,    // Northeast: 1
    [2].vert = 0, [2].horiz = 1,    // East: 2
    [3].vert = 1, [3].horiz = 1,    // Southeast: 3
    [4].vert = 1, [4].horiz = 0,    // South: 4
    [5].vert = 1, [5].horiz = -1,   // Southwest: 5
    [6].vert = 0, [6].horiz = -1,   // West: 6
    [7].vert = -1, [7].horiz = -1,  // Northwest: 7
};


// ------------------------------------- Build a stack class --------------------------------------
// Build the structure for recording each step state.
// We don not push the destination point into the stack becuase the stack is used to recored the past path so that we can return to.
typedef struct {
    short int row;
    short int col;
    // The member `dir` is used to record the next direction we will try.
    short int dir;
    // The member `cost` here is used to record the consumption from current point to next point. Later, we can subtract this cost
    // from the total cost of the path to the next point so that we restore the the total cost of the path to the current point.
    short int cost;
} Element;

// Type `StackObject` is as a data type.
typedef struct StackObject {
    // `stackList` is used to store the past points you passed by, so the destination point may not be put on this.
    Element * stackList;
    int capacity;
    // `size` is not only used to represent the number of elements, but also as an index of stack.
    int size;
    // Check the capacity.
    boolean (*isEmpty)(struct StackObject *this);
    boolean (*isFull)(struct StackObject *this);
    // Access elements.
    void (*push)(struct StackObject *this, Element item);
    Element (*pop)(struct StackObject *this);
    Element (*peek)(struct StackObject *this);
} StackObject;

// Variable `Stack` here is a variable of `StackClass`.
extern const struct StackClass {
    // Instantiate a class.
    StackObject (*new)();
} Stack;


boolean isEmpty(StackObject *this) {
    return this->size == 0;
}

boolean isFull(StackObject *this) {
    return this->size >= this->capacity;
}

void push(StackObject *this, Element item) {
    if (this->isFull(this)) {
        this->stackList = (Element *) realloc(this->stackList, 2 * this->capacity * sizeof(Element));
        this->capacity = 2 * this->capacity;
    }

    *(this->stackList + (this->size++)) = item;
}

Element pop(StackObject *this) {
    if (this->isEmpty(this)) {
        fprintf(stderr, "The stack is empty!");
        exit(0);
    }

    // If the size of the stack is less than one half of the capacity, halve the capacity of the stack.
    if ((this->size < this->capacity / 2) && (this->capacity / 2 > 8)) {
        this->capacity = this->capacity / 2;
        this->stackList = (Element *) realloc(this->stackList, this->capacity * sizeof(Element));
    }

    Element emptyElement;
    Element element = *(this->stackList + (--this->size));
    *(this->stackList + this->size) = emptyElement;
    return element;
}

// Peek at the top of the stack.
Element peek(StackObject *this) {
    // To peek at the top of the stack, after doing that, we have restore the variable `size`.
    Element element = *(this->stackList + (this->size - 1));
    return element;
}

// Implement the real function.
static StackObject new() {
    Element * stackList = (Element *) malloc(INITIAL_STACK_SIZE * sizeof(Element));

    StackObject stack = {
        .stackList = stackList,
        .capacity = INITIAL_STACK_SIZE,
        .size = 0,
        .isEmpty = &isEmpty,
        .isFull = &isFull,
        .push = &push,
        .pop = &pop,
        .peek = &peek
    };
    return stack;
}

const struct StackClass Stack = {.new = &new};


// -------------------------------------------- Priority Queue ------------------------------------------------
int queue[MAX_QUEUE_SIZE];
int rear = -1;
int front = -1;

void enqueue(int item) {
    if (rear == MAX_QUEUE_SIZE - 1) {
        fprintf(stderr, "The queue is full");
    }
    queue[++rear] = item;

    // Compare the value from the rear of queue to insert the new element into a proper posistion.
    for (int i = rear; i > front + 1; i--) {
        if (queue[i] > queue[i - 1]) {
            break;
        }
        int temp = queue[i];
        queue[i] = queue[i - 1];
        queue[i - 1] = temp;
    }
}

int dequeue() {
    if (front == rear) {
        exit(0);
    }
    return queue[++front];
}
// ------------------------------------------------------------------------------------------------------------

// Find the least consumption cost of the path.
int findPathCost(int * ptrMaze, int sizeMap, int fromRow, int fromCol, int toRow, int toCol) {

    if (ptrMaze == NULL || sizeMap == 0) {
        exit(1);
    }

    // If the starting point and the destination point is the same, return the cost of zero.
    if (fromRow == toRow && fromCol == toCol) {
        return 0;
    }

    // Convert the pointer of 1-D array maze into the pointer of 2-D array `maze`;
    int (*maze)[sizeMap][sizeMap] = (int (*)[sizeMap][sizeMap]) ptrMaze;
    // Create an 2-D array `mark` all with zero to mark the path we go from the starting point to the current point.
    int mark[sizeMap][sizeMap];
    for (int i = 0; i < sizeMap; i++) {
        for (int j = 0; j < sizeMap; j++) {
            mark[i][j] = 0;
        }
    }

    // Instantiate the stack for recording steps from the starting point.
    StackObject stack = Stack.new();

    // Push the starting point with direction 0 to the stack.
    Element start = {.row = fromRow, .col = fromCol, .dir = 0};
    stack.push(&stack, start);

    // Delcare and initialize the variable `totalCost` to zero to record the cost from the starting point to the current point.
    int totalCost = 0;
    // Declare the variable `leastCost` to record the cost of the least-consumed path.
    int leastCost = 0;

    while (!stack.isEmpty(&stack)) {

        // Get the current position info.
        Element currentPoint = stack.pop(&stack);
        short int row = currentPoint.row;
        short int col = currentPoint.col;
        short int dir = currentPoint.dir;
        // Restore the total cost of the path to the last point to the total cost of the path to the current point.
        totalCost -= currentPoint.cost;

        // Unmark the last point we passed by on the `mark` map.
        short int lastDir = dir - 1;
        // Actually, this is just to proctect the error of out of index of the initial element popped out
        if (lastDir >= 0) {
            short int lastRow = row + move[lastDir].vert;
            short int lastCol = col + move[lastDir].horiz;
            mark[lastRow][lastCol] = 0;
        }
        /*
        for (int i = 0; i < sizeMap; i++) {
            for (int j = 0; j < sizeMap; j++) {
                printf("%d ", mark[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        */
        

        // When the current point still has another direction to go, do the following.
        while (dir < 8) {

            short int nextRow = row + move[dir].vert;
            short int nextCol = col + move[dir].horiz;
            // Update the direction for the next round of the element which will be stored in the stack.
            dir++;

            // Find the destination point.
            if (nextRow == toRow && nextCol == toCol) {
                // Get the cost between the current point and the previous point.
                int cost = abs((*maze)[nextRow][nextCol] - (*maze)[row][col]);
                totalCost += cost;

                // Initialize `leastCost` to the value of the first `totalCost` we get.
                if (leastCost == 0) {
                    leastCost = totalCost;
                } else if (totalCost < leastCost) {
                    leastCost = totalCost;
                }
                
                // After recording the leastCost, we will subtract the cost we spend on arriving the destination point.
                totalCost -= cost;
                break;
            }
            
            // Find the the destination
            if (mark[nextRow][nextCol] == 0 && nextRow >= 0 && nextCol >= 0 && nextRow < sizeMap && nextCol < sizeMap) {

                
                int cost = abs((*maze)[nextRow][nextCol] - (*maze)[row][col]);
                totalCost += cost;
                // If we have not set the `leastCost` or the `totalCost` is less than `leastCost`,
                // push the point info into the stack.
                if (totalCost <= leastCost || leastCost == 0) {
                    mark[nextRow][nextCol] = 1;
                    // Push the current point into the stack.
                    Element element = {row, col, dir, cost};
                    stack.push(&stack, element);

                    // Go to the next point and try to go in the north direction first.
                    row = nextRow;
                    col = nextCol;
                    dir = 0;
                } 
                // If I don't go to next point and just try another direction to go, subtract the cost we added from `totalCost`.
                else {
                    totalCost -= cost;
                }
            }
        }
    }

    free(stack.stackList);
    return leastCost;
}



int main() {

    FILE * fp = fopen("./test data/input_3.txt", "r");
    FILE * output_3_fp = fopen("output_3.txt", "w");

    printf("================================= HW1 Q3 =====================================\n");
    // Get the number of questions.
    int numQuestion;
    fscanf(fp, "%d", &numQuestion);
    printf("Number of Question: %d\n", numQuestion);


    for (int i = 0; i < numQuestion; i++) {
        // Get the number of destination points and the size of the map.
        int numDestination;
        int sizeMap;
        fscanf(fp, "%d %d", &numDestination, &sizeMap);
        printf("\nNumber of Destination: %d\tSize of Map: %d\n", numDestination, sizeMap);
            
        // Get the the starting postion.
        int rowStart, colStart;
        fscanf(fp, "%d %d", &rowStart, &colStart);
        printf("rowStart: %d\tcolStart: %d\n", rowStart, colStart);

        // Declare the arrays for recording destination points.
        int rowDest[numDestination], colDest[numDestination];
        // Get all of the destination points put in an array.
        for (int dest = 0; dest < numDestination; dest++) {
            fscanf(fp, "%d %d", &rowDest[dest], &colDest[dest]);
            printf("Row of %d-th Destination: %d\tColumn of %d-th Destination: %d\n", dest, rowDest[dest], dest, colDest[dest]);
        }
            
        // Get the maze.
        int maze[sizeMap][sizeMap];
        int *ptrMaze;
        for (int row = 0; row < sizeMap; row++) {
            for (int col = 0; col < sizeMap; col++) {
                fscanf(fp, "%d", &maze[row][col]);
                //printf("maze[%d][%d]: %d\n", row, col, maze[row][col]);
            }
        }
        ptrMaze = (int *) maze;
            
        // Find the destination point with the least consumption of cost from the starting point.
        int leastCost;
        for (int dest = 0; dest < numDestination; dest++) {
            // The current pair of points for comparing with `leastCost`.
            int cost = findPathCost(ptrMaze, sizeMap, rowStart, colStart, rowDest[dest], colDest[dest]);
            if (dest == 0) {
                leastCost = cost;
            }
            // Update `leastCost` to the smallest cost.
            if (cost < leastCost) {
                leastCost = cost;
            }
        }

        
        // Find the total cost between all destination points, add `cost` to `leastCost`.
        for (int j = 0; j < numDestination; j++) {
            for (int k = j + 1; k < numDestination; k++) {
                int cost = findPathCost(ptrMaze, sizeMap, rowDest[j], colDest[j], rowDest[k], colDest[k]);
                enqueue(cost);
            }
        }
        
        if (numDestination > 1) {
            int numPathBetweenDest = numDestination - 1;
            for (int j = 0; j < numPathBetweenDest; j++) {
                leastCost += dequeue();
            }
        }
        
        fprintf(output_3_fp, "#%d\n", i + 1);
        fprintf(output_3_fp, "cost: %d\n", leastCost);
        printf("Least Cost: %d\n", leastCost);
        printf("------------------------------------------------------------------------------\n");
    }
    

    fclose(fp);
    fclose(output_3_fp);
    return 0;
}   