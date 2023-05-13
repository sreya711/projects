#include "../Headers/functions.h"
#include "../Headers/stack.h"
#include "../Headers/structs.h"

void find_MCQ(FILE *op, Paper *P, Bank *B)
{
    MCQ *trav;
    MCQ **possible = (MCQ **)malloc(sizeof(MCQ *));         //Will store the suitable questions [array]
    int found = 0;
    int no_req, no_ops;
    for (int i = 0; i < 11; i++)
    {
        no_req = P->mcq_reqs[i]->no_req;                    //To avoid repeatedly accessing
        no_ops = P->mcq_reqs[i]->no_ops;
        if (no_req == 0)                                    //All mcq requests done
        {
            break;
        }
        trav = B->mcq_list->next;                           
        found = 0;

        while (trav != NULL)                                                          //Traverse through linked
        {                                                                             //list and add suitable
            if (trav->no_ops >= no_ops && trav->diff >= P->mcq_reqs[i]->diff_lb &&    //questions to array using
                trav->diff <= P->mcq_reqs[i]->diff_ub)                                //realloc; "found" stores
            {                                                                         //total number found
                possible = (MCQ **)realloc(possible, (found + 1) * sizeof(MCQ *));
                possible[found++] = trav;
            }

            trav = trav->next;
        }
        if (found < no_req)                         //Not enough
        {
            printf("Not enough MCQs (%d found, %d required), not adding!\n", found, no_req);
        }
        else
        {
            select_MCQ(op, possible, found, no_req, no_ops);
            printf("MCQs found and added!\n");
        }
    }
}

void select_MCQ(FILE *op, MCQ **possible, int found, int no_req, int no_ops)
{
    int rem;
    srand(time(0));
    while (found != no_req)                             //Randomly removes
    {                                                   //questions from
        rem = rand() % found;                           //possible array
        for (int i = rem; i < found - 1; i++)           //until required
            possible[i] = possible[i + 1];              //number is left
        found--;
    }
    fprintf(op, "MCQ with %d options:\n", no_ops);      //Section heading
    for (int i = 0; i < found; i++)                     //Prints each
    {                                                   //question and
        fprintf(op, "%d) ", i + 1);                     //its options
        fileput_MCQ(op, possible[i], no_ops);       
    }
    fprintf(op, "--------------------\n");              //End of section
}

void fileput_MCQ(FILE* op, MCQ *M, int no_ops)
{ 
    srand(time(0));
    int r, corr = 0, wrong = 0;
    int lb, ub;
    int size_c = M->no_corr;                                         //corr_pr contains randomly selected options, which are
    char* corr_pr[size_c]; char* corr_shuf[size_c];                  //shuffled and put into corr_shuf
    int size_w = M->no_ops - size_c;    
    char* wrong_pr[size_w]; char* wrong_shuf[size_w];
    int size_c2, size_w2;
    for (int i = 0; i < size_c; i++) corr_pr[i] = M->corr[i];       //starts with all available options;
    for (int i = 0; i < size_w; i++) wrong_pr[i] = M->wrong[i];     //the number to be printed is randomly selected
        
    lb = (no_ops - size_w >= 1) ? no_ops - size_w : 1;              //lower and upper bounds for selecting the
    ub = (size_c >= no_ops) ? no_ops : size_c;                      //number of correct options to print

    if (ub == lb) corr = lb;
    else corr = (rand() % (ub - lb)) + lb;                          //number of correct options to print
    wrong = no_ops - corr;                                          

    while (size_c != corr)                      //Removing options
    {                                           //from corr_pr until
        r = rand() % size_c;                    //only required 
        for (int i = r; i < size_c; i++)        //number is left
            corr_pr[i] = corr_pr[i+1];
        size_c--;
    }
    while (size_w != wrong)                     //Removing options
    {                                           //from wrong_pr until
        r = rand() % size_w;                    //only required
        for (int i = r; i < size_w; i++)        //number is left
            wrong_pr[i] = wrong_pr[i+1];
        size_w--;
    }

    corr = 0; wrong = 0;
    size_c2 = size_c; size_w2 = size_w;
    while (corr != size_c)                                      //Shuffling corr_pr
    {                                                           //and putting in
        r = rand() % size_c2; corr_shuf[corr++] = corr_pr[r];   //corr_shuf
        for (int i = r; i < size_c2; i++)
            corr_pr[i] = corr_pr[i+1];
        size_c2--;
    }
    while (wrong != size_w)                                       //Shuffling wrong_pr
    {                                                             //and putting in
        r = rand() % size_w2; wrong_shuf[wrong++] = wrong_pr[r];  //wrong_shuf
        for (int i = r; i < size_w2; i++)
            wrong_pr[i] = wrong_pr[i+1];
        size_w2--;
    }

    fprintf(op, "%s\n", M->text);                                  //Print question text

    corr = 0; wrong = 0;
    for (int i = 0; i < no_ops; i++)                                            //Print options
    {                                                                           //with indentation
        if (corr < size_c && wrong < size_w)                                    //and indexed by
        {                                                                       //a, b, c etc
            r = rand() % 2;                                                     //
            if (r == 0)                                                         //Randomly prints
            {                                                                   //either a correct
                fprintf(op, "   %c) %s\n", i + 97, corr_shuf[corr++]);          //or a wrong one
            }                                                                   //at each iter until
            if (r == 1)                                                         //either one is over
            {
                fprintf(op, "   %c) %s\n", i + 97, wrong_shuf[wrong++]);
            }
        }
        else if (wrong == size_w)
            fprintf(op, "   %c) %s\n", i + 97, corr_shuf[corr++]);
        else
            fprintf(op, "   %c) %s\n", i + 97, wrong_shuf[wrong++]);
    }
}

void find_FITB(FILE *op, Paper *P, Bank *B)
{
    FITB *trav;
    FITB **possible = (FITB **)malloc(sizeof(FITB *));          //Stores the suitable questions [array]
    int found = 0;
    int no_req;
    no_req = P->fitb_reqs->no_req;                              //To avoid repeatedly accessing
    trav = B->fitb_list->next;
    found = 0;

    while (trav != NULL)                                                                    //Traverse through linked
    {                                                                                       //list and add suitable
        if (trav->diff >= P->fitb_reqs->diff_lb && trav->diff <= P->fitb_reqs->diff_ub)     //questions to array using
        {                                                                                   //realloc; "found" stores
            possible = (FITB **)realloc(possible, (found + 1) * sizeof(FITB *));            //total number found
            possible[found++] = trav;
        }

        trav = trav->next;
    }

    if (found < no_req)                     //Not enough
    {
        printf("Not enough FITBs (%d found, %d required), not adding!\n", found, no_req);
    }
    else
    {
        select_FITB(op, possible, found, no_req);
        printf("FITBs found and added!\n");
    }
}

void select_FITB(FILE *op, FITB **possible, int found, int no_req)
{
    int rem;
    srand(time(0));
    while (found != no_req)                         //Randomly removes
    {                                               //questions from
        rem = rand() % found;                       //possible array
        for (int i = rem; i < found - 1; i++)       //until required
            possible[i] = possible[i + 1];          //number is left
        found--;
    }
    fprintf(op, "FITB:\n");                         //Section heading
    for (int i = 0; i < found; i++)                 //Prints
    {                                               //each
        fprintf(op, "%d) ", i + 1);                 //question
        fileput_FITB(op, possible[i]);
    }
    fprintf(op, "--------------------\n");          //End of section
}

void fileput_FITB(FILE *op, FITB *F)
{
    srand(time(0));
    int r, corr = 0, wrong = 0;
    fprintf(op, "%s\n", F->text);               //Only print question text
}

void find_TF(FILE *op, Paper *P, Bank *B)
{
    TF *trav;
    TF **possible = (TF **)malloc(sizeof(TF *));        //Stores the suitable questions [array]
    int found = 0;
    int no_req;
    no_req = P->tf_reqs->no_req;                        //To avoid repeatedly accessing
    trav = B->tf_list->next;
    found = 0;

    while (trav != NULL)                                                                //Traverse through linked   
    {                                                                                   //list and add suitable 
        if (trav->diff >= P->tf_reqs->diff_lb && trav->diff <= P->tf_reqs->diff_ub)     //questions to array using
        {                                                                               //realloc; "found" stores  
            possible = (TF **)realloc(possible, (found + 1) * sizeof(TF *));            //total number found
            possible[found++] = trav;
        }

        trav = trav->next;
    }

    if (found < no_req)                                 //Not enough
    {
        printf("Not enough TFs (%d found, %d required), not adding!\n", found, no_req);
    }
    else
    {
        select_TF(op, possible, found, no_req);
        printf("TFs found and added!\n");
    }
}

void select_TF(FILE *op, TF **possible, int found, int no_req)
{
    int rem;
    srand(time(0));
    while (found != no_req)                         //Randomly removes
    {                                               //questions from
        rem = rand() % found;                       //possible array
        for (int i = rem; i < found - 1; i++)       //until required
            possible[i] = possible[i + 1];          //number is left
        found--;
    }
    fprintf(op, "TF:\n");                           //Section heading
    for (int i = 0; i < found; i++)                 //Prints
    {                                               //each
        fprintf(op, "%d) ", i + 1);                 //question
        fileput_TF(op, possible[i]);
    }
    fprintf(op, "--------------------\n");          //End of section
}

void fileput_TF(FILE *op, TF *T)
{
    srand(time(0));
    int r, corr = 0, wrong = 0;
    fprintf(op, "%s\n", T->text);               //Only print question text
}

void find_NUM(FILE *op, Paper *P, Bank *B)
{
    NUM *trav;
    NUM **possible = (NUM **)malloc(sizeof(NUM *));     //Stores the suitable question [array]
    int found = 0;
    int no_req;
    no_req = P->num_reqs->no_req;                       //To avoid repeatedly accessing
    trav = B->num_list->next;
    found = 0;

    while (trav != NULL)                                                                 //Traverse through linked  
    {                                                                                    //list and add suitable  
        if (trav->diff >= P->num_reqs->diff_lb && trav->diff <= P->num_reqs->diff_ub)    //questions to array using
        {                                                                                //realloc; "found" stores
            possible = (NUM **)realloc(possible, (found + 1) * sizeof(NUM *));           //total number found
            possible[found++] = trav;
        }

        trav = trav->next;
    }
    if (found < no_req)                                 //Not enough
    {
        printf("Not enough NUMs (%d found, %d required), not adding!\n", found, no_req);
    }
    else
    {
        select_NUM(op, possible, found, no_req);
        printf("NUMs found and added!\n");
    }
}

void select_NUM(FILE *op, NUM **possible, int found, int no_req)
{
    int rem;
    srand(time(0));
    while (found != no_req)                         //Randomly removes
    {                                               //questions from
        rem = rand() % found;                       //possible array
        for (int i = rem; i < found - 1; i++)       //until required
            possible[i] = possible[i + 1];          //number is left
        found--;
    }
    fprintf(op, "NUM:\n");                          //Section heading
    for (int i = 0; i < found; i++)                 //Prints
    {                                               //each
        fprintf(op, "%d) ", i + 1);                 //question
        fileput_NUM(op, possible[i]);
    }
    fprintf(op, "--------------------\n");          //End of section
}

void fileput_NUM(FILE *op, NUM *N)
{
    srand(time(0));
    int r, corr = 0, wrong = 0;
    fprintf(op, "%s\n", N->text);               //Only print question
}
