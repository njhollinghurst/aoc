#include <cstdio>
#include <cassert>
#include <vector>
using namespace std;

struct Number {
  long long val;  // changed to long long for part 2
  unsigned iprev;
  unsigned inext;
};

vector<Number> vn;
unsigned start = 0;

void rd()
{
  long long i;
  while (scanf("%lld", &i) > 0) {
    Number n{i,vn.size()-1,vn.size()+1};
    vn.push_back(n);
  }
  printf("%u numbers\n", vn.size());
  vn[0].iprev = vn.size()-1;
  vn[vn.size()-1].inext = 0;
  start = 0;
}

void print_in_order()
{
  unsigned pos = start;
  do {
    printf("%lld, ", vn[pos].val);
  } while ((pos = vn[pos].inext) != start);
  printf("\n");
}

void swap(unsigned i, unsigned j)
{
  // Hack: Only move the start marker from i to j! Not sure why this works...
  if (start == i)
    start = j;

  unsigned pi = vn[i].iprev;
  unsigned ni = vn[i].inext;
  unsigned pj = vn[j].iprev;
  unsigned nj = vn[j].inext;

  if (pi == j) { // j,i -> i,j
    assert(nj == i);
    vn[pj].inext = i;
    vn[i].iprev  = pj;
    vn[i].inext  = j;
    vn[j].iprev  = i;
    vn[j].inext  = ni;
    vn[ni].iprev = j;
  }
  else if (pj == i) { // i,j -> j,i
    assert(ni == j);
    vn[pi].inext = j;
    vn[j].iprev  = pi;
    vn[j].inext  = i;
    vn[i].iprev  = j;
    vn[i].inext  = nj;
    vn[nj].iprev = i;
  }
  else if (i != j) {
    assert(0); // Only need to swap neighbours
  }
}

void mix()
{
  for(unsigned i = 0; i < vn.size(); ++i) {
    long long v = vn[i].val;

    // Modulus trick, required for Part 2
    // Note that C++ % operator preserves sign
    v = v % (long long)(vn.size()-1);

    // Do the move as a sequence of swaps, which isn't optimal!
    if (v) {
      while (v > 0ll) {
        swap(i,vn[i].inext);
        v--;
      }
      while (v < 0ll) {
        swap(i, vn[i].iprev);
        v++;
      }
    }
  }
}

long long get_sum()
{
  unsigned n = 0;
  long long sum = 0;
  unsigned u;
  for(u = 0; vn[u].val != 0ll; u = vn[u].inext);
  while (n <= 3000) {
    if (n==1000 || n==2000 || n==3000) {
      printf("The value at position %u is %lld\n", n, vn[u].val);
      sum += vn[u].val;
    }
    u = vn[u].inext;
    n++;
  }
  return sum;
}

void reset_part2()
{
  static const long long K = 811589153;

  for(unsigned u = 0; u < vn.size(); ++u) {
    vn[u].val *= K;
    vn[u].iprev = u - 1;
    vn[u].inext = u + 1;
  }
  vn[0].iprev = vn.size() - 1;
  vn[vn.size() - 1].inext = 0;
  start = 0;
}

int main()
{
  rd();
  mix();
  printf("Part 1: %lld\n", get_sum());
  reset_part2();
  for(unsigned t = 0; t < 10; ++t) {
    mix();
  }
  printf("Part 2: %lld\n", get_sum());

  return 0;
}
