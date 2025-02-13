#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// yanal omar
// 1230156
// Dr. Ahmad Abu snina
typedef struct node *linkedList; // for easier usage i used linkedList to refer to the struct

struct node
{                    // this struct is used to save terms for each polynimal
    int coefficient; // the coeffecient of the term
    int exponent;    // the degree of the term
    linkedList next; // a pointer to the next term;
};

typedef struct stackNode *stack; // changed the name for easier accessibility

struct stackNode
{                              // this struct is used to save each linked list(polynomial)
    linkedList polynomialList; // the head of each polynomial that was loaded from the file
    stack next;                // a pointer to the next stack node
};

// Time Complexity: O(1)
int isEmpty(stack head)
{                              // this function checks if the stack is empty
    return head->next == NULL; // As i made the head to just point to the first node and has value, if the next node is null it
    // returns 1 (true), if not, it returns 0 (false);
}

// Time Complexity: O(1)
void push(stack head, linkedList headOfPolynomial)
{ // this is one of the stack functions to make the node the top of the stack
    if (head == NULL)
    { // handling that the stack is not found
        printf("Error: stack not found\n");
        return;
    }
    if (headOfPolynomial == NULL)
    { // handling that the list is not found
        printf("Error: element you want to push is not found\n");
        return;
    }
    stack temp = (stack)malloc(sizeof(struct stackNode)); // create a stack node so we put it in our stack
    temp->polynomialList = headOfPolynomial;              // assign the head of the polynomial to the stack node
    temp->next = head->next;                              // the next of the temp is the next of the head because we always want to append the stack node to the beginning of the stack
    head->next = temp;                                    // make the temp the first element in the stack(after the head)
}

// Time Complexity: O(n)
void deleteList(linkedList head)
{ // this function is used for deleting a full linked list
    if (head == NULL)
    { // handling the list does not exist
        printf("Error: list to delete is not found\n");
        return;
    }
    linkedList current = head->next; // so we dont lose track of the head i assigned the next of the head to the current (the head has no value so we just skip it)
    while (current != NULL)
    {                              // a loop to go over every element in the list
        linkedList temp = current; // assign the current to the temp so because if we just freed the current we will lose access to the next node
        current = temp->next;      // make the current the next node, same as ( current=current->next)
        free(temp);                // delete the node from the memory
    }
    //*** this function does not delete the head, meaning the list is still there but empty
}

// Time Complexity: O(1)
void pop(stack head)
{ // this function is used to remove the top element if the stack
    if (head == NULL)
    { // handle the case where no stack exists
        printf("Error: Stack not found\n");
        return;
    }
    if (head->next == NULL)
    { // handle the case where the stack is empty
        printf("Error:  Stack is empty\n");
        return;
    }
    stack current = head->next; // save the top element in the stack
    head->next = current->next; // connect the head to the next element after the top of the stack
    free(current);              // remove the top of the stack from memory
}

// Time Complexity: O(n)
void removeSpaces(char str[])
{                     // this funcion is used to remove spaces from the line taken from the file to ensure best results
    int i = 0, j = 0; // initialize 2 pointers
    while (str[i] != '\0')
    { // while the string does not reach the end
        if (str[i] != ' ')
        { // if the character is not a space increment the move the ith character into the jth character after this increment j by 1
            str[j++] = str[i];
        }
        i++; // increment i in each loop
    }
    str[j] = '\0'; // assign the jth character to \0 so when we use the str it will stop here.
}

// Time Complexity: O(1)
stack top(stack head)
{ // this function is used return the element on top of the class
    if (head == NULL)
    { // handle the case where the stack does not exist
        printf("Error: Stack not found\n");
        return NULL;
    }
    if (head->next == NULL) // if the stack is empty return NULL
        return NULL;

    return head->next; // return the first element in the stack
}

// Time Complexity: O(1)
void popForExit(stack head)
{ // this function is similar to the pop but it also frees the linked lists (polynomials) linked to it
    if (top(head) == NULL)
    { // handle the case where the stack is empty
        return;
    }
    stack current = head->next;          // save the top element in the stack
    head->next = current->next;          // connect the head to the next element after the top of the stack
    deleteList(current->polynomialList); // move on each element in the polynomial and free it from memory
    free(current);                       // delete the current node from the memory
}

// Time Complexity: O(n)
void insert(linkedList head, int coefficient, int exponent)
{ // this function is used to insert nodes (terms of polynomials) to the linked list
    if (head == NULL)
    { // handle the case where the list does not exist
        printf("no list exists");
        return;
    }
    linkedList temp = (linkedList)malloc(sizeof(struct node)); // create a node
    temp->exponent = exponent;                                 // assign the exponent value to the node
    temp->coefficient = coefficient;                           // assign the coeffecient value to the node
    temp->next = NULL;                                         // assign the next to null (temporarily)
    if (head->next == NULL)
    {                      // if the list is empty
        head->next = temp; // assign the temp to the next of the head to keep the head empty
    }
    else
    {                              // else if it is not empty we want to look for the right place(explained below) so it be in descending order of exponent. ordering them help us maintain the best time complexity
        linkedList current = head; // assign the current to be the head so we dont lose track of the head
        while (current->next != NULL && current->next->exponent > exponent)
        { // while our exponent is smaller than the next to the current exponent move in the loop
            current = current->next;
        }
        if (current->next != NULL && current->next->exponent == exponent)
        {                                              // if the next of the current exponent equals the exponent we want to insert
            current->next->coefficient += coefficient; // just add the coefficients of the term
            free(temp);                                // delete the temp node as a node with the same exponent exists
            // this function helps in making a more efficient time complexity if the program. especially in the multiplying polynomials function.
        }
        else
        {                               // if the exponents arent equal
            temp->next = current->next; // make the next of the temp the currents next
            current->next = temp;       // make the currents next is the temp
        }
    }
}

// Time Complexity: O(n)
void print(linkedList head)
{ // print the linked list (polynomials)
    if (head == NULL)
    { // handle the case where the list doesnt exist
        printf("Error: no list exists\n");
        return;
    }
    if (head->next == NULL)
    { // handle the case where the list is empty
        printf("Error: list is empty\n");
        return;
    }
    linkedList temp = head->next; // assign the temp to next of the head because the head has no value
    int firstTerm = 1;            // i made this variable to check if the term is the first term so i dont put a + sign before it
    if (temp != NULL && temp->next == NULL && temp->coefficient == 0)
    {                  // if the list only has one term and its coefficient is 0
        printf("0\n"); // print 0
        // i made this because if the coeffecient is 0 it will not do anything, and i didnt want the result to jus be empty
        return;
    }
    // this part is almost self explainatory
    while (temp != NULL)
    {
        if (temp->coefficient == 0)
            temp = temp->next;
        else
        {
            if (temp->coefficient < 0)
            {
                printf("-");
                if (temp->coefficient != -1 || temp->exponent == 0)
                {
                    printf("%d", -1 * (temp->coefficient));
                }
            }
            else
            {
                if (!firstTerm)
                {
                    printf("+");
                }
                if (temp->coefficient != 1 || temp->exponent == 0)
                {
                    printf("%d", temp->coefficient);
                }
            }
            if (temp->exponent > 0)
            {
                printf("X");
                if (temp->exponent > 1)
                {
                    printf("^%d", temp->exponent);
                }
            }
            temp = temp->next;
            firstTerm = 0; // after the first loop set the firstTerm to 0
        }
    }
    printf("\n");
}

// Time Complexity: O(n^2)
void loadPolynomials(stack head, stack addStack, stack subStack, stack multStack)
{                                         // this function is used to load polynomials
    FILE *file = fopen("input.txt", "r"); // open the file named input.txt
    if (file == NULL)
    { // handle the case where the file is not found
        printf("Error: can't open file\n\n");
        return;
    }
    // these empty the stacks so if you pressed 1 to load polynomials multiple times, it only put them once in the stacks
    while (!isEmpty(head))
    {
        popForExit(head);
    }
    while (!isEmpty(addStack))
    {
        popForExit(addStack);
    }
    while (!isEmpty(subStack))
    {
        popForExit(subStack);
    }
    while (!isEmpty(multStack))
    {
        popForExit(multStack);
    }

    char line[1000];
    while (fgets(line, sizeof(line), file))
    {                       // while the file didnt reach the end scan the line
        removeSpaces(line); // remove spaces from the line taken

        linkedList polynomial = (linkedList)malloc(sizeof(struct node)); // create a nod
        polynomial->next = NULL;                                         // make it point to NULL
        // i declared these values outside the loop so it doesnt create a new variable in every loop
        int coefficient = 1;
        int exponent = 0;
        int sign = 1;
        int i = 0; // set a pointer
        while (line[i] != '\0' && line[i] != '\n')
        { // while it didnt reach the end of the line
            coefficient = 1;
            exponent = 0;
            sign = 1;

            if (line[i] == '-')
            { // if the character is - make the sign -1 and move to the next character
                sign = -1;
                i++;
            }
            else if (line[i] == '+')
            { // else if the character is + just increment i as the default sign value is positive
                i++;
            }

            if (line[i] >= '0' && line[i] <= '9')
            {                    // if the character is a number
                coefficient = 0; // set the coeffecient to zero
                while (line[i] >= '0' && line[i] <= '9')
                { // handle the case where its multiple digits
                    coefficient = coefficient * 10 + (line[i] - '0');
                    i++;
                }
            }
            coefficient *= sign; // multiply the coeffecient we scanned with the sign

            if (line[i] == 'x' || line[i] == 'X')
            {                 // if the character is x
                exponent = 1; // the default value of the exponent is 1
                //** example: 2x is coefficient 2 and exponent 1
                i++;
                if (line[i] == '^')
                { // if the line after the x is ^ then there is an exponent
                    i++;
                    exponent = 0; // set the exponent to zero to read the exponent correctly
                    while (line[i] >= '0' && line[i] <= '9')
                    { // read the exponent
                        exponent = exponent * 10 + (line[i] - '0');
                        i++;
                    }
                }
            }

            insert(polynomial, coefficient, exponent); // insert the term we just found to the list (coefficient and exponent)
        }

        push(head, polynomial);      // add the polynomial to the main stack
        push(addStack, polynomial);  // add the polynomial to the Add stack
        push(subStack, polynomial);  // add the polynomial to the Subtract stack
        push(multStack, polynomial); // add the polynomial to the Multiply stack
    }
    fclose(file);
    printf("Polynomials loaded successfully!\n\n");
}

// Time Complexity: O(n^2)
void printPolynomials(stack head)
{ // this function is used to print the stack
    // this function differs from the print function that this prints a whole stack and it uses the print function that print the linked list associated to the each stack node
    if (head == NULL)
    { // handle the case where the stack is not found
        printf("Error: Stack not found\n");
        return;
    }
    if (isEmpty(head))
    { // handle the case where the stack is empty
        printf("Error:  Stack is empty\n");
        return;
    }
    stack current = head; // assign the head to the current so we dont lose track of the head
    while (!isEmpty(current))
    {                                                   // while stack is not empty
        linkedList temp = top(current)->polynomialList; // assign the value of the polynomial of the top of the stack to a temp variable
        if (temp != NULL)                               // handle the case where the polynomial does not exist
            print(temp);                                // print the list
        pop(current);                                   // remove the top of the stack
    }
    printf("Polynomials printed successfully!\n\n");
}

// Time Complexity: O(n)
linkedList addTwoLists(linkedList list1, linkedList list2)
{ // this function recives 2 lists and add them
    if (list1 == NULL)
    { // handle the case where the first list is not found
        printf("Error: list 1 is not found\n");
        return;
    }
    if (list2 == NULL)
    { // handle the case where the second list is not found
        printf("Error: list 2 is not found\n");
        return;
    }
    linkedList result = (linkedList)malloc(sizeof(struct node)); // create a list that we want to save results in
    result->next = NULL;
    linkedList temp = result; // assign result to temp so we dont lose track of the head of the list when we want to return

    list1 = list1->next; // go to the second node because the first one is head and it doesnt contain data
    list2 = list2->next; // go to the second node because the first one is head and it doesnt contain data
    while (list1 != NULL || list2 != NULL)
    { // while both lists are not null
        if (list1 == NULL)
        { // if the first list is null insert the data of the the node (term) of the node we reached in list 2
            insert(temp, list2->coefficient, list2->exponent);
            list2 = list2->next; // move to the next node
        }
        else if (list2 == NULL)
        { // if the second list is null insert the data of the the node (term) of the node we reached in list 1
            insert(temp, list1->coefficient, list1->exponent);
            list1 = list1->next; // move to the next term
        }
        else if (list1->exponent == list2->exponent)
        {                                                      // if the exponents equal each other
            int sum = list1->coefficient + list2->coefficient; // save the sum of both coefficients in a variable
            insert(temp, sum, list1->exponent);                // insert the node to the temp (result) list
            // move to the next node in both lists
            list1 = list1->next;
            list2 = list2->next;
        }
        else if (list1->exponent > list2->exponent)
        { // if list1 node exponent is greater than the second, insert the data of the node we reached in list 1
            insert(temp, list1->coefficient, list1->exponent);
            list1 = list1->next; // move to the next node in list1
        }
        else
        { // if list2 node exponent is greater than the first, insert the data of the node we reached in list 2
            insert(temp, list2->coefficient, list2->exponent);
            list2 = list2->next; // move to the next node in list2
        }
    }
    return result; // return the result polynomial
}

// Time Complexity: O(n^2)
void addPolynomials(stack addStack)
{ // this function is used to all the polynomials in the stack
    if (addStack == NULL)
    { // handle the case where the addStack does not exist
        printf("Error: Stack is not found\n");
        return;
    }
    if (isEmpty(addStack))
    { // handle the case where the stack is empty
        printf("Error: Stack is empty\n");
        return;
    }
    while (top(top(addStack)) != NULL)
    { // while the stack has at least 2 elements
        // i dont know if im allowed to use it like this "top(top(addStack)" the top inside return the head->next, the top outside returns if the head->next->next is null
        linkedList list1 = top(addStack)->polynomialList; // save the polynomial of the top element in a linkedlist variable;
        pop(addStack);                                    // pop so we access the element after
        linkedList list2 = top(addStack)->polynomialList; // save the polynomial of the top element in a linkedlist variable;
        pop(addStack);                                    // pop so we access the element after
        push(addStack, addTwoLists(list1, list2));        // call the add two lists function and push the result of the two lists in the stack
    }
    printf("Polynomials added Successfully, enter 6 to see the result\n");
}

// Time Complexity: O(n)
linkedList subTwoLists(linkedList list1, linkedList list2)
{ // this function is the same as the add except for - instead of +
    if (list1 == NULL)
    {
        printf("Error: list 1 is not found\n");
        return;
    }
    if (list2 == NULL)
    {
        printf("Error: list 2 is not found\n");
        return;
    }
    linkedList result = (linkedList)malloc(sizeof(struct node));
    result->next = NULL;
    linkedList temp = result;

    list1 = list1->next;
    list2 = list2->next;
    while (list1 != NULL || list2 != NULL)
    {
        if (list1 == NULL)
        {
            insert(temp, (list2->coefficient) * -1, list2->exponent);
            list2 = list2->next;
        }
        else if (list2 == NULL)
        {
            insert(temp, list1->coefficient, list1->exponent);
            list1 = list1->next;
        }
        else if (list1->exponent == list2->exponent)
        {
            int subtracted = list1->coefficient - list2->coefficient;
            insert(temp, subtracted, list1->exponent);
            list1 = list1->next;
            list2 = list2->next;
        }
        else if (list1->exponent > list2->exponent)
        {
            insert(temp, list1->coefficient, list1->exponent);
            list1 = list1->next;
        }
        else
        {
            insert(temp, (list2->coefficient) * -1, list2->exponent);
            list2 = list2->next;
        }
        temp = temp->next;
    }

    return result;
}

// Time Complexity: O(n^2)
void subtractPolynomials(stack subStack)
{ // also same as addPolynomials except we call the subTwoLists function
    if (subStack == NULL)
    {
        printf("Error: Stack is not found\n");
        return;
    }
    if (isEmpty(subStack))
    {
        printf("Error: Stack is empty\n");
        return;
    }
    while (top(top(subStack)) != NULL)
    {
        linkedList list1 = top(subStack)->polynomialList;
        pop(subStack);
        linkedList list2 = top(subStack)->polynomialList;
        pop(subStack);
        push(subStack, subTwoLists(list1, list2));
    }

    printf("Polynomials Subtracted Successfully, enter 6 to see the result\n");
}

// Time Complexity: O(n^2)
linkedList multTwoLists(linkedList list1, linkedList list2)
{                                                              // this function multiplies two lists
    linkedList list = (linkedList)malloc(sizeof(struct node)); // create a list to save the result
    list->next = NULL;
    list1 = list1->next; // the head has no value so we move to the next
    list2 = list2->next; // the head has no value so we move to the next
    // multiply each term in the list to the every term in list2
    while (list1 != NULL)
    {
        linkedList current2 = list2; // so we dont lose track of the head of list 2 we assigned it to current2
        while (current2 != NULL)
        {
            insert(list, (list1->coefficient) * (current2->coefficient), (list1->exponent) + (current2->exponent)); // insert a term with coefficient1*coefficient2 and exponent1+exponent2
            current2 = current2->next;                                                                              // move to the next node in current2
        }
        list1 = list1->next; // move to the next node in list1
    }
    return list;
}

// Time Complexity: O(n^3)
void multiplyPolynomials(stack multStack)
{ // same as the addPolynomials function
    if (multStack == NULL)
    {
        printf("Error: Stack is not found\n");
        return;
    }
    if (isEmpty(multStack))
    {
        printf("Error: Stack is empty\n");
        return;
    }
    while (top(top(multStack)) != NULL)
    {
        linkedList list1 = top(multStack)->polynomialList;
        pop(multStack);
        linkedList list2 = top(multStack)->polynomialList;
        pop(multStack);
        push(multStack, multTwoLists(list1, list2));
    }

    printf("Polynomials Multiplied Successfully, enter 6 to see the result\n");
}

// Time Complexity: O(n)
void printResultsToScreen(stack addStack, stack subStack, stack multStack)
{ // I used this function to print the results to the screen
    if (top(top(addStack)) != NULL)
        printf("No result for addition\n"); // if the second node in the stack is not null then it means that no addition was done
    else
    {
        printf("Addition result: ");
        print(top(addStack)->polynomialList); // use the print function to print the polynomial of the top element in the stack
    }
    if (top(top(subStack)) != NULL)
        printf("No result for Subtraction\n"); // if the second node in the stack is not null then it means that no Subtraction was done
    else
    {
        printf("Subtraction result: ");
        print(top(subStack)->polynomialList); // use the print function to print the polynomial of the top element in the stack
    }
    if (top(top(multStack)) != NULL)
        printf("No result for Multiplication\n"); // if the second node in the stack is not null then it means that no Multiplication was done
    else
    {
        printf("Multiplication result: ");
        print(top(multStack)->polynomialList); // use the print function to print the polynomial of the top element in the stack
    }
}

// Time Complexity: O(n)
void printResultsToFile(stack addStack, stack subStack, stack multStack)
{ // same as the print function except that its in a file, we use fprintf, and i wrote it 3 time to write the output of each stack(add, sub, mult)
    FILE *file = fopen("output.txt", "w");
    if (file == NULL)
    {
        printf("Error: file not found\n"); // handle the case where the file doesnt open
        return;
    }

    if (top(top(addStack)) != NULL)
    {
        fprintf(file, "No result for addition\n");
    }
    else
    {
        fprintf(file, "Addition result: ");
        linkedList addResult = addStack->next->polynomialList;
        linkedList temp = addResult->next;
        int firstTerm = 1;
        if (temp != NULL && temp->next == NULL && temp->coefficient == 0)
        {
            fprintf(file, "0");
        }
        else
        {
            while (temp != NULL)
            {
                if (temp->coefficient < 0)
                {
                    fprintf(file, "-");
                    if (temp->coefficient != -1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", -1 * (temp->coefficient));
                    }
                }
                else
                {
                    if (!firstTerm)
                    {
                        fprintf(file, "+");
                    }
                    if (temp->coefficient != 1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", temp->coefficient);
                    }
                }

                if (temp->exponent > 0)
                {
                    fprintf(file, "X");
                    if (temp->exponent > 1)
                    {
                        fprintf(file, "^%d", temp->exponent);
                    }
                }

                temp = temp->next;
                firstTerm = 0;
            }
        }
        fprintf(file, "\n");
    }

    if (top(top(subStack)) != NULL)
    {
        fprintf(file, "No result for subtraction\n");
    }
    else
    {
        fprintf(file, "Subtraction result: ");
        linkedList subResult = subStack->next->polynomialList;
        linkedList temp = subResult->next;
        int firstTerm = 1;
        if (temp != NULL && temp->next == NULL && temp->coefficient == 0)
        {
            fprintf(file, "0");
        }
        else
        {
            while (temp != NULL)
            {
                if (temp->coefficient < 0)
                {
                    fprintf(file, "-");
                    if (temp->coefficient != -1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", -1 * (temp->coefficient));
                    }
                }
                else
                {
                    if (!firstTerm)
                    {
                        fprintf(file, "+");
                    }
                    if (temp->coefficient != 1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", temp->coefficient);
                    }
                }

                if (temp->exponent > 0)
                {
                    fprintf(file, "X");
                    if (temp->exponent > 1)
                    {
                        fprintf(file, "^%d", temp->exponent);
                    }
                }

                temp = temp->next;
                firstTerm = 0;
            }
        }
        fprintf(file, "\n");
    }

    if (top(top(multStack)) != NULL)
    {
        fprintf(file, "No result for multiplication\n");
    }
    else
    {
        fprintf(file, "Multiplication result: ");
        linkedList multResult = multStack->next->polynomialList;
        linkedList temp = multResult->next;
        int firstTerm = 1;
        if (temp != NULL && temp->next == NULL && temp->coefficient == 0)
        {
            fprintf(file, "0");
        }
        else
        {
            while (temp != NULL)
            {
                if (temp->coefficient < 0)
                {
                    fprintf(file, "-");
                    if (temp->coefficient != -1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", -1 * (temp->coefficient));
                    }
                }
                else
                {
                    if (!firstTerm)
                    {
                        fprintf(file, "+");
                    }
                    if (temp->coefficient != 1 || temp->exponent == 0)
                    {
                        fprintf(file, "%d", temp->coefficient);
                    }
                }

                if (temp->exponent > 0)
                {
                    fprintf(file, "X");
                    if (temp->exponent > 1)
                    {
                        fprintf(file, "^%d", temp->exponent);
                    }
                }

                temp = temp->next;
                firstTerm = 0;
            }
        }
        fprintf(file, "\n");
    }
    printf("Results written to output.txt successfully!\n");
}

// Time Complexity: O(n)
void exitProgram(stack head, stack addStack, stack subStack, stack multStack)
{ // this function is used to free all elements
    while (!isEmpty(head))
    {                     // while the stack is not empty
        popForExit(head); // free all components of the stack
    }
    free(head); // delete the head of the stack
    while (!isEmpty(addStack))
    {                         // while the stack is not empty
        popForExit(addStack); // free all components of the stack
    }
    free(addStack); // delete the head of the stack
    while (!isEmpty(subStack))
    {                         // while the stack is not empty
        popForExit(subStack); // free all components of the stack
    }
    free(subStack); // delete the head of the stack
    while (!isEmpty(multStack))
    {                          // while the stack is not empty
        popForExit(multStack); // free all components of the stack
    }
    free(multStack); // delete the head of the stack
    printf("Thank you.\nexiting program :)");
}

int main()
{
    printf("Welcome to my polynomial program.\n\n\n");
    stack head = (stack)malloc(sizeof(struct stackNode)); // create a stack used for printing the polynomials loaded from the file
    head->next = NULL;
    stack addStack = (stack)malloc(sizeof(struct stackNode)); // create a stack used for printing the result of the addition of polynomials
    addStack->next = NULL;
    stack subStack = (stack)malloc(sizeof(struct stackNode)); // create a stack used for printing the result of the Subtraction of polynomials
    subStack->next = NULL;
    stack multStack = (stack)malloc(sizeof(struct stackNode)); // create a stack used for printing the result of the Multiplication of polynomials
    multStack->next = NULL;
    int choice;
    do
    {
        printf("1. Load the polynomials file\n");
        printf("2. Print the polynomials\n");
        printf("3. Add the polynomials\n");
        printf("4. Subtract the polynomials\n");
        printf("5. Multiply the polynomials\n");
        printf("6. Print the results to the screen\n");
        printf("7. Print the results to file\n");
        printf("8. Exit\n");
        printf("Enter an operation: ");
        scanf("%d", &choice);
        printf("\n");
        if (choice == 1)
            loadPolynomials(head, addStack, subStack, multStack);
        else if (choice == 2)
            printPolynomials(head);
        else if (choice == 3)
            addPolynomials(addStack);
        else if (choice == 4)
            subtractPolynomials(subStack);
        else if (choice == 5)
            multiplyPolynomials(multStack);
        else if (choice == 6)
            printResultsToScreen(addStack, subStack, multStack);
        else if (choice == 7)
            printResultsToFile(addStack, subStack, multStack);
        else if (choice == 8)
            exitProgram(head, addStack, subStack, multStack);
        else
            printf("Error: Invalid input\n\n");
    } while (choice != 8);
    return 0;
}

// Thanks for reading
