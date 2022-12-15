#include <vector>
#include <cstdio>
#include <cctype>
#include <memory>
#include <algorithm>
using namespace std;

struct Node;
typedef unique_ptr<Node> NodePtr;

struct Node {
  bool isList;
  int n;
  vector<NodePtr> v;

  Node(int i) : isList(false), n(i), v{} { }
  Node() : isList(true), n(0), v{} { }
  void append(Node *a) { v.push_back(NodePtr(a)); }

  int cmp(Node const &rhs) const {
    if (!isList && !rhs.isList) {
      return (n < rhs.n) ? -1 : (n > rhs.n);
    }
    else if (isList && rhs.isList) {
      unsigned u;
      for(u = 0; u < v.size() && u < rhs.v.size(); ++u) {
	int c = v[u]->cmp(*rhs.v[u]);
	if (c)
	  return c;
      }
      return (u < rhs.v.size()) ? -1 : (u < v.size()) ? 1 : 0;
    }
    else if (isList) {
      Node tmp;
      tmp.append(new Node(rhs.n));
      int c = cmp(tmp);
      return c;
    }
    else {
      Node tmp;
      tmp.append(new Node(n));
      int c = tmp.cmp(rhs);
      return c;
    }
  }

  void print(bool eol = false)
  {
    if (isList) {
      putchar('[');
      for(unsigned u = 0; u < v.size(); ++u) {
	if (u) putchar(',');
	v[u]->print(false);
      }
      putchar(']');
    }
    else {
      printf("%d", n);
    }
    if (eol) {
      putchar('\n');
    }
  }
};

Node * rd()
{
  int c;
  do c = fgetc(stdin); while (isspace(c));
  if (c=='[') {
    Node * ret = new Node;
    for(;;) {
      c = getchar();
      if (c==']') break;
      if (c==',') continue;
      ungetc(c,stdin);
      ret->append(rd());
    }
    return ret;
  }
  else if (isdigit(c)) {
    int i = 0;
    ungetc(c,stdin);
    scanf("%d", &i);
    return new Node(i);
  }
  return 0;
}

bool cmp_by_ptr(NodePtr const &a, NodePtr const &b)
{
  return a->cmp(*b) < 0;
}

int main()
{
  unsigned ix = 0, right_sum = 0;
  vector<NodePtr> allmsgs; // added for Part 2
  
  for(;;) {
    ix++;
    Node *l = rd();
    if (!l) break;
    Node *r = rd();
    if (!r) break;
    if (l->cmp(*r) <= 0) {
      right_sum += ix;
    }
    //delete r;
    //delete l;
    allmsgs.push_back(NodePtr(l));
    allmsgs.push_back(NodePtr(r));
  }
  printf("Part 1: %u\n", right_sum);

  // Part 2 -- thank goodness I'm not trying to do this in Rust
  {
    Node *a(new Node);
    Node *b(new Node);
    Node *sub(new Node);
    sub->append(new Node(2));
    a->append(sub);
    sub = new Node;
    sub->append(new Node(6));
    b->append(sub);
    allmsgs.push_back(NodePtr(a));
    allmsgs.push_back(NodePtr(b));
  }
  sort(allmsgs.begin(), allmsgs.end(), cmp_by_ptr);
  right_sum = 1;
  for(ix = 0; ix < allmsgs.size(); ix++) {
    //allmsgs[ix]->print(true);
    if (allmsgs[ix]->v.size() == 1 &&
	allmsgs[ix]->v[0]->isList && allmsgs[ix]->v[0]->v.size() == 1 &&
	(allmsgs[ix]->v[0]->v[0]->n & ~4) == 2)
      right_sum *= ix + 1;
  }
  printf("Part 2: %u\n", right_sum);
  return 0;
}
