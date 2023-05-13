# ExamTeX
DSA Miniproject, Spring 2021, IIIT Hyderabad

## Overview
This project is meant to generate question papers. Given a set of questions in a question bank file and a set of "requests" for questions of each type, the project randomly picks out the required number from all the available questions (satisfying the conditions) and adds them to the output file. The types supported are MCQs (single- or multi-correct), fill-in-the-blanks, true/false and numerical questions.  
Requests can filter questions according to number of options (in the case of MCQs) and difficulty (upper bound and lower bound). Note that the MCQs generated can have anywhere between 1 and all options correct. If the number of questions satisfying the conditions is not enough, no questions of that type are printed.  
Questions of different types (and MCQs having different numbers of options) are printed in separate sections.  

## Syntax
The syntax of the question bank file is as follows: for each type,  

    \begin{type=<type>}
    <list of questions>
    \end{type}

In the list of questions, each question's format is as follows: for MCQs,  

    \begin{question;<diff>}
    "question_text" \\ "corr_op1" & "corr_op2" \\ "wrong_op1" & "wrong_op2"
    \end{question}
The `\\` after the correct options is mandatory even if there are no wrong options.

for fill-in-the-blanks,

    \begin{question;<diff>}
    "question_text" \\ "ans"
    \end{question}

for numerical,

    \begin{question;<diff>}
    "question_text" \\ ans
    \end{question}

for true/false,  

    \begin{question;<diff>}
    "question_text" \\ ans
    \end{question}



The syntax of the question paper file is as follows: a "request" for each type of question has the format  

    \sample{type=<type>}{#=<num>}{diff in <LB>,<UB>}

In the case of MCQs, the number of options required must be given in parentheses (e.g. `mcq(3)`).  

Note that wherever there are spaces, indentation or newlines in the format, the user may put any number (0 or more) of whitespace characters (spaces, tabs or newlines). However, where there are no spaces, spaces should not be inserted.  

## Running Test Cases 
For compiling the program, clone the repo, `cd` into the it and run the `make` command. To run it, execute `./a.out`. If you want to run one of the included tests, please enter the file path at the prompt as follows:

    Enter question bank filename: Sample/QBanks/test1.txt
    Enter question paper filename: Sample/QPapers/test1.txt
    Enter output filename: sampleoutput.txt

The output file will be generated in the PWD with: the name `sampleoutput.txt`.  

Please note that if you are running your own test, the filenames (or filepaths) should not contain spaces, even if preceded by `\`.

Various stages of successful running will appear as terminal output (bank parsed successfully, paper parsed succesfully, questions of each type added/not added). When each question type's "added" or "not added" message has appeared, the program terminates and the output file can be opened.
