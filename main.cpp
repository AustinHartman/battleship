#include <iostream>
#include <vector>
#include "Ship.h"
#include <stdlib.h>
#include <algorithm>

using namespace std;


// water or unknown territory is denoted with a +
// a hit ship is dennoted with and x
// a friendly ship is dennoted with O
bool gameOver;
const int SIZE=10;
int ship_lengths[5] = {2,3,3,4,5};
string guess;
int enemy_guess_x=0, enemy_guess_y=0;
int hit_enemy_ship, hit_friendly_ship;
bool isWinner;
int x=0, y=0;
int lastHit[2] = {100, 0};

const char BLANK_SPACE = '-', SHIP = 'O', HIT_SHIP = 'X', MISS = '~';

vector< vector<char> > enemy_grid(SIZE, vector<char>(SIZE, BLANK_SPACE));
vector< vector<char> > my_grid(SIZE, vector<char>(SIZE, BLANK_SPACE));
vector< vector<char> > enemy_visible_grid(SIZE, vector<char>(SIZE, BLANK_SPACE));

bool alreadyGuessed(int x, int y, const vector< vector<char> > &grid) {
  return (grid[x][y] == HIT_SHIP or grid[x][y] == MISS) ? true : false;
}

void smartGuess(const vector< vector<char> > &grid, int &x, int &y, int lastHit[2]) {
  x = lastHit[0], y = lastHit[1];
  if (x!=100 and x+1 <= SIZE-1 and !alreadyGuessed(x+1, y, grid))      x++;
  else if (x!=100 and y+1 <= SIZE-1 and !alreadyGuessed(x, y+1, grid)) y++;
  else if (x!=100 and x-1 >= 0 and !alreadyGuessed(x-1, y, grid))      x--;
  else if (x!=100 and y-1 >= 0 and !alreadyGuessed(x, y-1, grid))      y--;
  else {
    do { y = rand()%(SIZE-1) + 1, x = rand()%(SIZE-1) + 1; } while (alreadyGuessed(x, y, grid));
  }
}

void randGuess(const vector< vector<char> > &grid, int &x, int &y) {
  do {
    x = rand()%(SIZE-1) + 1;
    y = rand()%(SIZE-1) + 1;
  } while (grid[x][y] == HIT_SHIP or grid[x][y] == MISS);
}

void manualGuess(const vector< vector<char> > &grid, int &x, int &y) {
  bool firstPass = true;
  do {
    if (!firstPass) cout << "invalid, guess again: " << endl;
    string guess;
    cout << "Guess (ex: a1): ";
    cin >> guess;
    x = guess[1] - 48;
    y = guess[0] - 96;
    firstPass = false;
  } while (grid[x][y] == HIT_SHIP or grid[x][y] == MISS);
}
// initialize grid with row and col labels and + in empty spaces
void initGridHeaders(vector< vector<char> > &grid) {
  char col_label = 'a';
  char row_label = '1';
  for (int i=0; i<grid.size(); ++i) {
    for (int n=0; n<grid[i].size(); ++n) {
      grid[i][n] = BLANK_SPACE;
    }
  }
  for (int i=1; i<grid.size(); ++i) {
    grid[0][i] = col_label, col_label++;
    grid[i][0] = row_label, row_label++;
  }
  grid[0][0] = ' ';
}

bool isValid(const vector< vector<char> > &grid, Ship s) {
  bool h = s.horizontal();

  // make sure ship is in board bounds
  if (h and SIZE < s.getRow()+s.getLength()) return false;
  if (!h and SIZE < s.getCol()+s.getLength())   return false;

  // make sure ship does not overlap w/ existing ships
  for (int i=0; i<s.getLength(); ++i) {
    if (h and grid[s.getRow()+i][s.getCol()] == SHIP) return false;
    else if (!h and grid[s.getRow()][s.getCol()+i] == SHIP) return false;
  }
  return true;
}

bool shipAtCoord(vector< vector<char> > coords, int i, int j) {
  if (coords[i][j] == SHIP) return true;
  else return false;
}

void printGrid(const vector< vector<char> > &grid) {
  for (int i=0; i<grid.size(); i++) {
    for (int j=0; j<grid[i].size(); j++) {
      cout << grid[j][i] << " ";
    }
    cout << endl;
  }
}

void Setup() {
  hit_enemy_ship = 0, hit_friendly_ship = 0;
  gameOver = false;
  /* initialize random seed: */
  initGridHeaders(my_grid);
  initGridHeaders(enemy_grid);
  initGridHeaders(enemy_visible_grid);


  /*
  // choose ship locations manually
  for (int i=0; i<5; ++i) {
    Ship ship(ship_lengths[i]);
    bool error = false;
    do {
      string loc;
      if (error) cout << "ERROR TRY AGAIN:" << endl;
      system("clear");
      printGrid(my_grid);
      error = true;
      cout << "where should the ship be placed? (col+row+hor: ex: b3h) ";
      cin >> loc;
      ship.setCoords(loc[0], loc[1]);
      (loc[2]=='h') ? ship.setOrient(true) : ship.setOrient(false);
    } while (!isValid(my_grid, ship));
    for (int n=0; n<ship.getLength(); ++n) {
      if (ship.horizontal()) my_grid[ship.getRow()+n][ship.getCol()] = SHIP;
      else my_grid[ship.getRow()][ship.getCol()+n] = SHIP;
    }
  }
  */

  for (int i=0; i<5; ++i) {
    Ship ship(ship_lengths[i]);
    do {
      // normally coord inputs are as the string of a col letter or row number
      ship.setCoords(rand()%(SIZE-1) + 96 + 1, rand()%(SIZE-1) + 48 + 1);
      (rand()%2==1) ? ship.setOrient(true) : ship.setOrient(false);
    } while (!isValid(my_grid, ship));
    for (int n=0; n<ship.getLength(); ++n) {
      if (ship.horizontal()) my_grid[ship.getRow()+n][ship.getCol()] = SHIP;
      else my_grid[ship.getRow()][ship.getCol()+n] = SHIP;
    }
  }

  // randomly spawn enemy ships
  for (int i=0; i<5; ++i) {
    Ship ship(ship_lengths[i]);
    do {
      // normally coord inputs are as the string of a col letter or row number
      ship.setCoords(rand()%(SIZE-1) + 96 + 1, rand()%(SIZE-1) + 48 + 1);
      (rand()%2==1) ? ship.setOrient(true) : ship.setOrient(false);
    } while (!isValid(enemy_grid, ship));
    for (int n=0; n<ship.getLength(); ++n) {
      if (ship.horizontal()) enemy_grid[ship.getRow()+n][ship.getCol()] = SHIP;
      else enemy_grid[ship.getRow()][ship.getCol()+n] = SHIP;
    }
  }
}

void Draw() {
  system("clear");
  cout << "       BATTLESHIP\n\n";
  printGrid(enemy_visible_grid);
  cout << "------------------" << endl;
  printGrid(my_grid);
}

void Input() {
  manualGuess(enemy_grid, x, y);

  // produce enemy guess randomly w/ func
  smartGuess(my_grid, enemy_guess_x, enemy_guess_y, lastHit);
}

void Logic() {
  // check quit
  if (guess== "quit") gameOver = true;

  // check location of coords and update gris
  if (enemy_grid[x][y] == SHIP) {
    enemy_grid[x][y] = HIT_SHIP;
    enemy_visible_grid[x][y] = HIT_SHIP;
    hit_enemy_ship++;
  }
  else enemy_grid[x][y] = MISS, enemy_visible_grid[x][y] = MISS;

  // process enemy guess
  if (my_grid[enemy_guess_x][enemy_guess_y] == SHIP) {
    my_grid[enemy_guess_x][enemy_guess_y] = HIT_SHIP;
    hit_friendly_ship++;
    lastHit[0]=enemy_guess_x, lastHit[1]=enemy_guess_y;
  }
  else my_grid[enemy_guess_x][enemy_guess_y] = MISS;

  if (hit_enemy_ship==17 && hit_friendly_ship==17) {
    (rand()%2==1) ? isWinner=true : isWinner=false;
    gameOver=true;
  }
  else if (hit_enemy_ship == 17) isWinner = true, gameOver = true;
  else if (hit_friendly_ship == 17) isWinner = false, gameOver = true;
}

int main() {
  srand (time(NULL));

  Setup();
  while (!gameOver) {
    Draw();
    Input();
    Logic();
  }
  return 0;
}
