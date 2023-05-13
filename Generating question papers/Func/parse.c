#include "../Headers/functions.h"
#include "../Headers/stack.h"
#include "../Headers/structs.h"

//###################################################//
//###################################################//
//#### Opening the sample file of the particular  ###//
//####  part while reading this code extends the  ###//
//#### understanding of the code even furthur.    ###//
//###################################################//
//###################################################//

Bank *parse_bank(FILE *B)
{
    Bank *bk = init_bank();
    char c;
    Stack part = create_empty();
    Stack brack = create_empty();
    char wd[10];
    fscanf(B, "%c", &c);
    while (1)
    {
        if (c == 13)                            //termination condition; CR character
            break;
        if (c == '\\')
        {
            for (int i = 0; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "begin"))           //Checks for "\begin"
            {
                printf("Unrecognised sequence \\%s in place of \\begin{type} in question bank\n", wd);
                exit(0);
            }
            fscanf(B, "%c", &c);
            if (c == '{')                      //Pushes '{' onto stack and enters next if
                push(brack, '{');
            else
            {
                printf("Unrecognised sequence \\begin%c in place of \\begin{type} in question bank\n", c);
                exit(0);
            }
        }

        if (top(brack) == '{')
        {
            for (int i = 0; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "type="))        //Checks for "\begin{type="
            {
                printf("Unrecognised sequence \\begin{%s in place of \\begin{type= in question bank\n", wd);
                exit(0);
            }
            push(part, 't');               //Indicates that the type is about to be read and the question is yet to be
            int j = 0;
            fscanf(B, "%c", &wd[j++]);       // Loop to take
            while (wd[j - 1] != '}')         // characters as
            {                                // input until '}'
                fscanf(B, "%c", &wd[j++]);   // is encountered.
            }                                // This should be
            wd[j - 1] = '\0';                // the type.
            pop(brack);
            fscanf(B, "%c", &c);             // Reads the first non-space character following [should be \]
            fseek(B, -1L, SEEK_CUR);         // and rewinds so as to be able to read it from inside parse_<type>()
            if (!strcmp(wd, "mcq"))
            {
                bk->mcq_list = parse_MCQ(B, part);      //reads from \begin{question;<diff>} until \end{type} and stores
            }
            if (!strcmp(wd, "fitb"))
            {
                bk->fitb_list = parse_FITB(B, part);    //reads from \begin{question;<diff>} until \end{type} and stores
            }
            if (!strcmp(wd, "num"))
            {
                bk->num_list = parse_NUM(B, part);      //reads from \begin{question;<diff>} until \end{type} and stores
            }
            if (!strcmp(wd, "tf"))
            {
                bk->tf_list = parse_TF(B, part);        //reads from \begin{question;<diff>} until \end{type} and stores
            }
        }

        fscanf(B, " %c", &c);       //Reads from after \end{type} until the next \begin{type=<type>}, if any
    }

    fclose(B);                      //Closes file

    return bk;
}

ListMCQ parse_MCQ(FILE *B, Stack part)
{
    char c;
    char wd[20];
    char text[50];
    float diff;
    int pos, opcount;
    Stack brack = create_empty();
    ListMCQ L = init_MCQ();
    MCQ *M = init_MCQ();

    fscanf(B, " %c", &c);
    while (1)
    {
        if (c == '\\')
        {
            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (!strcmp(wd, "end"))     // Checks for "\end"
            {
                fscanf(B, "%c", &c);
                if (c == '{')
                {
                    push(brack, c);
                    continue;
                }
            }
            for (int i = 3; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "begin"))        // Checks for "\begin"
            {
                printf("Unrecognised sequence \\%s in question bank\n", wd);
                exit(0);
            }
            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, c);
                continue;
            }
        }

        if (top(brack) == '{')
        {
            if (top(part) == 't')       // 't' represents type
            {
                for (int i = 0; i < 4; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[4] = '\0';
                if (!strcmp(wd, "type"))    // Checks for "type"
                {
                    fscanf(B, "%c", &c);
                    if (c != '}')
                    {
                        printf("Unrecognised sequence \\end{%s%c in place of \\end{type} in question bank\n", wd, c);
                        exit(0);
                    }
                    pop(brack);
                    pop(part);
                    break;
                }
                for (int i = 4; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))     //Checks for "\begin{question"
                {
                    printf("Unrecognised part {%s in place of \\begin{question; in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c != ';')
                {
                    printf("Unrecognised difficulty delimiter %c in question bank\n", c);
                    exit(0);
                }

                fscanf(B, "%f", &diff);     // reads the difficulty of the question
                M->diff = diff;

                fscanf(B, "%c", &c);
                if (c == '}')           // completes the reading of 
                {                       // "\begin{question;diff}"
                    pop(brack);
                }
                else
                {
                    printf("Difficulty NaN in question bank\n");
                    exit(0);
                }
                push(part, 'q');        // 'q' represents question
            }

            else if (top(part) == 'q')
            {
                for (int i = 0; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))     // checks "question"
                {
                    printf("Unrecognised question terminator {%s in place of {question} in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c == '}')
                {
                    pop(brack);
                }
                else
                {
                    printf("Unrecognised question terminator {%s%c in place of {question} in question bank\n", wd, c);
                    exit(0);
                }
                pop(part);
            }
        }

        if (top(part) == 'q')
        {
            fscanf(B, " %c", &c);
            if (c != '\"')
            {
                printf("\" missing before question text; difficulty %f\n", M->diff);
                exit(0);
            }
            pos = 0;
            fscanf(B, "%c", &c);
            while (c != '\"')           // starts reading the questions' text
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->text = (char *)malloc(strlen(text) * sizeof(char));
            strcpy(M->text, text);      // copies the questions' text to linked lists' node

            fscanf(B, " %c%c", &wd[0], &wd[1]);     // trying to read the "\\" after the 
            wd[2] = '\0';                           // question's text.

            if (strcmp(wd, "\\\\"))     
            {
                printf("Unrecognised question/option delimiter %s in question bank; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            opcount = 0;
            fscanf(B, " %c", &c);       // reading the " before the first correct option
            if (c != '\"')
            {
                printf("\" missing before correct option 1 in question bank; question \"%s\"\n", M->text);
                exit(0);
            }
            pos = 0;
            fscanf(B, "%c", &c);
            while (c != '\"')               // reading the option
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->corr = (char **)malloc(sizeof(char *));
            M->corr[0] = (char *)malloc(strlen(text) * sizeof(char));
            strcpy(M->corr[0], text);                            // copying opton to the linked list
            opcount++;                                           // increasing the count of the options

            fscanf(B, " %c", &c);               // checking the "&" which is
                                                // being used to separate options 
            while (c == '&')
            {
                fscanf(B, " %c", &c);           // reading the " before the option
                if (c != '\"')
                {
                    printf("\" missing before correct option %d in question bank; question \"%s\"\n", opcount, M->text);
                    exit(0);
                }
                pos = 0;
                fscanf(B, "%c", &c);
                while (c != '\"')               // reading the option
                {
                    text[pos++] = c;
                    fscanf(B, "%c", &c);
                }
                text[pos] = '\0';
                M->corr = (char **)realloc(M->corr, (opcount + 1) * sizeof(char *));
                M->corr[opcount] = (char *)malloc(strlen(text) * sizeof(char));
                strcpy(M->corr[opcount++], text);       // copying the option to the linked list
                                                        // and increasing the count of the options
                fscanf(B, " %c", &c);
            }

            M->no_corr = opcount;           // till now read only correct options
                                            // hence total options = correct options 

            if (c != '\\')
            {
                printf("Unrecognised correct/wrong option delimiter %c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            fscanf(B, "%c", &c);
            if (c != '\\')
            {
                printf("Unrecognised correct/wrong option delimiter \\%c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            opcount = 0;                    // intialisation to 0 as will be used again for wrong options

            fscanf(B, " %c", &c);
            M->wrong = (char **)malloc(sizeof(char *));
            if (c == '\"')
            {
                fseek(B, -1L, SEEK_CUR);            // goes back 1 letter in the file
                c = '&';
                while (c == '&')
                {
                    fscanf(B, " %c", &c);
                    if (c != '\"')
                    {
                        printf("\" missing before wrong option %d in question bank; question \"%s\"\n", opcount + 1, M->text);
                        exit(0);
                    }
                    pos = 0;
                    fscanf(B, "%c", &c);
                    while (c != '\"')          // starts reading the wrong options
                    {
                        text[pos++] = c;
                        fscanf(B, "%c", &c);
                    }
                    text[pos] = '\0';
                    M->wrong = (char **)realloc(M->wrong, (opcount + 1) * sizeof(char *));
                    M->wrong[opcount] = (char *)malloc(strlen(text) * sizeof(char));
                    strcpy(M->wrong[opcount++], text);      // coppying the test of wrong option to the linked list

                    fscanf(B, " %c", &c);        //checks if more options are available 
                }
            }
            M->no_ops = M->no_corr + opcount;               // updates the total options count in the linked list

            InsertMCQ(L, M);            // updates the linked list with insertion of new entries

            if (c != '\\')
            {
                printf("Unrecognised question terminator %c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (strcmp(wd, "end"))
            {
                printf("Unrecognised question terminator sequence \\%s; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            M = init_MCQ();             // initialsation to dummy state again

            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, '{');
                continue;
            }
        }

        fscanf(B, " %c", &c);   // will read the "\" before begin or end
    }

    return L;
}

ListFITB parse_FITB(FILE *B, Stack part)
{
    char c;
    char *ans;
    char wd[10];
    char text[50];
    float diff;
    int pos;
    Stack brack = create_empty();
    ListFITB L = init_FITB();
    FITB *M = init_FITB();

    fscanf(B, " %c", &c);
    while (1)
    {
        if (c == '\\')
        {
            for (int i = 0; i < 3; i++)         
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (!strcmp(wd, "end"))         // Checks for "\end"
            {
                fscanf(B, "%c", &c);
                if (c == '{')
                {
                    push(brack, c);
                    continue;
                }
            }
            for (int i = 3; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "begin"))        // Checks for "\begin"
            {
                printf("Unrecognised sequence \\%s in question bank\n", wd);
                exit(0);
            }
            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, c);
                continue;
            }
        }

        if (top(brack) == '{')
        {
            if (top(part) == 't')       // 't' represents type
            {
                for (int i = 0; i < 4; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[4] = '\0';
                if (!strcmp(wd, "type"))               // Checks for "type"
                {
                    fscanf(B, "%c", &c);        
                    if (c != '}')
                    {
                        printf("Unrecognised sequence \\end{%s%c in place of \\end{type} in question bank\n", wd, c);
                        exit(0);
                    }
                    pop(brack);
                    pop(part);
                    break;
                }
                for (int i = 4; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))               //Checks for "question"
                {
                    printf("Unrecognised part {%s in place of \\begin{question; in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c != ';')
                {
                    printf("Unrecognised difficulty delimiter %c in question bank\n", c);
                    exit(0);
                }

                fscanf(B, "%f", &diff);     // reads the difficulty of the question
                M->diff = diff;             //copying the difficulty to the linked list

                fscanf(B, "%c", &c);
                if (c == '}')
                {
                    pop(brack);
                }
                else
                {
                    printf("Difficulty NaN in question bank\n");
                    exit(0);
                }
                push(part, 'q');
            }

            else if (top(part) == 'q')
            {
                for (int i = 0; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))         //checks for "question"
                {
                    printf("Unrecognised question terminator {%s in place of {end} in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);            
                if (c == '}')                           // completes the reading of 
                {                                       // "\begin{question;diff}"
                    pop(brack);
                }
                else
                {
                    printf("Unrecognised question terminator {%s%c in place of {end} in question bank\n", wd, c);
                    exit(0);
                }
                pop(part);
            }
        }

        if (top(part) == 'q')
        {
            fscanf(B, " %c", &c);
            if (c != '\"')
            {
                printf("\" missing before question text; difficulty %f\n", M->diff);
                exit(0);
            }

            pos = 0;
            fscanf(B, "%c", &c);
            while (c != '\"')           // starts reading the questions' text
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->text = (char *)malloc((strlen(text) + 1) * sizeof(char));
            strcpy(M->text, text);      // copies the questions' text to linked lists' node

            fscanf(B, " %c%c", &wd[0], &wd[1]);         // trying to read the "\\" after the 
            wd[2] = '\0';                               // questions's text

            if (strcmp(wd, "\\\\"))
            {
                printf("Unrecognised question/answer delimiter %s in question bank; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            fscanf(B, " %c", &c);
            if (c != '\"')
            {
                printf("\" missing before answer in question bank; question \"%s\"\n", M->text);
                exit(0);
            }

            pos = 0;
            fscanf(B, "%c", &c);        
            while (c != '\"')           // reads the correct option
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->ans = (char *)malloc((strlen(text) + 1) * sizeof(char));
            strcpy(M->ans, text);           // copies answer to the linked list

            InsertFITB(L, M);               // updates the linked list with insertion of new entries

            fscanf(B, " %c", &c);

            if (c != '\\')
            {
                printf("Unrecognised question terminator %c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (strcmp(wd, "end"))
            {
                printf("Unrecognised question terminator sequence \\%s; question \"%s\"\n", wd, M->text);
                exit(0);
            }
            
            M = init_FITB();                // initialsation to dummy state again

            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, '{');
                continue;
            }
        }

        fscanf(B, " %c", &c);               // will read the "\" before begin or end
    }
    return L;
}

ListTF parse_TF(FILE *B, Stack part)
{
    char c;
    char ans;
    char wd[10];
    char text[50];
    float diff;
    int pos;
    Stack brack = create_empty();
    ListTF L = init_TF();
    TF *M = init_TF();

    fscanf(B, " %c", &c);
    while (1)
    {
        if (c == '\\')
        {
            for (int i = 0; i < 3; i++) 
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (!strcmp(wd, "end"))                 // Checks for "\end"
            {
                fscanf(B, "%c", &c);
                if (c == '{')
                {
                    push(brack, c);
                    continue;
                }
            }
            for (int i = 3; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "begin"))                // Checks for "\begin"
            {
                printf("Unrecognised sequence \\%s in question bank\n", wd);
                exit(0);
            }
            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, c);
                continue;
            }
        }

        if (top(brack) == '{')
        {
            if (top(part) == 't')                   // 't' represents type
            {
                for (int i = 0; i < 4; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[4] = '\0';
                if (!strcmp(wd, "type"))            // Checks for "type" 
                {
                    fscanf(B, "%c", &c);
                    if (c != '}')
                    {
                        printf("Unrecognised sequence \\end{%s%c in place of \\end{type} in question bank\n", wd, c);
                        exit(0);
                    }
                    pop(brack);
                    pop(part);
                    break;
                }
                for (int i = 4; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))              //Checks for "question"
                {
                    printf("Unrecognised part {%s in place of \\begin{question; in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c != ';')
                {
                    printf("Unrecognised difficulty delimiter %c in question bank\n", c);
                    exit(0);
                }

                fscanf(B, "%f", &diff);                 // reads the difficulty of the question
                M->diff = diff;                         //copying the difficulty to the linked list

                fscanf(B, "%c", &c);
                if (c == '}')
                {
                    pop(brack);
                }
                else
                {
                    printf("Difficulty NaN in question bank\n");
                    exit(0);
                }
                push(part, 'q');
            }

            else if (top(part) == 'q')
            {
                for (int i = 0; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))             //checks for "question"
                {
                    printf("Unrecognised question terminator {%s in place of {end} in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c == '}')                           // completes the reading of 
                {                                       // "\begin{question;diff}"
                    pop(brack);
                }
                else
                {
                    printf("Unrecognised question terminator {%s%c in place of {end} in question bank\n", wd, c);
                    exit(0);
                }
                pop(part);
            }
        }

        if (top(part) == 'q')
        {
            fscanf(B, " %c", &c);
            if (c != '\"')
            {
                printf("\" missing before question text; difficulty %f\n", M->diff);
                exit(0);
            }

            pos = 0;
            fscanf(B, "%c", &c);
            while (c != '\"')                       // starts reading the questions' text
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->text = (char *)malloc((strlen(text) + 1) * sizeof(char));
            strcpy(M->text, text);                  // copies the questions' text to linked lists' node

            fscanf(B, " %c%c", &wd[0], &wd[1]);     
            wd[2] = '\0';

            if (strcmp(wd, "\\\\"))                     //read the "\\" after the questions text
            {
                printf("Unrecognised question/answer delimiter %s in question bank; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            fscanf(B, " %c", &ans); 
            M->ans = ans;                           // copies answer to the linked list    

            InsertTF(L, M);                         // updates the linked list with insertion of new entries

            fscanf(B, " %c", &c);

            if (c != '\\')
            {
                printf("Unrecognised question terminator %c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (strcmp(wd, "end"))
            {
                printf("Unrecognised question terminator sequence \\%s; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            M = init_TF();                           // initialsation to dummy state again

            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, '{');
                continue;
            }
        }

        fscanf(B, " %c", &c);                       // will read the "\" before begin or end
    }

    return L;
}

ListNUM parse_NUM(FILE *B, Stack part)
{
    char c;
    int ans;
    char wd[10];
    char text[50];
    float diff;
    int pos;
    Stack brack = create_empty();
    ListNUM L = init_NUM();
    NUM *M = init_NUM();

    fscanf(B, " %c", &c);
    while (1)
    {
        if (c == '\\')
        {
            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (!strcmp(wd, "end"))                     // Checks for "\end"
            {
                fscanf(B, "%c", &c);
                if (c == '{')
                {
                    push(brack, c);
                    continue;
                }
            }
            for (int i = 3; i < 5; i++)
                fscanf(B, "%c", &wd[i]);
            wd[5] = '\0';
            if (strcmp(wd, "begin"))                    // Checks for "\begin"
            {
                printf("Unrecognised sequence \\%s in question bank\n", wd);
                exit(0);
            }
            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, c);
                continue;
            }
        }

        if (top(brack) == '{')
        {
            if (top(part) == 't')                       // 't' represents type
            {
                for (int i = 0; i < 4; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[4] = '\0';
                if (!strcmp(wd, "type"))                // Checks for "type" 
                {
                    fscanf(B, "%c", &c);
                    if (c != '}')
                    {
                        printf("Unrecognised sequence \\end{%s%c in place of \\end{type} in question bank\n", wd, c);
                        exit(0);
                    }
                    pop(brack);
                    pop(part);
                    break;
                }
                for (int i = 4; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))             //Checks for "question"
                {
                    printf("Unrecognised part {%s in place of \\begin{question; in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c != ';')
                {
                    printf("Unrecognised difficulty delimiter %c in question bank\n",c);
                    exit(0);
                }

                fscanf(B, "%f", &diff);                     // reads the difficulty of the question
                M->diff = diff;                             //copying the difficulty to the linked list

                fscanf(B, "%c", &c);
                if (c == '}')
                {
                    pop(brack);
                }
                else
                {
                    printf("Difficulty NaN in question bank\n");
                    exit(0);
                }
                push(part, 'q');
            }

            else if (top(part) == 'q')
            {
                for (int i = 0; i < 8; i++)
                    fscanf(B, "%c", &wd[i]);
                wd[8] = '\0';
                if (strcmp(wd, "question"))                 //checks for "question"
                {
                    printf("Unrecognised question terminator {%s in place of {end} in question bank\n", wd);
                    exit(0);
                }
                fscanf(B, "%c", &c);
                if (c == '}')                               // completes the reading of 
                {                                           // "\begin{question;diff}"
                    pop(brack);
                }
                else
                {
                    printf("Unrecognised question terminator {%s%c in place of {end} in question bank\n", wd, c);
                    exit(0);
                }
                pop(part);
            }
        }

        if (top(part) == 'q')
        {
            fscanf(B, " %c", &c);
            if (c != '\"')              
            {
                printf("\" missing before question text; difficulty %f\n", M->diff);
                exit(0);
            }

            pos = 0;
            fscanf(B, "%c", &c);
            while (c != '\"')                       // starts reading the questions' text
            {
                text[pos++] = c;
                fscanf(B, "%c", &c);
            }
            text[pos] = '\0';
            M->text = (char *)malloc((strlen(text) + 1) * sizeof(char));
            strcpy(M->text, text);                  // copies the questions' text to linked lists' node

            fscanf(B, " %c%c", &wd[0], &wd[1]);
            wd[2] = '\0';

            if (strcmp(wd, "\\\\"))                 //read the "\\" after the questions text
            {
                printf("Unrecognised question/answer delimiter %s in question bank; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            fscanf(B, " %d", &ans);
            M->ans = ans;                           // copies answer to the linked list

            InsertNUM(L, M);                        // updates the linked list with insertion of new entries

            fscanf(B, " %c", &c);

            if (c != '\\')
            {
                printf("Unrecognised question terminator %c in question bank; question \"%s\"\n", c, M->text);
                exit(0);
            }

            for (int i = 0; i < 3; i++)
                fscanf(B, "%c", &wd[i]);
            wd[3] = '\0';
            if (strcmp(wd, "end"))
            {
                printf("Unrecognised question terminator sequence \\%s; question \"%s\"\n", wd, M->text);
                exit(0);
            }

            M = init_NUM();                         // initialsation to dummy state again

            fscanf(B, "%c", &c);
            if (c == '{')
            {
                push(brack, '{');
                continue;
            }
        }

        fscanf(B, " %c", &c);                       // will read the "\" before begin or end
    }

    return L;
}

Paper *parse_paper(FILE *P)
{
    Paper *pr = init_paper();
    char c;
    int k;
    char wd[10];
    fscanf(P, "%c", &c);
    while (1)
    {
        if (c == '}')                   //termination condition; '}' character
            break;
        if (c != '\\')
        {
            printf("Unrecognised beginning character %c in question paper file\n", c);
            exit(0);
        }

        for (int i = 0; i < 6; i++)
            fscanf(P, "%c", &wd[i]);
        wd[6] = '\0';
        if (strcmp(wd, "sample"))       //Checks for "\sample"
        {
            printf("Unrecognised sequence \\%s in place of \\sample in question paper file\n", wd);
            exit(0);
        }

        fscanf(P, "%c", &c);
        if (c != '{')
        {
            printf("Unrecognised sequence \\sample%c in place of \\sample{ in question paper file\n", c);
            exit(0);
        }
        for (int i = 0; i < 5; i++)
            fscanf(P, "%c", &wd[i]);
        wd[5] = '\0';
        if (strcmp(wd, "type="))        //Checks for "\sample{type="
        {
            printf("Unrecognised sequence {%s in place of {type= in question paper file\n", wd);
            exit(0);
        }
        fscanf(P, "%c", &c);            //Reads first character of type and proceeds
        if (c == 'm')
        {
            wd[0] = 'm';
            fscanf(P, "%c%c", &wd[1], &wd[2]);
            wd[3] = '\0';
            if (strcmp(wd, "mcq"))      //Makes sure type is "mcq"
            {
                printf("Unrecognised type %s\n", wd);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '(')
            {
                printf("No_ops not given for mcq\n");
                exit(0);
            }
            for (k = 0; k < 10; k++)                // Finds first
                if (pr->mcq_reqs[k]->no_ops == 0)   // non-empty mcq_req
                    break;                          // in the paper
            if (k == 10)                    //All mcq_reqs are filled
            {
                printf("Too many mcq requests!\n");
                exit(0);
            }
            fscanf(P, "%d", &pr->mcq_reqs[k]->no_ops); //Reads mcq(no_ops)
            fscanf(P, "%c", &c);
            if (c != ')')
            {
                printf("No_ops NaN for mcq(%d)\n", pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%c", &c);          
            if (c != '}')               //Reads till "\sample{type=mcq(no_ops)}"
            {
                printf("Intervening characters between ) and } in mcq(%d) in question paper file\n", pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between } and { in mcq(%d) in question paper file\n", pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%c%c", &wd[0], &wd[1]);
            wd[2] = '\0';
            if (strcmp(wd, "#="))       //Reads "{#="
            {
                printf("Unrecognised sequence %s in place of #=  in mcq(%d) in question paper file\n", wd, pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%d", &pr->mcq_reqs[k]->no_req);
            fscanf(P, "%c", &c);
            if (c != '}')               //Reads till "{#=no_req}"
            {
                printf("Intervening characters between #=%d and } in mcq(%d) in question paper file\n", pr->mcq_reqs[k]->no_req, pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between } and { in mcq(%d) in question paper file\n", pr->mcq_reqs[k]->no_ops);
                exit(0);
            }

            for (int i = 0; i < 8; i++)
                fscanf(P, "%c", &wd[i]);
            wd[8] = '\0';
            if (strcmp(wd, "diff in "))     //Reads "{diff in "
            {
                printf("Unrecognised sequence %s in place of diff in mcq(%d) in question paper file\n", wd, pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%f", &pr->mcq_reqs[k]->diff_lb); //Reads LB
            fscanf(P, "%c", &c);
            if (c != ',')                   //Reads separator ,
            {
                printf("Unrecognised LB/UB delimiter %c in mcq(%d) in question paper file\n", c, pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
            fscanf(P, "%f", &pr->mcq_reqs[k]->diff_ub); //Reads LB
            fscanf(P, "%c", &c);
            if (c != '}')                    //Reads till "{diff in LB,UB}"
            {
                printf("UB NaN in mcq(%d)\n", pr->mcq_reqs[k]->no_ops);
                exit(0);
            }
        }
        if (c == 'f')
        {
            wd[0] = 'f';
            fscanf(P, "%c%c%c", &wd[1], &wd[2], &wd[3]);
            wd[4] = '\0';
            if (strcmp(wd, "fitb"))     //Makes sure type is "fitb"
            {
                printf("Unrecognised type %s\n", wd);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '}')               //Reads till "\sample{type=fitb}"
            {
                printf("Unrecognised type fitb%c\n", c);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening character %c before } and {\n", c);
                exit(0);
            }
            fscanf(P, "%c%c", &wd[0], &wd[1]);
            wd[2] = '\0';
            if (strcmp(wd, "#="))       //Reads "#="
            {
                printf("Unrecognised sequence %s in place of #= in fitb in question paper\n", wd);
                exit(0);
            }
            fscanf(P, "%d", &pr->fitb_reqs->no_req);
            fscanf(P, "%c", &c);
            if (c != '}')               //Reads till "{#=no_req}"
            {
                printf("No_req NaN in fitb in question paper\n");
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between #=%d} and { in fitb in question paper\n", pr->fitb_reqs->no_req);
                exit(0);
            }

            for (int i = 0; i < 8; i++)
                fscanf(P, "%c", &wd[i]);
            wd[8] = '\0';
            if (strcmp(wd, "diff in "))     //Reads "{diff in"
            {
                printf("Unrecognised sequence %s in place of diff in fitb in question paper\n", wd);
                exit(0);
            }
            fscanf(P, "%f", &pr->fitb_reqs->diff_lb); //Reads LB
            fscanf(P, "%c", &c);
            if (c != ',')
            {
                printf("Unrecognised LB/UB delimiter %c in fitb in question paper\n", c);
                exit(0);
            }
            fscanf(P, "%f", &pr->fitb_reqs->diff_ub); //Reads UB
            fscanf(P, "%c", &c);
            if (c != '}')               //Reads till "{diff in LB,UB}"
            {
                printf("UB NaN in fitb in question paper\n");
                exit(0);
            }
        }
        if (c == 't')
        {
            wd[0] = 't';
            fscanf(P, "%c", &wd[1]);
            wd[2] = '\0';
            if (strcmp(wd, "tf"))       //Makes sure type is "tf"
            {
                printf("Unrecognised type %s in the question paper\n", wd);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '}')       //Reads till "\sample{type=tf}"
            {
                printf("Unrecognised type tf%c in the question paper\n", c);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between tf and { in question paper file\n");
                exit(0);
            }
            fscanf(P, "%c%c", &wd[0], &wd[1]);
            wd[2] = '\0';
            if (strcmp(wd, "#="))   //Reads "#="
            {
                printf("Unrecognised sequence %s in place of #= in tf in question paper file\n", wd);
                exit(0);
            }
            fscanf(P, "%d", &pr->tf_reqs->no_req);
            fscanf(P, "%c", &c);
            if (c != '}')           //Reads till "{#=no_req}"
            {
                printf("No_req in tf in question paper file NaN");
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between } and { in fitb in question paper file\n");
                exit(0);
            }

            for (int i = 0; i < 8; i++)
                fscanf(P, "%c", &wd[i]);
            wd[8] = '\0';
            if (strcmp(wd, "diff in "))     //Reads "{diff in"
            {
                printf("Unrecognised sequence %s\n", wd);
                exit(0);
            }
            fscanf(P, "%f", &pr->tf_reqs->diff_lb); //Reads LB`
            fscanf(P, "%c", &c);
            if (c != ',')
            {
                printf("Unrecognised LB/UB delimiter %c\n", c);
                exit(0);
            }
            fscanf(P, "%f", &pr->tf_reqs->diff_ub); //Reads UB
            fscanf(P, "%c", &c);
            if (c != '}')           //Reads till "{diff in LB,UB}"
            {
                printf("UB NaN\n");
                exit(0);
            }
        }
        if (c == 'n')
        {
            wd[0] = 'n';
            fscanf(P, "%c%c", &wd[1], &wd[2]);
            wd[3] = '\0';
            if (strcmp(wd, "num"))          //Makes sure type is "num"
            {
                printf("Unrecognised type %s\n", wd);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '}')           //Reads till "\sample{type=num}"
            {
                printf("Unrecognised type num%c\n", c);
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between } and {\n");
                exit(0);
            }
            fscanf(P, "%c%c", &wd[0], &wd[1]);
            wd[2] = '\0';
            if (strcmp(wd, "#="))       //Reads "#="
            {
                printf("Unrecognised sequence %s in place of #= in num in question paper\n", wd);
                exit(0);
            }
            fscanf(P, "%d", &pr->num_reqs->no_req);
            fscanf(P, "%c", &c);
            if (c != '}')           //Reads till "{#=no_req}"
            {
                printf("No_req NaN in num in question paper");
                exit(0);
            }
            fscanf(P, "%c", &c);
            if (c != '{')
            {
                printf("Intervening characters between #=%d} and { in num in question paper\n",pr->num_reqs->no_req);
                exit(0);
            }

            for (int i = 0; i < 8; i++)
                fscanf(P, "%c", &wd[i]);
            wd[8] = '\0';
            if (strcmp(wd, "diff in "))     //Reads "{diff in"
            {
                printf("Unrecognised sequence %s in place of diff in num in question paper\n", wd);
                exit(0);
            }
            fscanf(P, "%f", &pr->num_reqs->diff_lb); //Reads LB
            fscanf(P, "%c", &c);
            if (c != ',')
            {
                printf("Unrecognised LB/UB delimiter %c in num in question paper\n", c);
                exit(0);
            }
            fscanf(P, "%f", &pr->num_reqs->diff_ub); //Reads UB
            fscanf(P, "%c", &c);
            if (c != '}')       //Reads till "{diff in LB,UB}"
            {
                printf("UB NaN in num in question paper\n");
                exit(0);
            }
        }

        fscanf(P, " %c", &c);       //Starts reading next request, if any
    }

    fclose(P);          //Closes file

    return pr;
}
