import strutils, times, os

type Level* {.pure.} = enum
  debug, info, warn, error, fatal

var logLevel* = Level.debug

proc debug*(args: varargs[string, `$`]) =
  if logLevel <= Level.debug:
    echo "[$# $#]: $#" % [getDateStr(), getClockStr(),
      join args]

proc expensiveDebuggingInfo*: string =
  sleep(milsecs = 1000)
  result = "Everything looking good!"

debug expensiveDebuggingInfo()
