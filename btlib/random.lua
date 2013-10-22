random = {
	mt = mt_random.new()
}

----------------------------------------
-- random:xdy()
--
-- x <= r <= (x*y)
----------------------------------------
function random:xdy(x, y)
	local value = 0

	for i = 1,x do
		value = value + ((self.mt() % y) + 1)
	end

	return value
end


--------------------------------
-- random:betweenInclusive()
--
-- x <= r <= y
--------------------------------
function random:betweenInclusive(x, y)
	if (x == y) then
		return x
	end

	-- Make sure x is the lower number
	if (x > y) then
		local t = x
		x = y
		y = t
	end

	return self.mt:value(x, y)
end

--------------------------------
-- random:between()
--
-- x < r < y
--------------------------------
function random:between(x, y)
	return random:betweenInclusive(x+1, y-1)
end

--------------------------------
-- random:xdy_z()
--
-- This function generates the
-- random number XdY minus a
-- number of the smallest rolls
--------------------------------
function random:xdy_z(x, y, z)
	local rolls = {}
	local value = 0
	local i, j
	local minimum

	if (z >= x) then
		return 0
	end

	for i = 1,x do
		rolls[i] = self.mt(y)
	end

	table.sort(rolls)

	for i = z+1,x do
		value = value + rolls[i]
	end

	return value
end

----------------------------------------
-- random:band()
--
-- r & x
----------------------------------------
function random:band(x)
	return bit32.band(self.mt(), x)
end

----------------------------------------
-- random:random:rnd()
--
-- Return a random number
----------------------------------------
function random:rnd()
	return self.mt()
end

----------------------------------------
-- random:hash()
--
-- Return a random hexadecimal string
-- for use as a hash key
----------------------------------------
function random:hash()
	return string.format("0x%08X", self.mt())
end
