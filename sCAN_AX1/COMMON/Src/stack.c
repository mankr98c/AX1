/*
 * Stack.c
 *
 *  Created on: Apr 29, 2024
 *      Author: CEINFO
 */
#include <stack.h>

// Function to initialize the stack
void initializeStack(Stack *stack) {
    stack->top = -1; // Initialize top index to -1 (empty stack)
}

// Function to check if the stack is full
int full(Stack *stack) {
    return stack->top == MAX_STACK_DATA - 1; // Stack is full if top is at max index
}

// Function to check if the stack is empty
int empty(Stack *stack) {
    return stack->top == -1; // Stack is empty if top is -1
}

// Function to push an array onto the stack
bool push(Stack *stack, const uint8_t *array,uint32_t size) {
    if (full(stack)) {
        printc(MinDelay,"Error: Stack overflow\n");
        return false; // Exit function if stack is full
    }

    stack->top++; // Increment top index
    clearBuffer(stack->data[stack->top],sizeof(stack->data[stack->top]));
    memcpy(stack->data[stack->top], array, size); // Copy array to stack
//    print("Pushed array onto the stack\n");
    return true;
}

// Function to pop an array from the stack
bool pop(Stack *stack, uint8_t *result) {
    if (empty(stack)) {
        printc(MinDelay,"Error: Stack underflow\n");
        return false; // Exit function if stack is empty
    }
    strcpy(result,stack->data[stack->top]);
    //memcpy(result, stack->data[stack->top], size); // Copy array from stack
    stack->top--; // Decrement top index
    return true;
}

