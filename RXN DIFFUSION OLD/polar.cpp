#include "raylib.h"
#include <cmath>
#include <vector>

int zoom = 1000;
const int textSize = 18;
const int lim = 1000000;
std::vector<int> prime_list;
std::vector<int> num_list;

void generatePrimes(void);
void generateNums(void);
void draw(std::vector<int>);
void controlZoom(void);

int main(void)
{
    InitWindow(800, 450, "primes");         
    generatePrimes();
    //generateNums();
    
    while (!WindowShouldClose())
    {
       BeginDrawing();
            ClearBackground((Color){00,00,00,255});
            draw(prime_list);
            DrawText(TextFormat("Zoom: %i",zoom), 5, 5, textSize, (Color){255,255,255,255}); 
       EndDrawing();
       controlZoom();
    }
    CloseWindow();    
}


void generateNums()
{
    for (int i=0;i<lim;i+=5)
        num_list.push_back(i);
}

void generatePrimes()
{
    for(int i=0;i<lim;i++)
    {
        bool flag = true;
        for(int j = 2;j<sqrt(i);j++)
        {
             if (i%j==0)
            {
                flag=false;
                break;
            }
        }   
        if(i>1 && flag == true)
            prime_list.push_back(i);
    }
}

void draw(std::vector<int> vec)
{
    for(int a=0;a<vec.size();a++)
    {
        int x = ((vec[a]*cos(vec[a]))/zoom)+400;
        int y = ((vec[a]*sin(vec[a]))/zoom)+225;
        DrawPixel(x, y, LIME);
    }     
}

void controlZoom()
{
    if(IsKeyDown(KEY_I))
        zoom -= (100 * GetFrameTime());
    if (IsKeyDown(KEY_D))
        zoom += (100 * GetFrameTime());
}
