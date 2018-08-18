#include "Ship.h"

// CTOR
Ship::Ship(int len) { row = 0, col = 0, length = len; }

int Ship::getLength()  { return length;    }
bool Ship::horizontal() { return hor; }
int Ship::getRow()     { return row;       }
int Ship::getCol()     { return col;       }

void Ship::setOrient(bool h)           { hor = h; }
void Ship::setCoords(int c, int r) { row = r-48, col = c-96; }
