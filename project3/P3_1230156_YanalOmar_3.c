// Yanal Omar
// 1230156
// Section 3
// Dr. Ahmad Abusnaina
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define stringLengthForNodeData 20 //This constant is for the max string length of the town and district
//I defined this constant so you can just change it from here not from everywhere

int tableSize = 10; // initialized the table size to 10

struct node{// node of the AVL tree
    char district[stringLengthForNodeData];
    char town[stringLengthForNodeData];
    int population;
    int elevation;
    int hasManucipility;
    struct node* left;// pointer to the left tree node
    struct node* right;// pointer to the right tree node
    int height;// the height of the node
};

typedef struct node* avlTree;// rename the (struct node*) to avlTree for easier naming

struct hashNode{// node to save the data of every town and save it in the hashTable
    char district[stringLengthForNodeData];
    char town[stringLengthForNodeData];
    int population;
    int elevation;
    int hasManucipility;
};

typedef struct hashNode* hashTable; // rename the (struct hashNode*) to hashTable for easier naming

hashTable* table;// create an array of hashNodes

void initializeTable() {// this function is to initialize the table with null values and reserve a memory location for the array with length tableSize
    table = (hashTable) malloc(tableSize * sizeof(hashTable));
    for (int i = 0; i < tableSize; i++) {
        table[i] = NULL;
    }
}

int getHeight(avlTree tree){// return the height of a tree node
    if(tree==NULL) return -1;
    else return tree->height;
}

int max(int a, int b) {// returns the maximum value between 2 integers
    return (a > b) ? a : b;// is a more than b ? if yes, return a, else return b
}

avlTree singleRotationLeft(avlTree tree){// this function to single rotate to the left, basic implementation like the lecture notes, nothing added
    avlTree y;
    y=tree->right;
    tree->right=y->left;
    y->left=tree;

    tree->height=1+max(getHeight(tree->left), getHeight(tree->right));
    y->height=1+max(getHeight(tree), getHeight(y->right));
    return y;
}

avlTree singleRotationRight(avlTree tree){// this function to single rotate to the right, basic implementation like the lecture notes, nothing added
    avlTree x;
    x=tree->left;
    tree->left=x->right;
    x->right=tree;

    tree->height=1+max(getHeight(tree->right), getHeight(tree->left));
    x->height=1+max(getHeight(tree), getHeight(x->left));

    return x;
}

avlTree doubleRotationLeft(avlTree tree){// this function to double rotate to the left, basic implementation like the lecture notes, nothing added
    tree->left=singleRotationLeft(tree->left);
    return singleRotationRight(tree);
}

avlTree doubleRotationRight(avlTree tree){// this function to double rotate to the right, basic implementation like the lecture notes, nothing added
    tree->right=singleRotationRight(tree->right);
    return singleRotationLeft(tree);
}

avlTree insert(avlTree tree, char district[], char town[], int population, int elevation, int hasManucipility) {// insert function to insert nodes to the AVL tree (recursive)
    if (tree == NULL) {// if the tree is NULL, the tree is empty, so we just make the root
        tree = (struct node *)malloc(sizeof(struct node));
        strcpy(tree->district, district);// strcpy function used to copy data of the string "district" and put it in the district of the node
        strcpy(tree->town, town);// strcpy function used to copy data of the string "town" and put it in the town of the node
        tree->population = population;
        tree->elevation = elevation;
        tree->hasManucipility = hasManucipility;
        tree->height = 0;// initialize the height to 0 since its at the end
        tree->left = tree->right = NULL;// There is no left or right nodes so we assign them to NULL
    } else if (strcasecmp(town, tree->town) < 0) {// i used the town as the sort factor of the tree, strcasecmp is used to compare the strings alphabetically
        //strcasecmp(str1, str2) < 0; means str1 is less than str2
        //strcasecmp(str1, str2) > 0; means str1 is greater than str2
        //strcasecmp(str1, str2) = 0; means str1 equals str2
        //unlike strcmp, this function is case insensitive
        tree->left = insert(tree->left, district, town, population, elevation, hasManucipility);// since town < the town of the parent, we will append it to the left recursively
        if (getHeight(tree->left) - getHeight(tree->right) >= 2) {// check the balance to see if it needs rotation
            if (strcasecmp(town, tree->left->town) < 0){// if the town is less than the town of the left node we only need single rotation right
//                3        2
//              2    =>  1   3
//            1
                tree = singleRotationRight(tree);
            }else {// else we need double rotation Left
//               3           3         2
//             1     =>    2    =>   1   3
//               2       1
                tree = doubleRotationLeft(tree);
            }
        }
    } else if (strcasecmp(town, tree->town) > 0) {
        tree->right = insert(tree->right, district, town, population, elevation, hasManucipility);// since town > the town of the parent, we will append it to the left recursively
        if (getHeight(tree->right) - getHeight(tree->left) >= 2) {// check the balance to see if it needs rotation
            if (strcasecmp(town, tree->right->town) > 0){// // if the town is more than the town of the left node we only need single rotation left
//              1              2
//                2    =>    1   3
//                  3
                tree = singleRotationLeft(tree);
            }else{
                tree = doubleRotationRight(tree);
//              1          1              2
//                3  =>      2    =>    1   3
//              2              3
            }
        }
    } else {// else if they are equal, then the town already exists
        printf("Town Already exists");
    }
    tree->height = 1 + max(getHeight(tree->left), getHeight(tree->right));// increase the height by 1
    return tree;
}


avlTree findTown(avlTree root, char townName[]){// function to find a town (recursive function)
    if (root == NULL || strcasecmp(root->town, townName)==0)// if the root is NULL (you reached the end without finding the town) or the town of the root equals the town we are looking for, it will return this root
        return root;// this returns either NULL or the nod we are searching for, i handle the 2 cases from where i called the function
    if (strcasecmp(townName, root->town)<0)// if the town we are searching for is less than the town of the root, go to the left
        return findTown(root->left, townName);
    return findTown(root->right, townName);// else go to the right
}

void printTree(avlTree root) {// function to print the tree recursively
    // i used in-order traversal to print them alphabetically
    if (root != NULL) {
        printTree(root->left);
        char strHasManucipility[4];
        strcpy(strHasManucipility, root->hasManucipility == 0 ? "no" : "yes");
        printf("District: %s, Town: %s, Population: %d, Elevation: %d, Has Municipality: %s\n",root->district, root->town, root->population, root->elevation, strHasManucipility);
        printTree(root->right);
    }
}


void printTownsWithAPopulationAboveANumber(avlTree root, int num){// reused the printTowns function but with an if condition
    if (root != NULL) {
        printTownsWithAPopulationAboveANumber(root->left, num);
        if(root->population>num){
            char strHasManucipility[4];
            strcpy(strHasManucipility, root->hasManucipility == 0? "no":"yes");
            printf("District: %s, Town: %s, population: %d, elevation: %d, Has Manucipility: %s\n", root->district, root->town, root->population, root->elevation, strHasManucipility);
        }
        printTownsWithAPopulationAboveANumber(root->right, num);
    }
}

void printTownsMuncipipality(avlTree root, int choice){// reused the printTowns function but with an if condition
    if (root != NULL) {
        printTownsMuncipipality(root->left, choice);
        if(root->hasManucipility == choice){
            char strHasManucipility[4];
            strcpy(strHasManucipility, root->hasManucipility == 0? "no":"yes");
            printf("District: %s, Town: %s, population: %d, elevation: %d, Has Manucipility: %s\n", root->district, root->town, root->population, root->elevation, strHasManucipility);
        }
        printTownsMuncipipality(root->right, choice);
    }
}


avlTree deleteTown(avlTree root, char townName[]) {
    if (root == NULL)
        return root;

    // search for the town we want to delete
    if (strcasecmp(townName, root->town) < 0)
        root->left = deleteTown(root->left, townName);
    else if (strcasecmp(townName, root->town) > 0)
        root->right = deleteTown(root->right, townName);
    else {
        // node to be deleted found
        //we have 3 cases: 1- No children, 1 child, 2 children
        //Case 1: no children
        if (root->left == NULL && root->right == NULL) {//this case handles the case where there are no children
            free(root);
            return NULL;
        } else if (root->left == NULL) {//this case handles the case where it has only a right child
            avlTree temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {// handles the case where it has only a left child
            avlTree temp = root->left;
            free(root);
            return temp;
        }
        //Case 3: node with two children, we either find the maximum of the left child or the minimum of the right child.
        // in my case, i found the minimum of the right child
        avlTree temp = root->right;
        while (temp->left != NULL)
            temp = temp->left;

        // Copy the min of the right node content to the node we want to delete
        strcpy(root->town, temp->town);
        strcpy(root->district, temp->district);
        root->population = temp->population;
        root->elevation = temp->elevation;
        root->hasManucipility = temp->hasManucipility;

        // delete min of the right node recursively
        root->right = deleteTown(root->right, temp->town);
    }

    // update height of the current node
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // get balance factor and perform rotations if needed
    int balance = getHeight(root->left) - getHeight(root->right);

    //handle the rotations
    if (balance > 1 && (getHeight(root->left->left) >= getHeight(root->left->right)))
        return singleRotationRight(root);
    if (balance < -1 && (getHeight(root->right->right) >= getHeight(root->right->left)))
        return singleRotationLeft(root);
    if (balance > 1 && (getHeight(root->left->left) < getHeight(root->left->right)))
        return doubleRotationRight(root);
    if (balance < -1 && (getHeight(root->right->right) < getHeight(root->right->left)))
        return doubleRotationLeft(root);

    return root;
}


void saveTreeToFileHelper(avlTree root, FILE *file) {// this function is a helper function to the save to file, it prints them in the file recursively
    if (root != NULL) {// pre-order traversal
        fprintf(file, "%s:%s:%d:%d:%s\n", root->district, root->town, root->population, root->elevation, root->hasManucipility ? "yes" : "no");
        saveTreeToFileHelper(root->left, file);
        saveTreeToFileHelper(root->right, file);
    }
}

void saveTreeToFile(avlTree root, char *filename) {// function to save the tree in a file, it just opens the file, call the helper function, close the file
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    saveTreeToFileHelper(root, file);
    fclose(file);
    printf("Tree successfully saved to %s.\n", filename);
}


void freeTree(avlTree root) {// function to delete the tree from memory to make the program more efficient (recursive)
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int findNextPrime(int n) {// this function is used to find the next the prime number, its used to find the new size of the table
    while (1) {//while true ( it will keep moving until it finds the smallest prime number)
        int isPrime = 1;// i added this as a flag to check it in every loop, if its 1 then the number is prime.
        for (int i = 2; i <=(n/2); i++) {// a second loop to see if the i divides our number
            if (n % i == 0) {
                isPrime = 0;
                break;// no need to continue in the loop if we found a single number that divides our number
            }
        }
        if (isPrime) {
            return n;
        }
        n++;
    }
}

int hash(char key[]) {// a function to hash to the town string
    // the function adds the first 5 letters and return the added integer modules the table size
    int hashValue = 0;
    int i = 0;
    while (i < 5 && key[i] != '\0') {
        hashValue += tolower(key[i]);// convert the letters to lower case because we are ignoring the case when compring
        i++;
    }
    return hashValue%tableSize;
}

int getNumOfElementsInHashTable() { //function to count the elemnets of the hash table
    int totalElements = 0;
    for (int i = 0; i < tableSize; i++) {
        if (table[i] != NULL) {// If the town is not empty add 1 to the count
            totalElements++;
        }
    }
    return totalElements;
}

float getLoadFactor() {// function to return the load factor
    return (float)getNumOfElementsInHashTable()/tableSize;
}


void resizeHashTable() {//function to resize the hash table if the load factor exceeds the 0.7
    int currentSize = tableSize;
    tableSize = findNextPrime(tableSize * 2);

    hashTable *newTable = (hashTable *)malloc(tableSize * sizeof(hashTable));// initialize a new array with length of the next prime number of twice the old length
    for (int i = 0; i < tableSize; i++) {
        newTable[i]= NULL;// initialize the new table values to NULL
    }

    for (int i = 0; i < currentSize; i++) {
        if (table[i]!=NULL) {
            int index = hash(table[i]->town);// find the new index since its a new size
            while (newTable[index] != NULL) {// this while loop uses the linear probing, where it saves to the next empty index if the index is occupied
                index = (index + 1) % tableSize;
            }
            newTable[index] = table[i];
        }
    }

    free(table);// delete the table from memory
    table = newTable;// move the new table to table
}

int collisions(char town[]) {
    int index = hash(town);  // find the initial index
    int collisions = 0;       // initialize a counter for collisions

    while (table[index] != NULL) {
        if (strcasecmp(table[index]->town, town) == 0) {
            return collisions;  // return the collision count if the town is found
        }
        collisions++;  // increment the collision counter
        index = (index + 1) % tableSize;  // move to the next index using linear probing
    }

    return collisions;  // if town is not found, return 0
}


void insertHash(char district[], char town[], int population, int elevation, int hasMunicipality) {// function to insert a town to the hash function
    hashTable temp = (hashTable)malloc(sizeof(struct hashNode));// create a temp node so we put data in it and add it to the hash
    //here i just fill the data
    strcpy(temp->district, district);
    strcpy(temp->town, town);
    temp->population = population;
    temp->elevation = elevation;
    temp->hasManucipility = hasMunicipality;

    int index = hash(town);// find the index where we should save the town, and if the index is occupied execute linear probing until we find an empty index
    while (table[index] != NULL) {
        index = (index + 1) % tableSize;
    }
    table[index] = temp;

    if (getLoadFactor() > 0.65) {// check that the load factor doesnt exceed the 0.65
        resizeHashTable();
    }
}


void printHashTable() {// function to print the hash, it simply only iterates on each index
    printf("\nContent of Hash Table:\n");
    for (int i = 0; i < tableSize; i++) {
        if (table[i]!=NULL) {
            printf("[%d]: District: %s, Town: %s, Population: %d, Elevation: %d, Has Municipality: %s\n",
                   i, table[i]->district, table[i]->town, table[i]->population, table[i]->elevation,
                   table[i]->hasManucipility ? "yes" : "no");
        }else{
            printf("[%d]: Empty Place\n", i);
        }
    }
}

hashTable findTownInHashTable(char key[]) {// find a town
    int index = hash(key);// find the index its supposed to be at
    while (table[index]!=NULL) {// since we are using linear probing, it may not be the first element so we iterate over them until we find it
        if (strcasecmp(table[index]->town, key) == 0) {// compare strings to see if we found it
            return table[index];
        }
        index = (index + 1) % tableSize;// increment the index
    }
    return NULL;// if the table[index] becomes null and we didnt find the town yet we return null
}

void deleteTownFromHash(char town[]) {// function to delete a node from table
    int index = hash(town);// again, we want to find the node we want to delete
    while (table[index]!=NULL) {
        if (strcasecmp(table[index]->town, town) == 0) {
            table[index]=NULL;// when we find it, we just assign it to NULL
            printf("\nTown deleted successfully.\n");
            return;
        }
        index = (index + 1) % tableSize;
    }
    printf("\nTown not found.\n");
}

void saveHashToFile(char *filename) {// function to save hash to file
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    // we just iterate over the whole array and if its not null, we print the contents of the node
    for (int i = 0; i < tableSize; i++) {
        if (table[i] != NULL) {
            fprintf(file, "%s:%s:%d:%d:%s\n", table[i]->district, table[i]->town, table[i]->population, table[i]->elevation, table[i]->hasManucipility ? "yes" : "no");
        }
    }

    fclose(file);
    printf("\nHash table successfully saved to %s.\n", filename);
}


int main(){
    initializeTable();// initialize the table for the hash table
    avlTree root = NULL;
    char line[300];// this where i will save each line of the file
    //initialize data types to save data from the file
    char district[stringLengthForNodeData];
    char town[stringLengthForNodeData];
    int population;
    int elevation;
    int hasMunicipality;
    FILE* file = fopen("districts.txt", "r");//open the districts file for reading
    if (file == NULL) {// error handling if the file wasnt found
        printf("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file)) {// while we didnt reach the end of the file, read the next line in the file
        //just strtoks by ':' to save data in the tree
        char* token = strtok(line, ":");
        strncpy(district, token, stringLengthForNodeData - 1);
        district[stringLengthForNodeData - 1] = '\0';// put the end of the string character so no leaks happen
        token = strtok(NULL, ":");
        strncpy(town, token, stringLengthForNodeData - 1);
        town[stringLengthForNodeData - 1] = '\0';
        token = strtok(NULL, ":");
        population = atoi(token);
        token = strtok(NULL, ":");
        elevation = atoi(token);
        token = strtok(NULL, "\n");
        hasMunicipality = (strcasecmp(token, "no") == 0) ? 0 : 1;// save the has Municipality as an int for easier handling
        root = insert(root, district, town, population, elevation, hasMunicipality);// insert it to the avl tree
    }
    fclose(file);
    printf("Welcome to the districts management system!\n\n");
    while(1){// basic program menu for the avl tree
        int choice;
        printf("\n");
        printf("1. Insert a new town\n");
        printf("2. Find or update information of a town\n");
        printf("3. List towns information in alphabetical order\n");
        printf("4. List all towns with population above an input number\n");
        printf("5. List all towns that have municipalities or not\n");
        printf("6. Delete a town\n");
        printf("7. Save to file and go to hash\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            char tempDistrict[stringLengthForNodeData];
            char tempTown[stringLengthForNodeData];
            int tempPopulation;
            int tempElevation;
            char tempHasManucipility;
            printf("Enter district name: ");
            scanf("%s", tempDistrict);
            printf("Enter town name: ");
            scanf("%s", tempTown);
            printf("Enter the population number: ");
            scanf("%d", &tempPopulation);
            printf("Enter elevation: ");
            scanf("%d", &tempElevation);
            printf("Does this town have a manucipility [y/n]: ");
            while (getchar() != '\n');// consume the line so we can scan the character correctly
            scanf("%c", &tempHasManucipility);
                if (tempHasManucipility == 'n' || tempHasManucipility == 'y') {
                    int hasM = (tempHasManucipility == 'y') ? 1 : 0;
                    root = insert(root, tempDistrict, tempTown, tempPopulation, tempElevation, hasM);
                } else
                    printf("Incorrect input\n");
        }else if (choice == 2) {
            char townName[stringLengthForNodeData];
            printf("Enter town name: ");
            scanf("%s", townName);
            avlTree searchedTown = findTown(root, townName);// look for a town and print its data if found
            if (searchedTown != NULL) {
                char strHasManucipility[4];
                strcpy(strHasManucipility, searchedTown->hasManucipility == 0 ? "no" : "yes");
                printf("District: %s, Town: %s, Population: %d, Elevation: %d, Has Municipality: %s\n", searchedTown->district, searchedTown->town, searchedTown->population, searchedTown->elevation, strHasManucipility);
                printf("Do you want to update data? [y/n]: ");// ask if the user wants to update data
                char updateChoice;
                while (getchar() != '\n');
                scanf("%c", &updateChoice);
                while (updateChoice != 'y' && updateChoice != 'n') {
                    printf("Wrong input, Do you want to update data? [y/n]: ");
                    while (getchar() != '\n');
                    scanf("%c", &updateChoice);
                }
                if (updateChoice == 'y') {// if he wants to update the data he asks if he want to update population and/or elevation and/or Municipality
                    int newPopulation, newElevation;
                    char newHasManucipility;
                    printf("Do you want to update the population? [y/n]: ");
                    char updatePopulation;
                    while (getchar() != '\n');
                    scanf("%c", &updatePopulation);
                    while (updatePopulation != 'y' && updatePopulation != 'n') {
                        printf("Wrong input, Do you want to update the population? [y/n]: ");
                        while (getchar() != '\n');
                        scanf("%c", &updatePopulation);
                    }
                    if (updatePopulation == 'y') {
                        printf("Enter the new population: ");
                        scanf("%d", &newPopulation);
                        searchedTown->population = newPopulation;
                    }
                    printf("Do you want to update the Elevation? [y/n]: ");
                    char updateElevation;
                    while (getchar() != '\n');
                    scanf("%c", &updateElevation);
                    while (updateElevation != 'y' && updateElevation != 'n') {
                        printf("Wrong input, Do you want to update the Elevation? [y/n]: ");
                        while (getchar() != '\n');
                        scanf("%c", &updateElevation);
                    }
                    if (updateElevation == 'y') {
                        printf("Enter the new Elevation: ");
                        scanf("%d", &newElevation);
                        searchedTown->elevation = newElevation;
                    }
                    printf("Do you want to update the has Manucipility? [y/n]: ");
                    char updateManucipility;
                    while (getchar() != '\n');
                    scanf("%c", &updateManucipility);
                    while (updateManucipility != 'y' && updateManucipility != 'n') {
                        printf("Wrong input, Do you want to update the has Manucipility? [y/n]: ");
                        while (getchar() != '\n');
                        scanf("%c", &updateManucipility);
                    }
                    if (updateManucipility == 'y') {
                        while (getchar() != '\n');
                        printf("Does the town has a Manucipility [y/n]: ");
                        scanf("%c", &newHasManucipility);
                        while (newHasManucipility != 'y' && newHasManucipility != 'n') {
                            printf("Wrong input, Does the town has a Manucipility [y/n]: ");
                            while (getchar() != '\n');
                            scanf("%c", &newHasManucipility);
                        }
                        searchedTown->hasManucipility = (newHasManucipility == 'y') ? 1 : 0;
                    }
                }
            } else
                printf("Town not found\n");
        }else if(choice == 3){
            printf("List of the towns in alphabetical order:\n");
            printTree(root);
        }else if(choice == 4){
            int number;
            printf("Enter a number: ");
            scanf("%d", &number);
            printf("List of the towns with population above %d:\n", number);
            printTownsWithAPopulationAboveANumber(root, number);
        }else if(choice == 5){
            int mChoice;
            printf("do you want a list of towns with(1) or without(2) municipalities: ");
            scanf("%d", &mChoice);
            while(mChoice!=1 && mChoice!=2){
                printf("Wrong input, do you want a list of towns with(1) or without(2) municipalities: ");
                scanf("%d", &mChoice);
            }
            if(mChoice==1){
                printf("List of the towns that has a municipality:\n");
            }else if(mChoice==2){
                printf("List of the towns that dont have a municipality:\n");
                mChoice=0;
            }
            printTownsMuncipipality(root, mChoice);
        }else if(choice == 6){
            char townName[stringLengthForNodeData];
            printf("Enter name of the town you want to delete: ");
            scanf("%s", townName);
            avlTree searchedTown = findTown(root, townName);
            if(searchedTown!=NULL)
                root = deleteTown(root, townName);
            else
                printf("Town not found");
        }else if(choice == 7){
            saveTreeToFile(root, "towns.txt");
            freeTree(root);// delete the tree from the memory for more efficiency
            break;
        }else{
            printf("Wrong input, please enter a valid choice");
        }
    }
    FILE* reOpenfile = fopen("towns.txt", "r");
    if (reOpenfile == NULL) {
        printf("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), reOpenfile)){// do the same as i did in the avl tree but for the hash and with the towns.txt file
        char* token = strtok(line, ":");
        strncpy(district, token, stringLengthForNodeData - 1);
        district[stringLengthForNodeData - 1] = '\0';
        token = strtok(NULL, ":");
        strncpy(town, token, stringLengthForNodeData - 1);
        town[stringLengthForNodeData - 1] = '\0';
        token = strtok(NULL, ":");
        population = atoi(token);
        token = strtok(NULL, ":");
        elevation = atoi(token);
        token = strtok(NULL, "\n");
        hasMunicipality = (strcasecmp(token, "no") == 0) ? 0 : 1;
        insertHash(district, town, population, elevation, hasMunicipality);
    }
    fclose(reOpenfile);
    while(1){
        int choice;
        printf("\n");
        printf("1. Print the hash table\n");
        printf("2. Print the size of the hash table and the load factor\n");
        printf("3. Insert a new record into the hash table\n");
        printf("4. Search for a specific town and print the number of collisions to reach the town\n");
        printf("5. Delete a specific record\n");
        printf("6. Save data to file and exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 1){
            printf("HashTable contents:\n");
            printHashTable();
        }else if(choice == 2){
            float loadFactor= getLoadFactor();
            int size=getNumOfElementsInHashTable();
            printf("The size of the Hash Table: %d, The load factor: %f", tableSize, loadFactor);
        }else if(choice == 3){
            char tempDistrict[stringLengthForNodeData];
            char tempTown[stringLengthForNodeData];
            int tempPopulation;
            int tempElevation;
            char tempHasManucipility;
            printf("Enter district name: ");
            scanf("%s", tempDistrict);
            printf("Enter town name: ");
            scanf("%s", tempTown);
            printf("Enter the population number: ");
            scanf("%d", &tempPopulation);
            printf("Enter elevation: ");
            scanf("%d", &tempElevation);
            printf("Does this town have a manucipility[y/n]: ");
            getchar();
            scanf("%c", &tempHasManucipility);
            int hasM = tempHasManucipility == 'y' ? 1: 0;
            insertHash(tempDistrict, tempTown, tempPopulation, tempElevation, hasM);
        }else if(choice == 4){
            char searchForTown[stringLengthForNodeData];
            printf("Enter town name to search: ");
            scanf("%s", searchForTown);
            hashTable searchedTown = findTownInHashTable(searchForTown);
            if(searchedTown==NULL) printf("No such town");
            else{
                printf("District: %s, Town: %s, population: %d, elevation: %d, Has Manucipility: %s\n", searchedTown->district, searchedTown->town, searchedTown->population, searchedTown->elevation, searchedTown->hasManucipility ? "yes" : "no");
                printf("The number of collisions: %d", collisions(searchedTown->town));
            }
        }else if(choice == 5){
            char townName[stringLengthForNodeData];
            printf("Enter town name to delete: ");
            scanf("%s", townName);
            deleteTownFromHash(townName);
        }else if(choice == 6){
            saveHashToFile("towns.txt");
            break;
        }else{
            printf("Wrong input, please enter a valid choice");
        }
    }
    free(table);// delete the table from the memory for more efficiency
    printf("\nThanks for using the program!\nGoodBye!");
    return 0;
}
