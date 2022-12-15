#include <stdio.h>

int main()
{
  int s = 0;
  int s2 = 0;
  char str[8];
  while (fgets(str,8,stdin)==str) {
    s += 3*((str[2]-str[0]+2)%3) + str[2]-'W';
    s2 += 3*(str[2]-'X') + 1 + (str[0]+str[2]+2)%3;
  }
  printf("%d %d\n", s, s2);
  return 0;
}
