#!/bin/python3

import sys

sum = 0
seen = set()
part2 = True

ranges = sys.stdin.read().split(',')
for r in ranges:
    r = r.strip()
    ab = r.split('-')
    lo = int(ab[0])
    hi = int(ab[1])
    for digs in range(len(str(lo)), len(str(hi)) + 1):
        lo1 = max(lo, 10**(digs-1))
        hi1 = min(hi+1, 10**digs)
        for parts in range(2, digs if part2 else 3):
            if digs % parts:
                continue
            base = 0
            for j in range(0, parts):
                base += 10**((digs*j)//parts)
            for i in range(lo1,hi1):
                if i % base == 0 and i not in seen:
                    seen.add(i)
                    sum += i

print(sum)
