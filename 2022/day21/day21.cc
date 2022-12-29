#include <cstdio>
#include <cstdint>
#include <cassert>
#include <map>
#include <vector>
using namespace std;

static inline uint32_t MY4CC(char const *p) {
  return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3];
}

struct Monkey {
  char op;
  uint32_t ref[2];
  long long value;
};

vector<Monkey> monkeys;
unsigned root = 0, humn = 0;

void rd()
{
  map<uint32_t, uint32_t> names;
  char str[256];

  while (fgets(str, 256, stdin) == str) {
    Monkey m{0,{0,0},0};
    uint32_t name = MY4CC(str);
    if (name == MY4CC("root"))
      root = monkeys.size();
    if (name == MY4CC("humn"))
      humn = monkeys.size();
    if (sscanf(str+6, "%lld", &m.value) < 1) {
      m.ref[0] = MY4CC(str + 6);
      m.ref[1] = MY4CC(str + 13);
      m.op = str[11];
      assert(m.op == '+' || m.op == '-' || m.op == '*' || m.op == '/');
    }
    names[name] = monkeys.size();
    monkeys.push_back(m);
  }

  for (auto &m : monkeys) {
    if (m.op) {
      assert(names.count(m.ref[0]) && names.count(m.ref[1]));
      m.ref[0] = names[m.ref[0]];
      m.ref[1] = names[m.ref[1]];
    }
  }
}

long long resolve(uint32_t ix)
{
  if (!monkeys[ix].op)
    return monkeys[ix].value;

  long long lhs, rhs;
  lhs = resolve(monkeys[ix].ref[0]);
  rhs = resolve(monkeys[ix].ref[1]);
  switch (monkeys[ix].op) {
  case '+': return lhs + rhs;
  case '-': return lhs - rhs;
  case '*': return lhs * rhs;
  }
  assert(monkeys[ix].op =='/' && rhs != 0ll);
  return lhs / rhs;
}

bool uses_humn(uint32_t ix)
{
  if (ix == humn)
    return true;
  if (!monkeys[ix].op)
    return false;
  return uses_humn(monkeys[ix].ref[0]) || uses_humn(monkeys[ix].ref[1]);
}

long long do_part2(uint32_t ix, long long target)
{
  if (ix == humn)
    return target;

  Monkey &m = monkeys[ix];

  bool h0 = uses_humn(m.ref[0]);
  bool h1 = uses_humn(m.ref[1]);
  assert(h0 != h1); // XXX we can only cope with one human at once!
  unsigned nonhuman = h0 ? 1 : 0;

  long long v = resolve(m.ref[nonhuman]);
  if (ix != root) {
    switch (m.op) {
    case '+':
      v = target - v;
      break;
    case '-':
      v = (h0) ? target + v : v - target;
      break;
    case '*':
      v = target/v;
      break;
    case '/':
      v = (h0) ? target * v : v / target;
      break;
    default: assert(0);
    }
  }

  return do_part2(m.ref[1-nonhuman], v);
}

int main()
{
  rd();
  printf("Part 1: %lld\n", resolve(root));
  printf("Part 2: %lld\n", do_part2(root, 0));

  return 0;
}
