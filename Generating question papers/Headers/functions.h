#ifndef __FUNCS
#define __FUNCS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stack.h"
#include "structs.h"

// Header file declaring functions

//Initialisation
Bank *init_bank();
MCQ *init_MCQ();
FITB *init_FITB();
TF *init_TF();
NUM *init_NUM();
Paper *init_paper();
Request* init_request();

//Creating nodes
MCQ *create_MCQ(char *text, float diff);
FITB *create_FITB(char *text, char *ans, float diff);
TF *create_TF(char *text, char ans, float diff);
NUM *create_NUM(char *text, int ans, float diff);

//Parsing QBank
Bank *parse_bank(FILE *B);
ListMCQ parse_MCQ(FILE *B, Stack part);
ListFITB parse_FITB(FILE *B, Stack part);
ListTF parse_TF(FILE *B, Stack part);
ListNUM parse_NUM(FILE *B, Stack part);
Paper *parse_paper(FILE *P);

//Generating output file
void find_MCQ(FILE *op, Paper *P, Bank *B);
void find_FITB(FILE *op, Paper *P, Bank *B);
void find_TF(FILE *op, Paper *P, Bank *B);
void find_NUM(FILE *op, Paper *P, Bank *B);

void select_MCQ(FILE *op, MCQ **possible, int found, int no_req, int no_ops);
void select_FITB(FILE *op, FITB **possible, int found, int no_req);
void select_TF(FILE *op, TF **possible, int found, int no_req);
void select_NUM(FILE *op, NUM **possible, int found, int no_req);

void fileput_MCQ(FILE *op, MCQ *M, int no_ops);
void fileput_FITB(FILE *op, FITB *M);
void fileput_TF(FILE *op, TF *M);
void fileput_NUM(FILE *op, NUM *M);

// Inserting nodes in linked list
void InsertMCQ(ListMCQ L, MCQ *X);
void InsertFITB(ListFITB L, FITB *X);
void InsertTF(ListTF L, TF *X);
void InsertNUM(ListNUM L, NUM *X);

// Freeing memory
void freeMCQ(MCQ *M);
void freeFITB(FITB *F);
void freeTF(TF *T);
void freeNUM(NUM *N);
void freeBank(Bank *B);
void freePaper(Paper *P);

#endif
