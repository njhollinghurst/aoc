#!/bin/python3

import sys

sum = 0
sum2 = 0

for line in sys.stdin:
    line=line.strip()

    #part 1
    pos1 = max(range(len(line)-1), key=lambda x:line[x])
    pos2 = max(range(pos1+1,len(line)), key=lambda x:line[x])
    sum += 10 * int(line[pos1]) + int(line[pos2])

    #part 2
    posx = 0
    for j in range(0,12):
        posx = max(range(posx, len(line)+j-11), key=lambda x:line[x])
        sum2 += 10**(11-j) * int(line[posx])
        posx += 1
    
print(sum, sum2)
