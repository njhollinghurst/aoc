#!/bin/python3

import sys

boxes = []
for line in sys.stdin:
    xyz = line.strip().split(',')
    n = len(boxes)
    boxes.append([n, int(xyz[0]), int(xyz[1]), int(xyz[2])])

# part 1 -- somewhat optimized version (compared to brute force)
# find the N shortest distances between any pairs
n = 10 if len(boxes) == 20 else 1000
bestn = [(99999999, -1, -1)] * n
for (i,b) in enumerate(boxes):
    for j in range(i+1,len(boxes)):
        c = boxes[j]
        dist =  (b[1]-c[1])**2 + (b[2]-c[2])**2 + (b[3]-c[3])**2
        if dist < bestn[n-1][0] and (i,j):
            bestn[n-1] = (dist, i, j)
            bestn.sort()
for i in range(n):
    bg0 = boxes[bestn[i][1]][0]
    bg1 = boxes[bestn[i][2]][0]
    for i in range(len(boxes)):
        if boxes[i][0] == bg1:
            boxes[i][0] = bg0;
product = 1
for k in range(3):
    biggest = 0
    biggest_group = -1
    for i in range(len(boxes)):
        count = 0
        group = boxes[i][0]
        if group < 0:
            continue
        for b in boxes:
            if b[0] == group:
                count += 1
        if count > biggest:
            biggest_group = group
            biggest = count
    print(f"group {biggest_group} has size {biggest}")
    product *= biggest
    for i in range(len(boxes)):
        if boxes[i][0] == biggest_group:
            boxes[i][0] = -1

print(product)

# part 2 -- new faster approach: Enumerate *all* the possible pairs and sort them by distance!

allpairs = []
for i in range(len(boxes)):
    boxes[i][0] = i # reset group labels to individual boxes again
    b = boxes[i]
    for j in range(i+1, len(boxes)):
        c = boxes[j]
        dist = (b[1]-c[1])**2 + (b[2]-c[2])**2 + (b[3]-c[3])**2
        allpairs.append((dist, i, j))
allpairs.sort()
for p in allpairs:
        bg0 = boxes[p[1]][0]
        bg1 = boxes[p[2]][0]
        groupsize = 0
        for i in range(len(boxes)):
            if boxes[i][0] == bg1:
                boxes[i][0] = bg0;
            if boxes[i][0] == bg0:
                groupsize += 1
        if groupsize == len(boxes):
            print(f"{boxes[p[1]][1] * boxes[p[2]][1]}")
            break

