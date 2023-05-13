#include "../Headers/structs.h"
#include "../Headers/functions.h"

// Freeing the memory of MCQ linked list node
void freeMCQ(MCQ *M)
{
    free(M->text);
    for (int i = 0; i < M->no_corr; i++)
        free(M->corr[i]);
    for (int i = 0; i < M->no_ops - M->no_corr; i++)
        free(M->wrong[i]);
    free(M);
}

// Freeing the memory of FITB linked list nooe
void freeFITB(FITB *F)
{
    free(F->text);
    free(F->ans);
    free(F);
}

// Freeing the memory of TF linked list node
void freeTF(TF *T)
{
    free(T->text);
    free(T);
}

// Freeing the memory of NUM linked list node
void freeNUM(NUM *N)
{
    free(N->text);
    free(N);
}

// Freeing the memory of the Question Bank 
// by calling each of the above functions
void freeBank(Bank *B)
{
    MCQ* trav1 = B->mcq_list;
    MCQ* temp1;
    while (trav1->next != NULL)             // To visit and free all nodes except first one
    {
        temp1 = trav1->next;
        trav1->next = temp1->next;
        freeMCQ(temp1);
    }
    freeMCQ(trav1);                         //For the free of first node

    FITB* trav2 = B->fitb_list;
    FITB* temp2;
    while (trav2->next != NULL)             // To visit and free all nodes except first one
    {
        temp2 = trav2->next;
        trav2->next = temp2->next;
        freeFITB(temp2);
    }
    freeFITB(trav2);                        //For the free of first node

    TF* trav3 = B->tf_list;
    TF* temp3;
    while (trav3->next != NULL)             // To visit and free all nodes except first one
    {
        temp3 = trav3->next;
        trav3->next = temp3->next;
        freeTF(temp3);
    }
    freeTF(trav3);                          //For the free of first node

    NUM* trav4 = B->num_list;
    NUM* temp4;
    while (trav4->next != NULL)             // To visit and free all nodes except first one
    {
        temp4 = trav4->next;
        trav4->next = temp4->next;
        freeNUM(temp4);
    }
    freeNUM(trav4);                         //For the free of first node
}

// Freeing the memory of Question Paper linked list
void freePaper(Paper *P)
{
    for (int i = 0; i < 10; i++)
        free(P->mcq_reqs[i]);
    free(P->fitb_reqs);
    free(P->tf_reqs);
    free(P->num_reqs);
}
