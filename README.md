# GCC's Semaphore Implementation

As far as I can tell, this sample should wake all threads, according to the
semantics of semaphores. However, with my local copy of gcc (11.0.1 commit
a18ebd6c439), it instead only wakes a single thread, despite the repeated
semaphore releases.

Obviously, this is a contrived minimal example, for which I could run the
`wakeSemaphore.release(threadCount)` line instead of waking the semaphore
multiple times. For another project however, this was not feasible, since I
needed to wake threads individually as needed.

Replacing the `std::counting_semaphore` implementation with the equivalent POSIX
semaphore implementation instead also resolves this issue. Given that the
standard states that [threads may block waiting until counter >= 1 (section
33.7.2)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0514r4.pdf),
and I have observed that sleeping threads observe a `counter` value >= 1 yet
still remain sleeping, I think this implementation is non-standards-conforming.

I believe the issue is that all wake events coalesce into one, before any
threads are awoken, thus only one thread observes the wake event. Evidence to
support this is that initiating a wait between each semaphore release permits
all threads to wake correctly. This could be a function of scheduling behaviour
allowing other threads to operate whilst the main thread is de-scheduled,
however, rather than the actual delay itself.
