#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <climits>
#include <cstdint>
#include <utility>
#include <algorithm>
using namespace std;

vector<string> grid;
vector<vector<unsigned> > dist;
vector<vector<bool> > visited;
vector<pair<uint16_t,uint16_t> > candidates;

unsigned sy, sx, dy, dx;

static inline bool reachable(unsigned y0, unsigned x0, unsigned y1, unsigned x1)
{
  return grid[y1][x1] <= grid[y0][x0]+1;
}

static inline void update(unsigned y, unsigned x, unsigned d)
{
  if (dist[y][x] > d) {
    dist[y][x] = d;
    pair<uint16_t,uint16_t> p{y,x};
    if (find(candidates.begin(), candidates.end(), p) == candidates.end()) {
      candidates.push_back(p);
    }
  }
}

void visit(unsigned y,unsigned x)
{
  visited[y][x] = true;
  unsigned d = dist[y][x] + 1;
  
  if (y > 0 && !visited[y-1][x] && reachable(y,x,y-1,x)) {
    update(y-1,x,d);
  }
  if (y+1 < grid.size() && !visited[y+1][x] && reachable(y,x,y+1,x)) {
    update(y+1,x,d);
  }
  if (x > 0 && !visited[y][x-1] && reachable(y,x,y,x-1)) {
    update(y,x-1,d);
  }
  if (x+1 < grid[0].size() && !visited[y][x+1] && reachable(y,x,y,x+1)) {
    update(y,x+1,d);
  }  
}

int main()
{
  string str;
  while (cin >> str) {
    auto pos = str.find('S');
    if (pos != str.npos) {
      sy = grid.size();
      sx = pos;
      str[pos] = 'a';
    }
    pos = str.find('E');
    if (pos != str.npos) {
      dy = grid.size();
      dx = pos;
      str[pos] = 'z';
    }
    grid.push_back(str);
    dist.push_back(vector<unsigned>(str.size(), UINT_MAX));
    visited.push_back(vector<bool>(str.size(), false));
  }

#define PART2
#ifdef PART2
  for(unsigned i = 0; i < grid.size(); ++i) {
    for(unsigned j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j]=='a'  && (i!=sy || j!=sx)) {
	dist[i][j] = 0;
	visit(i,j);
      }
    }
  }
#endif

  dist[sy][sx] = 0;
  unsigned y = sy, x = sx;
  while (y != dy ||  x != dx) {
    visit(y,x);

    if (candidates.empty())
      break;
    unsigned d = UINT_MAX;
    auto best = candidates.begin();
    for(auto it = candidates.begin(); it != candidates.end(); ++it) {
      pair<uint16_t,uint16_t> p = *it;
      if (d == UINT_MAX || dist[p.first][p.second] < d) {
	d = dist[p.first][p.second];
	y = p.first;
	x = p.second;
	best = it;
      }
    }
    candidates.erase(best);
  }
  
  cout << dist[dy][dx] << endl;

  return 0;
}
