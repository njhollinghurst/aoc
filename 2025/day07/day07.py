#!/bin/python3

import sys

beams = []
splits = 0

# Using a single copy of "beams" works only if there are never two splitters adjacent

for line in sys.stdin:
    line = line.strip()
    while len(line) > len(beams):
        beams.append(0)
    for (i,c) in enumerate(line):
        b = beams[i]
        if c == 'S':
            beams[i] = 1
        elif c =='^' and b > 0:
            splits += 1
            beams[i] = 0
            #if i > 0:
            #    beams[i-1] += 0.5 * b  # incorrectly anticipating part 2 during part 1
            #if i + 1 < len(beams):
            #    beams[i+1] += 0.5 * b
            if i > 0:
                beams[i-1] += b  # in fact for part 2 we want the number of "timelines"
            if i + 1 < len(beams):
                beams[i+1] += b

print(splits)
print(sum(beams))
