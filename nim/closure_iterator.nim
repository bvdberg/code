import math

proc powers(m: int): auto =
  #return iterator: int {.closure.} = # Make a closure explicitly
  return iterator: int = # Compiler makes this a closure for us
    for n in 0..int.high:
      yield n^m

var
  squares = powers(2)
  cubes = powers(3)

for i in 1..4:
  echo "Square: ", squares() # 0, 1, 4, 9
for i in 1..4:
  echo "Cube: ", cubes()     # 0, 1, 8, 27
echo "Square: ", squares()   # 16
echo "Cube: ", cubes()       # 64

for x in squares(): # Go through all the remaining squares
  echo "Square: ", x         # 25, 36, 49, 64, ...
