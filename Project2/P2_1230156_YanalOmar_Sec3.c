//Yanal Omar
//1230156
//Sec 3
#include <stdio.h>
#include <stdlib.h>

struct stackNode {//creat a stack Node
    int data;
    struct stackNode* next;
};

typedef struct stackNode* stack;//rename the stack node struct to stack for easier usage

void push(stack head, int k) {//push function to put elements at the top of the stack
    if (head == NULL) return; //handle the case where the stack is not found
    stack temp = (stack)malloc(sizeof(struct stackNode));// create a node to put data and push it to the stack
    temp->data = k;//the node we just created will hold the value k
    temp->next = head->next;//the previous next of the head becomes the next of the node we created
    head->next = temp;// connect the node we created to the head so it becomes the first element
}

void pop(stack head) {// pop to remove the top element in the stack
    if (head == NULL || head->next == NULL) return;// handle cases where the stack is empty or not found
    stack temp = head->next;// put the next of the head to a temp node so we can free it later
    head->next = temp->next;// connect the next of the head to the next of temp (head->next->next)
    free(temp);// remove the node we deleted from the memory
}

stack top(stack head) {// function to return the top of the stack
    return head->next;// we return the next of the head because the head has no data
}

int isEmptyStack(stack head) {// function to check if the stack is empty
    return head->next == NULL;
}

struct queueNode {// create a queue
    int front;
    int rear;
    int size;
    int items[10000];
};

typedef struct queueNode* queue;//name the struct queue nod to queue

int isEmpty(queue q) {// a function to return if the queue is empty
    return q->size == 0;
}

int isFull(queue q) {// a function to return if the queue is full
    return q->size == 10000;
}

int Succ( int value, queue q ){// a function to check if the elemnet at the end is at the last index, it goes back to 0 to save the new value
    if( ++value == 10000 )
        value = 0;
    return value;
}

void enqueue(queue q, int value) {// a function to put data into the queue
    if (isFull(q)) return;// check if the queue is full
    else{
        q->size++;//add 1 to the size
        q->rear = Succ( q->rear, q);//find where to save the element
        q->items[q->rear] = value;//assign the value to the rear index of the array
    }
}

int front(queue q) {// function to return the first element in the queue
    return isEmpty(q) ? 10001 : q->items[q->front];// is the stack empty? if yes return 10001 else return the items at index front
    // i chose to return 10001 because it out of bound for our test cases
}

void dequeue(queue q) {// function to remove the first element in the queue
    if (isEmpty(q)) return;// check if the queue is empty
    else{
        q->size--;//reduce the size by one
        q->front = Succ( q->front, q);// now we update the front value
    }
}

void printQueue(queue q) {// a function to print the queue
    if (isEmpty(q)) return;//check if the queue is empty
    int i = q->front;// i initalized a pointer so we move on each one of them without dequeing
    while (i != q->rear) {
        printf("%d\n", q->items[i]);
        i = (i + 1) % 10000;
    }
    printf("%d", q->items[q->rear]);
}

int findWinner(queue players) {// a function to find the winner of the tournement
    //two helper stacks that helped me through the code
    stack helperStack1=(stack)malloc(sizeof(struct stackNode));
    helperStack1->next=NULL;
    stack helperStack2=(stack)malloc(sizeof(struct stackNode));
    helperStack2->next=NULL;
    queue helperQueue = (queue)malloc(sizeof(struct queueNode));// also a helper queue that helped me through the code
    //creating the queue and initalizing the values
    helperQueue->front = 0;
    helperQueue->rear = -1;
    helperQueue->size = 0;
    int round = 1;// initialize a round counter to see if the round number is odd or even
    while (players->size > 1) {//while the players queue size is more than 1
        int initialSize = players->size;// save the size of the queue in a variable
        // i initialized this variable so 1- we move only on the players for this round 2- reuse our queue
        for (int i = 0; i < initialSize; i += 2) {// move in a loop on all the players, this loop move in 2's because each time we compare 2 players
            int player1 = front(players);// save the id of the player because when we dequeue to get the second id we will lose it if we just dequeued
            dequeue(players);// dequeue to get access to the second id

            if (!isEmpty(players)) {// if the queue is not empty
                int player2 = front(players);// save the second player
                dequeue(players);// dequeue

                int chosen;//a variable to save the winner of this game
                if (round % 2 == 1) {// if the round is odd the player with smaller id will win
                    chosen = (player1 < player2) ? player1 : player2;
                } else {// if the round is even the player with larger id will win
                    chosen = (player1 > player2) ? player1 : player2;
                }
                push(helperStack1, chosen);//push the winner in helper stack because if there is an odd number of players, we want the last player to be at the beginning of the queue
            } else {// else if the queue is empty, this means that the nimber of players are odd
                enqueue(helperQueue, player1);// we enqueue to the helper queue to be the first element as the assignment asks
            }
        }
        while(!isEmpty(helperStack1)){//the players in this stack are in reverse order, so we want to re reverse them.
            push(helperStack2, top(helperStack1)->data);
            pop(helperStack1);
        }
        while(!isEmpty(helperStack2)){//in this stack, they are in the right order
            enqueue(helperQueue, top(helperStack2)->data);
            pop(helperStack2);
        }
        while (!isEmpty(helperQueue)) {// now we put the players in the helper queue in the players queue
            enqueue(players, front(helperQueue));
            dequeue(helperQueue);
        }
        round++;//increment the round
    }
    //when the loops end, we are left with one id(the winner)
    int winner = front(players);
    // free every helper stack or queue for better space complexity
    dequeue(players);
    free(helperQueue);
    free(helperStack1);
    free(helperStack2);
    return winner;
}


void findPlayersOfRunnerUp(stack s, queue players, int winner){// this function is used to find the players of the runner up tournement
    // this function is almost the same as the previous function. however, in this function we compare the playing teams to see if any of
    // them is playing against the winner of the tournement, if yes, we push them into a stack
    stack helperStack1=(stack)malloc(sizeof(struct stackNode));
    helperStack1->next=NULL;
    stack helperStack2=(stack)malloc(sizeof(struct stackNode));
    helperStack2->next=NULL;
    queue helperQueue = (queue)malloc(sizeof(struct queueNode));
    helperQueue->front = 0;
    helperQueue->rear = -1;
    helperQueue->size = 0;
    int round = 1;
    while (players->size > 1) {
        int initialSize = players->size;
        for (int i = 0; i < initialSize; i += 2) {
            int player1 = front(players);
            dequeue(players);

            if (!isEmpty(players)) {
                int player2 = front(players);
                dequeue(players);

                int chosen;
                if (round % 2 == 1) {
                    chosen = (player1 < player2) ? player1 : player2;
                } else {
                    chosen = (player1 > player2) ? player1 : player2;
                }
                push(helperStack1, chosen);

                if(player1==winner) push(s, player2);
                else if(player2==winner) push(s, player1);
            } else {
                enqueue(helperQueue, player1);
            }
        }
        while(!isEmpty(helperStack1)){
            push(helperStack2, top(helperStack1)->data);
            pop(helperStack1);
        }
        while(!isEmpty(helperStack2)){
            enqueue(helperQueue, top(helperStack2)->data);
            pop(helperStack2);
        }
        while (!isEmpty(helperQueue)) {
            enqueue(players, front(helperQueue));
            dequeue(helperQueue);
        }
        round++;
    }
}


int main() {
    queue answers = (queue)malloc(sizeof(struct queueNode));// create a queue to save answers so we print them when n is -9999
    answers->front = 0;
    answers->rear = -1;
    answers->size = 0;

    while (1) {//while true, this loop will stay looping until something makes it break
        int n;
        scanf("%d", &n);
        if (n == -9999) break;
        queue players = (queue)malloc(sizeof(struct queueNode));// this queue is to save the players
        players->front = 0;
        players->rear = -1;
        players->size = 0;
        queue helperPlayers = (queue)malloc(sizeof(struct queueNode));//this queue is also to save players
        helperPlayers->front = 0;
        helperPlayers->rear = -1;
        helperPlayers->size = 0;
        //i saved the players into 2 queues because we will use the queue twice
        for (int i = 0; i < n; i++) {//read the ids
            int id;
            scanf("%d", &id);
            enqueue(players, id);
            enqueue(helperPlayers, id);
        }// if the queue has only one id, just put it in the winners queue
        if (n == 1) {
            enqueue(answers, front(players));
        } else {
            stack s=(stack)malloc(sizeof(struct stackNode));// a stack to save the players played against the winner team
            s->next=NULL;
            queue runnerUpPlayers = (queue)malloc(sizeof(struct queueNode));//a queue to save the players played against the winner team
            runnerUpPlayers->front = 0;
            runnerUpPlayers->rear = -1;
            runnerUpPlayers->size = 0;
            int winner = findWinner(players);// find the real winner
            findPlayersOfRunnerUp(s, helperPlayers, winner);//find the runner up tournement players
            while(!isEmptyStack(s)){//empty the stack in the queue
                enqueue(runnerUpPlayers, top(s)->data);
                pop(s);
            }
            int runnerUpWinner= findWinner(runnerUpPlayers);// find the winner of the runner up tournement
            enqueue(answers, runnerUpWinner);// put the winner in the queue
        }
        free(players);
    }

    printQueue(answers);// print the winners of the runner ups tournement
    free(answers);
    return 0;
}
