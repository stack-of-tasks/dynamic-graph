dynamic-graph
=============

[![Building status](https://gepgitlab.laas.fr/stack-of-tasks/dynamic-graph/badges/master/pipeline.svg)](https://gepgitlab.laas.fr/stack-of-tasks/dynamic-graph/commits/master)
[![Coverage report](https://gepgitlab.laas.fr/stack-of-tasks/dynamic-graph/badges/master/coverage.svg?job=doc-coverage)](http://projects.laas.fr/stack-of-tasks/doc/stack-of-tasks/dynamic-graph/master/coverage/)
[![License](https://opensource.org/licenses/BSD-2-Clause"><img src="https://img.shields.io/badge/License-BSD%202--Clause-green.svg)](https://opensource.org/licenses/BSD-2-Clause"><img src="https://img.shields.io/badge/License-BSD%202--Clause-green.svg)

This software provides an efficient way to modelize a C++ data-flow.

A dynamic graph data-flow is composed of:
 * entities (graph nodes)
 * signals (input/output of a graph action)

Output signals can then be plugged into input signals to data
transmission.

An efficient caching mechanism avoid useless data recomputation and a
simple built-in language can be used to control the graph actions.

It is released under the [BSD-clause 2](COPYING.LESSER) license.


**Warning:** this repository contains [Git
submodules][git-submodules]. Please clone this repository using the
`git clone --recursive` command. If you already have cloned the
repository, you can run `git submodule init && git submodule update`
to retrieve the submodules.


Documentation
-------------

To get started with this library, please read the [online Doxygen
documentation][doxygen-documentation].

It can also be generated locally by running the `make doc`
command. After the package is installed, the documentation will be
located in the `$prefix/share/doc/dynamic-graph` directoy where
`$prefix` is your installation prefix (`/usr/local` by default).


Getting Help
------------

Support is provided through issues on the github interface.

How can I install dynamic-graph?
--------------------------------

### Installing dependencies

The matrix abstract layer depends on several packages which
have to be available on your machine.

 - Libraries:
   - [Boost][] (>= 1.40)
   - Doxygen
   - Eigen3
   - pthread
 - System tools:
   - [CMake][] (>=2.6)
   - [pkg-config][]
   - usual compilation tools (GCC/G++, make, etc.)
     If you are using Ubuntu, these tools are gathered in the `build-essential` package.



### Compiling and installing the package

The manual compilation requires two steps:

 1. configuration of the build and generation of the build files
 2. compilation of the sources and installation of the package

dynamic-graph uses [CMake][] to generate build files. It is
recommended to create a separate build directory:

```sh
mkdir _build         # (1) Create a build directory
cd _build            # (2) Go to the newly created build directory
cmake [options] ..   # (3) Generate the build files
```

Options which can be passed to CMake are detailed in the next section.

```sh
make                 # (4) Compile the package
make test            # (5) Execute the package tests
make install         # (6) Install the package into the prefix (see step 3)
```


### Options

Additional options can be set on the command line through the
following command: `-D<option>=<value>`.

For instance: `cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..` will set
the `CMAKE_BUILD_TYPE` option to the value `RelWithDebInfo`.


Available options are:

- `CMAKE_BUILD_TYPE` set the build profile that should be used (debug,
  release, etc.). We recommend `RelWithDebInfo` as it will provide
  performances while keeping debugging symbols enabled.
- `CMAKE_INSTALL_PREFIX` set the installation prefix (the directory
  where the software will be copied to after it has been compiled).


### Running the test suite

The test suite can be run from your build directory by running:

```sh
   make test
```

Please open a ticket if some tests are failing on your computer, it
should not be the case.


Contributing
------------

If you want to contribute, please refer to the
[CONTRIBUTING.md](CONTRIBUTING.md) file

Credits
-------

This package authors are credited in the [AUTHORS](AUTHORS) file.



Available Packages
------------------

 * RobotPkg :
   http://robotpkg.openrobots.org/robotpkg/motion/dynamic-graph/index.html


[doxygen-documentation]: http://stack-of-tasks.github.io/dynamic-graph/doxygen/HEAD/

[git-submodules]: http://git-scm.com/book/en/Git-Tools-Submodules

[Boost]: http://www.boost.org/
[CMake]: htttp://www.cmake.org/
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Eigen]: http://eigen.tuxfamily.org/
[Git]: http://git-scm.com/
[Libtool]: https://www.gnu.org/software/libtool/
[RobotPkg]: http://robotpkg.openrobots.org/
[log4cxx]: https://logging.apache.org/log4cxx/
[pkg-config]: http://www.freedesktop.org/wiki/Software/pkg-config/
