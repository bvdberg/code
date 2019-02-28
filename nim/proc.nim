proc min[T](x, y: T): T =
  if x < y: x else: y

echo min(123, 456) # more explicitly: min[int](2, 3)
echo min("foo", "bar") # min[string]("foo", "bar")
