#!/bin/python3

import sys

fresh = []
avail = []
num = 0

doFresh=True
for line in sys.stdin:
    line=line.strip()
    if doFresh:
        if len(line)==0:
            doFresh = False
            continue
        ll=line.split('-')
        fresh.append((int(ll[0]), int(ll[1])))
    else:
        avail.append(int(line))

fresh.sort()
avail.sort()

#part 1
pos = 0
for a in avail:
    while pos < len(fresh) and fresh[pos][1] < a:
        pos += 1
    if pos >= len(fresh):
        break
    if fresh[pos][0] <= a and a <= fresh[pos][1]:
        num += 1
        
print(num)

#part 2
pos = 0
while pos + 1 < len(fresh):
    if fresh[pos+1][0] <= fresh[pos][1]:
        fresh[pos] = (fresh[pos][0], max(fresh[pos+1][1], fresh[pos][1]))
        fresh.pop(pos+1)
    else:
        pos += 1

num = 0
for f in fresh:
    num += f[1] + 1 - f[0]

print(num)
