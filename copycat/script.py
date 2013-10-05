#!/usr/bin/python

import subprocess
import sys
import time as t


bufferList = [2**x for x in range(1,19)]
timeDiff = []
simRes = open('calc/simRes_20M_3.csv','w')

for bufferSize in bufferList:
    print("BufferSize: " + str(bufferSize))
    t1 = t.time()
    subprocess.call(["./copycat", "-o", "largeOutput.txt","-b", str(bufferSize), "20MFile"])
    elapsedTime = t.time()-t1
    timeDiff.append(elapsedTime)
    print("---- " + str(elapsedTime))

throughPut = zip(bufferList,timeDiff)
simRes.write('\n'.join('%s, %s' % x for x in throughPut))
