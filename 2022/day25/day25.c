#include <stdio.h>
#include <stdint.h>

void print_num(int64_t a)
{
  uint64_t b = (a+((a>0)?2:-2)) / 5;
  if (b) {
    print_num(b);
    a -= 5*b;
  }
  putchar("=-012"[a+2]);
}

int main()
{
  int64_t a = 0, n = 0;
  int c;

  while ((c = getchar()) > 0) {
    if (c <= ' ') {
      a += n;
      n = 0;
    }
    else {
      n *= 5;
      n += (c=='=') ? -2 : (c=='-') ? -1 : c-'0';
    }
  }

  print_num(a);
  putchar('\n');

  return 0;
}
