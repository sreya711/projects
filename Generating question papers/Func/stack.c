#include "../Headers/stack.h"

// Pushes value on the stack
void push(Stack S, char val)
{
    PtrToNode new = create_node(val);
    new->next = S->next;
    S->next = new;
}

// Pops a value off the stack
char pop(Stack S)
{
    PtrToNode temp = S->next;
    char c = temp->part;
    S->next = temp->next;
    free(temp);
    return c;
}

// Creates an empty stack
Stack create_empty()
{
    PtrToNode temp;
    temp = (PtrToNode)malloc(sizeof(Node));
    temp->part = ' ';
    temp->next = NULL;
    return temp;
}

// Helps in pushing a value on the stack
// by creating a new node
PtrToNode create_node(char val)
{
    Node *val1;
    val1 = (Node *)malloc(sizeof(Node));
    val1->part = val;
    val1->next = NULL;
    return val1;
}

// Tells the top-most value on the stack
char top(Stack S)
{
    return (S->next->part);
}
