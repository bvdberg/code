iterator reverseItems(x: string): char =
  for i in countdown(x.high, x.low):
    yield x[i]

for c in "foo".reverseItems:
  echo c
