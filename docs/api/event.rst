.. _api/event:

======
Events
======

.. highlight:: c++

Events are (currently) manually triggerd/scheduled callbacks. There exist two event
types of events, a delayed event and a timer event. Both event-types will run user-defined
callbacks. Delayed events represent single-execution callbakcks while timers represent
recurring actions.

Events are created and then must be registered on the reactor in order to be scheduled for
execution. Once registered, they can be cancelled and they exist as RAII types, so destructing
the object will also cause the callback to be de-registered from the reactor.

**Example**

.. code-block::
   :linenos:

   auto reactor = pembroke::reactor().build();
   auto x = 0;

   auto event = pembroke::event::TimerEvent(100us, [&]() -> void {
       x += 1;
   });
   reactor->register_event(event);

If this event is not canceled or destructe******d it will run on the next iteration of the reactor's
event-loop and will continue to run every 100 microseconds.

**************
**TimerEvent**
**************

.. doxygenclass:: pembroke::event::TimerEvent
   :members:

****************
**DelayedEvent**
****************

.. doxygenclass:: pembroke::event::DelayedEvent
   :members:
