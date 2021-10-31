#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define MAX_STACK_SIZE 50

// Enumerate all operator we would use as tokens for corresponding to "in-stack precedence" and "incoming prcedence" except operand.
typedef enum {lparen, rparen, positive, negative, plus, minus, times, divide, mod, largerthan, smallerthan, bitAND, bitXOR, bitOR, eos, operand} precedence;

// Arrays to correspond to in-stack precedence and incoming precedence
int isp[] = {17, 0, 15, 15, 12, 12, 13, 13, 13, 10, 10, 8, 7, 6, 0};
int icp[] = {17, 18, 15, 15, 12, 12, 13, 13, 13, 10, 10, 8, 7, 6, 0};


// Get the responding token from character in text.
precedence getToken(char expr[], char *symbol, int *n) {
    *symbol = expr[(*n)--];
    switch (*symbol) {
        case '(': return lparen;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '*': return times;
        case '/': return divide;
        case '%': return mod;
        case '>': return largerthan;
        case '<': return smallerthan;
        case '&': return bitAND;
        case '^': return bitXOR;
        case '|': return bitOR;
        case '\0': return eos;
        // Operands: 1. character, 2. digit, 3. dot (.)
        default: return operand;
    }
}


// Get the corresponding symbol from the token.
char getSymbol(precedence token) {
    switch (token) {
        case lparen: return '(';
        case rparen: return ')';
        case positive: return '+';
        case negative: return '-';
        case plus: return '+';
        case minus: return '-';
        case times: return '*';
        case divide: return '/';
        case mod: return '%';
        case largerthan: return '>';
        case smallerthan: return '<';
        case bitAND: return '&';
        case bitXOR: return '^';
        case bitOR: return '|';
        case eos: return '\0';
        default: return -1;
    }
}



// Declare an array as a stack to store "operator's tokens."
precedence stack[MAX_STACK_SIZE];
int top = -1;

int isEmpty() {
    if (top < 0) return 1;
    else return 0;
}

int isFull() {
    if (top >= MAX_STACK_SIZE) return 1;
    else return 0;
}

void stackFull() {
    fprintf(stderr, "Stack is full! Can not push any new element.");
    exit(EXIT_FAILURE);
}

precedence stackEmpty() {
    //fprintf(stderr, "Stack is empty! Can not pop any element.");
    return -1;
}

void push(precedence item) {
    if (isFull()) {
        stackFull();
    }
    stack[++top] = item;
}

precedence pop() {
    if (isEmpty()) {
        return stackEmpty();
    }
    return stack[top--];
}

// -----------------------------------------------------------------------------------------------
// Declare an array as a stack to store "inverse prefix expression."
char inversePrefixStack[MAX_STACK_SIZE];
int topInversePrefix = -1;

int isEmpty_IP() {
    if (topInversePrefix < 0) return 1;
    else return 0;
}

int isFull_IP() {
    if (topInversePrefix >= MAX_STACK_SIZE) return 1;
    else return 0;
}

void stackFull_IP() {
    fprintf(stderr, "Stack is full! Can not push any new element.");
    exit(EXIT_FAILURE);
}

void stackEmpty_IP() {
    fprintf(stderr, "Stack is empty! Can not pop any element.");
    //return -1;
}

void push_IP(char item) {
    if (isFull()) {
        stackFull();
    }
    
    inversePrefixStack[++topInversePrefix] = item;
}
 
void pop_IP(char * symbol) {
    if (isEmpty()) {
        stackEmpty();
    }
    char topValue = inversePrefixStack[topInversePrefix];
    inversePrefixStack[topInversePrefix--] = '\0';
    *symbol = topValue;
    //return topValue;
}



char * prefix(char exprInfix[]) {
    
    // Record the number of characters excluding '\n' in each line.
    int numSymbol = -1;
    for (int i = 0; exprInfix[i] != '\n'; i++) {
        numSymbol++;
    }


    // Variable `symbol` is delcared for getting the symbol from the infix expression.
    char symbol;
    precedence token;
    // The index for scaning the infix expression from the end.
    int indexInfix = numSymbol - 1;

    // Initialize the stack for infix operators.
    top = 0;
    stack[0] = eos;
    // Initialize the stack for the inverse prefix expression.
    topInversePrefix = 0;
    inversePrefixStack[0] = '\0';

    // Declare an array for storing the prefix expression.
    int indexPrefix = 0;
    static char exprPrefix[MAX_STACK_SIZE];

    // For checking if the previous symbol is plus or minus sign.
    int plusMinusSign = FALSE;
    // For checking if the previous symbol is operand.
    int operandSymbol = FALSE;
    
    for (token = getToken(exprInfix, &symbol, &indexInfix); token != eos; token = getToken(exprInfix, &symbol, &indexInfix)) {

        if (token == operand) {
            if (!plusMinusSign) {
                if (!operandSymbol && topInversePrefix != 0) {
                    push_IP(' ');
                }
                push_IP(symbol);
                operandSymbol = TRUE;
            }
            // When the previous symbol is a "plus" or "minus" sign, because in the "plus" or "minus" sign rouond we don't do anything
            // for the symbol, then we get back to deal with that.
            else {
                precedence prevToken = pop();
                while (isp[stack[top]] >= icp[prevToken]) {
                    push_IP(' ');
                    push_IP(getSymbol(pop()));
                }
                push(prevToken);
                push_IP(' ');
                push_IP(symbol);

                // Renew `plusMinusSign` flag.
                plusMinusSign = FALSE;
                operandSymbol = TRUE;
            }
        }
        else if (token == lparen) {
            // If the top of the stack is "plus" or "minus" sign, it must belong to "positive" or "negative" symbol.
            if (stack[top] != rparen && plusMinusSign == TRUE) {
                push_IP(getSymbol(pop()));
            }

            while (stack[top] != rparen) {
                push_IP(' ');
                push_IP(getSymbol(pop()));
            }
            // Pop out the left parenthesis.
            pop();
            
            operandSymbol = FALSE;
        }
        else {

            // Check whether this symbol is plus or minus if this is the first plus or minus sign.
            if (!plusMinusSign && (token == plus || token == minus)) {
                plusMinusSign = TRUE;
            }
            // If the next symbol of the first plus or minus sign is operators, we .
            else if (plusMinusSign && token != operand && token != rparen) {
                precedence prevToken = pop();
                if (prevToken == plus) {
                    push_IP(getSymbol(positive));
                } else {
                    push_IP(getSymbol(negative));
                }
                plusMinusSign = FALSE;
            }

            // Check if the precedence of top of the token stack is larger than the new token and 
            // the current operator is not a "plus" or "minus" sign, then pop out.
            // When the current is a "plus" or "minus" sign, the precedence of the symbol msut pend until next round.
            while (isp[stack[top]] > icp[token] && !plusMinusSign) {
                symbol = getSymbol(pop());
                push_IP(' ');
                push_IP(symbol);
            }
            push(token);

            operandSymbol = FALSE;
        }
        
    }
    
    // Pop out the remaining tokens on the stack.
    while ((token = pop()) != eos) {
        symbol = getSymbol(token);
        push_IP(' ');
        push_IP(symbol);
    }
    pop();    


    // Get the prefix expression from inverse prifix expression.
    pop_IP(&symbol);
    while (symbol != '\0') {
        exprPrefix[indexPrefix++] = symbol;
        pop_IP(&symbol);
    }
    exprPrefix[indexPrefix] = symbol;

    return exprPrefix;
}



int main() {

    FILE *fp = fopen("./test data/input_1.txt", "r");
    FILE *output_1_fp = fopen("output_1.txt", "w");
    char exprInfix[50];
    char * exprPrefix;
    
    printf("================== HW1 Q1 ==================\n");
    // Input the Infix expression line by line.
    while (fgets(exprInfix, 50, fp) != NULL) {

        exprPrefix = prefix(exprInfix);
        printf("Prefix Expression: %s\n", exprPrefix);
        fprintf(output_1_fp, "%s\n", exprPrefix);
    }

    fclose(fp);
    fclose(output_1_fp);
    return 0;
}