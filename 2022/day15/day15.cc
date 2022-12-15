#include <vector>
#include <set>
#include <cmath>
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;

struct Diamond {
  int sx; // centre x
  int sy; // centre y
  int r;  // L1 radius, inclusive
  bool intersects_row(int y) const {
    return y >= sy-r && y <= sy+r;
  }
  int lbound(int y) const {
    return sx - r + abs(y - sy);
  }
  int rbound(int y) const {
    return sx + r - abs(y - sy);
  }
};

// for Part 1
vector<pair<int,int> > spans_on_row;
set<int> beacons_on_row;

// for Part 2
vector<vector<pair<int,int> > > all_spans;

int main()
{
  int sx=0, sy=0, bx=0, by=0;
  while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n",
               &sx, &sy, &bx, &by) >= 4) {
    printf("%d %d %d %d\n", sx, sy, bx, by);
    int l1 = abs(bx-sx) + abs(by-sy);
    Diamond d{sx, sy, l1};
  
    // Part 1
    static const int Y_SCAN = 2000000;
    if (d.intersects_row(Y_SCAN)) {
      int lo = d.lbound(Y_SCAN);
      int hi = d.rbound(Y_SCAN) + 1;
     for(unsigned u = 0; u < spans_on_row.size();) {
        if (spans_on_row[u].second > lo && spans_on_row[u].first < hi) {
          lo = min(lo, spans_on_row[u].first);
          hi = max(hi, spans_on_row[u].second);
          spans_on_row.erase(spans_on_row.begin() + u);
          continue;
        }
        u++;
      }
      spans_on_row.emplace_back(pair<int,int>(lo,hi));
    }
    if (by == Y_SCAN)
      beacons_on_row.insert(bx);
    
    // Part 2, the brute force way
    if (min(4000000, sy + l1) >= (int)(all_spans.size())) {
      all_spans.resize(min(4000000, sy + l1));
    }
    for(int y = max(0, sy - l1); y <= sy + l1 && y <= 4000000; ++y) {      
      int lo = d.lbound(y);
      int hi = d.rbound(y) + 1;
      for(unsigned u = 0; u < all_spans[y].size();) {
        if (all_spans[y][u].second >= lo && all_spans[y][u].first <= hi) {
          if (all_spans[y][u].first <= lo && all_spans[y][u].second >= hi)
            goto absorbed;
          lo = min(lo, all_spans[y][u].first);
          hi = max(hi, all_spans[y][u].second);
          all_spans[y][u] = *all_spans[y].rbegin();
          all_spans[y].pop_back();
          continue;
        }
        u++;
      }
      all_spans[y].emplace_back(pair<int,int>(lo,hi));
    absorbed:;
    }
  }

  // Part 1
  unsigned total = 0;
  for(unsigned u = 0; u < spans_on_row.size(); ++u) {
    total += spans_on_row[u].second - spans_on_row[u].first;
  }
  total -= beacons_on_row.size();
  printf("%u\n", total);

  // Part 2
  for(unsigned y = 0; y < all_spans.size(); ++y) {
    if (all_spans[y].size() > 1) {
      sort(all_spans[y].begin(), all_spans[y].end());
      for(unsigned u = 0; u+1 < all_spans[y].size(); ++u) {
        if (all_spans[y][u+1].first == all_spans[y][u].second + 1) {
          printf("Gap at %d %d   %llu\n", all_spans[y][u].second, y,  4000000*(unsigned long long)(all_spans[y][u].second) + y);
        }
      }
    }
  }
  
  return 0;
}
