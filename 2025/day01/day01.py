#!/bin/python3

import sys

pos=50
count=0
part2=0

for line in sys.stdin:
    line = line.strip()
    if line[0]=='R':
        pos += int(line[1:])
        while pos >= 100:
            part2 += 1
            pos -= 100
    elif line[0]=='L':
        if pos == 0:
            pos = 100
        pos -= int(line[1:])
        while pos <= 0:
            part2 += 1
            pos += 100
    else:
        print("Error\n")
    pos %= 100
    #print(f"The dial is rotated by {line} to point at {pos}; {part2}")
    if pos == 0:
        count+=1
        
print(count, part2)
