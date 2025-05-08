while true do
	if receiveMessage() == "hi" then
		sendMessage("hello")
	end
	sleep(1)
end
