/*
Pattern Parameters

Pattern        | f       | k
---------------|---------|---------
Mitosis        | 0.0367  | 0.0649
Coral Growth   | 0.0545  | 0.062
Fingerprint    | 0.055   | 0.062
Spirals        | 0.018   | 0.051
Worms          | 0.078   | 0.061
Maze           | 0.029   | 0.057
Bubbles        | 0.098   | 0.057
Spots and Loops| 0.039   | 0.058
Waves          | 0.014   | 0.054
Splotches      | 0.026   | 0.051
Solitions      | 0.03    | 0.06
Dots           | 0.05    | 0.065
Stripes        | 0.025   | 0.06
Cross Waves    | 0.012   | 0.045
Mixed          | 0.022   | 0.051
*/
#include "raylib.h"

// Define constants
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIDE 5

const int LEN_COLUMN = SCREEN_HEIGHT / CELL_SIDE;
const int LEN_ROW = SCREEN_WIDTH / CELL_SIDE;

// Structure to represent each cell in the grid
typedef struct Cell {
    float A;  // Concentration of chemical A
    float B;  // Concentration of chemical B
} Cell;

// Initialize the main and temporary grids with default values
Cell grid[LEN_ROW][LEN_COLUMN] = {{{1, 0}}};    // Initial state with high A, low B
Cell temp_grid[LEN_ROW][LEN_COLUMN] = {{{1, 0}}}; // Temporary grid for calculations

// Reaction-diffusion parameters
float D_A = 1.0;
float D_B = 0.5;
float f = 0.0545;  // Feed rate
float k = 0.062;  // Kill rate

// Flag to pause the simulation
bool pause = false;

// Function declarations
void drawGrid(void);
void mouseActions(void);
double constrain(double value, double low, double high);
float LaplacianFunc(int x, int y, bool type);
void simulate(void);

int main(void) {
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reaction Diffusion Simulation");

    // Main game loop
    while (!WindowShouldClose()) {
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        drawGrid();
        EndDrawing();

        // Simulation and user interactions
        if (!pause) simulate();
        mouseActions();
    }

    // Close window and clean up
    CloseWindow();

    return 0;
}

// Function to constrain a value within a specified range
double constrain(double value, double low, double high) {
    return value > high ? high : (value < low ? low : value);
}

// Function to compute the Laplacian with wrapping at the edges
float LaplacianFunc(int x, int y, bool type) {
    int left = (x == 0) ? LEN_ROW - 1 : x - 1;
    int right = (x == LEN_ROW - 1) ? 0 : x + 1;
    int up = (y == 0) ? LEN_COLUMN - 1 : y - 1;
    int down = (y == LEN_COLUMN - 1) ? 0 : y + 1;

    return (type ?
        (0.2 * (grid[right][y].A + grid[left][y].A + grid[x][down].A + grid[x][up].A) +
         0.05 * (grid[left][up].A + grid[right][up].A + grid[left][down].A + grid[right][down].A) - grid[x][y].A) :
        (0.2 * (grid[right][y].B + grid[left][y].B + grid[x][down].B + grid[x][up].B) +
         0.05 * (grid[left][up].B + grid[right][up].B + grid[left][down].B + grid[right][down].B) - grid[x][y].B));
}

// Function to simulate the reaction-diffusion process
void simulate(void) {
    for (int row = 0; row < LEN_ROW; row++) {
        for (int column = 0; column < LEN_COLUMN; column++) {
            // Reaction-diffusion equations
            temp_grid[row][column].A = constrain(grid[row][column].A + (D_A * LaplacianFunc(row, column, true) -
                grid[row][column].A * grid[row][column].B * grid[row][column].B + f * (1 - grid[row][column].A)), 0, 1);
            temp_grid[row][column].B = constrain(grid[row][column].B + (D_B * LaplacianFunc(row, column, false) +
                grid[row][column].A * grid[row][column].B * grid[row][column].B - (k + f) * grid[row][column].B), 0, 1);
        }
    }

    // Update grid and reset temp_grid
    for (int i = 0; i < LEN_ROW; i++) {
        for (int j = 0; j < LEN_COLUMN; j++) {
            grid[i][j] = temp_grid[i][j];
            temp_grid[i][j] = (Cell){1, 0}; // Reset the temporary grid cell
        }
    }
}

// Function to draw the grid
void drawGrid(void) {
    for (int row = 0; row < LEN_ROW; row++) {
        for (int column = 0; column < LEN_COLUMN; column++) {
            // Calculate grayscale color based on concentrations
            int color = constrain(255 - 255 * (grid[row][column].A - grid[row][column].B), 0, 255);
            Color cellColor = (Color){color, color, color, 255};

            // Draw rectangle representing each cell
            DrawRectangle(row * CELL_SIDE, column * CELL_SIDE, CELL_SIDE, CELL_SIDE, cellColor);
        }
    }
}

// Function to handle mouse actions
void mouseActions(void) {
    // Toggle cells on mouse click
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        int x = GetMouseX() / CELL_SIDE;
        int y = GetMouseY() / CELL_SIDE;
        if (x >= 0 && x < LEN_ROW && y >= 0 && y < LEN_COLUMN) {
            grid[x][y] = (Cell){1, 1};
        }
    }

    // Pause/unpause simulation on space key press
    if (IsKeyPressed(KEY_SPACE)) pause = !pause;

    // Reset grid on enter key press
    if (IsKeyPressed(KEY_ENTER)) {
        for (int row = 0; row < LEN_ROW; row++) {
            for (int column = 0; column < LEN_COLUMN; column++) {
                grid[row][column] = (Cell){1, 0};
            }
        }
    }
}
