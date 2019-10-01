# ToDo

### Development Tasks

  - [ ] Scheduler for timed events
  - [x] Internal logging system that can hook into whatever the client wants
      to use (with some `on_log` hook somewhere)
  - [ ] File I/O support
  - [ ] Discover available libevent configuration options on startup
        (such as file-io support, edge-triggering support, etc)

### Support Tasks

  - [ ] Setup tests to run with clang sanitizer(s)
  - [x] Generate documentation with Doxygen
    - [x] Get it working (documented somewhere)
    - [x] Style it with something _not_ hideous
    - [x] Figure out how we can expand it into a user-guide of sorts (like with package-docs in Java/Scala(doc))
    - [x] Make it a task within CMake
    - [x] Publish the documentation somewhere (likely github pages)
    - [x] ~~Make publishing a post-build task on `master` with circle-ci~~
