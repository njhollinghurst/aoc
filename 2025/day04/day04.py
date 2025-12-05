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
while True:
    removed = 0
    grid1 = grid
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j]=='@':
                n = 0
                for ii in range(max(0,i-1),min(len(grid),i+2)):
                    for jj in range(max(0,j-1),min(len(grid[i]),j+2)):
                        if grid[ii][jj]=='@':
                            n += 1
                if n < 5:
                    removed += 1
                    grid1[i][j]='x'
    sum += removed
    grid = grid1
    print("R", removed, grid1)
    if removed == 0:
        break

print(sum)
