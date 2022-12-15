#include <string>
#include <vector>
#include <iostream>
using namespace std;

vector<string> grid;
vector<string> viz;

int main()
{
  string str;
  while (getline(cin, str)) {
    grid.push_back(str);
    viz.push_back(str);
  }

  // part1
  unsigned nv = 0;
  for(unsigned i = 0; i < grid.size(); ++i) { // rows
    char h = ' ';
    for(unsigned j = 0; j < grid[i].size(); ++j) { // from left
      if (grid[i][j] > h) {
	if (viz[i][j]!='V') ++nv;
	viz[i][j]='V';
	h = grid[i][j];
      }
    }
    h = ' ';
    for(unsigned j = grid[i].size(); j-- > 0;) { // from right
      if (grid[i][j] > h) {
	if (viz[i][j]!='V') ++nv;
	viz[i][j]='V';
	h = grid[i][j];
      }
    }
  }
  for(unsigned j = 0; j < grid[0].size(); ++j) { // cols
    char h = ' ';
    for(unsigned i = 0; i < grid.size(); ++i) { // from top
      if (grid[i][j] > h) {
	if (viz[i][j]!='V') ++nv;
	viz[i][j]='V';
	h = grid[i][j];
      }
    }
    h = ' ';
    for(unsigned i = grid.size(); i-- > 0;) { // from bottom
      if (grid[i][j] > h) {
	if (viz[i][j]!='V') ++nv;
	viz[i][j]='V';
	h = grid[i][j];
      }
    }
  }
  cout << "Number visible: " << nv << endl;

  // part2
  unsigned most_scenic = 0;
   for(unsigned i = 0; i < grid.size(); ++i) {
     for(unsigned j = 0; j < grid[i].size(); ++j) {
       unsigned s;
       unsigned n = 0;
       for(unsigned k = 1; k <= j; ++k) {
	 n++;
	 if (grid[i][j-k] >= grid[i][j]) break;
       }
       s = n;
       n = 0;
       for(unsigned k = 1; j+k < grid[i].size(); ++k) {
	 n++;
	 if (grid[i][j+k] >= grid[i][j]) break;
       }
       s *= n;
       n = 0;
       for(unsigned k = 1; k <= i; ++k) {
	 n++;
	 if (grid[i-k][j] >= grid[i][j]) break;
       }
       s *= n;
       n = 0;
       for(unsigned k = 1; i+k < grid.size(); ++k) {
	 ++n;
	 if (grid[i+k][j] >= grid[i][j]) break;
       }
       s *= n;
       if (s > most_scenic) {
	 printf("Winner at %d %d %d\n", i, j, s);
	 most_scenic = s;
       }
     }
   }
  cout << "Most scenic: " << most_scenic << endl;
  
  return 0;
}
