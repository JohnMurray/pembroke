# CircleCI

This folder contains all the configuration files and scripts necessary to run tests on
CircleCI.

| __File__              | __Function__                                                     |
|-----------------------|------------------------------------------------------------------|
| `conan_setup.sh`      | Installs [conan][conan] along with a configuration to use Clang, and downloads and compiles 3rd party dependencies |
| `clang.conan_profile` | Clang configuration file installed by `conan_setup.sh`           |
| `cmake_setup.sh`      | Configure cmake with sanitizer and optimization options          |
| `config.yml`          | CircleCI configuration with all job definitions                  |


  [conan]: https://conan.io/