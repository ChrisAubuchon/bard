require "bt1test"
require "dataio"
require "roster"

print(roster:nameExists("CHRIS"))
print(roster:nameExists("FOO"))

local function f(inFunc)
	print(inFunc)
	inFunc("CHRIS")
end

f(roster.printMember)
