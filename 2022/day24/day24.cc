#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdio>
#include <iostream>
#include <climits>
#include <cassert>
#include <cstdint>
#include <utility>
#include <algorithm>
using namespace std;

vector<string> puzzle_data;
vector<vector<vector<bool> > > danger;
unsigned H, W, N;

static unsigned lcm(unsigned a, unsigned b)
{
  unsigned prod = a * b;
  while (b) {
    unsigned r = a % b;
    a = b;
    b = r;
  }
  return prod / a;
}

void rd()
{
  string s;
  while (cin >> s)
    puzzle_data.push_back(s);

  H = puzzle_data.size();
  W = puzzle_data[0].size();
  N = lcm(H-2, W-2);

  danger.resize(H);
  for (unsigned y = 0; y < H; ++y) {
    for (unsigned x = 0; x < W; ++x) {
      danger[y].emplace_back(vector<bool>(N,puzzle_data[y][x] == '#'));
    }
  }
  for (unsigned y = 0; y < H; ++y) {
    for (unsigned x = 0; x < W; ++x) {
      switch (puzzle_data[y][x]) {
      case '>':
        for(unsigned t = 0; t < N; ++t)
          danger[y][(x-1+t)%(W-2)+1][t] = true;
        break;
      case '<':
        for(unsigned t = 0; t < N; ++t)
          danger[y][(x+N-1-t)%(W-2)+1][t] = true;
        break;
      case 'v':
        for(unsigned t = 0; t < N; ++t)
          danger[(y-1+t)%(H-2)+1][x][t] = true;
        break;
      case '^':
        for(unsigned t = 0; t < N; ++t)
          danger[(y+N-1-t)%(H-2)+1][x][t] = true;
        break;
      default:
        assert(puzzle_data[y][x] == '.' || puzzle_data[y][x] == '#');
      }
    }
  }
}

struct State {
  unsigned y;
  unsigned x;
  unsigned t;
  bool operator<(State const &r) const {
    if (t != r.t) return t < r.t;
    if (x != r.x) return x < r.x;
    return y < r.y;
  }
};

set<State> visited;
set<State> candidates_by_time;
map<State,unsigned> candidates_by_phase;

void update(unsigned y, unsigned x, unsigned t)
{
  unsigned phase = t%N;
  State s0{y,x,phase};
  auto it = candidates_by_phase.find(s0);
  if (it != candidates_by_phase.end()) {
    if (t < it->second) {
      candidates_by_time.erase(State{y,x,it->second});
      goto foo;
    }
    return;
  }
 foo:
  candidates_by_time.insert(State{y,x,t});
  candidates_by_phase[s0] = t;
}

void visit(State const & c)
{
  unsigned y = c.y;
  unsigned x = c.x;
  unsigned t = c.t;

  State c0{y,x,t%N};
  if (visited.count(c0))
    return;

  unsigned phase = (++t) % N;
  if (y > 0 && !danger[y-1][x][phase]) {
    update(y-1,x,t);
  }
  if (y + 1 < H && !danger[y+1][x][phase]) {
    update(y+1,x,t);
  }
  if (x > 0 && !danger[y][x-1][phase]) {
    update(y,x-1,t);
  }
  if (x + 1 < W && !danger[y][x+1][phase]) {
    update(y,x+1,t);
  }
  if (!danger[y][x][phase]) {
    update(y,x,t);
  }
}

unsigned part1(unsigned t0 = 0) // argument "t0" added for part 2
{
  visit(State{0,1,t0});
  while (!candidates_by_time.empty()) {
    State c = *candidates_by_time.begin();
    candidates_by_time.erase(candidates_by_time.begin());
    candidates_by_phase.erase(candidates_by_phase.find(State{c.y,c.x,c.t%N}));
    if (c.y == H-1)
      return c.t;
    visit(c);
  }
  return 0;
}

static inline void clr()
{
  visited.clear();
  candidates_by_time.clear();
  candidates_by_phase.clear();
}

// Part 2 is fairly easy because the start and end postions are
// refuges from the storm; there is no penalty for arriving early.
// Therefore we can split the journey into 3 independent stages.

unsigned part2(unsigned t1)
{
  clr();
  visit(State{H-1,W-2,t1});
  while (!candidates_by_time.empty()) {
    State c = *candidates_by_time.begin();
    candidates_by_time.erase(candidates_by_time.begin());
    candidates_by_phase.erase(candidates_by_phase.find(State{c.y,c.x,c.t%N}));
    if (c.y == 0) {
      clr();
      return part1(c.t);
    }
    visit(c);
  }
  return 0;
}

void viz(unsigned t)
{
  for(unsigned y = 0; y < H; ++y) {
    for(unsigned x = 0; x < W; ++x) {
      cout << ((puzzle_data[y][x]=='#')?'#':(danger[y][x][t%N])?'@':'.');
    }
    cout << endl;
  }
}

int main()
{
  rd();
  unsigned t = part1();
  cout << "Part 1: " << t << endl;

  t = part2(t);
  cout << "Part 2: " << t << endl;

  return 0;
}
