# ExamTeX
DSA Miniproject Report, Spring 2021, IIIT Hyderabad

Group number: 17  
Group members:  
* Nukit Tailor (2020114012)  
* Abhinav S Menon (2020114001)  
* Sreya Garapati (2020102055)  
* Tarun Jindal (2020102056)  

## Data Structures Used
Each question type is represented by a different struct, containing all the necessary information. There is also a struct for the question bank, and one for the question paper. Further, there is a struct for a "request" of a number of questions of a certain difficulty and (in the case of MCQs) having a certain number of options.  
In the question bank struct, a **linked list** of each type is included. Questions are inserted _in the front_ of their corresponding type's LL as they are read.  
In the question paper struct, there is a single request for each of fill-in-the-blank, true/false, and numerical types. For MCQs, there is an **array** of 10 requests, to accommodate the possibility of different MCQs having different number of options in the final output.  

In the parsing process, two **stacks** of characters are used: one for brackets, and one for maintaing if a question is to be read (*i.e.* a type and some of its questions have been fully read) or if a question is being read.  

Alll structs use only pointers for non-numerical fields, in order to save time when passing structs to functions.

## Algorithm
### Question Bank Parsing
Once the file is opened, the characters are read one by one. If a character is a backslash, the sequence following it should be `begin`; if it is, it is read and the opening bracket following it is pushed onto a stack.  
If the top of the bracket stack is an opening bracket, the word sequence `type=` is expected. If it appears, `t` is pushed on the "part" stack (indicating that the type is about to be read and a question will be read next). Then the type is read, the closing bracket is read (and the opening bracket popped off the bracket stack). Once the type is known, the corresponding parser function is called to read the questions.  
In each question, the backslash is read first, followed by the sequence `begin` and an opening bracket, which is pushed onto the bracket stack. If the top of the bracket stack is an opening bracket, and the top of the "part" stack is `t`, the sequence `question;` is read, followed by its difficulty and the closing bracket (and a pop off the bracket stack). Thereafter, `q` is pushed onto the "part" stack, and the question is parsed according to the type.
Then, the sequence `\end` is read and the opening bracket following it is pushed on the stack once more. Then, since `q` is still on top of the "part" stack, the `question` sequence is expected. Once this and the corresponding closing bracket are read and popped, `q` is popped off the part stack; now, since `t` is on top, `\end{type}` is read and it is popped.  
Here the control returns to the `parse_bank()` function, and continues to parse other types of questions, finally storing them all in the question bank struct.  

### Question Paper Parsing
Once the paper is opened, the characters are read one by one. If a character is a backslash, it must be followed by `sample`; this is read, followed by its opening bracket and the sequence `type=`, and then the type. Since all the types start with different letters, the first letter is read and checked. Inside the `if` statement of each type, subsequent letters are verified. The number of options is taken in at this point (in case of MCQs; also in this case, the first unread request also needs to be found); then the bracket is closed and the next opening bracket is read. Here the sequence `#=` followed by the number of questions required is expected, and a closing bracket.  
The last bracket is the difficulty range; this begins with the sequence `diff in `, followed by the lower and upper bounds separated by a comma, and a closing bracket. Then the next request is read and parsed.  

### Output File Generation
For each request, the bank is filtered into an array of questions satisfying the conditions, keeping track of the number. If the total is less than the number requested, nothing of the type is printed; otherwise, the requested number is randomly selected from the array. Then each question is printed.  
In case of MCQs, the selection as well as the order of correct and wrong options is randomised.  

## Future Versions
Some features that can be added to this project in the future are:  
* Marks file  
    Since the correct answer to each question is stored in the bank, answer sheets (in a certain format) can be parsed and graded. According to the percentage of students who got a certain question correct, that questions difficulty "score" can be updated. Based on this score and the predicted difficulty, a new difficulty can be calculated.   
    
* Line numbers. 
    The position of syntax errors in the bank and paper files can be made more clear by displaying the line number. In C, this means counting the number of newline characters until the position of the error.   
    
* Appending questions to output file  
    When an existing filename is given as the output filename, it is erased and the questions are added. It is possible, however, to insert the new questions in the relevant sections of the output file if it already has some (*i.e.*, new true/false questions in the true/false section, and so on).  
    
* Avoid repetition of questions  
    If MCQs of two different numbers of options are requested (say $m$ and $n$), it is possible for a question having total numbers of options greater than $m$ and $n$ to be added in *both* the sections. This is not desirable; if the program can keep track of which MCQs have been printed in a certain paper, it can be avoided.


## Division of Work
Each member worked on the functions relating to a certain question type:  
* MCQ: Abhinav  
* Numerical: Nukit  
* True/False: Sreya  
* Fill-in-the-blanks: Tarun.  

For each type, these were the functions made:
* initialisation (`init_type()`)
* node-creation (`create_type()`)
* insertion (`insertType()`) 
* finding (`find_type()`) 
* selection (`select_type()`)
* printing (`fileput_type()`).

The stack functions were also split among the members:  
* `push()` and `top()`: Abhinav  
* `create_node()`: Tarun  
* `create_empty()`: Sreya  
* `pop()`: Nukit  

`main()`, `parse_bank()` and `parse_paper()` were written by Nukit and Abhinav.
