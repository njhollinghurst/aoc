#include <vector>
#include <map>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <memory>
#include <cstring>
#include <algorithm>
using namespace std;

struct Valve {
  char name[4];
  int flow;
  vector<unsigned> links;
  vector<unsigned> route; // added for part 2
  vector<unsigned> dist; // added for part 2
};

vector<Valve> valves;
unsigned first_good_valve = ~0u; // added for part 2

unsigned rd()
{
  unsigned aa = 0;
  char str[1024];
  while (fgets(str, 1024, stdin) == str) {
    Valve v;
    int ret = sscanf(str, "Valve %2s has flow rate=%d", v.name, &v.flow);
    v.name[2]='\0';
    assert (ret == 2);
    (void)ret;
    char const *ptr = strstr(str+20, "valve");
    assert(ptr);
    while (*ptr) {
      while (isalpha(*ptr)) ++ptr;
      while (*ptr && !isalpha(*ptr)) ++ptr;
      if (!*ptr) break;
      v.links.push_back((uint8_t)ptr[0] + 256u*(uint8_t)ptr[1]);
      ptr += 3;
    }
    if (v.name[0]=='A' && v.name[1]=='A') aa = valves.size();
    if (v.flow > 0 && first_good_valve == ~0u)
      first_good_valve = valves.size();    
    valves.push_back(v);
  }
  for(unsigned i = 0; i < valves.size(); ++i) {
    for(unsigned j = 0; j < valves[i].links.size(); ++j) {
      unsigned k;
      for(k = 0; k < valves.size(); ++k) {
        if (valves[i].links[j] == (uint8_t)valves[k].name[0] + 256u*(uint8_t)valves[k].name[1])
          break;
      }
      assert(k < valves.size());
      valves[i].links[j] = k;
    }
  }
  printf("There are %u valves\n", valves.size());  
  assert(valves.size() <= 64);
  return aa;
}

/////// PART 1

struct State {
  unsigned t;
  unsigned score;
  unsigned cur;
  uint64_t blocks;
  uint64_t unopened;
};
  
unsigned part1(State const &s)
{
  unsigned n = s.score;
  uint64_t mask = 1ull << s.cur;

  if (s.t < 29) {
    if (s.unopened & mask) {
      State s1(s);
      s1.unopened &= ~mask;
      s1.blocks = mask;
      s1.t++;
      s1.score += (29-s.t)*valves[s.cur].flow;
      n = part1(s1);
    }

    State s1(s);
    s1.blocks |= mask;
    s1.t++;
    for(unsigned u = 0; u < valves[s.cur].links.size(); ++u) {
      unsigned j = valves[s.cur].links[u];
      if ((s1.blocks & (1ull << j)) == 0ull) {
        s1.cur = j;
        unsigned n1 = part1(s1);
        if (n1 > n) n = n1;
      }
    }
  }

  return n;
}
  
/////// PART 2

void route()
{
  // This function was added to speed up Part 2 (but is not optimized!)
  // For each good valve, work out the best way to get there from anywhere
  for(unsigned i = 0; i < valves.size(); ++i) {
    valves[i].route.resize(valves.size(), 0x7FFFFFFF);
    valves[i].dist.resize(valves.size(), 0x7FFFFFFF);
  }
  for(unsigned i = 0; i < valves.size(); ++i) {
    valves[i].route[i] = i;
    valves[i].dist[i] = 0;
    if (valves[i].flow > 0) {
      for(;;) {
        bool changed = false;
        for(unsigned j = 0; j < valves.size(); ++j) {
          for(unsigned lk = 0; lk < valves[j].links.size(); ++lk) {
            if (valves[valves[j].links[lk]].dist[i] + 1 < valves[j].dist[i]) {
              valves[j].dist[i] = valves[valves[j].links[lk]].dist[i] + 1;
              valves[j].route[i] = valves[j].links[lk];
              changed = true;
            }
          }
        }
        if (!changed)
          break;
      }
    }
  }
  setbuf(stdout, 0);
  printf("Routed");
}

struct State2 {
  unsigned t;
  unsigned score;
  unsigned cur[2];  // the second element is the elephant
  unsigned dest[2]; // destination valve number
  uint64_t unopened;
  uint64_t chosen;
};

unsigned part2(State2 s, unsigned phase = 0)
{
 again:
  if (s.t >= 25)
    return s.score;

  if (s.dest[phase] < valves.size() && (s.unopened & (1ull << s.dest[phase]))) {
    if (s.cur[phase] != s.dest[phase]) {
      // We (man and/or beast) are heading towards our chosen destination
      // valve, which has not yet been opened. Don't deviate from the route.
      s.cur[phase] = valves[s.cur[phase]].route[s.dest[phase]];
    }
    else {
      // Destination reached, and valve not yet opened, so open it!
      s.unopened &= ~(1ull << s.cur[phase]);
      s.score += (25-s.t)*valves[s.cur[phase]].flow;
    }
    // Now let the other creature take a turn
    s.t += phase;
    phase = !phase;
    goto again;
  }

  // We need to pick a new destination valve. Make sure it's not already open
  // (nor chosen by the other creature when they're sure to get there first)
  // and can be reached before 26 minutes is up. Recurse over all possibilities.
  // [As a symmetry breaker, the elephant may not operate the first valve.]
  unsigned n = s.score;
  for(unsigned nd = first_good_valve + phase; nd < valves.size(); ++nd) {
    if (nd != s.dest[phase] && valves[nd].flow > 0 &&
        ((s.unopened & ~s.chosen) & (1ull << nd)) &&
        valves[s.cur[phase]].dist[nd] < 26 - s.t) {
      if (s.t==0) {
        if (phase==0)
          printf("\nTop level %s ", valves[nd].name);
        else
          putchar('.');
      }
      State2 s1(s);
      s1.dest[phase] = nd;
      if (valves[s1.cur[phase]].dist[nd] < valves[s1.cur[1-phase]].dist[nd]) {
        s1.chosen |= 1ull << nd;
      }
      unsigned n1 = part2(s1, phase);
      if (n1 > n)
        n = n1;
    }
  }
  return n;
}

int main()
{
  unsigned aa = rd();
  
  uint64_t nzflows = 0;
  for(unsigned u = 0; u < valves.size(); ++u)
    if (valves[u].flow > 0)
      nzflows |= (1ull << u);

  unsigned n = part1(State{0,0,aa,0,nzflows});
  printf("Part 1: %d\n", n);

  route();
  n = part2(State2{0,0,{aa,aa},{~0u,~0u},nzflows,0ull});
  printf("\nPart 2: %d\n", n);
  
  return 0;
}
