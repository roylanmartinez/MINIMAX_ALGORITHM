
/*
 *
Connect-4 Minimax Algorithm

Part of the UAB SUMMER SCHOOL ADVANCED PROGRAMMING COURSE

Guided by the Ph.D Vicenç Soler

Made by the student Roylan Martinez Vargas

NIU: 1539069

*/

#include <stdio.h>
#include <stdlib.h>

#define N 5

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
    char rowPoints = 0, result = 0;
    for (int row = 0; row < N; row++){
        for (int column = 0; column < N; column++){
            if (passedNode->board[row][column] == symbol){
                rowPoints++;
                if (rowPoints == 4){
                    return 1;
                }
            }
            else{
                rowPoints = 0;
            }
        }
        rowPoints = 0;
    }
    // Check vertically
    for (int column = 0; column < N; column++){
        for (int row = 0; row < N; row++){
            if (passedNode->board[row][column] == symbol){
                rowPoints++;
                if (rowPoints == 4){
                    return 1;
                }
            }
            else{
                rowPoints = 0;
            }
        }
        rowPoints = 0;
    }
    // Check diagonally
    for (int border = 1; border < N - 4 + 1; border++){

    }
}

void initBoard(Node *board){
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < N; column++) {
            board->board[row][column] = ' ';
        }
    }
}

void printBoard(Node *passedNode){
    printf("\n  ");
    for (int columnNames = 0; columnNames < N; columnNames++){
        printf("   %i  ", (columnNames));
    };
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
            };
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

    test.board[0][1] = 'o';
    test.board[1][1] = 'o';
    test.board[3][1] = 'o';
    test.board[2][1] = 'o';
    printBoard(&test);
    printf("%i", wonPosition(&test, 'o'));
    return 0;
}
