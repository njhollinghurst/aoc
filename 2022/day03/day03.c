#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

uint64_t get_item_type_map(char const *ptr, int len)
{
  uint64_t ret = 0;
  while (len > 0) {
    if (islower(*ptr))
      ret |= 1ull << (*ptr - 'a');
    else if (isupper(*ptr))
      ret |= 1ull << (26 + *ptr - 'A'));
    ptr++;
    len--;
  }
  return ret;
}

int main()
{
  char str[1024];
  uint64_t s = 0, s_badges = 0, all_carry = UINT64_MAX;
  int phase = 0;
  
  while (fgets(str, 1024, stdin)==str) {
    int sz = strlen(str);
    while (sz > 0 && (str[sz-1])<=' ') --sz;
    uint64_t items0 = get_item_type_map(str, sz/2);
    uint64_t items1 = get_item_type_map(str+sz/2, sz/2);
    uint64_t overlap = items0 & items1;
    for(int prio = 1; overlap; prio++,overlap>>=1)
      s += prio*(overlap & 1);
    all_carry &= items0 | items1;
    if (++phase == 3) {
      for(int prio = 1; all_carry; prio++,all_carry>>=1)
	s_badges += prio*(all_carry & 1);
      phase = 0;
      all_carry = UINT64_MAX;
    }   
  }
  
  printf("%llu %llu\n", s, s_badges);
  return 0;
}
