#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a, b) (a >= b ? a : b)
#define MIN(a, b) (a <= b ? a : b)
#define STOVE_PROCESS_TIME 5
#define CUTTING_PROCESS_TIME 3

typedef enum {FALSE, TRUE} bool;
typedef enum {None = -1, Stove, Cutting} ingred_t;

/* -------------------------------------------- Food Structure --------------------------------------------- */

/* Build a recipe structure to repeat using the same recipe for different order. */
typedef struct {
    char foodName[20];
    int numStove;
    int numCut;
    char ** stoveIngredList;
    char ** cutIngredList;
    int foodPresentTime;
} Recipe;


/* -------------------------------------------- Order Structure ---------------------------------------------- */
typedef struct {
    // Order info
    int orderID;
    int arrival;
    int deadline;
    // Unprocessed ingredient info
    int numStove;
    int numCut;
    // Food presentation time
    int foodPresentTime;
    
    // Profit info
    int earn;
    int punishment;
} Order;


/* ------------------------------------------- Schedule Structure -------------------------------------------- */
typedef struct {
    //int playerID;
    bool occupied;
    ingred_t ingredType;
} Time;


typedef struct timePoint * timePointPtr;
typedef struct timePoint {
    int time;
    timePointPtr next;
} timePoint;


/* Create a schedule info structure to output them in later. */
typedef struct {
    int playerID;
    int start;
    int orderID;
    char command;
    char ingredName[20];
} scheduleInfo;


/* ----------------------------- Create a Singly Linked List for timePointList -------------------------------- */

// Create a singly linked list and reserve the first node of the singly linked list.
timePointPtr createTimePointList() {

    timePointPtr front = (timePointPtr) malloc(sizeof(timePoint));
    if (front == NULL) {
        fprintf(stderr, "Error of creating the linked list.");
    }

    front->next = NULL;
    return front;
}

// Push the node into the back of the singly linked list
void pushTimePoint(timePointPtr timePointList, int time) {

    if (timePointList == NULL) {
        fprintf(stderr, "The linked list is empty!");
        exit(1);
    }

    // Create a new node for storing the new element.
    timePointPtr newNode = (timePointPtr) malloc(sizeof(timePoint));
    newNode->time = time;
    newNode->next = NULL;

    // Find the back node of the singly linked list.
    timePointPtr currentNode = timePointList;
    for (; currentNode->next; currentNode = currentNode->next);

    currentNode->next = newNode;
}


// Peek the time point at specified `index`. 
int peekTimePointAt(timePointPtr timePointList, int index) {

    timePointPtr currentNode = timePointList;
    currentNode = currentNode->next;
    for (int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }

    return currentNode->time;
}


// Increment or decrement the specific `index` of time point by specific value.
void shiftOffsetAt(timePointPtr timePointList, int index, int offset) {

    timePointPtr currentNode = timePointList;
    currentNode = currentNode->next;
    for (int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }

    currentNode->time = currentNode->time + offset;
}


// Clear the whole singly list.
void clear(timePointPtr timePointList) {

    timePointPtr prevNode;
    while (timePointList != NULL) {
        prevNode = timePointList;
        timePointList = timePointList->next;
        free(prevNode);
    }
}

/* ------------------------------------------------------------------------------------------------------------- */

// Sort a priority queue for orders.
Order * sortOrders(Order orders[], int numOrders) {

    int i, j;
    double profitRate;
    Order insertedOrder;

    for (int i = 1; i < numOrders; i++) {

        insertedOrder = orders[i];
        profitRate = (double) (orders[i].earn - orders[i].punishment) / (double) (orders[i].deadline - orders[i].arrival);
        printf("earn: %d, punishment: %d, deadline: %d, arrival: %d", orders[i].earn, orders[i].punishment, orders[i].deadline, orders[i].arrival);
        printf("Proftit Rate: %f\n", profitRate);
        j = i - 1;

        double comparedProfitRate = (double) (orders[j].earn - orders[j].punishment) / (double) (orders[j].deadline - orders[j].arrival);
        while (j >= 0 && profitRate > comparedProfitRate) {

            orders[j + 1] = orders[j];

            j = j - 1;
            comparedProfitRate = (double) (orders[j].earn - orders[j].punishment) / (double) (orders[j].deadline - orders[j].arrival);
        }
        orders[j + 1] = insertedOrder;
    }
    
    return orders;
}


/* Allocate time to different players to process the ingredient where we can switch players to process the same ingredient. */
void scheduleIngredProcessing(Time * player1, Time * player2, Order orders[], int numOrders, Recipe recipes[], scheduleInfo * schedules) {

    // Declare a schedule info array with a size of 0.
    int numSchedule = 0;
    schedules = (scheduleInfo *) malloc(sizeof(scheduleInfo));

    for (int i = 0; i < numOrders; i++) {

        // Record the arrival and deadline of the order.
        bool orderFail = FALSE;
        int arrival = orders[i].arrival;
        int deadline = orders[i].deadline;
        // Record the start, end and available period of non-occupied time block.
        int numBlock_1 = 0;
        int numBlock_2 = 0;
        timePointPtr wholeStartTime_1 = createTimePointList();
        timePointPtr wholeStartTime_2 = createTimePointList();
        timePointPtr wholeStoveEndTime_1 = createTimePointList();
        timePointPtr wholeStoveEndTime_2 = createTimePointList();
        timePointPtr wholeCutEndTime_1 = createTimePointList();
        timePointPtr wholeCutEndTime_2 = createTimePointList();
        timePointPtr wholeAvailStovePeriod_1 = createTimePointList();
        timePointPtr wholeAvailStovePeriod_2 = createTimePointList();
        timePointPtr wholeAvailCutPeriod_1 = createTimePointList();
        timePointPtr wholeAvailCutPeriod_2 = createTimePointList();
        

        // The various process times of the food.
        int longerProcessTime, shorterProcessTime;
        int stoveProcessTime = orders[i].numStove * STOVE_PROCESS_TIME;
        int cutProcessTime = orders[i].numCut * CUTTING_PROCESS_TIME;
        int foodPresentTime = orders[i].foodPresentTime;
        // Find the longer and shorter process time of an order.
        longerProcessTime = MAX(stoveProcessTime, cutProcessTime);
        shorterProcessTime = MIN(stoveProcessTime, cutProcessTime);

        // Record how many times of the specific ingredient process type one can process.
        int availNumStove_1 = 0;
        int availNumCut_1 = 0;
        int availNumStove_2 = 0;
        int availNumCut_2 = 0;
        int lastStoveEndTime_1;
        int lastCutEndTime_1;
        int lastStoveEndTime_2;
        int lastCutEndTime_2;

        // Find every block of available time in player 1.
        for (int currentTime_1 = deadline; currentTime_1 >= arrival; ) {

            for (; player1[currentTime_1].occupied == TRUE && currentTime_1 >= arrival; currentTime_1--);
            if (currentTime_1 >= arrival) {

                // Find the available end time.
                pushTimePoint(wholeStoveEndTime_1, currentTime_1);
                pushTimePoint(wholeCutEndTime_1, currentTime_1);
                // Find how many time units the the empty time block has.
                for (; player1[currentTime_1].occupied == FALSE; currentTime_1--) {
                    shiftOffsetAt(wholeAvailStovePeriod_1, numBlock_1, 1);
                    shiftOffsetAt(wholeAvailCutPeriod_1, numBlock_1, 1);
                }
                // Get the start of non-occupied time block.
                int startTime_1 = peekTimePointAt(wholeStoveEndTime_1, numBlock_1) - peekTimePointAt(wholeAvailStovePeriod_1, numBlock_1) + 1;

                numBlock_1++;
            }
        }

        // Find every block of available time in player 2.
        for (int currentTime_2 = deadline; currentTime_2 >= arrival; ) {
            
            for (; player2[currentTime_2].occupied == TRUE && currentTime_2 >= arrival; currentTime_2--);
            if (currentTime_2 >= arrival) {
                
                // Find the available end time.
                pushTimePoint(wholeStoveEndTime_2, currentTime_2);
                pushTimePoint(wholeCutEndTime_2, currentTime_2);
                // Find how many time units the the empty time block has.
                for (; player1[currentTime_2].occupied == FALSE; currentTime_2--) {
                    shiftOffsetAt(wholeAvailStovePeriod_2, numBlock_2, 1);
                    shiftOffsetAt(wholeAvailCutPeriod_2, numBlock_2, 1);
                }
                // Get the start of non-occupied time block.
                int startTime_2 = peekTimePointAt(wholeStoveEndTime_2, numBlock_2) - peekTimePointAt(wholeAvailStovePeriod_2, numBlock_2) + 1;

                numBlock_2++;
            }
        }


        // Choose which player we allocate food presentation to.
        int indexBlock_1 = 0;
        int indexBlock_2 = 0;
        while (indexBlock_1 < numBlock_1 && indexBlock_2 < numBlock_2) {

            // When the `endTime_1` is equal to or larger than `endTime_2`, we allocate food presentaion process to player 1.
            int currentStoveEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);
            int currentStoveEndTime_2 = peekTimePointAt(wholeStoveEndTime_2, indexBlock_2);
            if (currentStoveEndTime_1 >= currentStoveEndTime_2) {

                // If another player is processing `Stove` type ingredient.
                int currentAvailStovePeriod_1 = peekTimePointAt(wholeAvailStovePeriod_1, indexBlock_1);
                if (currentAvailStovePeriod_1 < foodPresentTime) {
                    indexBlock_1++;
                }
                // When the available time block of player 1 is enough to process food presentation.
                else { 
                    // Set the end time of time block before food presentation process.
                    shiftOffsetAt(wholeAvailStovePeriod_1, indexBlock_1, -foodPresentTime);
                    shiftOffsetAt(wholeAvailCutPeriod_1, indexBlock_1, -foodPresentTime);
                    shiftOffsetAt(wholeStoveEndTime_1, indexBlock_1, -foodPresentTime);
                    shiftOffsetAt(wholeCutEndTime_1, indexBlock_1, -foodPresentTime);

                    // Record the start time of the food presentation.
                    schedules[numSchedule].command = 'f';
                    schedules[numSchedule].start = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1) - foodPresentTime + 1;
                    numSchedule++;
                }
            }
            // When the `endTime_1` is less than `endTime_2`, we allocate food presentaion process to player 2.
            else {
                
                int currentAvailStovePeriod_2 = peekTimePointAt(wholeAvailStovePeriod_2, indexBlock_2);
                if (currentAvailStovePeriod_2 < foodPresentTime) {
                    indexBlock_2++;
                }
                // When the available time block of player 2 is enough to process food presentation.
                else {
                    // Set the end time of time block before food presentation process.
                    shiftOffsetAt(wholeAvailStovePeriod_2, indexBlock_2, -foodPresentTime);
                    shiftOffsetAt(wholeAvailCutPeriod_2, indexBlock_2, -foodPresentTime);
                    shiftOffsetAt(wholeStoveEndTime_2, indexBlock_2, -foodPresentTime);
                    shiftOffsetAt(wholeCutEndTime_2, indexBlock_2, -foodPresentTime);

                    // Record the start time of the food presentation.
                    schedules[numSchedule].command = 'f';
                    schedules[numSchedule].start = peekTimePointAt(wholeStoveEndTime_2, indexBlock_2) - foodPresentTime + 1;
                    numSchedule++;
                }
            }
        }


        // Estimate how many stove or cutting processes the player 1 can do.
        while (indexBlock_1 < numBlock_1) {

            int currentStoveEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);
            if (currentStoveEndTime_1 < schedules[numSchedule - 1].start) {

                lastStoveEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);
                lastCutEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);

                // When another player is processing `Stove`
                if (player2[lastStoveEndTime_1].ingredType == Stove) {

                    // Find the end time which another player is not processing `Stove`.
                    while (player2[lastStoveEndTime_1].ingredType == Stove) {
                        shiftOffsetAt(wholeStoveEndTime_1, indexBlock_1, -1);
                        shiftOffsetAt(wholeAvailStovePeriod_1, indexBlock_1, -1);
                    }
                    // Calculate how many times player 1 can process `Stove`.
                    int currentAvailStovePeriod_1 = peekTimePointAt(wholeAvailStovePeriod_1, indexBlock_1);
                    while ((availNumStove_1 + 1) * STOVE_PROCESS_TIME <= currentAvailStovePeriod_1) {
                        availNumStove_1++;
                    }
                }

                // When another player is processing `Cutting`
                else if (player2[lastCutEndTime_1].ingredType == Cutting) {

                    // Find the end time which another player is not processing `Cutting`.
                    while (player2[lastCutEndTime_1].ingredType == Cutting) {
                        shiftOffsetAt(wholeStoveEndTime_1, indexBlock_1, -1);
                        shiftOffsetAt(wholeAvailStovePeriod_1, indexBlock_1, -1);
                    }
                    // Calculate how many times player 1 can process `Cutting`.
                    int currentAvailStovePeriod_1 = peekTimePointAt(wholeAvailStovePeriod_1, indexBlock_1);
                    while ((availNumCut_1 + 1) * CUTTING_PROCESS_TIME <= currentAvailStovePeriod_1) {
                        availNumCut_1++;
                    }
                }
            }
            indexBlock_1++;
        }
       

        // Estimate how many stove or cutting processes the player 2 can do.
        while (indexBlock_2 < numBlock_2) {

            int currentStoveEntTime_2 = peekTimePointAt(wholeStoveEndTime_2, indexBlock_2);
            if (currentStoveEntTime_2 < schedules[numSchedule - 1].start) {

                lastStoveEndTime_2 = currentStoveEntTime_2;
                lastCutEndTime_2 = currentStoveEntTime_2;

                // When another player is processing `Stove`
                if (player1[lastStoveEndTime_2].ingredType == Stove) {

                    // Find the end time which another player is not processing `Stove`.
                    while (player1[lastStoveEndTime_2].ingredType == Stove) {
                        shiftOffsetAt(wholeStoveEndTime_2, indexBlock_2, -1);
                        shiftOffsetAt(wholeAvailStovePeriod_2, indexBlock_2, -1);
                    }
                    // Calculate how many times player 2 can process `Stove`.
                    int currentAvailStovePeriod_2 = peekTimePointAt(wholeAvailStovePeriod_2, indexBlock_2);
                    while ((availNumStove_2 + 1) * STOVE_PROCESS_TIME <= currentAvailStovePeriod_2) {
                        availNumStove_2++;
                    }
                }

                // When another player is processing `Cutting`
                else if (player1[lastCutEndTime_2].ingredType == Cutting) {

                    // Find the end time which another player is not processing `Cutting`.
                    while (player1[lastCutEndTime_2].ingredType == Cutting) {
                        shiftOffsetAt(wholeStoveEndTime_2, indexBlock_2, -1);
                        shiftOffsetAt(wholeAvailStovePeriod_2, indexBlock_2, -1);
                    }
                    // Calculate how many times player 2 can process `Cutting`.
                    int currentAvailStovePeriod_2 = peekTimePointAt(wholeAvailStovePeriod_2, indexBlock_2);
                    while ((availNumCut_2 + 1) * CUTTING_PROCESS_TIME <= currentAvailStovePeriod_2) {
                        availNumCut_2++;
                    }
                }
            }
            indexBlock_2++;
        }

        
        // Check if there is sufficient empty time to process the food.
        if (availNumStove_1 >= orders[i].numStove && availNumCut_2 >= orders[i].numCut) {

            indexBlock_1 = 0;
            while (indexBlock_1 < numBlock_1) {

                int currentStoveEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);
                if (currentStoveEndTime_1 < schedules[numSchedule - 1].start) {
                    while ((STOVE_PROCESS_TIME <= currentStoveEndTime_1) && (availNumStove_1 > 0)) {

                        int stoveEndTime_1 = peekTimePointAt(wholeStoveEndTime_1, indexBlock_1);
                        // Mark the time info.
                        for (int mark = 0; mark < STOVE_PROCESS_TIME; mark++) {
                            player1[stoveEndTime_1 - mark].occupied = TRUE;
                            player1[stoveEndTime_1 - mark].ingredType = Stove;
                        }
                        schedules[numSchedule].orderID = orders[]

                        shiftOffsetAt(wholeStoveEndTime_1, indexBlock_1, -STOVE_PROCESS_TIME);
                        shiftOffsetAt(wholeAvailStovePeriod_1, indexBlock_1, -STOVE_PROCESS_TIME);
                        
                        availNumStove_1--;
                        numSchedule++;
                    }
                }
            }


        }
        else if (availNumStove_2 >= orders[i].numStove && availNumCut_1 >= orders[i].numCut) {

        }
        // Players can not complete this order, so cancel the schedule info of food presentation.
        else {
            
        }

        clear(wholeStartTime_1);
        clear(wholeStartTime_2);
        clear(wholeStoveEndTime_1);
        clear(wholeStoveEndTime_2);
        clear(wholeCutEndTime_1);
        clear(wholeCutEndTime_2);
        clear(wholeAvailStovePeriod_1);
        clear(wholeAvailStovePeriod_2);
        clear(wholeAvailCutPeriod_1);
        clear(wholeAvailCutPeriod_2);
    }
    free(schedules);
}



int main() {

    FILE * orders_fp[3];
    orders_fp[0] = fopen("./homework_2/test_case_2-1/orders.txt", "r");
    orders_fp[1] = fopen("./homework_2/test_case_2-2/orders.txt", "r");
    orders_fp[2] = fopen("./homework_2/test_case_2-3/orders.txt", "r");
    
    FILE * recipes_fp[3];
    recipes_fp[0] = fopen("./homework_2/test_case_2-1/recipes.txt", "r");
    recipes_fp[1] = fopen("./homework_2/test_case_2-1/recipes.txt", "r");
    recipes_fp[2] = fopen("./homework_2/test_case_2-1/recipes.txt", "r");

    FILE * players_fp[3];
    players_fp[0] = fopen("players-1.txt", "w");
    players_fp[1] = fopen("players-1.txt", "w");
    players_fp[2] = fopen("players-1.txt", "w");

    int numRecipes;
    int numOrders;
    char foodName[20];
    char ingredNames[30];
    char * ingredName;
    int endTime = 0;
    
    for (int i = 0; i < 2; i++) {

        // Read recipes info.
        fscanf(recipes_fp[i], "%d", &numRecipes);
        Recipe recipes[numRecipes];

        for (int j = 0; j < numRecipes; j++) {

            // Read foodName.
            fscanf(recipes_fp[i], "%s", foodName);
            strcpy(recipes[j].foodName, foodName);


            // Read the ingredient which is for stove.
            recipes[j].numStove = 0;
            fscanf(recipes_fp[i], "%s", ingredNames);
            if (strcmp(ingredNames, "x") != 0) {

                recipes[j].stoveIngredList = (char **) malloc(sizeof(char *));
                ingredName = strtok(ingredNames, ",");
                int numStove = recipes[j].numStove;
                while (ingredName != NULL) {

                    if (numStove > 0) {
                        recipes[j].stoveIngredList = (char **) realloc(recipes[j].stoveIngredList, (numStove + 1) * sizeof(char *));
                    }
                    recipes[j].stoveIngredList[numStove] = (char *) malloc(20 * sizeof(char));

                    printf("Stove Ingredient: %s\n", ingredName);
                    strcpy(recipes[j].stoveIngredList[numStove], ingredName);
                    recipes[j].numStove++;

                    numStove++;
                    ingredName = strtok(NULL, ",");
                }
            }
            

            // Read the ingredient which is for cutting table.
            recipes[j].numCut = 0;
            fscanf(recipes_fp[i], "%s", ingredNames);
            if (strcmp(ingredNames, "x") != 0) {

                recipes[j].cutIngredList = (char **) malloc(sizeof(char *));
                ingredName = strtok(ingredNames, ",");
                int numCut = recipes[j].numCut;
                
                while (ingredName != NULL) {

                    if (numCut > 0) {
                        recipes[j].cutIngredList = (char **) realloc(recipes[j].cutIngredList, (numCut + 1) * sizeof(char *));
                    }
                    recipes[j].cutIngredList[numCut] = (char *) malloc(20 * sizeof(char));

                    printf("Cutting table Ingredient: %s\n", ingredName);
                    strcpy(recipes[j].cutIngredList[numCut], ingredName);
                    recipes[j].numCut++;

                    numCut++;
                    ingredName = strtok(NULL, ",");
                }
            }
            

            // Read other ingredients and calculate how many time units we have to make the food presentation.
            recipes[j].foodPresentTime = 1;
            fscanf(recipes_fp[i], "%s", ingredNames);
            if (strcmp(ingredNames, "x") != 0) {

                ingredName = strtok(ingredNames, ",");
                while (ingredName != NULL) {

                    recipes[j].foodPresentTime += 1;
                    ingredName = strtok(NULL, ",");
                }
            }
        }
        



        // Read orders info.
        fscanf(orders_fp[i], "%d", &numOrders);
        Order orders[numOrders];

        for (int j = 0; j < numOrders; j++) {

            fscanf(orders_fp[i], "%d", &orders[j].orderID);

            fscanf(orders_fp[i], "%s", foodName);
            for (int indexRecipe = 0; indexRecipe < numRecipes; indexRecipe++) {

                if (strcmp(foodName, recipes[indexRecipe].foodName) == 0) {

                    orders[j].numStove = recipes[indexRecipe].numStove;
                    orders[j].numCut = recipes[indexRecipe].numCut;
                    orders[j].foodPresentTime = recipes[indexRecipe].foodPresentTime;
                }
            }

            fscanf(orders_fp[i], "%d", &orders[j].arrival);
            fscanf(orders_fp[i], "%d", &orders[j].deadline);
            fscanf(orders_fp[i], "%d", &orders[j].earn);
            fscanf(orders_fp[i], "%d", &orders[j].punishment);

            // Update the latest end time among all of orders.
            if (orders[j].deadline > endTime) {
                endTime = orders[j].deadline;
            }
        }
        
        
        
        // Sort the orders by a descending order of profit rate.
        Order * sortedOrders = sortOrders(orders, numOrders);

        // Build a sufficient time array which occupied field is all initialized with `FALSE` for the whole order requests.
        Time * player1 = (Time *) malloc((endTime + 1) * sizeof(char));
        Time * player2 = (Time *) malloc((endTime + 1) * sizeof(char));
        for (int j = 0; j <= endTime; j++) player1[j].ingredType = None;
        for (int j = 0; j <= endTime; j++) player2[j].ingredType = None;
        
        // Generate a schedule to arrange the jobs for player 1 and player 2.
        scheduleInfo * schedules;
        scheduleIngredProcessing(player1, player2, sortedOrders, numOrders, recipes, schedules);

        

       
        
        for (int j = 0; j < numRecipes; j++) {
            free(recipes[j].stoveIngredList);
            free(recipes[j].cutIngredList);
        }
        free(player1);
        free(player2);

        fclose(recipes_fp[i]);
        fclose(orders_fp[i]);
        fclose(players_fp[i]);
    }
    

    return 0;
}