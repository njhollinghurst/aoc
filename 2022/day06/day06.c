#include <stdio.h>

int main()
{
  char cc[4];
  char kk[16] = { 0 }, dist[16] = { 0 };
  int i, c;
  int pos = 0;
  int seen_first_sop = 0;

  while ((c = getchar()) > 0) {

    // inserted for part 2
    kk[pos&15] = c;
    for(i = 1; i < 14; ++i) {
      if (kk[(pos-i)&15] == c)
	break;
    }
    dist[pos&15] = i;

    // original part 1 solution
    cc[pos&3] = c;
    if (++pos < 4)
      continue;
    if (cc[0]==cc[1] || cc[0]==cc[2] || cc[0]==cc[3])
      continue;
    if (cc[1]==cc[2] || cc[1]==cc[3] || cc[2]==cc[3])
      continue;
    if (!seen_first_sop) {
      printf("SOP after character %d\n", pos);
      seen_first_sop = 1;
    }

    // part 2
    if (i >= 14 && pos >= 14) {
      for(i = 2; i < 14; ++i) {
	if (dist[(pos-i)&15] < 15-i)
	  break;
      }
      if (i >= 14)
        printf("SOM after character %d\n", pos);
    }     
  }
  
  return 0;  
}
