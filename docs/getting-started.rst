===============
Getting Started
===============

.. highlight:: c++

Be sure that you have successfully [installed](./installing.md) Pembroke before reading
this section.

----

Pembroke offers a simplified reactor pattern using the LibEvent C framework. The main object
you'll be dealing with is the ``Reactor``. This represents a single-threaded event-loop and is
the main interface for any sort of event scheduling and loop-execution.

You can create a simple loop using the default configuration and running it with as little as:

.. code-block::
   :linenos:

   #include <pembroke/pembroke.hpp>

   auto r = pembroke::reactor().build();
   r->run_blocking();

``reactor()`` returns a simple configuration builder and ``build()`` produces a ``Reactor`` object
using the default configuration (in this case). Then the reactor is started by calling
``run_blocking()``. However, since we don't have any tasks to execute, we are essentially
blocking the current thread of execution and don't have a way to continue. We could use a
timed event to do some work and stop the loop as such:

.. code-block::
   :linenos:

   auto r = pembroke::reactor().build();
   
   r->new_timer([&r]() -> void {
       std::cout << "Hello, from a timer\n";
       r->stop();
   }, 100us);
   
   r->run_blocking();

Now we have scheduled a callback to run once 100us from when it was scheduled. The reactor
will block until this timed event is ready, run the callback, and then exit.

If we wanted to control how the reactor runs, we could instead use the ``tick()`` method which
will cause the reactor to run until their are no more events to execute. This means we could
rewrite out first "no-op" example to be non-blocking with:

.. code-block::
   :linenos:

   auto r = pembroke::reactor().build();
   r->tick();

However, if we register new events after ``tick()`` is called, or if new events become ready,
we'll need to call this function again.


************
Timed Events
************

************
File I/O
************

************
Network I/O
************