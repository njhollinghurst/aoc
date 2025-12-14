#!/bin/python3

import sys
import numpy as np

################################################################
# Try to attack the problem with linear algebra!
#
# For convenience, everything is transposed wrt Ax = b
# instead we have: [answer] * [[matrix]] = [joltages].
#
# We might need a pseudo-inverse "A+", because there could
# be either "too many" switches (multiple solutions), or
# "too many" (not independent) joltages, or both. Note that
# the least-squares solution might not be in non-negative
# integers, and isn't necessarily the cheapest.
#
# Where there are multiple solutions, the matrix I - (A+ A)
# is nonzero. We need to work out which switches can vary.
#
# Five possible outcomes:
#
# (a) There is no solution. Fail.
#
# (b) The unique or least-squares solution gives fractional
#     or negative values, for switches that can't vary. Fail.
#
# (c) The least-squares solution gives fractional or negative
#     values for switches that can vary, and non-negative
#     integers elsewhere. An acceptable solution might exist.
#     Return a partial solution with a bitmap of switch
#     indices to search over.
#
# (d) The least-squares solution gives non-negative integers
#     everywhere but has degrees of freedom in switch values.
#     This is a solution but we don't know if it's optimal.
#     Return an upper bound on cost, and a partial solution
#     with a bitmap of switch indices to search over.
#
# (e) There is a unique solution, either because the matrix
#     was square and invertible, or the pseudo-inverse
#     solution had no degrees of freedom in switch values;
#     and all switch values are non-negative integers.
#     This is a confirmed success. Return the cost.
################################################################

def foo(matrix, joltages):
    n = len(matrix)
    unique = len(matrix[0]) == n and np.linalg.det(matrix) >= 0.5
    if unique:
        Ainv_T = np.linalg.inv(matrix)
    else:
        Ainv_T = np.linalg.pinv(matrix)
    v = np.linalg.matmul(joltages, Ainv_T)
    q = np.linalg.matmul(v, matrix)
    for j in range(len(joltages)):
        if abs(q[j] - joltages[j]) >= 0.03125:
            return (999999,[],0) # no valid solution found
    bitmap = 0
    if not unique:
        ImAinvA_T = np.eye(n) - np.linalg.matmul(matrix, Ainv_T)
        for i in range(n):
            if np.max(np.fabs(ImAinvA_T[i])) >= 1.0e-9:
                bitmap |= (1 << i)
    frac = False
    for i in range(len(v)):
        if v[i] < -0.001 or abs(v[i]-round(v[i])) > 0.001:
            if (bitmap & (1<<i)) == 0:
                return (999999,[],0) # only fractional solutions
            frac = True
        v[i] = round(v[i])
    v = v.astype('int')
    n = 999999 if frac else sum(v)
    return (n,v,bitmap)

def bar(matrix, joltages):
    if len(joltages) == 0 or max(joltages) == 0:
        return 0
    if len(matrix) == 0:
        return 999999

    # Do the maths
    (c,v,bitmap) = foo(matrix, joltages)
    if bitmap == 0:
        return c

    extracost = 0
    joltages1 = joltages.copy()
    if bitmap + 1 < (1 << len(matrix)):
        # Some switches now have known values. Prune them out.
        matrix0 = []
        for i in range(len(matrix)):
            if bitmap & (1 << i):
                matrix0.append(matrix[i].copy())
            else:
                extracost += v[i]
                for j in range(len(joltages1)):
                    joltages1[j] -= v[i] * matrix[i][j]
                    if joltages1[j] < 0:
                        return 999999
        # Now remove any joltages that are already satisfied,
        # along with switches that would try to change them.
        if min(joltages1) == 0:
            j = 0
            while j < len(joltages1):
                if joltages1[j] == 0:
                    s = 0
                    while s < len(matrix0):
                        if matrix0[s][j] != 0:
                            matrix0.pop(s)
                        else:
                            matrix0[s].pop(j)
                            s += 1
                    joltages1.pop(j)
                else:
                    j += 1
        if len(joltages1) == 0 or max(joltages1) == 0:
            return 0
        if len(matrix0) == 0:
            return 999999
    else:
        matrix0 = matrix

    # Find a switch that affects the smallest joltage
    swx = 0
    limit = -1
    for s in range(len(matrix0)):
        s_limit = -1
        for j in range(len(joltages1)):
            if matrix0[s][j] != 0:
                if s_limit < 0 or joltages1[j] < s_limit:
                    s_limit = joltages1[j]
        if limit < 0 or s_limit <= limit:
            swx = s
            limit = s_limit

    # Recursively search over all permissible values
    matrix1 = matrix0[:swx] + matrix0[swx+1:]
    for i in range(0, limit+1):
        c1 = i + bar(matrix1, joltages1)
        if c1 < c:
            c = c1
        for j in range(len(joltages1)):
            joltages1[j] -= matrix0[swx][j]

    return c + extracost


## MAIN CODE FOLLOWS

total = 0
total2 = 0
count = 0

for line in sys.stdin:
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
    matrix=[]
    for s in switches:
        sv = []
        z = 0
        for i in range(npattern):
            sv.append(1 if (s & (1 << i)) else 0)
        matrix.append(sv)
    joltages = []
    for (i,j) in enumerate(line[-1].strip('{}').split(',')):
        joltages.append(int(j))

    print(f'{len(matrix)}/{len(joltages)}:{max(joltages)}')
    c = bar(matrix, joltages)
    assert(c < 999999)
    print("Part 2:", c)
    total2 += c

print("Total (part 1)", total)
print("Total (part 2)", total2)
