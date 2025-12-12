#!/bin/python3

import sys

outputs = dict()

memo=dict()
def nroutes(a,b):
    s = a + b
    if s in memo:
        return memo[s]
    if a not in outputs:
        return 0
    if b in outputs[a]:
        return 1
    n = 0
    for o in outputs[a]:
        n += nroutes(o,b)
    memo[s] = n
    return n

for line in sys.stdin:
    # parse input
    line = line.strip().split()
    assert(line[0][-1]==':')
    n0 = line[0][0:-1]
    outputs[n0] = line[1:]

# part 1
print(nroutes("you","out"))

# part 2
a = nroutes("svr", "fft")
b = nroutes("fft", "dac")
c = nroutes("dac", "out")
x = nroutes("svr", "dac")
y = nroutes("dac", "fft")
z = nroutes("fft", "out")
print(a*b*c + x*y*z)
