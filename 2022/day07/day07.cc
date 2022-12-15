#include <cstdint>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
using namespace std;

struct Node {
  Node * parent;
  string name;
  bool listed;
  unsigned long long file_size;
  unsigned long long total_size;
  vector<unique_ptr<Node> > sub;
  Node(Node *p, string s)
    : parent(p), name(s), listed(false), file_size(0), total_size(0), sub{}
  { }
};

void part1(Node *n, unsigned long long limit, unsigned long long &total)
{
  unsigned long long sz = n->file_size;
  for(auto &i : n->sub) {
    part1(&(*i), limit, total);
    sz += i->total_size;
  }
  n->total_size = sz;
  if (sz <= limit) {
    total += sz;
  }
}

void part2(Node *n, unsigned long long limit, unsigned long long &smallest)
{
  if (n->total_size >= limit && n->total_size < smallest)
    smallest = n->total_size;

  for(auto &i : n->sub) {
    part2(&(*i), limit, smallest);
  }
}

int main()
{
  Node root(0,"/");
  Node *cwd = &root;
  char str[1024];
  bool skipdir = true;

  // Parse input
  while (fgets(str, 1024, stdin) == str) {
    char * eptr = str + strlen(str);
    while (eptr != str && eptr[-1] <= ' ') --eptr;
    *eptr = '\0';

    if (str[0]=='$' && str[2]=='l') {
      skipdir = cwd->listed;
    }
    else if (str[0]=='$' && str[2]=='c') {
      if (str[5]=='/') {
	cwd = &root;
      }
      else if (str[5]=='.' && str[6]=='.') {
	cwd = cwd->parent;
      }
      else {
	for(auto &i : cwd->sub) {
	  if (i->name == str+5) {
	    cwd = &(*i);
	    break;
	  }
	}
      }
    }
    else if (str[0]=='d') {
      if (!skipdir)
	cwd->sub.push_back(unique_ptr<Node>(new Node(cwd,str+4)));
      cwd->listed = true;
    }
    else if (isdigit(str[0])) {
      if (!skipdir)
	cwd->file_size += atoll(str);
      cwd->listed = true;
    }
    else {
      printf("Unable to parse: %s", str);
    }
  }

  unsigned long long total_upto_100k = 0;
  part1(&root, 100000, total_upto_100k);
  printf("%llu\n", total_upto_100k);  

  unsigned long long smallest_to_delete = root.total_size;
  part2(&root, root.total_size - 40000000, smallest_to_delete);
  printf("%llu\n", smallest_to_delete);
  
  return 0;
}
