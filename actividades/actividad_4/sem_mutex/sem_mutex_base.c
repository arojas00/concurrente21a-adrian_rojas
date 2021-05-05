shared count := 0
shared mutex := ?

main:
	// Read integer from standard input
	thread_count := read_integer()
	create_thread(thread_count, secondary)

secondary:
	// Critical section
	count := count + 1