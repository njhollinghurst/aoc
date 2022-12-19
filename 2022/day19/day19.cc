#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <vector>
#include <set>
using namespace std;

struct State {
  unsigned res[4]; // ore, clay, obsidian, grodes
  unsigned robots[4];
  bool operator<(State const &r) const { //added for Part 2
    if (robots[3] != r.robots[3]) return (robots[3] < r.robots[3]);
    if (res[3] != r.res[3]) return (res[3] < r.res[3]);
    if (robots[2] != r.robots[2]) return (robots[2] < r.robots[2]);
    if (res[2] != r.res[2]) return (res[2] < r.res[2]);
    if (robots[1] != r.robots[1]) return (robots[1] < r.robots[1]);
    if (res[1] != r.res[1]) return (res[1] < r.res[1]);
    if (robots[0] != r.robots[0]) return (robots[0] < r.robots[0]);
    return (res[0] < r.res[0]);
  }
};

struct Blueprint {
  uint16_t cost[4][2]; // cost of each robot in terms of each applicable resource [ore][n-1]
  unsigned maxcost;
};

vector<Blueprint> blueprints;

void rd()
{
  while (!feof(stdin)) {
    Blueprint b;
    int c;
    unsigned u, v;
    memset(&b, 0, sizeof(b));
    do c = getchar(); while (c > 0 && c != 'B');
    if (c <= 0) break;
    scanf("lueprint %u:", &u);
    assert(u == blueprints.size() + 1);
    for(unsigned i = 0; i < 4; ++i) {
      do c = getchar(); while (c > 0 && c != 'E');
      assert(c > 0);
      u = 0;
      v = 0;
      c = scanf("ach %*s robot costs %u ore and %u", &u, &v);
      assert(c >= 1 && (i < 2 || c >= 2));
      b.cost[i][0] = u;
      b.cost[i][1] = v;
      if (u > b.maxcost) b.maxcost = u;
      if (v > b.maxcost) b.maxcost = v;
      //printf("BP%d robot %u cost %u %u\n", blueprints.size()+1,  i, u, v);
    }
    blueprints.push_back(b);
  }
}

unsigned search(State const &s, Blueprint const &b, unsigned t, set<State> *pset = 0)
{
  unsigned score = s.res[3] + t * s.robots[3];

  // Added for part 2 -- record the 16384 "best" finishing states
  if (pset && t <= b.maxcost) {
    static const unsigned NBEST = 16384;
    State s0(s);
    s0.res[0] += t*s.robots[0];
    s0.res[1] += t*s.robots[1];
    s0.res[2] += t*s.robots[2];
    s0.res[3] += t*s.robots[3];
    if (!pset->count(s0)) {
      if (pset->size() < NBEST || *pset->begin() < s0) {
        pset->insert(s0);
        if (pset->size() >= NBEST)
          pset->erase(pset->begin());
      }
    }
  }

  if (t == 0) {
    return score;
  }
  assert(s.res[0] || s.robots[0]);
  for(unsigned what2build = 0; what2build < 4; ++what2build) {
    if (what2build < 2 || s.robots[what2build-1] > 0) {
      unsigned tx = 0;
      if (b.cost[what2build][0] > s.res[0])
        tx = (b.cost[what2build][0] - s.res[0] + s.robots[0] - 1) / s.robots[0];
      if (what2build >= 2 && b.cost[what2build][1] > s.res[what2build-1])
        tx = max(tx, (b.cost[what2build][1] - s.res[what2build-1] + s.robots[what2build-1] - 1) / s.robots[what2build-1]);
      if (++tx <= t) {
        State s1(s);
        s1.res[0] += tx * s.robots[0];
        s1.res[1] += tx * s.robots[1];
        s1.res[2] += tx * s.robots[2];
        s1.res[3] += tx * s.robots[3];
        s1.res[0] -= b.cost[what2build][0];
        if (what2build >= 2) s1.res[what2build-1] -= b.cost[what2build][1];
        s1.robots[what2build]++;
        unsigned score1 = search(s1, b, t-tx, pset);
        if (score1 > score)
          score = score1;
      }
    }
  }
  return score;
}

set<State> states_after_part1[3]; // Added for part 2

void part1()
{
  unsigned long long total = 0;
  for(unsigned bi = 0; bi < blueprints.size(); ++bi) {
    State s;
    memset(&s, 0, sizeof(s));
    s.robots[0] = 1;
    unsigned score = search(s, blueprints[bi], 24, (bi < 3) ? &states_after_part1[bi] : 0);
    total += (bi+1) * (unsigned long long)score;
    printf("Blueprint %u scores %u\n", bi+1, score);
  }
  printf("Total: %llu\n", total);
}

void part2()
{
  unsigned long long product = 1;
  for(unsigned bi = 0; bi < blueprints.size() && bi < 3; ++bi) {
      unsigned score = 0;
      for(auto const &s : states_after_part1[bi]) {
        unsigned score1 = search(s, blueprints[bi], 8, 0);
        if (score1 > score)
          score = score1;
      }
      product *= (unsigned long long)score;
      printf("Blueprint %u scores %u\n", bi+1, score);
  }
  printf("Product: %llu\n", product);
}

int main()
{
  rd();
  part1();
  part2();
  return 0;
}
