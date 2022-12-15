#include <cstdio>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <cassert>
#include <cstdint> // for part2, now using uint64_t for worries
using namespace std;

//#define PART1

enum Op {
	 OP_ADD,
	 OP_MULT,
	 OP_SQUARE
};

struct Monkey {
  vector<uint64_t> items;
  Op op;
  uint32_t param;
  uint32_t test;
  unsigned true_monkey;
  unsigned false_monkey;
  uint32_t count;
};

static vector<Monkey> mm;
uint64_t global_modulus = 1; // added for part2

static void round()
{
  for(auto &m : mm) {
    for(uint64_t w : m.items) {
      switch (m.op) {
      case OP_ADD:
	w += m.param;
	break;
      case OP_MULT:
	w *= m.param;
	break;
      default:
	w *= w;
      }
#ifdef PART1
      w /= 3;
#else
      w %= global_modulus; // admittedly this trick wouldn't work with very many large primes
#endif
      // Assume m never throws to itself, or w loop would be invalidated!
      mm[(w % m.test == 0) ? m.true_monkey : m.false_monkey].items.push_back(w);
    }
    m.count += m.items.size();
    m.items.clear();
  }
}

int main()
{
  char str[256];
  Monkey *m = 0;
  
  while(fgets(str, 256, stdin)==str) {
    // All lines of interest contain a colon
    char const *ptr = strchr(str, ':');
    if (!ptr)
      continue;
    ++ptr;
    while (*ptr && isspace(*ptr)) ++ptr;

    if (strstr(str, "Monkey")) {
      unsigned u;
      sscanf(str, "Monkey %u", &u);
      assert(u == mm.size()); // check that monkeys are in order
      mm.emplace_back(Monkey{});
      m = &mm.back();
      m->count = 0;
      m->true_monkey = 0;
      m->false_monkey = 0;
    }
    else if (strstr(str, "Starting items")) {
      while (*ptr) {
	while (*ptr && !isdigit(*ptr)) ++ptr;
	if (!*ptr) break;
	m->items.push_back(strtoull(ptr, 0, 0));
	while (*ptr && isdigit(*ptr)) ++ptr;
      }  
    }
    else if (strstr(str, "Operation")) {
      ptr = strchr(ptr, '=');
      assert(ptr);
      ptr++;
      while (*ptr && *ptr != '+' && *ptr != '*') ++ptr;
      m->op = (*ptr=='+') ? OP_ADD : OP_MULT;
      ptr++;
      while (*ptr && isspace(*ptr)) ++ptr;
      m->param = strtoul(ptr, 0, 0);
      if (!isdigit(*ptr))
	m->op = OP_SQUARE;
    }
    else if (strstr(str, "Test")) {
      sscanf(ptr, "divisible by %llu", &m->test);
      if (global_modulus % m->test) // added for part2
	global_modulus *= m->test;
    }
    else if (strstr(str, "If true")) {
      sscanf(ptr, "throw to monkey %u", &m->true_monkey);
      assert(m->true_monkey != mm.size()-1);
    }
    else if (strstr(str, "If false")) {
      sscanf(ptr, "throw to monkey %u", &m->false_monkey);
      assert(m->false_monkey != mm.size()-1);
    }
    else {
      puts(str);
    }    
  }

#ifdef PART1
  for(int i = 0; i < 20; ++i)
    round();
#else
  for(int i = 0; i < 10000; ++i)
    round();
#endif
  
  uint32_t max1 = 0, max2 = 0;
  for(unsigned u = 0; u < mm.size(); ++u) {
    printf("Monkey %u inspected %u items\n", u, mm[u].count);
    if (mm[u].count > max2) {
      max2 = mm[u].count;
      if (max2 > max1)
	max2 ^= max1 ^= max2 ^= max1;
    }
  }
  printf("%llu\n", (uint64_t)max1 * (uint64_t)max2);
  
  return 0;
}
