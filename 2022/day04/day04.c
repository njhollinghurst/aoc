#include <stdio.h>

int main()
{
  int n_fully=0, n_overlap=0;
  unsigned a,b,c,d;
  while(scanf("%u-%u,%u-%u",&a,&b,&c,&d)==4) {
    if ((c>=a && d<=b) || (a>=c && b<=d))
      n_fully++;
    if ((c>=a && c<=b) || (d>=a && d<=b) || (c<=a && d >=b))
      n_overlap++;
  }
  printf("%d %d\n", n_fully, n_overlap);
  return 0;
}
