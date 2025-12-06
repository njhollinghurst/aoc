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
    for i in range(len(ll)):
        if ll[i][0] >= '0' and ll[i][0] <= '9':
            n = int(ll[i])
            if i >= len(sums):
                sums.append(0)
                products.append(1)
            sums[i] += n
            products[i] *= n
        else:
            total += sums[i] if ll[i][0] == '+' else products[i]

    # part 2 -- keep a running decimal value for each column of characters (watch out for 0)
    for i in range(len(line)):
        while i >= len(colnum):
            colnum.append(-1)
        if line[i] >= '0' and line[i] <= '9':
            colnum[i] = (10 * colnum[i] if colnum[i] > 0 else 0) + int(line[i])
        elif line[i] == '+':
            sum = 0
            while i < len(colnum) and colnum[i] >= 0:
                sum += colnum[i]
                i += 1
            total2 += sum
        elif line[i] == '*':
            prod = 1
            while i < len(colnum) and colnum[i] >= 0:
                prod *= colnum[i]
                i += 1
            total2 += prod

print(total, total2)
