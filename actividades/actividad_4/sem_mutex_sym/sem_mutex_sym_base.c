main:
	shared count := 0
	shared mutex := semaphore(1)
	create_thread(thread_a)
	create_thread(thread_b)

thread_a:
	wait(mutex)
	count := count + 1
	signal(mutex)

thread_b:
	wait(mutex)
	count := count + 1
	signal(mutex)
