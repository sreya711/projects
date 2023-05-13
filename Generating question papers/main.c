#include "Headers/structs.h"
#include "Headers/stack.h"
#include "Headers/functions.h"

int main(void)
{
    char paper[50], bank[50], output[50];
    char c;
    FILE *bk, *pr, *op;
    Bank *B;
    Paper *P;
    
    printf("Enter question bank filename: ");
    scanf("%s", bank);

    printf("Enter question paper filename: ");
    scanf("%s", paper);

    printf("Enter output filename: ");
    scanf("%s", output);

    // Opens the question bank in read mode 
    bk = fopen(bank, "r");                                 
    if (bk == NULL)
    {
        printf("Bank file %s failed to open.\n", bank);
        exit(0);
    }

    // Opens the question paper in read mode 
    pr = fopen(paper, "r");
    if (pr == NULL)
    {
        printf("Paper file %s failed to open.\n", paper);
        exit(0);
    }

    // Starts reading the question bank
    B = parse_bank(bk);
    printf("Bank parsed successfully.\n");

    // Starts reading the question paper
    P = parse_paper(pr);
    printf("Paper parsed successfully.\n");

    // Opens the output file in write mode
    op = fopen(output, "w");
    if (op == NULL)
    {
        printf("Output file %s failed to open.\n", output);
        exit(0);
    }
    printf("Output file opened in write mode\n");

    // Finds MCQ type question to 
    // be added in the Output file
    find_MCQ(op, P, B);

    // Finds Fill in the blank type question to 
    // be added in the Output file
    find_FITB(op, P, B);

    // Finds True/False type question to 
    // be added in the Output file
    find_TF(op, P, B);

    // Finds Numerical type question to 
    // be added in the Output file
    find_NUM(op, P, B);

    // Closes the Output file
    fclose(op);

    freeBank(B);
    freePaper(P);
}
