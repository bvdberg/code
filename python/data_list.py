#!/usr/bin/python

a = [66.25, 333, 333, 1, 1234.5]
print 'count 333', a.count(333)
print 'count 66.25', a.count(66.25)
print 'count x', a.count('x')

a.insert(2, -1)
a.append(333)
print a
a.index(333)
a.remove(333)
print a
a.reverse()
print a
a.sort()
print a

