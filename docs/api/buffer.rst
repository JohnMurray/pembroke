======
Buffer
======

Buffers are all about reading and writing data. The ``evbuffer`` is a simple utility
within libevent that is used throughout the code-base where data is involved. The ``Buffer``
class in Pembroke wraps that functionality, but with an interface that is more idiomatic
to C++. 

********************
``pembroke::Buffer``
********************

.. doxygenclass:: pembroke::Buffer
   :members:

***********************
``pembroke::ByteSlice``
***********************

.. doxygenstruct:: pembroke::ByteSlice
   :members: