// Make sure that the cursor of txt file is placed in the next line of the last row of maze.

#include <stdio.h>
#include <stdlib.h>

struct queue
{
    int size;
    int h;
    int t;
    int * arr;
};

int isEmpty(struct queue * s){
    if(s->h == s->t){
        return 1;
    }
    else{
        return 0;
    }
}
int isFull(struct queue * s){
    if(s->h == s->size-1){
        return 1;
    }
    else{
        return 0;
    }
}
int enqueue(struct queue * s, int value){
    if(!isFull(s)){
        s->h++;
        s->arr[s->h] = value;
        return 1;
    }
    return 0;
}
int dequeue(struct queue * s){
    if(!isEmpty(s)){
        s->t++;
        return 1;
    }
    return 0;
}

int main(){
    FILE * file;
    file = fopen("maze12.txt","r");

    int numRows = 0;
    int numCols = 0;
    int charCount = 0;
    char currentChar;
    int strtR, strtC;
    
    //Parsing
    while ((currentChar = fgetc(file)) != EOF) {
        if (currentChar == '\n') {
            numRows++;
            numCols = charCount;
            charCount = 0;
        } 
        else {
            charCount++;
        }
    }
    //Initializing a 2D array to store the maze
    char m[numRows][numCols+1];

    // Reset file pointer to the beginning
    fseek(file, 0, SEEK_SET);
    int EndR, EndC;

    // Storing it in the 2D array and also storing the coordinates of S.
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols+1; j++)
        {   
            m[i][j] = fgetc(file);
            if( m[i][j] == 'S'){
                strtR = i;
                strtC = j;
            }
        }        
    }

    //Initializing the queue
    struct queue * q = (struct queue *)malloc(sizeof(struct queue));
    q->size = numRows*(numCols);
    q->t = q->h = -1;
    q->arr = (int *)malloc(q->size*sizeof(int));

    //initializing an array to mark visited coordinates
    int visited[numRows][numCols];

    //Creating an adjacency list and for each coordinate.
    //0-up, 1-down, 2-left, 3-right (For each coordinate only four connections are possible)
    int adj_list[numRows*numCols][4];

    //Initializing visited and adjacency list array to 0. 
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            visited[i][j] = 0;
        }
    }
    for (int i = 0; i < numRows*numCols; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            adj_list[i][j] = 0;
        }
    }

    // If there is a connection between the neighbour coordinates, then marking that position as 1 in adjacency list.
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if(i-1 >= 0 && (m[i-1][j] == ' ' || m[i-1][j] == 'S' || m[i-1][j] == 'E') ){

                adj_list[i*numCols+j][0] = 1;
            }
            if(i+1 <= numRows && (m[i+1][j] == ' ' || m[i+1][j] == 'S' || m[i+1][j] == 'E')){

                adj_list[i*numCols+j][1] = 1;
            }
            if(j-1 >= 0 && (m[i][j-1] == ' ' || m[i][j-1] == 'S' || m[i][j-1] == 'E')){

                adj_list[i*numCols+j][2] = 1;
            }
            if(j+1 <= numCols && (m[i][j+1] == ' ' || m[i][j+1] == 'S' || m[i][j+1] == 'E')){

                adj_list[i*numCols+j][3] = 1;
            }
        }     
    }

    // enqueuing the start coordinate.
    enqueue(q,strtR*numCols+strtC);
    visited[strtR][strtC] = 1;
    int c = 0;

    //Implementing BFS while Q is empty or End is reached
    while (!isEmpty(q))
    {   
        //Checking visited coordinates for each of the 4 directions and accordinglt enqueuing and marking them as visited.
        for (int i = 0; i < 4; i++)
        {
            if (adj_list[q->arr[q->t+1]][i] == 1)
            {
                if (i == 0 && (visited[(q->arr[q->t+1]/numCols)-1][q->arr[q->t+1]%numCols] == 0))
                {   
                    if (m[(q->arr[q->t+1]/numCols)-1][q->arr[q->t+1]%numCols] == 'E')
                    {   
                        c=1;
                        break;
                    }
                    
                    enqueue(q,q->arr[q->t+1]-numCols); 
                    visited[(q->arr[q->t+1]/numCols)-1][q->arr[q->t+1]%numCols] = 1;
                    m[(q->arr[q->t+1]/numCols)-1][q->arr[q->t+1]%numCols] = '.';
                }
                else if (i == 1 && (visited[(q->arr[q->t+1]/numCols)+1][q->arr[q->t+1]%numCols] == 0))
                {
                    if (m[(q->arr[q->t+1]/numCols)+1][q->arr[q->t+1]%numCols] == 'E')
                    {
                        c=1;
                        break;
                    }
                    
                    enqueue(q,q->arr[q->t+1]+numCols);
                    visited[(q->arr[q->t+1]/numCols)+1][q->arr[q->t+1]%numCols] = 1;
                    m[(q->arr[q->t+1]/numCols)+1][q->arr[q->t+1]%numCols] = '.';
                }
                else if (i == 2 && (visited[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)-1] == 0))
                {   
                    if (m[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)-1] == 'E')
                    {
                        c=1;
                        break;
                    }
                    
                    enqueue(q,q->arr[q->t+1]-1);
                    visited[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)-1] = 1;
                    m[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)-1] = '.';
                }
                else if(i == 3 && (visited[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)+1] == 0))
                {
                    if (m[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)+1] == 'E')
                    {
                        c=1;
                        break;
                    }
                    
                    enqueue(q,q->arr[q->t+1]+1);
                    visited[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)+1] = 1;
                    m[(q->arr[q->t+1]/numCols)][(q->arr[q->t+1]%numCols)+1] = '.';
                }
            }    
        }
        if (c == 1)
        {
            break;
        }
        
        dequeue(q);
    }

    //Printing the final maze.
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols+1; j++)
        {
            printf("%c",m[i][j]);
        }
        
    }        
    return 0;
}