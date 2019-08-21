# ToDo

Development Tasks

  - [ ] Base/basic `task` class
  - [ ] Testing reactor execution with `task`
  - [ ] Timed and scheduled `task`
  - [x] Internal logging system that can hook into whatever the client wants
      to use (with some `on_log` hook somewhere)

Support Tasks

  - [ ] Setup tests to run with clang sanitizer(s)
  - [ ] Generate documentation with Doxygen
    - [ ] Get it working (documented somewhere)
    - [ ] Style it with something _not_ hideous
    - [ ] Figure out how we can expand it into a user-guide of sorts (like with package-docs in Java/Scala(doc))
    - [ ] Make it a task within CMake
    - [ ] Publish the documentation somewhere (likely github pages)
    - [ ] Make publishing a post-build task on `master` with circle-ci
