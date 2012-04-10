#!/usr/bin/python

def fib(n):
    """Print a Fibonacci series up to n."""
    a, b = 0, 1
    while a < n:
        print a,
        a, b = b, a+b

fib(2000)

# default args
print '\nappending:'
def f(a, L=[]):
    L.append(a)
    return L

print f(1)
print f(2)
print f(3)

