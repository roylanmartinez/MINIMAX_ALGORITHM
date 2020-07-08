
/*
 *
Connect-4 Minimax Algorithm

Part of the UAB SUMMER SCHOOL ADVANCED PROGRAMMING COURSE

Guided by the Ph.D Vicenç Soler

Made by the student Roylan Martinez Vargas

NIU: 1539069

July 5, 2020

*/

#include <stdio.h>
#include <stdlib.h>

#define N 8

// NODE BODY
typedef struct node {
    char board[N][N];   //-128,127
    struct node *children[N];
    float value;   //heuristic value
} Node;

void initBoard(Node *passedNode){
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < N; column++) {
            passedNode->board[row][column] = ' ';
        }
    }
}

int wonPosition(Node *passedNode, char symbol){
    // Check Horizontally
    char rowPoints[2];
    rowPoints[0] = 0; rowPoints[1] = 0;
    for (int row = 0; row < N; row++){
        for (int column = 0; column < N; column++){
            if (passedNode->board[row][column] == symbol){
                rowPoints[0]++;
                if (rowPoints[0] == 4){
                    return 1;
                }
            }
            else{
                rowPoints[0] = 0;
            }
        }
        rowPoints[0] = 0;
    }
    // Check vertically
    for (int column = 0; column < N; column++){
        for (int row = 0; row < N; row++){
            if (passedNode->board[row][column] == symbol){
                rowPoints[0]++;
                if (rowPoints[0] == 4){
                    return 1;
                }
            }
            else{
                rowPoints[0] = 0;
            }
        }
        rowPoints[0] = 0;
    }
    // Check central diagonals
    for (int borderL = 0; borderL < N; borderL++){
        if (passedNode->board[borderL][borderL] == symbol){
            rowPoints[0]++;
        }
        else {
            rowPoints[0] = 0;
        }
        if (passedNode->board[N - borderL - 1][borderL] == symbol){
            rowPoints[1]++;
        }
        else {
            rowPoints[1] = 0;
        }
        if (rowPoints[0] == 4 || rowPoints[1] == 4){
            return 1;
        }
    }

    rowPoints[0] = 0; rowPoints[1] = 0;

    // Check diagonally not central diagonals
    for (int borderR = 1; borderR < N - 4 + 1; borderR++){

        // Check bottom left side to right upper side
        for (int borderL = 0; borderL < N - borderR; borderL++){
            if (passedNode->board[borderL][borderL + borderR] == symbol){
                rowPoints[0]++;
            }
            else {
                rowPoints[0] = 0;
            }
            if (passedNode->board[borderL + borderR][borderL] == symbol){
                rowPoints[1]++;
            }
            else {
                rowPoints[1] = 0;
            }
            if (rowPoints[0] == 4 || rowPoints[1] == 4){
                return 1;
            }
        }

        rowPoints[0] = 0; rowPoints[1] = 0;

        // Check upper left side to bottom right side
        for (int borderL = 0; borderL < N - borderR; borderL++){
            if (passedNode->board[borderL][N - borderL - borderR - 1] == symbol){
                rowPoints[0]++;
            }
            else {
                rowPoints[0] = 0;
            }
            if (passedNode->board[borderL + borderR][N - borderL - 1] == symbol){
                rowPoints[1]++;
            }
            else {
                rowPoints[1] = 0;
            }
            if (rowPoints[0] == 4 || rowPoints[1] == 4){
                return 1;
            }
        }
    }
    return 0;
}

void printBoard(Node *passedNode){
    // Print numbers in the top
    for (int columnNames = 0; columnNames < N; columnNames++){
        printf("   %i  ", (columnNames));
    };
    // print the board
    printf("  \n");
    for (int row = 0; row < N; row++){
        printf("%i ", row);
        for (int column = 0; column < N; column++) {
            if (passedNode->board[row][column] == 'x'){
                printf("|  %c  ", 'x');
            }
            else if (passedNode->board[row][column] == 'o'){
                printf("|  %c  ", 'o');
            }
            else {
                printf("|     ");
            }

        };
        printf("|\n");
    }
};

void copyBoard(Node *nodeToBoard, Node *nodeFromBoard) {
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < N; column++) {
            nodeToBoard->board[row][column] = nodeFromBoard->board[row][column];
        };
    }
};

int applyThrow(Node *passedNode, int numChild, char symbol){
    if (symbol == 'o'){
        if (passedNode->board[0][numChild] == ' ') {
            for (int row = 0; row < N; row++){
                if (passedNode->board[N - row - 1][numChild] == ' '){
                    passedNode->board[N - row - 1][numChild] = symbol;
                    return 1;
                }
            }

        }
        else {
            for (int column = 0; column < N; column++){
                if (passedNode->board[0][column] == ' ') {
                    for (int row = 0; row < N; row++){
                        if (passedNode->board[N - row - 1][column] == ' '){
                            passedNode->board[N - row - 1][column] = symbol;
                            return 1;
                        }
                    }

                }
            }
        }
    }
    else {
        if (passedNode->board[0][numChild] == ' ') {
            for (int row = 0; row < N; row++){
                if (passedNode->board[N - row - 1][numChild] == ' '){
                    passedNode->board[N - row - 1][numChild] = symbol;
                    return 1;
                }
            }
        }
        else {
            return 0;
        }
    }
    return 0;
};

Node *createNode(Node *parent) {
    Node *newNode = malloc(sizeof(Node));
    copyBoard(newNode, parent);
    return newNode;
}

//We consider root node already created and n_children already set.
void setUpTree(Node *passedNode) {
    for (int node = 0; node < N; node++) {
        passedNode->children[node] = createNode(passedNode);
        applyThrow(passedNode->children[node], node, 'o');
    }
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++) {
            passedNode->children[node]->children[subNode] = createNode(passedNode->children[node]);
            applyThrow(passedNode->children[node]->children[subNode], subNode, 'x');
        }
    }
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++){
            for (int subSubNode = 0; subSubNode < N; subSubNode++){
                passedNode->children[node]->children[subNode]->children[subSubNode]
                = createNode(passedNode->children[node]->children[subNode]);
                applyThrow(passedNode->children[node]->children[subNode]->children[subSubNode], subSubNode, 'o');
            }
        }
    }
}

void deleteTree(Node *passedNode) {
    for (int node = 0; node < N; node++) {
        free(passedNode->children[node]);
    }
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++) {
            free(passedNode->children[node]->children[subNode]);
        }
    }
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++){
            for (int subSubNode = 0; subSubNode < N; subSubNode++){
                free(passedNode->children[node]->children[subNode]->children[subSubNode]);
            }
        }
    }
}

int isDraw(Node *passedNode){
    for (int column = 0; column < N; column++){
        if (passedNode->board[0][column] == ' '){
            return 0;
        }
    }
    return 1;
}

int heuristicBestMove(Node *passedNode) {

    // Value to leaf nodes
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++) {
            for (int subSubNode = 0; subSubNode < N; subSubNode++) {
                if (wonPosition(passedNode->children[node]->children[subNode]->children[subSubNode], 'o')) {
                    passedNode->children[node]->children[subNode]->children[subSubNode]->value = 1;
                }
                else {
                    passedNode->children[node]->children[subNode]->children[subSubNode]->value = 0;
                }
            }
        }
    }
    // Heuristic value for the second children generation
    float heuristicValue = 0;
    for (int node = 0; node < N; node++) {
        for (int subNode = 0; subNode < N; subNode++) {
            float sumWin = 0;
            for (int subSubNode = 0 ; subSubNode < N; subSubNode++) {
                sumWin += passedNode->children[node]->children[subNode]->children[subSubNode]->value;
            }
            heuristicValue = sumWin / (float)N;
            passedNode->children[node]->children[subNode]->value = heuristicValue;
        }
    }
    // Heuristic value for the first children generation
    for (int node = 0; node < N; node++) {
        float lowestValue = passedNode->children[node]->children[0]->value;
        for (int subNode = 0; subNode < N; subNode++) {
            if (lowestValue >= passedNode->children[node]->children[subNode]->value){
                lowestValue = passedNode->children[node]->children[subNode]->value;
            }
        }
        passedNode->children[node]->value = lowestValue;
    }
    // Value to main root
    float highestValue = passedNode->children[0]->value;
    int bestMoveResult = 0;

    for (int node = 0; node < N; node++) {
        if (highestValue <= passedNode->children[node]->value){
            highestValue = passedNode->children[node]->value;
            bestMoveResult = node;
        }
    }

    // Does the human win in next move? If so avoid that

    Node *copyOfNode = malloc(sizeof(Node));
    copyBoard(copyOfNode, passedNode);
    applyThrow(copyOfNode, bestMoveResult, 'o');

    if (wonPosition(copyOfNode, 'o') == 1){
        return bestMoveResult;
    }
    else{
        for (int node = 0; node < N; node++){
            copyOfNode->children[node] = createNode(passedNode);
            applyThrow(copyOfNode->children[node], node, 'x');
            if (wonPosition((copyOfNode)->children[node], 'x')) {
                free(copyOfNode->children[node]);
                free(copyOfNode);
                return node;
            }
        }
    }
    return 0;
}

int main() {
    int choice, bestMove;
    Node test;
    initBoard(&test);
    printf("\n");
    printBoard(&test);
    while (1) {
        printf("\nYour turn, please select a column between 0 and %i:\n", N-1);
        scanf("%i", &choice);
        while (1){
            if (applyThrow((&test), choice, 'x') == 0){
                printf("\nThe column %i is full, please select a different one\n", choice);
                scanf("%i", &choice);
            }
            else {
                break;
            }
        }
        printBoard((&test));
        setUpTree(&test);
        if (wonPosition(&test, 'x')) {
            printf("\nYOU WERE LUCKY, NEXT TIME WON'T BE THAT EASY\n\n");
            break;
        }
        if (isDraw(&test)) {
            printf("\nWELL, IT LOOKS LIKE A DRAW\n\n");
            break;
        }
        bestMove = heuristicBestMove((&test));
        applyThrow((&test), bestMove, 'o');
        printf("\n\nComputer's turn move in the column %i\n", bestMove);
        printBoard((&test));
        if (wonPosition(&test, 'o')) {
            printf("\nIT SEEMS I WON, DOESN'T IT? ;)\n\n");
            break;
        }
        if (isDraw(&test)) {
            printf("\nWELL, IT LOOKS LIKE A DRAW\n\n");
            break;
        }
    }
    deleteTree(&test);
    return 0;
}
