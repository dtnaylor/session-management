#!/usr/bin/python
import datetime
import sys
import numpy
from collections import defaultdict

unencrypted_size = 71
encrypted_size = 80

def getData(data):
    lens = [int(l.split('length')[1]) for l in data]
    total_bytes = sum(lens)


    start = datetime.datetime.strptime(data[0].split(' ')[0].split('.')[0], "%H:%M:%S")
    start = start.replace(microsecond = int(data[0].split(' ')[0].split('.')[1]))
    end = datetime.datetime.strptime(data[-1].split(' ')[0].split('.')[0], "%H:%M:%S")
    end = end.replace(microsecond = int(data[-1].split(' ')[0].split('.')[1]))

    duration = (end-start).seconds+((end-start).microseconds*10**(-6))
    print total_bytes/encrypted_size / duration, len(data), total_bytes, total_bytes/unencrypted_size, duration
    return total_bytes/encrypted_size / duration

relevant = [l for l in open(sys.argv[1]).read().split('\n') if 'localhost.ircd >' in l]
relevant = [l for l in relevant if int(l.split('length')[1]) > 50]
clients = []
for l in relevant:
    clients.append(l.split('>')[1].split(':')[0].split('.')[1])
clients = list(set(clients))
d = defaultdict(int)
for l in relevant:
    for c in clients:
        if c in l:
            d[c] += 1
client = max(d, key = d.get)
#print sorted(d.values())
out = []
for c in [client]:
    out.append(getData([l for l in relevant if c in l]))
print numpy.mean(out)
