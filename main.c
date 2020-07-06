
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

#define N 6

// NODE BODY
typedef struct node {
    char board[N][N];   //-128,127
    char result;
    struct node *children[N];
    int n_children;
    double value;   //heuristic value
} Node;

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

void initBoard(Node *board){
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < N; column++) {
            board->board[row][column] = ' ';
        }
    }
}

void printBoard(Node *passedNode){
    // Print numbers in the top
    printf("\n  ");
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

void applyThrow(Node *passedNode, int numChild, int isCircle){
    char symbol;
    if (isCircle) {
        symbol = 'o';
    } else {
        symbol = 'x';
    }
    if (passedNode->board[0][numChild] == ' ') {
        passedNode->board[0][numChild] = symbol;
    } else {
        passedNode->result = -1;
    }
};

int numOfChildren(Node *p){
    char freeColumns = 0;
    for (int column = 0; column < N; column++){
        if (p->board[0][column] == ' '){
            freeColumns++;
        }
    }
    return freeColumns;
}

Node *createNode(Node *parent, int numChild, int level) {
    Node *p=malloc(sizeof(Node));
    copyBoard(p,parent);
    applyThrow(p,numChild, 1);
    if (level<2) {
        p->n_children=numOfChildren(p);
    }
    else {
        p->n_children=0;
    }
    return p;
}

void createChildren(Node *parent,int level) {
    int i;
    for (i=0;i<parent->n_children;i++) {
        parent->children[i]=createNode(parent,i,level);
    }
}

//We consider root node already created and n_children already set.
void createTree(Node *root) {
    int i;
    root->n_children=numOfChildren(root);
    createChildren(root,1);   //1st generation
    for(i=0;i<root->n_children;i++) {       //creates the 2nd generation
        root->children[i]->n_children = numOfChildren(root->children[i]);
        createChildren(root->children[i],2);
    }
}

int main(){
    int choice;
    Node test;
    initBoard(&test);

    test.board[5][0] = 'o';
    test.board[4][1] = 'o';
    test.board[3][2] = 'o';
    test.board[2][3] = 'o';
    printBoard(&test);
    printf("%i", wonPosition(&test, 'o'));
    return 0;
}
