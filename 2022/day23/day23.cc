#include <cstdio>
#include <utility>
#include <set>
#include <cassert>
#include <map>
using namespace std;

set<pair<int,int> > elves;
int xymax[2] = { 0, 0 };
int xymin[2] = { 9999, 9999 };

void rd()
{
  int c, x = 0, y = 0;
  while ((c = getchar()) > 0) {
    assert(c=='\n' || c=='.' || c=='#');
    if (c=='#') {
      if (x > xymax[0]) xymax[0] = x;
      if (y > xymax[1]) xymax[1] = y;
      if (x < xymin[0]) xymin[0] = x;
      if (y < xymin[1]) xymin[1] = y;
      elves.insert(pair<int,int>(x,y));
    }
    if (c=='\n')
      x = 0, y++;
    else
      x++;
  }
}

#define Z(s,x,y) (((s).count(pair<int,int>(x,y)))==0)

bool mv(unsigned phase)
{
  set<pair<int,int> > new_elves(elves);
  set<pair<int,int> > proposals;
  set<pair<int,int> > collisions;
  bool any_move = false;

  for(unsigned stage = 0; stage < 2; ++stage) {
    for(auto const & e : elves) {
      int x = e.first;
      int y = e.second;
      if (Z(elves,x-1,y-1) && Z(elves,x-1,y) && Z(elves,x-1,y+1) &&
          Z(elves,x,y-1) && Z(elves,x,y+1) &&
          Z(elves,x+1,y-1) && Z(elves,x+1,y) && Z(elves,x+1,y+1))
        continue; // Elf has no need to move

      any_move = true;
      for(unsigned u = 0; u < 4; ++u) {
        int dx = 0, dy = 0;
        switch ((phase+u) & 3) {
        case 0: // N
          dy = -1;
          break;
        case 1: // S
          dy=1;
          break;
        case 2: // W
          dx = -1;
          break;
        default: // E
          dx = 1;
        }
        if (Z(elves,x+dx,y+dy) && Z(elves,x+dx+dy,y+dy+dx) && Z(elves,x+dx-dy,y+dy-dx)) {
          if (stage==0) {
            if (Z(proposals,x+dx,y+dy))
              proposals.insert(pair<int,int>(x+dx,y+dy));
            else
              collisions.insert(pair<int,int>(x+dx,y+dy));
          }
          else {
            if (Z(collisions,x+dx,y+dy)) {
              new_elves.erase(pair<int,int>(x,y));
              x += dx;
              y += dy;
              if (x > xymax[0]) xymax[0] = x;
              if (y > xymax[1]) xymax[1] = y;
              if (x < xymin[0]) xymin[0] = x;
              if (y < xymin[1]) xymin[1] = y;
              new_elves.insert(pair<int,int>(x,y));
            }
          }
          break;
        }
      }
    }
  }
  assert(elves.size() == new_elves.size());
  elves = move(new_elves);
  return any_move;
}


void viz()
{
  for(int y = xymin[1]; y <= xymax[1]; ++y) {
    for(int x = xymin[0]; x <= xymax[0]; ++x) {
      if (elves.count(pair<int,int>(x,y))) putchar('#'); else putchar('.');
    }
    printf("\n");
  }
  printf("\n");
}

int main()
{
  rd();

  unsigned round = 0;
  for(int part = 1; part <= 2; part++) {
    printf("Part %d:\n", part);
    //viz();
    for(; (part > 1 || round < 10); round++) {
      if (!mv(round)) {
        printf("Terminate in round %u\n", round+1);
        break;
      }
      //viz();
    }

    xymin[0]=xymin[1]=9999;
    xymax[0]=xymax[1]=0;
    for(auto const &e : elves) {
      int x = e.first;
      int y = e.second;
      if (x > xymax[0]) xymax[0] = x;
      if (y > xymax[1]) xymax[1] = y;
      if (x < xymin[0]) xymin[0] = x;
      if (y < xymin[1]) xymin[1] = y;
    }

    printf("Bounding box: %d,%d..%d,%d; area: %lld; elves: %u; space: %lld\n",
           xymin[0], xymin[1], xymax[0], xymax[1],
           (long long)(xymax[0]-xymin[0]+1)*(long long)(xymax[1]-xymin[1]+1),
           elves.size(),
           (long long)(xymax[0]-xymin[0]+1)*(long long)(xymax[1]-xymin[1]+1) - elves.size());
  }

  return 0;
}
