Makefile
======================================
- Added the -std=c++17 flag to enable better constexpr support.
- Removed optimizations as the system would crash with them.
  The BlueScreen would show an empty callstack on crashes with -O2 so I guess the problem is the missing ebp or something like that.
- Added qemu-stdio target for easy access to the serial output
  
Changes
======================================
- Removed the Input.h/Input.cc files
- Removed the Queue.h/Queue.cc/Chain.h files
- Many changes to the scheduler (strategy is still round robin):
  - Merged dispatcher into the scheduler (removed Dispatch.h/Dispatch.cc)
  - Scheduler no longer uses queue but vector
  - Scheduler owns threads (no outside references)
  - Threads are addressed by tid from the outside
  - Retrieve threads when killing them
  - Threads are deallocated when killed and not retrieved
  - Scheduler manages blocked threads
- Simplified thread switching by storing registers on the stack (removed ThreadState.h, Thread.inc)

Additions
======================================
- Some stream manipulators for CGA_Stream (color) and OutStream (fixed width)
- Logging with serial port output
- Main menu to start demos
- A very basic system to broadcast keyboard events to threads
- Basic vector and iterator
- Basic unique_ptr
- Basic fixed size array (just a wrapper for C-style arrays)

Bugs
======================================
- Can't exit the PCSPKdemo (bluescreen)
- After exiting the VBEdemo the paging isn't reactivated (so the pagefault bluescreen no longer works)
- The PreemptiveThreadDemo LoopThreads can't be exited, the demo can't be launched multiple times
- Tree Allocator (uses a red black tree to find best-fit blocks) crashes when freelist root is removed
- Can't restart the OS (bluescreen)
- Random bluescreens that are hard to pinpoint