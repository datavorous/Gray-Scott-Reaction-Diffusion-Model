#include "raylib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define CELL_SIDE 5
const int LEN_COLUMN = SCREEN_HEIGHT / CELL_SIDE;
const int LEN_ROW = SCREEN_WIDTH / CELL_SIDE;
typedef struct Cell{
    float A;
    float B;
}Cell;
Cell grid[LEN_ROW][LEN_COLUMN] = {{{1,0}}};
Cell temp_grid[LEN_ROW][LEN_COLUMN] = {{{1,0}}};
float D_A = 1.0;  
float D_B = 0.5; 
float f = 0.0545; 
float k = 0.062; 
/**
Mitosis:
f = 0.0367
k = 0.0649

Coral Growth:
f = 0.0545
k = 0.062
*/
float pause = false;
void drawGrid(void);
void mouseActions(void);
double constrain(double value,double low,double high)
{
    return value>high?high:(value<low?low:value);
}
float LaplacianFunc(int x,int y, bool type)
{
    return (type?
    	((0.2*(grid[x+1][y].A+grid[x-1][y].A+grid[x][y+1].A+grid[x][y-1].A) +
    	    	 0.05*(grid[x-1][y-1].A+grid[x+1][y-1].A+grid[x-1][y+1].A+grid[x+1][y+1].A)) - grid[x][y].A):
    	((0.2*(grid[x-1][y].B+grid[x][y+1].B+grid[x+1][y].B+grid[x][y-1].B) + 
    	    	 0.05*(grid[x-1][y-1].B+grid[x-1][y+1].B+grid[x+1][y+1].B+grid[x+1][y-1].B)) - grid[x][y].B));
}
void simulate()
{
    for(int row = 1; row<LEN_ROW-1; row++)
    {
        for(int column = 1; column<LEN_COLUMN-1; column++)
        {
            temp_grid[row][column].A = constrain((grid[row][column].A + (D_A* LaplacianFunc(row,column,true)- grid[row][column].A*grid[row][column].B*grid[row][column].B
                                     + f*(1-grid[row][column].A))*1),0,1);
            temp_grid[row][column].B = constrain((grid[row][column].B + (D_B* LaplacianFunc(row,column,false)+ grid[row][column].A*grid[row][column].B*grid[row][column].B
                                     - (k+f)*grid[row][column].B)*1),0,1);
        }
    }
    for(int i = 0;i<LEN_ROW;i++)
    {
        for(int j = 0;j<LEN_COLUMN;j++)
        {
            grid[i][j] = temp_grid[i][j];
            temp_grid[i][j] = {1,0};
        }  
    }           
}
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reaction Diffusion"); 
    while (!WindowShouldClose())
    {
    	BeginDrawing();
	        ClearBackground((Color){0,0,0,255});
	        drawGrid();
	    EndDrawing();
	    if(!pause)simulate();
        mouseActions();
    }
    CloseWindow();    
}
void drawGrid()
{
    for(int row=0; row<LEN_ROW; row++)
        for(int column=0; column<LEN_COLUMN; column++)
                DrawRectangle(row*CELL_SIDE, column*CELL_SIDE, CELL_SIDE, CELL_SIDE, (Color){
                	constrain(255-255*(grid[row][column].A-grid[row][column].B),0,255),
                	constrain(255-255*(grid[row][column].A-grid[row][column].B),0,255),
                	constrain(255-255*(grid[row][column].A-grid[row][column].B),0,255),
                	constrain(255-255*(grid[row][column].A-grid[row][column].B),0,255)}); 
}
void mouseActions()
{ 
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))grid[(int)(GetMouseX()/CELL_SIDE)][(int)(GetMouseY()/CELL_SIDE)] = {1,1};
    if (IsKeyPressed(KEY_SPACE))pause = !pause;
    if (IsKeyPressed(KEY_ENTER))
        for(int row=0; row<LEN_ROW; row++)
            for(int column=0; column<LEN_COLUMN; column++)
                grid[row][column] = {1,0};
}
