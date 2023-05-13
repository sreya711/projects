#include "../Headers/functions.h"
#include "../Headers/structs.h"

// Inserts a node of Fill in the blanks type question details into
// another linked list of same type; is used while parsing
void InsertFITB(ListFITB L, FITB *X)
{
    X->next = L->next;
    L->next = X;
}

// Inserts a node of Numerical type question details into
// another linked list of same type; is used while parsing
void InsertNUM(ListNUM L, NUM *X)
{
    X->next = L->next;
    L->next = X;
}

// Inserts a node of MCQ type question details into
// another linked list of same type; is used while parsing
void InsertMCQ(ListMCQ L, MCQ *X)
{
    X->next = L->next;
    L->next = X;
}

// Inserts a node of True/False type question details into
// another linked list of same type; is used while parsing
void InsertTF(ListTF L, TF *X)
{
    X->next = L->next;
    L->next = X;
}
