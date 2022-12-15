#include <stdio.h>

static int maxe[3] = { 0, 0, 0 };

void insert_elf(int e)
{
  int tmp;
  if (e > maxe[2]) {
    maxe[2] = e;
    if (maxe[2] > maxe[1]) {
      tmp = maxe[2];
      maxe[2] = maxe[1];
      maxe[1] = tmp;
    }
    if (maxe[1] > maxe[0]) {
      tmp = maxe[0];
      maxe[0] = maxe[1];
      maxe[1] = tmp;
    }
  }
}

int main()
{
  char str[256];
  int e = 0;
  
  while (fgets(str, 256, stdin)==str) {
    int c;
    if (sscanf(str, "%d", &c) >= 1) {
      e += c;
    }
    else {
      insert_elf(e);
      e = 0;
    }
  }
  insert_elf(e);
  printf("Part1: %d  Part2: %d\n", maxe[0], maxe[0]+maxe[1]+maxe[2]);
  return 0;
}
