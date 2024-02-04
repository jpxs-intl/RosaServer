local function log(...)
	local prefix = "\27[34;1m[Test]\27[0m "
	print(prefix .. string.format(...))
end

local function requireTest(test)
	log("Starting test: " .. test)
	require(test)()
	log("Ended test: " .. test)
end

local function runTests()
	requireTest("tests.accounts")
	requireTest("tests.bonds")
	requireTest("tests.bullets")
	requireTest("tests.chat")
	requireTest("tests.crypto")
	requireTest("tests.events")
	requireTest("tests.fileWatcher")
	requireTest("tests.http")
	requireTest("tests.humans")
	requireTest("tests.image")
	requireTest("tests.items")
	requireTest("tests.itemTypes")
	requireTest("tests.memory")
	requireTest("tests.os")
	requireTest("tests.physics")
	requireTest("tests.players")
	requireTest("tests.rigidBodies")
	requireTest("tests.rotMatrix")
	requireTest("tests.server")
	requireTest("tests.sqlite")
	requireTest("tests.streets")
	requireTest("tests.vector")
	requireTest("tests.vehicles")
	requireTest("tests.worker")
	requireTest("tests.zlib")
end

local function testsPassed()
	log("\27[32;1m✔\27[0m All tests passed")
	os.exit(0)
end

local function protectedFailCall(func, ...)
	local success, result = pcall(func, ...)
	if not success then
		log("\27[31;1m✘\27[0m Test failed at %s", tostring(result))
		os.exit(1)
	end
end

local handlers = {}

local tick = 0

hook.enable("Logic")
function hook.run(event, ...)
	if event == "Logic" then
		tick = tick + 1

		log("Tick %i...", tick)

		if tick == 1 then
			protectedFailCall(runTests)
		else
			for i = #handlers, 1, -1 do
				local handler = handlers[i]
				handler.ticksToWait = handler.ticksToWait - 1
				if handler.ticksToWait <= 0 then
					table.remove(handlers, i)
					protectedFailCall(handler.func)
				end
			end

			if #handlers == 0 then
				testsPassed()
			end
		end
	end

	return false
end

function nextTick(func, ticksToWait)
	ticksToWait = ticksToWait or 1
	table.insert(handlers, {
		func = func,
		ticksToWait = ticksToWait,
	})
end

function assertAddsEvent(func, message)
	local numEvents = #events
	func()
	if #events ~= numEvents + 1 then
		error(message or "event assertion failed!", 2)
	end
end
