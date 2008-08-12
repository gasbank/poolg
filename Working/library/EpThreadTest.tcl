package require Thread

namespace eval EpThreadTest {
	thread::create {
		puts "xxx"
		variable cx 0
		proc passTheTime { } {
			variable cx
			puts "cx: $cx"
			incr cx
			after 1000 passTheTime
		}
		passTheTime
		vwait forever
	}
}
