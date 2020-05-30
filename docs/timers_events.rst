.. _timers_events:

===============
Timers & Events
===============

.. highlight:: c++

Timers or, more generically, Events are useful abstractions for deferring execution of certain
logic and to avoid blocking the reactor. For example, assume that you want to emit metrics every
30 seconds. The basis for scheduling your metrics emission within Pembroke might look like:


.. code-block::
   :linenos:

   #include <chrono>
   #include <pembroke/pembroke.hpp>
   using namespace pembroke;
   using namespace std::chrono_literals;
   
   class MetricsEmitter {
     event::TimerEvent m_te;

     public:

     MetricsEmitter(const reactor &r) {
       m_te = event::TimerEvent(30s, [this]() -> {
         this->emit_metrics();
       });
       assert(r.register_event(m_te));
     }

     void emit_metrics();
   }

See the :ref:`API Docs <api/event>` for more details on the types of events that can be used
within Pembroke.