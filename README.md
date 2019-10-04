# Pembroke

[![CircleCI](https://circleci.com/gh/JohnMurray/pembroke/tree/master.svg?style=svg)](https://circleci.com/gh/JohnMurray/pembroke/tree/master)

Pembroke aims to be a composable wrapper around the [libevent][libevent_homepage] C
library, providing low level primitives (reactors, I/O buffers, etc) as well as higher
level abstractions (web-server, HTTP clients) in an intuitive, simple, and
well-documented interface.

This project is a __for-fun__ project that aims to be a big boy one day. Until that
time gets closer, much of the work is unlikely to be tracked in tickets. If you are
interested in contributing, please open an issue with a `Question` label so we can
discuss.

## W.I.P.

See project issues for details on current progress and planning, bugs, and progress
toward any tracked milestones.

## Goal(s)

I want to build a framework that is easy and fun to use while still being powerful and
useful for doing real-world work. If the result of this is a series of levers and knobs
and strange encantations, then I can consider this project a failure.

To help get to this end destination, I've defined three simple'ish points that should be
taken into consideration when proposing features or submitting pull-requests.

  + Provide a simple interface that is approachable to C++ devs of all level and
    background, including those new to the language or currently learning.
  + Provide an extensible interface to expose advanced mechanics/concepts when needed,
    but if not needed then sane defaults should be provided without (mental) cost to
    the user.
  + Expose a clean, safe, and sane interface to working with event-loops, using the
    LibEvent framework.

## Documentation

While this project is _definitely_ a W.I.P. I will try to have documentation available
for the bits that are _usable_ (not necessarily production ready). Documentation is
auto-generated and published to ReadTheDocs and are available here:

[pembroke.readthedocs.io](https://pembroke.readthedocs.io/)

Anything not documented here is likely something I'm not considering "usable" at
the moment.

## License

This project is licensed under the permissive MIT license, but if you find this code
useful and have used it in production code I would love to hear about your experience. 

See [LICENSE.txt][license_file] for more details.

  [libevent_homepage]: https://libevent.org/
  [license_file]: ./LICENSE.txt
  [todo_file]: ./TODO.md