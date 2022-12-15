#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

vector<string> stax;

static constexpr bool PART_TWO = true;

int main()
{
  string str;
  while (getline(cin, str)) {
    unsigned col = 0;
    bool any = false;
    while ((col = str.find('[', col)) != string::npos) {
      while ((col>>2) >= stax.size())
	stax.push_back(string());
      stax[col>>2].insert(0,1,str[col+1]);
      any = true;
      col += 4;
    }
    if (!any)
      break;
  }

  while (getline(cin, str)) {
    unsigned n, src, dst;
    if(sscanf(str.c_str(), "move %u from %u to %u", &n, &src, &dst) >= 3) {
      if (n > 0 && src != dst && n <= stax[src-1].size()) {
        while (dst-1 >= stax.size())
          stax.push_back(string());
	if (PART_TWO) {
	  stax[dst-1] += stax[src-1].substr(stax[src-1].size()-n, n);
	  stax[src-1].resize(stax[src-1].size()-n);
	}
	else {
	  for(; n > 0; n--) {
	    stax[dst-1] += stax[src-1].back();
	    stax[src-1].pop_back();
	  }
	}
      }
    }
  }

  for(unsigned j = 0; j < stax.size(); ++j)
    cout << stax[j].back();
  cout << endl;

  return 0;
}
