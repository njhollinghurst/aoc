#include <stdio.h>
#include <stdlib.h>

long total = 0;

void cycle(int t, int x)
{
  if (t%40 == 20)
    total += t * x;

  // added for part 2
  int col = (t-1)%40;
  putchar((col >= x-1 && col <= x+1) ? '#' : '.');
  if (col==39) putchar('\n');
}

int main()
{
  int t = 0, x = 1;
  
  char str[64];
  while (fgets(str,64,stdin)==str) {
    if (str[0]=='a') {
      int n = atoi(str+5);
      cycle(++t, x);
      cycle(++t, x);
      x += n;
    }
    else if (str[0]=='n') {
      cycle(++t, x);
    }
    else {
      puts(str);
    }    
  }

  printf("%ld\n", total);

  return 0;  
}
