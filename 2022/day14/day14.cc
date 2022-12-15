#include <set>
#include <utility>
#include <cstdio>
#include <cstdint>
#include <cctype>
using namespace std;

typedef pair<uint16_t,uint16_t> Pos;

set<Pos> cave;
int xmin = 0x7FFFFFFF, xmax = 0;
int ymin = 0x7FFFFFFF, ymax = 0;

void rd()
{
  int c, x = -1, y = -1;
  while ((c = getchar()) > 0) {
    if (c=='\n' || c=='\r') {
      x = -1;
    }
    else if (isdigit(c)) {
      int x1 = 0, y1 = 0;
      ungetc(c, stdin);
      scanf("%d,%d", &x1, &y1);
      if (x1 < xmin) xmin = x1;
      if (x1 > xmax) xmax = x1;
      if (y1 < ymin) ymin = y1;
      if (y1 > ymax) ymax = y1;
      if (x != -1) {
        while (x != x1 || y != y1) {
          cave.insert(Pos{x,y});
          if (x1 > x) x++;
          if (x1 < x) x--;
          if (y1 > y) y++;
          if (y1 < y) y--;
        }
      }
      x = x1;
      y = y1;
      cave.insert(Pos{x,y});
    }
    else if (c=='-') {
      (void)getchar();
    }
    else if (!isspace(c)) {
      printf("[%c]?\n", c);
    }
  }
}

void fall(int part)
{
  while (true) {
    uint16_t x = 500, y = 0; // uh, let's allow x wraparound, just in case it's useful
    while (y <= ymax) {
      if (cave.count(Pos{x,y+1}) == 0) {
        y++;
        continue;
      }
      if (cave.count(Pos{x-1,y+1}) == 0) {
        x--;
        y++;
        continue;
      }
      if (cave.count(Pos{x+1,y+1}) == 0) {
        x++;
        y++;
        continue;
      }
      break;
    }

    // Sand has reached ymax+1; in Part 1 this terminates.
    if (y > ymax && part == 1)
      break;

    cave.insert(Pos{x,y});
    if (cave.count(Pos{500,0}))
      break;
  }
}

int main()
{
  rd();
  unsigned nrock = cave.size();
  printf("Rock ranges from %d,%d to %d,%d (%u)\n", xmin, ymin, xmax, ymax, nrock);

  fall(1);
  printf("Sand at rest: %u\n", cave.size() - nrock);

  fall(2);
  printf("Sand at rest: %u\n", cave.size() - nrock);
  
  return 0;
}
