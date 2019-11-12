======
Events
======

.. highlight:: c++

Events are a central data-type to "eventing" libraries such as `libevent`_. Whenever you
trigger an action that has a delayed response, such as setting a timer or reading from the
network, an event is involved.

Events are essentially a handle representing your callback. While they cannot be composed
as you might do with higher level constructs such as a ``Future`` or ``Pomise``, they can
be canceled which can be useful if developing higher level abstractions on top of
``Event``.

**Example**

.. code-block::
   :linenos:

   auto reactor = pembroke::reactor().build();
   auto x = 0;

   auto event = reactor->new_timer([&]() -> void {
       x += 1;
   }, 100us);

If this event is not canceled, then in 100 micro-seconds the callback will run and ``x`` will
have a value of 1. Cancelling the event *may* prevent this from happening if done before the
callback triggers.

.. code-block::
   :linenos:

   event->cancel();
   assert(x == 0);

   // OR if called after

   std::this_thread.sleep_for(200us);
   reactor->tick();
   bool ret = event->cancel();

   assert(ret == true); // successfully called cancel
   assert(x == 1);      // BUT still ran event because it was cancelled too late


*******************
``pembroke::Event``
*******************

.. doxygenclass:: pembroke::Event
   :members:

**************************
``pembroke::EventContext``
**************************

.. doxygenstruct:: pembroke::EventContext
   :members:

******************************
``pembroke::EventContextHash``
******************************

.. doxygenstruct:: pembroke::EventContextHash
   :members:


.. _libevent: https://libevent.org/