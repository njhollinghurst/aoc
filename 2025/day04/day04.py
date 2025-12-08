#!/bin/python3

import sys

grid = []
sum = 0

for line in sys.stdin:
    line=line.strip()
    grid.append(list(line))

#part 1
for i in range(len(grid)):
    for j in range(len(grid[i])):
        if grid[i][j]=='@':
            n = 0
            for ii in range(max(0,i-1),min(len(grid),i+2)):
                for jj in range(max(0,j-1),min(len(grid[i]),j+2)):
                    if grid[ii][jj]=='@':
                        n += 1
            if n < 5:
                sum += 1

print(sum)

#part 2
sum = 0
ats = set()
for i in range(len(grid)):
    for j in range(len(grid[i])):
        if grid[i][j]=='@':
            ats.add((i,j))
while True:
    removed = 0
    grid1 = grid.copy()
    ats1 = set()
    for (i,j) in ats:
        if grid[i][j]=='@':
            n = 0
            for ii in range(max(0,i-1),min(len(grid),i+2)):
                for jj in range(max(0,j-1),min(len(grid[ii]),j+2)):
                    if grid[ii][jj]=='@':
                        n += 1
            if n < 5:
                removed += 1
                grid1[i][j]='x'
                for ii in range(max(0,i-1),min(len(grid),i+2)):
                    for jj in range(max(0,j-1),min(len(grid[ii]),j+2)):
                        if grid[ii][jj]=='@' and (ii != i or jj != j):
                            ats1.add((ii,jj))
    sum += removed
    grid = grid1
    ats = ats1
    if removed == 0:
        break

print(sum)
