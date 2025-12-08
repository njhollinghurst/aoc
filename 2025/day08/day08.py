#!/bin/python3

import sys

boxes = []
connections = set()

for line in sys.stdin:
    xyz = line.strip().split(',')
    n = len(boxes)
    boxes.append([n, int(xyz[0]), int(xyz[1]), int(xyz[2])])

# part 1 -- very slow, dumb brute force approach!
# note we are connecting boxes that aren't already directly connected
# regardless of whether they are indirectly connected! But we also store
# which "group" a box is connected to...

n = 10 if len(boxes) == 20 else 1000
for k in range(n):
    best = (-1, -1, 999999999)
    for (i,b) in enumerate(boxes):
        for j in range(i+1, len(boxes)):
            if (i,j) not in connections:
                c = boxes[j]
                dist =  (b[1]-c[1])**2 + (b[2]-c[2])**2 + (b[3]-c[3])**2
                if dist < best[2]:
                    best = (i, j, dist)
    connections.add((best[0],best[1]))
    bg0 = boxes[best[0]][0]
    bg1 = boxes[best[1]][0]
    for i in range(len(boxes)):
        if boxes[i][0] == bg1:
            print(f"box {i} joins group {bg0}")
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


