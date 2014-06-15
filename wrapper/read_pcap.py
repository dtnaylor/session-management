#!/usr/bin/python
import datetime

relevant = [l for l in open('pcap').read().split('\n') if 'localhost.ircd >' in l]
relevant = [l for l in relevant if int(l.split('length')[1]) > 50]
relevant = [l for l in relevant if int(l.split('length')[1]) < 1000]

start = datetime.datetime.strptime(relevant[0].split(' ')[0].split('.')[0], "%H:%M:%S")
start = start.replace(microsecond = int(relevant[0].split(' ')[0].split('.')[1]))
end = datetime.datetime.strptime(relevant[-1].split(' ')[0].split('.')[0], "%H:%M:%S")
end = end.replace(microsecond = int(relevant[-1].split(' ')[0].split('.')[1]))

duration = (end-start).seconds+((end-start).microseconds*10**(-6))
print len(relevant) / duration
print len(relevant), duration
