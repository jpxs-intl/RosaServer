return function()
	local testString = ("Good morning Dr. Chandra. This is Hal. I am ready for my first lesson."):rep(8)

	local compressed = lz4.compress(testString)
	assert(#compressed < #testString)

	local uncompressed = lz4.uncompress(compressed, #testString)
	assert(uncompressed == testString)
end
