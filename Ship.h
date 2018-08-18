class Ship {
  public:
    Ship(int len);

    int getLength();

    void setOrient(bool h);
    bool horizontal();

    // r is row c is column. Column should really be inputted as type char and
    // will be then be converted to ascii equivalent
    void setCoords(int c, int r);
    int getRow();
    int getCol();

  private:
    int length;
    int row;
    int col;
    // true is horiz. false is vertical
    bool hor;
};
