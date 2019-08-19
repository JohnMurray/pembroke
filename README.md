# Pembroke

Pembroke aims to be a composable wrapper around the [libevent][libevent_homepage] C library, providing low
level primitives (reactors, I/O buffers, etc) as well as higher level abstractions (web-server, HTTP clients)
in an intuitive, simple, and well-documented interface.

This project is a for-fun project that aims to be a big boy one day. Until that time gets closer, much of the
work is unlikely to be tracked in tickets. If you are interested in contributing, please open an issue so we
can discuss.

## Goals

The end-state for the project should ideally meet the following criteria.

  + Provide a simple interface that is approachable to C++ devs of all level and background.
  + Provide an extensible interface to expose advanced mechanics/concepts when needed, but if not needed then
    sane defaults should be provided without cost to the user.
  + Expose LibEvent with modern C++ classes and abstractions, providing additional safety where possible

## License

This project is licensed under the permissive MIT license, but if you find this code useful and have used it
in production code I would love to hear about your experience. 

See LICENSE.txt for more details.

  [libevent_homepage]: https://libevent.org/
