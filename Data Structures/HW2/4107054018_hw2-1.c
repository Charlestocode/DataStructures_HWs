#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a data type, Node.
typedef struct Node *nodePtr;
typedef struct Node {
    int data;
    nodePtr next;
} Node;


// -------------------------------------- Create a Singly Linked List ------------------------------------------
/* *************** Note: when create a new node, you should allocate memory to this new node. **************** */

// Declaare a pointer `back` to point to the back of the linked list.
nodePtr back;
// `size` is the number of nodes in the linked list excluding the first reserved node.
int size;

// Create a singly linked list and reserve the first node of the singly linked list.
nodePtr createTimePointList() {

    nodePtr front = (nodePtr) malloc(sizeof(Node));
    if (front == NULL) {
        fprintf(stderr, "Error of creating the linked list.");
    }

    back = front;
    return front;
}


// Push the node into the back of the singly linked list
void add(nodePtr *back, int value) {

    if (back == NULL) {
        fprintf(stderr, "The linked list is empty!");
    }

    // Create a new node for storing the new element.
    nodePtr newNode = malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    (*back)->next = newNode;
    (*back) = (*back)->next;
    size++;
}


// Delete the i-th node.
// Note: the `index` start from 1, so actually we fetch it with `index - 1`.
void deleteAt(nodePtr *list, int index) {

    nodePtr currentNode = *list;
    nodePtr prevNode = NULL;
    for (int i = 0; i < index; i++) {
        
        prevNode = currentNode;
        currentNode = currentNode->next;

        // If the index is beyond the number of nodes, exit the function.
        if (currentNode == NULL) {
            return;
        }
    }
    
    prevNode->next = currentNode->next;
    // If the current node which is going to be deleted, then update the node `back` with its previous node.
    if (currentNode->next == NULL) {
        back = prevNode;
    }
    size--;
    free(currentNode);
}


// Insert a node with an assigned value after the i-th node.
void insertAt(nodePtr *list, int index, int value) {

    nodePtr newNode = malloc(sizeof(Node));
    newNode->data = value;

    nodePtr currentNode = *list;
    for (int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }

    newNode->next = currentNode->next;
    currentNode->next = newNode;
    size++;

    // If the new inserted node is the last node, update the node `back` with that.
    if (newNode->next == NULL) {
        back = newNode;
    }
}


// Multiply the `toLast` to the last node with a `multiple`.
void mulFromLast(nodePtr *list, int toLast, int multiple) {

    nodePtr currentNode = *list;
    // Make the variable `index` as the `index` in insertAt() which starts from 1 to `size`.
    int index = size - toLast + 1;
    for (int i = 0; i < index; i++) {
        currentNode = currentNode->next;
        int data = currentNode->data;
    }

    currentNode->data = currentNode->data * multiple;
}


// Reverse a singly linked list. You do not have to assgine the return value to a variable. 
// You can just call the fucntion which has the same result.
// `front` store the first node of the unreversed list.
nodePtr reverse(nodePtr front, int group) {

    // `list` is as a return variable.
    nodePtr list = front;
    // `tailNode` store the last node in the current reversed linked list.
    nodePtr tailNode = front;
    nodePtr middle, trail;

    // Ignore the reserved node.
    front = front->next;
    while (front != NULL) {

        middle = NULL;
        for (int i = 0; i < group && front != NULL; i++) {
            trail = middle;
            middle = front;
            front = front->next;
            middle->next = trail;
        }

        // Connect the `tailNode` with the the head of newly reversed list, and update `tailNode` to the end of the reversed list.
        tailNode->next = middle;
        for (; tailNode->next != NULL; tailNode = tailNode->next);
    }
    back = tailNode;

    return list;
}


// Show the current singly list.
void show(nodePtr list, FILE * output_fp) {

    // Ignore the first reserved node.
    list = list->next;
    for (int i = 0; i < size; i++) {
        
        fprintf(output_fp, "%d ", list->data);
        printf("%d ", list->data);
        list = list->next;
    }
    fprintf(output_fp, "\n");
    printf("\n");
}


// Clear the whole singly list.
void clear(nodePtr list) {

    nodePtr prevNode;
    while (list != NULL) {
        prevNode = list;
        list = list->next;
        free(prevNode);
    }
    size = 0;
}
// -------------------------------------------------------------------------------------------------------------------

void call(nodePtr *list, const char * instruction, FILE * fp, FILE * output_fp) {

    // `list` in function `call()` is now a pointer of the pointer of the singly linked list.
    int value;
    int index;
    int toLast;

    if (strcmp(instruction, "add") == 0) {
        fscanf(fp, "%d", &value);
        add(&back, value);
        return;
    }

    if (strcmp(instruction, "del") == 0) {
        fscanf(fp, "%d", &value);
        deleteAt(list, value);
        return;
    }

    if (strcmp(instruction, "ins") == 0) {
        fscanf(fp, "%d %d", &index, &value);
        insertAt(list, index, value);
        return;
    }

    if (strcmp(instruction, "mul") == 0) {
        fscanf(fp, "%d %d", &toLast, &value);
        mulFromLast(list, toLast, value);
        return;
    }

    if (strcmp(instruction, "rev") == 0) {
        fscanf(fp, "%d", &value);
        reverse(*list, value);
        return;
    }

    if (strcmp(instruction, "show") == 0) {
        show(*list, output_fp);
        return;
    }
}


int main() {

    printf("============================ HW2 Q1 =============================\n");
    FILE *fp[3];
    fp[0] = fopen("./homework_2/test_case_1-1/input_1.txt", "r");
    fp[1] = fopen("./homework_2/test_case_1-2/input_1.txt", "r");
    fp[2] = fopen("./homework_2/test_case_1-3/input_1.txt", "r");

    FILE * output_1_fp[3];
    output_1_fp[0] = fopen("./output_1-1.txt", "w");
    output_1_fp[1] = fopen("./output_1-2.txt", "w");
    output_1_fp[2] = fopen("./output_1-3.txt", "w");


    for (int i = 0; i < 3; i++) {

        printf("test_case_1-%d: \n", i + 1);

        if (fp[i] == NULL) {
            fprintf(stderr, "The file pointer is NULL!");
            exit(1);
        }

        int value;
        int index;
        int toLast;
        int numInstruct;
        char buffer;
        char instruction[10];
        

        nodePtr list = createList();
        // Read the inital list and the number of instructions.
        while (fscanf(fp[i], "%d", &value) != EOF && fscanf(fp[i], "%c", &buffer) != EOF) {

            add(&back, value);
            if (buffer == '\r' || buffer == '\n') {
                fscanf(fp[i], "%d", &numInstruct);
                break;
            }
        }

        // Read each instruction.
        for (int j = 0; j < numInstruct; j++) {

            fscanf(fp[i], "%s", instruction);
            call(&list, instruction, fp[i], output_1_fp[i]);
        }

        clear(list);
        fclose(fp[i]);
        printf("\n");
    }
    

    return 0;
}