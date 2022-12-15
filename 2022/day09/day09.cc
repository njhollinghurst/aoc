#include <set>
#include <cstdint>
#include <cstdio>
using namespace std;

#define LENGTH 9 // 1 for part1, 9 for part2

int16_t h[2] = { 0, 0 };
int16_t t[LENGTH][2] = { { 0, 0 } };
set<uint32_t> visited;
unsigned nvisited;

void step(int x, int y)
{
  for(int i = 0; i < LENGTH; ++i) {
    if (x>t[i][0]+1 || x<t[i][0]-1 || y>t[i][1]+1 || y<t[i][1]-1) {
      if (x==t[i][0]) {
	if (y>t[i][1]+1) t[i][1]++;
	else if (y<t[i][1]-1) t[i][1]--;
      }
      else if (y==t[i][1]) {
	if (x>t[i][0]+1) t[i][0]++;
	else if (x<t[i][0]-1) t[i][0]--;
      }
      else if (x>t[i][0] && y>t[i][1]) t[i][0]++,t[i][1]++;
      else if (x<t[i][0] && y>t[i][1]) t[i][0]--,t[i][1]++;
      else if (x>t[i][0] && y<t[i][1]) t[i][0]++,t[i][1]--;
      else if (x<t[i][0] && y<t[i][1]) t[i][0]--,t[i][1]--;
    }
    x = t[i][0];
    y = t[i][1];
  }
  uint32_t u = (x&0xFFFF) + (y<<16);
  if (visited.find(u) == visited.end()) {
    visited.insert(u);
    ++nvisited;
  }
}

int main()
{
  char dir;
  unsigned num;
  while (scanf("%1s %u", &dir, &num) >= 2) {
    switch (dir) {
    case 'U':
      while (num-- > 0) {
	h[1]--;
	step(h[0],h[1]);
      }
      break;
    case 'D':
      while (num-- > 0) {
	h[1]++;
	step(h[0],h[1]);
      }
      break;
    case 'L':
      while (num-- > 0) {
	h[0]--;
	step(h[0],h[1]);
      }
      break;
    case 'R':
      while (num-- > 0) {
	h[0]++;
	step(h[0],h[1]);
      }
      break;
    default:
      puts("?");
    }
  }

  printf("%u\n", nvisited);
  return 0;
}
