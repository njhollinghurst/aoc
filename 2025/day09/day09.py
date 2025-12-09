#!/bin/python3

import sys
import bisect

tiles = []

for line in sys.stdin:
    xy = line.strip().split(',')
    tiles.append((int(xy[1]), int(xy[0])))

# part 1

maxa = 0
for (i,t) in enumerate(tiles):
    for j in range(i+1, len(tiles)):
        u = tiles[j]
        a = (1 + abs(u[0]-t[0])) * (1 + abs(u[1]-t[1]))
        if a > maxa:
            maxa = a
print(maxa)


# part 2

hset = set() # horizontal boundary segments (y,x0,x1) inclusive of corner tiles
vset = set() # vertical boundary segments (x,y0,y1) inclusive of corner tiles
for (i,t) in enumerate(tiles):
    j = (i + 1) % len(tiles)
    u = tiles[j]
    if u[0]==t[0]:
        hset.add((u[0],min(u[1],t[1]), max(u[1],t[1])))
    elif u[1]==t[1]:
        vset.add((u[1],min(u[0],t[0]), max(u[0],t[0])))
    else:
        print("Eh?")

hedges = []
vedges = []
for h in hset:
    hedges.append(h)
for v in vset:
    vedges.append(v)
hedges.sort() # horizontal edges sorted by Y position
vedges.sort() # vertical edges sorted by X position

def overlap_inclusive(a,b,p,q):
    return min(b,q) >= max(a,p)

rects = []
for (i,t) in enumerate(tiles):
    for j in range(i+1, len(tiles)):
        u = tiles[j]
        a = (1 + abs(u[0]-t[0])) * (1 + abs(u[1]-t[1]))
        rects.append((a,min(u[0],t[0]),max(u[0],t[0]),min(u[1],t[1]),max(u[1],t[1])))
rects.sort(reverse=True)

for r in rects:
    #print(r)
    # Check that no edges intersect the interior of the rectangle (ignoring its own edges and corners)
    # XXX is that the correct criterion? It seems to give the right answer anyway...
    bad = False
    ix = bisect.bisect_left(hedges, (r[1]+1,0,0))
    while ix < len(hedges):
        e = hedges[ix]
        if e[0] >= r[2]:
            break
        if overlap_inclusive(r[3]+1,r[4]-1,e[1],e[2]):
            bad = True
        ix += 1
    if bad:
        continue
    ix = bisect.bisect_left(vedges, (r[3]+1,0,0))
    while ix < len(vedges):
        e = vedges[ix]
        if e[0] >= r[4]:
            break
        if overlap_inclusive(r[1]+1,r[2]-1,e[1],e[2]):
            bad = True
        ix += 1
    if not bad:
        print(r[0])
        break
