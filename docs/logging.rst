=======
Logging
=======

.. highlight:: c++

Pembroke does not enforce any kind of logging standard on you, so feel free to
use whatever tickles your fancy. That being said, Pembroke does produce some
internal logs that may be useful for debugging or that you may want to enable
for certain log levels.

To keep logging neutral, Pembroke defines hooks that can be used to wire up your
logging framework of choice. By default these hooks are a no-op, meaning any logs
produces by Pembroke are discarded. To provide your own hook, simply add the
following to your code:

.. code-block::
   :linenos:

   #include <string_view>
   #include <pembroke/logging.hpp>
  
   using namespace pembroke;
  
   logger::register_handler([&my_logger](logger::Level lvl, std::string_view msg) -> void {
       if (lvl == logger::Level::Error) {
           my_logger.error(msg);
       }
   });

If multiple handlers are registered, only the last one will be used. If you require the use
of multiple loggers, you'll need to wire this up yourself in the handlers. Although this should
not be a common use-case.