.. Pembroke documentation master file, created by
   sphinx-quickstart on Sun Sep 29 16:14:56 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

########
Pembroke
########

Welcome to `Pembroke`_!

Pembroke aims to be a composable wrapper around the `libevent`_
C library, providing low level primitives (reactors, I/O buffers, etc) as well as
higher level abstractions (web-server, HTTP clients) in an intuitive, simple, and
well-documented interface.

The following docs will help you get started, cover some common use-cases, and provide
higher-level documentation for building an application. See next section for API docs.

This wiki provides some basic instruction on the major features and should
be helpful to get up and running with your application.

.. toctree::
   :maxdepth: 2
   :caption: Guide

   installing
   getting-started
   logging
   timers_schedules
   file_io
   network_io


.. toctree::
   :maxdepth: 2
   :caption: API Docs

    Reactor <api/reactor>
    Buffer <api/buffer>
    Events <api/event>

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`



.. _Pembroke: https://github.com/JohnMurray/pembroke
.. _libevent: https://libevent.org/