#include <cstdio>
#include <cctype>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
using namespace std;

string jetpattern;
vector<uint8_t> tower;
uint64_t base_trim = 0;
unsigned jet_index = 0;
unsigned shape_index = 0;
unsigned faller_height = 0;
uint8_t faller[4];

struct Signature { // added for Part 2
  uint8_t a[32];
  bool operator<(const Signature &rhs) const {
    return memcmp(a, rhs.a, 32) < 0;
  }
};
map<Signature, pair<uint64_t,uint64_t>  > sigs;

void new_shape()
{
  faller_height = tower.size() + 3;
  switch (shape_index) {
  case 0:
    faller[3] = 0;
    faller[2] = 0;
    faller[1] = 0;
    faller[0] = 0x3C;
    shape_index = 1;
    break;
  case 1:
    faller[3] = 0;
    faller[2] = 0x08;
    faller[1] = 0x1C;
    faller[0] = 0x08;
    shape_index = 2;
    break;
  case 2:
    faller[3] = 0;
    faller[2] = 0x10;
    faller[1] = 0x10;
    faller[0] = 0x1C;
    shape_index = 3;
    break;
  case 3:
    faller[3] = 0x04;
    faller[2] = 0x04;
    faller[1] = 0x04;
    faller[0] = 0x04;
    shape_index = 4;
    break;
  default:
    faller[3] = 0;
    faller[2] = 0;
    faller[1] = 0x0C;
    faller[0] = 0x0C;
    shape_index = 0;
  }
}

void jet()
{
  unsigned th = tower.size();
  if (jetpattern[jet_index]=='>') {
    if (!((faller[3] | faller[2] | faller[1] | faller[0]) & 0x40) &&
        (th <= faller_height+3 || !(tower[faller_height+3] & (faller[3] << 1))) &&
        (th <= faller_height+2 || !(tower[faller_height+2] & (faller[2] << 1))) &&
        (th <= faller_height+1 || !(tower[faller_height+1] & (faller[1] << 1))) &&
        (th <= faller_height+0 || !(tower[faller_height+0] & (faller[0] << 1)))) {
      faller[3] <<= 1; // I'm using the confusing endian, left is right...
      faller[2] <<= 1;
      faller[1] <<= 1;
      faller[0] <<= 1;
      //puts("Jet pushes rock right");
    }
    //else puts("Jet blocked");
  }
  else {
    assert(jetpattern[jet_index]=='<');
    if (!((faller[3] | faller[2] | faller[1] | faller[0]) & 0x01) &&
        (th <= faller_height+3 || !(tower[faller_height+3] & (faller[3] >> 1))) &&
        (th <= faller_height+2 || !(tower[faller_height+2] & (faller[2] >> 1))) &&
        (th <= faller_height+1 || !(tower[faller_height+1] & (faller[1] >> 1))) &&
        (th <= faller_height+0 || !(tower[faller_height+0] & (faller[0] >> 1)))) {
      faller[3] >>= 1;
      faller[2] >>= 1;
      faller[1] >>= 1;
      faller[0] >>= 1;
      //puts("Jet pushes rock left");
    }
    //else puts("Jet blocked");
  }
  if (++jet_index >= jetpattern.size())
    jet_index = 0;
}

bool fall()
{
  unsigned th = tower.size();
  if (faller_height > 0 &&
      (th <= faller_height-1 || !(tower[faller_height-1] & faller[0])) &&
      (th <= faller_height   || !(tower[faller_height  ] & faller[1])) &&
      (th <= faller_height+1 || !(tower[faller_height+1] & faller[2])) &&
      (th <= faller_height+2 || !(tower[faller_height+2] & faller[3]))) {
    faller_height--;
    //puts("Rock falls 1 unit");
    return true;
  }
  else {
    unsigned h = faller_height;
    for(unsigned u = 0; u < 4; ++u, ++h) {
      while (tower.size() <= h) tower.push_back(0);
      tower[h] |= faller[u];
      if (h > 0 && tower[h] == 0x7F) {
        tower.erase(tower.begin(), tower.begin()+h);
        base_trim += h;
        h = 0;
      }
    }
    //puts("Rock comes to rest");
    while (*tower.rbegin() == 0)
      tower.pop_back();
    return false;
  }
}
  
void viz()
{
  for(auto rit = tower.crbegin(); rit != tower.crend(); rit++) {
    cout << '|'
         << ((*rit&0x01)?'#':'.')
         << ((*rit&0x02)?'#':'.')
         << ((*rit&0x04)?'#':'.')
         << ((*rit&0x08)?'#':'.')
         << ((*rit&0x10)?'#':'.')
         << ((*rit&0x20)?'#':'.')
         << ((*rit&0x40)?'#':'.') << '|' << endl;
  }
  cout << "+ " << base_trim << " rows below" << endl << endl;
}

int main()
{
  cin >> jetpattern;
  while (jetpattern.size() > 0 && jetpattern[jetpattern.size()-1 != '<'] && jetpattern[jetpattern.size()-1] != '>')
    jetpattern.pop_back();

  // Part 1
  for(unsigned u = 0; u < 2022; ++u) {
    new_shape();
    do jet(); while (fall());
    //viz();
  }
  cout << "Part 1: " << base_trim + tower.size() << endl;

  // Part 2
  static const uint64_t N = 1000000000000ull;
  for(uint64_t u = 2022; u < N; ++u) {
    unsigned th = tower.size();
    Signature s;
    s.a[0] = shape_index;
    s.a[1] = jet_index;
    for(unsigned q = 0; q < 30; ++q) s.a[2+q] = (th+q<30) ? 0x7F : tower[th+q-30];
    if (sigs.count(s)) {
      uint64_t iter_step = u - sigs[s].first;
      uint64_t depth_step = base_trim + tower.size() - sigs[s].second;
      uint64_t leaps = (N - u) / iter_step;
      sigs[s] = pair<uint64_t,uint64_t>(u, base_trim + tower.size());
      u += leaps * iter_step;
      base_trim += leaps * depth_step;
      assert(u <= N);
      if (u >= N)
        break;
    }
    else {
      sigs[s] = pair<uint64_t,uint64_t>(u, base_trim + tower.size());
    }
    new_shape();
    do jet(); while (fall());
  }
  cout << "Part 2: " << base_trim + tower.size() << endl;

  return 0;
}
