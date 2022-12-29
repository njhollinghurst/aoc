#include <cstdio>
#include <cassert>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <unistd.h>
using namespace std;

struct Row {
  vector<char> v;
  unsigned x_start;
  unsigned x_end;
  bool has_wall;
};
vector<Row> map;

struct Move {
  unsigned dist;
  unsigned dir;
};
vector<Move> moves;

unsigned pos_x = 0, pos_y = 0;
unsigned facing = 0;

static inline char C(unsigned y, unsigned x)
{
  if (y >= map.size()) return ' ';
  if (x >= map[y].v.size()) return ' ';
  return map[y].v[x];
}

void rd()
{
  int c;
  while ((c=getchar()) > 0) {
    if (c=='\n' || c=='\r')
      continue;

    ungetc(c,stdin);
    if (isdigit(c)) {
      break;
    }
    vector<char> s;
    unsigned x_start = 0, x_end = 0;
    bool has_wall = false;
    bool seen_any = false;
    while ((c=getchar()) >= ' ') {
      if (c=='.' || c=='#') {
        x_end = s.size()+1;
        if (!seen_any)
          x_start = s.size();
        seen_any = true;
        if (c=='#')
          has_wall = true;
      }
      s.push_back(c);
    }
    map.push_back(Row{move(s),x_start,x_end,has_wall});
  }

  Move m;
  char dirchar = '\0';
  while (scanf("%u%c", &m.dist, &dirchar) >= 1) {
    if (dirchar=='R')
      m.dir = 1;
    else if (dirchar=='L')
      m.dir = 3;
    else
      m.dir = 0;
    moves.push_back(m);
    if (dirchar != 'R' && dirchar != 'L')
      break;
    dirchar='\0';
  }

  pos_x = map[0].x_start;
}

void exec(Move const &m)
{
  for(unsigned d = m.dist; d > 0; d--) {
    unsigned new_x = pos_x;
    unsigned new_y = pos_y;
    switch (facing) {
    case 0: // right
      if (new_x + 1 >= map[pos_y].x_end) {
        new_x = map[pos_y].x_start;
      }
      else {
        new_x = pos_x + 1;
      }
      break;
    case 2: // left
      if (pos_x <= map[pos_y].x_start) {
        new_x = map[pos_y].x_end - 1;
      }
      else {
        new_x = pos_x - 1;
      }
      break;
    case 3: // up
      if ((pos_y == 0 || C(pos_y-1,pos_x)==' ')) {
        new_y = map.size() - 1;
        while(C(new_y,pos_x) == ' ')
          new_y--;
      }
      else {
        new_y--;
      }
      break;
    case 1: // down
      if (C(pos_y+1,pos_x)==' ') {
        new_y = 0;
        while(C(new_y,pos_x) == ' ')
          new_y++;
      }
      else {
        new_y++;
      }
      break;
    default:
      assert(0);
    }

    if (map[new_y].v[new_x]=='#') {
      break;
    }
    else {
      pos_x = new_x;
      pos_y = new_y;
      assert(map[pos_y].v[pos_x]=='.');
    }
  }
  facing = (facing + m.dir) & 3;
}

void exec2(Move const &m) // for Part 2
{
  for(unsigned d = m.dist; d > 0; d--) {
    unsigned new_x = pos_x;
    unsigned new_y = pos_y;
    unsigned new_facing = facing; // needed to add "new_facing" for Part 2
    switch (facing) {
    case 0: // right
      new_x = pos_x + 1;
      break;
    case 2: // left
      new_x = pos_x - 1;
      break;
    case 3: // up
      new_y = pos_y - 1;
      break;
    case 1: // down
      new_y = pos_y + 1;
      break;
    default:
      assert(0);
    }

    if (C(new_y,new_x)==' ') {
      // XXX Horrible by-hand wrapping for Part 2, only works with my personal input!
      switch (facing) {
      case 0: // Right
        if ((pos_y/50)%2 == 0) { // F
          new_facing = 2;
          new_y = 149-pos_y;
          new_x = (pos_x > 110) ? 99: 149;
        }
        else if (pos_y < 100) { // C
          new_facing = 3;
          new_y = 49;
          new_x = pos_y+50;
        }
        else { // B
          new_facing = 3;
          new_y = 149;
          new_x = pos_y-100;
        }
        break;
      case 1: // Down
        if (pos_x < 50) { // E
          new_x += 100;
          new_y = 0;
        }
        else if (pos_x < 100) { // B
          new_facing = 2;
          new_y = pos_x+100;
          new_x = 49;
        }
        else { //C
          new_facing = 2;
          new_y = pos_x-50;
          new_x = 99;
        }
        break;
      case 2: // Left
        if (pos_y < 50) { // G
          new_facing = 0;
          new_x = 0;
          new_y = 149-pos_y;
        }
        else if (pos_y < 100) { // D
          new_facing=1;
          new_y = 100;
          new_x = pos_y-50;
        }
        else if (pos_y < 150) { //G
          new_facing = 0;
          new_x = 50;
          new_y = 149-pos_y;
        }
        else { // A
          new_facing = 1;
          new_y = 0;
          new_x = pos_y-100;
        }
        break;
      default: // Up
        if (pos_x < 50) { // D
          new_facing = 0;
          new_x = 50;
          new_y = pos_x+50;
        }
        else if (pos_x < 100) { // A
          new_facing = 0;
          new_x = 0;
          new_y = pos_x+100;
        }
        else { // E
          new_x -= 100;
          new_y = 199;
        }
      }
    }

    if (map[new_y].v[new_x]=='#') {
      break;
    }
    else {
      pos_x = new_x;
      pos_y = new_y;
      facing = new_facing;
      assert(map[pos_y].v[pos_x]=='.');
    }
  }
  facing = (facing + m.dir) & 3;
}

int main()
{
  // Part 1
  rd();
  for(auto const &m : moves)
    exec(m);
  printf("Part 1: Final facing %d, position %3u,%3u: %u\n", facing, pos_x, pos_y, 1000*(pos_y+1) + 4*(pos_x+1) + facing);

  if (map.size() != 200) {
    printf("Sorry, I can only do Part 2 for my input\n");
    return 1;
  }

  facing = 0;
  pos_y = 0;
  pos_x = map[0].x_start;
  for(auto const &m : moves)
    exec2(m);
  printf("Part 2: Final facing %d, position %3u,%3u: %u\n", facing, pos_x, pos_y, 1000*(pos_y+1) + 4*(pos_x+1) + facing);

  return 0;
}
