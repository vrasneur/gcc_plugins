#include <iostream>

using namespace std;

class Point {
    int _x, _y;

public:
    Point() : _x(0), _y(0) {}
    Point(int x, int y) : _x(x), _y(y) {}
    void setX(const int val);
    void setY(const int val);
    int getX() { return _x; }
    int getY() { return _y; }
};

typedef Point Point2;

int main(void)
{
  Point apoint;
  Point2 dpoint;
  Point cpoint(1, 2);
  Point *bpoint = new Point();
  int a;

  cout << apoint.getX() << endl;

  return 0;
}
