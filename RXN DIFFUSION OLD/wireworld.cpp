#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define CELL_SIDE 10

const int LEN_COLUMN = SCREEN_HEIGHT / CELL_SIDE;
const int LEN_ROW = SCREEN_WIDTH / CELL_SIDE;

int grid[LEN_ROW][LEN_COLUMN] = {{0}};
int grid2[LEN_ROW][LEN_COLUMN] = {{0}};

bool run = true,showGridLines = false;
short type = 1,no_heads = 0; 
/**
0 - empty
1 - Strong eletron head
2 - Strong electron tail
3 - Strong conductor
4 - Weak eletron head
5 - Weak electron tail
6 - Weak conductor
*/
void implementRules(){
    for(int row=1;row<LEN_ROW-1;row++){
            for(int column=1;column<LEN_COLUMN-1;column++){
                    switch(grid[row][column]){
                        case 1:
                            grid2[row][column] = 2;
                        break;
                        
                        case 2:
                            grid2[row][column] = 3;
                        break;
                        
                        case 3:
                            no_heads=0;
                            for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){if (grid[row+i][column+j]==1){no_heads++;}}}
                            grid2[row][column] = (no_heads==1 || no_heads==2)?1:3;
                            no_heads=0;
                            for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){if (grid[row+i][column+j]==4){no_heads++;}}}
                            grid2[row][column] = (no_heads==2)?1:grid2[row][column];break;
                        case 6:
                            no_heads=0;
                            for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){if (grid[row+i][column+j]==4){no_heads++;}}}
                            grid2[row][column] = (no_heads==1 || no_heads==2)?4:6;
                            no_heads=0;
                            for(int i=-1;i<=1;i++){for(int j=-1;j<=1;j++){if (grid[row+i][column+j]==1){no_heads++;}}}
                            grid2[row][column] = (no_heads==1)?4:grid2[row][column];
                        break;

                        case 4:
                            grid2[row][column] = 5;
                            break;
                        
                        case 5:
                            grid2[row][column] = 6;
                            break;

                        default:
                            grid2[row][column] = grid[row][column];
                            break;}}}}
void updateGridValues(){
    for (int i = 0;i<LEN_ROW ;i++ )
        {
            for (int j=0;j<LEN_COLUMN ;j++ )
            {
                grid[i][j] = grid2[i][j];grid2[i][j] = 0;
            }
        }
}

void drawGrid(){
    for(int row=0; row<LEN_ROW; row++)
        for(int column=0; column<LEN_COLUMN; column++){
                if (grid[row][column]==1)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){40,40,255,255}); 
                if (grid[row][column]==2)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){255,40,40,255}); 
                if (grid[row][column]==3)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){60,60,60,255});

                if (grid[row][column]==4)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){100,100,255,255}); 
                if (grid[row][column]==5)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){255,100,100,255}); 
                if (grid[row][column]==6)DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){100,100,100,255});
            }}

void mouseActions(){
    if (IsKeyPressed(KEY_SPACE))run = !run;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))grid[(int)(GetMouseX()/CELL_SIDE)][(int)(GetMouseY()/CELL_SIDE)] = type;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))grid[(int)(GetMouseX()/CELL_SIDE)][(int)(GetMouseY()/CELL_SIDE)] = 0;
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)){type++;if (type==7)type=1;}
    if (IsKeyPressed(KEY_ENTER))updateGridValues();
    if (IsKeyPressed(KEY_G))showGridLines = !showGridLines;}

void drawLineGrid(){
    for (int i = 0;i<=LEN_ROW ; i++ )DrawLine(i*CELL_SIDE,0,i*CELL_SIDE,SCREEN_HEIGHT,(Color){50,50,50,255});
    for (int i2 = 0; i2 <= LEN_COLUMN; i2++)DrawLine(0,i2*CELL_SIDE,SCREEN_WIDTH,i2*CELL_SIDE,(Color){50,50,50,255});}

int main(void){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Wireworld"); 
    SetTargetFPS(24);
    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground((Color){10,10,10,255});
            drawGrid();
            if (showGridLines)drawLineGrid();
            char* name;
            if     (type==1)name="Strong Electron Head";
            else if(type==2)name="Strong Electron Tail";
            else if(type==3)name="Strong Conductor";
            else if(type==4)name="Weak Electron Head";
            else if(type==5)name="Weak Electron Tail";
            else if(type==6)name="Weak Conductor";
            DrawText(name, 3, 3, 12, (Color){255,255,255,255}); 
        EndDrawing();
        if (run)
        {
            implementRules();
            updateGridValues();
        }
        mouseActions();
    }
    CloseWindow();
}
