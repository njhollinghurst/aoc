#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <vector>
#include <set>
#include <list>
#include <utility>
using namespace std;

// PART 1
struct XYZD {
  uint16_t xyz[3];
  uint16_t d;
  bool operator<(XYZD const &r) const {
    return memcmp(this, &r, sizeof(*this)) < 0;
  };
};
set<XYZD> facets;

void part1_cube(uint16_t x, uint16_t y, uint16_t z)
{
  XYZD f{{x,y,z},0};
  for(unsigned d = 0; d < 3; ++d) {
    f.d = d;
    if (facets.count(f))
      facets.erase(f);
    else
      facets.insert(f);
    f.xyz[d]++;
    if (facets.count(f))
      facets.erase(f);
    else
      facets.insert(f);
    f.xyz[d]--;
  }
}

// PART 2
struct XYZ {
  uint16_t xyz[3];
  bool operator<(XYZ const &r) const {
    return memcmp(this, &r, sizeof(*this)) < 0;
  };
};
set<XYZ> cubes;
uint16_t mincube[3] = { 0xFFFF, 0xFFFF, 0xFFFF };
uint16_t maxcube[3] = { 0, 0, 0 };
list<XYZ> boundary_cubes;
set<XYZ> water_cubes;

unsigned part2()
{
  for(unsigned d = 0; d < 3; ++d) {
    for(unsigned i = mincube[(d+1)%3]; i <= maxcube[(d+1)%3]; ++i) {
      for(unsigned j = mincube[(d+2)%3]; j < maxcube[(d+2)%3]; ++j) {
        XYZ c;
        c.xyz[d] = mincube[d];
        c.xyz[(d+1)%3] = i;
        c.xyz[(d+2)%3] = j;
        if (!cubes.count(c))
          boundary_cubes.push_back(c);
        c.xyz[d] = maxcube[d];
        if (!cubes.count(c))
          boundary_cubes.push_back(c);
      }
    }
  }
  while (boundary_cubes.size()) {
    XYZ c = boundary_cubes.front();
    boundary_cubes.pop_front();
    if (water_cubes.count(c))
      continue;
    water_cubes.insert(c);
    for(unsigned d = 0; d < 3; ++d) {
      if (c.xyz[d] < maxcube[d]) {
        c.xyz[d]++;
        if (!cubes.count(c) && !water_cubes.count(c)) {
          boundary_cubes.push_back(c);
        }
        c.xyz[d]--;
      }
      if (c.xyz[d] > mincube[d]) {
        c.xyz[d]--;
        if (!cubes.count(c) && !water_cubes.count(c)) {
          boundary_cubes.push_back(c);
        }
        c.xyz[d]++;
      }
    }
  }
  printf("Water volume %u, infer there are %u cubes\n", water_cubes.size(),
         (maxcube[0]+1-mincube[0])*(maxcube[1]+1-mincube[1])*(maxcube[2]+1-mincube[2])-water_cubes.size());

  unsigned a = 0;
  for(auto const &f : facets) {
    XYZ c_hi{f.xyz[0],f.xyz[1],f.xyz[2]};
    XYZ c_lo = c_hi;
    c_lo.xyz[f.d]--;
    if (c_hi.xyz[f.d]<=mincube[f.d] || c_lo.xyz[f.d]>=maxcube[f.d] ||
        water_cubes.count(c_lo) || water_cubes.count(c_hi)) {
      a++;
    }
  }
  return a;
}

int main()
{
  int x, y, z;
  unsigned nc = 0;
  while (scanf("%d,%d,%d\n", &x, &y, &z) >= 3) {
    part1_cube(x,y,z);
    cubes.insert(XYZ{(uint16_t)x, (uint16_t)y, (uint16_t)z});
    if (x < mincube[0]) mincube[0] = x;
    if (y < mincube[1]) mincube[1] = y;
    if (z < mincube[2]) mincube[2] = z;
    if (x > maxcube[0]) maxcube[0] = x;
    if (y > maxcube[1]) maxcube[1] = y;
    if (z > maxcube[2]) maxcube[2] = z;
    nc++;
  }
  printf("There were %u cubes\n", nc);
  printf("Surface area: %u\n", facets.size());
  printf("External area: %u\n", part2());
  return 0;
}
