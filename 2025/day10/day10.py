#!/bin/python3

import sys
import numpy
import copy

total = 0
total2 = 0
count = 0

################################################################
# Try to attack the problem with linear algebra!
#
# Four possible outcomes (excluding zero cases):
#
# (a) The matrix is square, invertible and has a positive
#     integer solution. This is a confirmed success.
#
# (b) The matrix is square, invertible and has a non-integer
#     or negative solution. This is a confirmed failure.
#
# (c) The pseudo-inverse finds a positive integer solution.
#     This minimizes the Euclidean metric, not necessarily
#     the Manhattan metric. This is a provisional success
#     (except in a few cases where it must be optimal).
#
# (c) The pseudo-inverse finds a non-integer or negative
#     solution. An integer solution might still exist.
#     This is a provisional failure.
#
# (d) The pseudo-inverse does not find a valid solution.
#     This is a confirmed failure.
################################################################

def foo(matrix, joltages):
    if len(joltages) == 0 or max(joltages) == 0:
        return (True,0)
    if len(matrix) == 0 or len(matrix[0]) == 0:
        return (True,999999)
    unique = False
    try:
        assert(numpy.linalg.det(matrix) >= 0.5)
        M = numpy.linalg.inv(matrix)
        unique = True
    except:
        M = numpy.linalg.pinv(matrix)
    v = numpy.linalg.matmul(joltages, M)
    q = numpy.linalg.matmul(v, matrix)
    for j in range(len(joltages)):
        if abs(q[j] - joltages[j]) >= 0.0625:
            return (True,999999)
    for i in range(len(v)):
        if v[i] < -0.001 or abs(v[i]-round(v[i])) > 0.001:
            return (unique,999999)
        v[i] = round(v[i])
    n = round(sum(v))
    if n < 3 or max(v) == n:
       unique = True
    return (unique,n)

ticks = 10
memo = dict()  # not sure if this is helping tbh

def bar(matrix, joltages):
    global memo, ticks
    if len(joltages) == 0 or max(joltages) == 0:
        return 0
    keystr = None
    if len(matrix) < 8:
        keystr = str(matrix) + str(joltages)
        if keystr in memo:
            return memo[keystr]
    if len(matrix) >= ticks:
        print('.', end='', flush=True)
    if min(joltages) == 0:
        # Joltages contains zeroes. This triggers a pruning step
        # to remove any zero elements from joltages, remove any
        # switches that would try to set it, and remove the zero
        # element from all remaining switches!
        matrix = copy.deepcopy(matrix)
        joltages = joltages.copy()
        i = 0
        while i < len(joltages):
            if joltages[i] < 0.01:
                j = 0
                while j < len(matrix):
                    if matrix[j][i] != 0:
                        matrix.pop(j)
                        continue
                    matrix[j].pop(i)
                    j += 1
                joltages.pop(i)
                continue
            i += 1
    if len(matrix) == 0:
        return 999999 if sum(joltages) > 0.01 else 0
    (unique,c) = foo(matrix, joltages)
    if unique:
        if keystr is not None:
            memo[keystr] = c
        return c
    matrix1 = matrix[1:]
    joltages1 = joltages.copy()
    limit = -1
    for j in range(len(joltages)):
        if matrix[0][j] != 0 and (limit == -1 or limit > joltages[j]):
            limit = joltages[j]
    seen_any_good = 0
    seen_num_bad = 0
    for i in range(limit+1):
        for j in range(len(joltages)):
            joltages1[j] = joltages[j] - i * matrix[0][j]
        c1 = i + bar(matrix1, joltages1)
        if c1 < 999999:
            seen_any_good += 1
            seen_num_bad = 0
        else:
            seen_num_bad += 1
            if seen_num_bad >= 32 and seen_any_good * seen_num_bad >= 64:  # XXX risky/hacky heuristic for speed!
                break
        if c1 < c:
            c = c1
    if keystr is not None:
            memo[keystr] = c
    return c

for line in sys.stdin:
    # parse input
    count += 1
    print(f"=== Line {count} ===")
    line = line.strip().split()
    assert(line[0][0]=='[')
    assert(line[-1][-1]=='}')
    pattern = 0
    npattern = len(line[0][1:-1])
    for (i,c) in enumerate(line[0][1:-1]):
        pattern += (1<<i) if c=='#' else 0
    switches = []
    for s in line[1:-1]:
        n = 0
        for v in s.strip('()').split(','):
            n += 1 << int(v)
        switches.append(n)

    # part 1
    nswitches = len(switches)
    bestcost = nswitches
    best = (1 << nswitches) - 1
    for i in range(1,best):
        c = i.bit_count()
        if c >= bestcost:
            continue
        z = 0
        for (j,s) in enumerate(switches):
            if i & (1<<j):
                z = z ^ s
        if z == pattern and c < bestcost:
            bestcost = c
            best = i
    print("Part 1:", bestcost)
    total += bestcost

    # part 2
    memo.clear()
    matrix=[]
    floorcost = 0
    for s in sorted(switches, key = lambda x : x.bit_count()): # not sure why but this sort order seems faster
        sv = []
        z = 0
        for i in range(npattern):
            sv.append(1 if (s & (1 << i)) else 0)
        matrix.append(sv)

    target = 0
    joltages = []
    for (i,j) in enumerate(line[-1].strip('{}').split(',')):
        joltages.append(int(j))

    i = 0
    while i < len(matrix):
        unique = matrix[i].copy()
        for j in range(len(matrix)):
            if j != i:
                for k in range(len(joltages)):
                    unique[k] *= 1 - matrix[j][k]
        if max(unique) > 0:
            ix = numpy.argmax(unique)
            num = joltages[ix]
            for j in range(len(joltages)):
                joltages[j] -= num * matrix[i][j]
                assert(joltages[j] >= 0)
            floorcost += num
            print(f"Fix switch {matrix[i]} which solely covers dial {ix} with {num}")
            matrix.pop(i)
            continue
        i += 1

    print(f'{len(matrix)}/{len(joltages)}:{max(joltages)}', end='')
    ticks = max(5, len(matrix) - 1)
    c = floorcost + bar(matrix, joltages)
    assert(c < 999999)
    print()
    print("Part 2:", c)
    total2 += c

print("Total (part 1)", total)
print("Total (part 2)", total2)
