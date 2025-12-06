#!/bin/python3

import sys

sums = []
products = []
total = 0

colnum = []  # added for part 2
total2 = 0

for line in sys.stdin:
    # part 1 -- keep a running sum and product of each column of numbers; choose at the end
    ll=line.strip().split()
    while len(ll) > len(sums):
        sums.append(0)
        products.append(1)
    for (i,v) in enumerate(ll):
        if v.isdigit():
            n = int(v)
            sums[i] += n
            products[i] *= n
        else:
            total += sums[i] if v[0] == '+' else products[i]

    # part 2 -- keep a running decimal value for each column of characters (watch out for 0)
    while len(line) > len(colnum):
        colnum.append(-1)
    for (i,c) in enumerate(line):
        if c >= '0' and c <= '9':
            colnum[i] = 10 * max(0, colnum[i]) + int(c)
        elif c == '+':
            while i < len(colnum) and colnum[i] >= 0:
                total2 += colnum[i]
                i += 1 # (this does not affect the for loop)
        elif c == '*':
            prod = 1
            while i < len(colnum) and colnum[i] >= 0:
                prod *= colnum[i]
                i += 1
            total2 += prod

print(total, total2)
