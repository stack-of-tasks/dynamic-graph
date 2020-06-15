^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package dynamic-graph
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

4.1.1 (2020-06-15)
------------------
* Fix Eigen3 issues
* v4.1.0
* sync submodule
* [CMake] fix PACKAGE_EXTRA_MACROS use
* sync submodule
* [tests] Add command test for return value.
* [command] Add a template to return value from command.
  Only up to 2 arguments.
  Variadic template might be implemented but I found no
  simple solution.
* Merge branch 'master' into devel
* v4.0.0
* [CMake] clean: keep minimal required instructions
* [CMake] fix SUFFIX_SO_VERSION
* [CMake] ADD_PROJECT_DEPENDENCY is ready for Eigen3
  thanks to PKG_CONFIG_REQUIRES
* Merge pull request `#62 <https://github.com/Rascof/dynamic-graph/issues/62>`_ from jmirabel/devel
  [TracerRealTime] Check that output file is valid.
* [TracerRealTime] Fix addSignalToTrace for exception handling
* [TracerRealTime] Check that output file is valid.
* Update tests/debug-real-time-tracer.cpp
* Make TracerRealTime::getBufferSize and setBufferSize public.
* Merge pull request `#60 <https://github.com/Rascof/dynamic-graph/issues/60>`_ from florent-lamiraux/signal-empty-string
  Fix Signal<std::string>
* Merge pull request `#59 <https://github.com/Rascof/dynamic-graph/issues/59>`_ from florent-lamiraux/suffix-so-file
  Add Cmake option to deactivate suffix of so files.
* Do not use --disable-new-dtags that is not supported by all linker versions.
* Partially revert 5daf1d390dbd1249a153d015ca00c6cfb847734c:
  "[signal-cast-helper] Removed unused template specialization".
  DefaultCastRegisterer<std::string>::cast template specialization is
  required to set empty string to a signal.
* Add a test that sets a signal to empty string.
* Add Cmake option to deactivate suffix of so files.
  Force tests to link with the library in build directory.
* clang format.
* [cmake] Synchronize
* Add tests/signal-all.cpp
* [tests] Improve coverage of signal-cast-registered.
* [tests] Improve overall coverage to check the behavior and clean the code.
* [signal-cast-helper] Removed unused template specialization
  This commit removes template specialization not exposed in header
  files, and better handled by the default implementation.
* Fix bugs related to loggers.
* Provide backward compatibility + reorganize headers.
* Clean code.
* Improve efficiency of class Logger
* Give access to the Entity::logger\_ + fix doc.
* Split real-time-logger.h into two files
  Put class declarations in one and macro definitions in the other.
* Enhance interface of RealTimeLogger and RTLoggerStream
* Add unit tests.
* Add Value::operator==
* Add vector of value as value type.
* Remove unused file.
* Merge branch 'master' into devel
* [linter] Apply cppcheck
* Remove file include/dynamic-graph/null-ptr.hh
* [README.md] Fix link to logo.
* [README.md] Add license BSD-v2 logo.
* [tests] Fix style of signal-time-dependent
* [tests] Improve tests and coverage of class Value.
* [tests] Improve coverage of signal-ptr.
* [tests] Improve coverage of entity.
* [tests] Improve tests and coverage of real-time tracer.
* [tests] Coverage with improved precision.
* [tests] Improve command coverage.
* [signal] Add comments and docs.
* [signal-ptr] Add comments and doc.
* [command-setter.t.cpp] Fix style.
* [tests] Improve value coverage.
* [tests] Improve coverage of signals.
* [tests] Improve coverage of command.
* [headers] writeGraph simplified
  The code is not used.
* [headers] Remove warnings from Eigen.
* [linear_algebra] Remove warnings with Eigen on bionic.
* [tests] Add missing tests of the signal interface.
* [tests] Add real-time-tracer test.
* [tests] Add test for direct-getter and direct-setter.
* [tests] Improve tests on value.cpp
* [tests] Improve value.cpp coverage.
* [tests] Add unit tests for Value.
* [travis] Add devel and remove debian branch
* [README.md] Update README
* v3.8.1
* add missing includes
* [CMake] clean
* [CMake] replace main LIBRARY_NAME by PROJECT_NAME
  Just to avoid the confusion of having 2 things with the same name
  without a clear reason
* [CMake] put headers in library definition
* [CMake] remove empty file
* [CMake] revert cmake-exports for eigen3 & boost
* [travis] Remove dependency to jrl-mal and jrl-mathtools.
  Thus remove the dependency to lapack and gfortran too.
* [travis] Synchronize.
* [travis] Fetch completely the repo to avoid missing ref when fetching tags.
  Remove depth flag for git.
  Add build_custom to avoid callking catkin_workspace.
  Install needed binary packages.
  Add common.sh to travis_custom
  Remove fetch unshallow
  Add fortran to test the package.
  Add blas in the set of packages.
  Add lapack-dev as package to install
* v3.8.1
* add missing includes
* [CMake] clean
* [CMake] replace main LIBRARY_NAME by PROJECT_NAME
  Just to avoid the confusion of having 2 things with the same name
  without a clear reason
* [CMake] put headers in library definition
* [CMake] remove empty file
* [CMake] revert cmake-exports for eigen3 & boost
* v3.8.0: add package.xml
* [CMake] sync submodule
* [cmake] Remove header.cmake from CMakeLists.txt
  Feedback from Guilhem Saurel.
* [package.xml] Remove roscpp and fix license.
* [cmake] Remove SETUP_PROJECT_PACKAGE_FINALIZE
* [cmake] Remove useless CMakeLists.txt informations.
  Based on feedback from G. Saurel.
* [cmake] Synchronize
* Disable error on warnings
* [cmake] Synchronize
* CMakeLists.txt switch to CMake Boost detection.
* package.xml: Change from dynamic_graph to dynamic-graph
* [cmake] Add target_include_directories.
* [catkin] Update package.xml
* [cmake] Update CMakeLists.txt
* [super build] Add package.xml
* [cmake] Remove display of all the variables.
* [cmake] Remove PROJECT_SETUP()
* [cmake] Fix CMake package
* [cmake] Synchronize jrl-cmake modules
* [cmake] Switch to CMake export
* Fix style
* Fix DefaultCastRegisterer<std::string>::cast
  - when signal value contained a space, only the part before the space
  was stored in the signal.
  - add a test on this case.
* Remove outdated comment.
* Specialize template DefaultCastRegisterer<std::string>::disp
  Default implementation adds std::endl at the end of the stream.
  In the case of string this extra '\n' is not desired.
* Add a test on plugging signals of type string
* Specialize template DefaultCastRegisterer<std::string>::cast
  When type is string and input istringstream contains an empty string,
  operator>> make method fail return true.
  This commit overcome this issue by not testing the result.
* Fix style.
* Improve doc.
  Fix debug-tracer.cpp
* Add signals of type std::string.
* Merge pull request `#45 <https://github.com/Rascof/dynamic-graph/issues/45>`_ from jmirabel/devel
  Add TimeDependency::addDependencies + doc of SignalTimeDependent
* [CMake] Synchronize module.
* Update documentation of SignalTimeDependent + fix doxygen errors.
* Add convenience function TimeDependency::addDependencies
* [clang] Update format.
* [debug] Add toString logger tests + uncomment line used for scalar types.
* [clang-format] Comply to Google style.
* Fix typo. Enforce 80 columns policy.
* Merge branch 'coverage' into 'devel'
  Merge Coverage into devel
  See merge request stack-of-tasks/dynamic-graph!2
* Format all the code
* Merge pull request `#43 <https://github.com/Rascof/dynamic-graph/issues/43>`_ from corentinberge/coverage
  Coverage
* Update process-list.cpp
  Delete the cout and the include of iostream
* Update process-list.cpp
  total_periode is a long long unsigned int, not a double
* Update process-list.cpp
  Include iostream because cout need that to be part on the namespace std
* Update logger.cpp
  Change the input mode to ostringstream
* Merge remote-tracking branch 'origin/coverage' into coverage
* Update logger.cpp
  Made a bug when I was added line, I delete the bug
* Update signal-cast-registerer.cpp
  Make the code easier to read by importing the matrix m in OS instead of importing elements 1 by 1
* Update signal-cast-registerer.cpp
  Delete the end of file (type checking)
* Update factory.cpp
* Update logger.cpp
* Update signal-ptr.h
  Delete isplugged instead of unplug
* Delete command.cpp
  command-test was already created, didn't have this test when I pull
* Merge remote-tracking branch 'origin/coverage' into coverage
* command-test was already created
* Merge branch 'master' into coverage
* Commit for PR
* Commit command.cpp and CMkaLists.txt for analysis
* Increase the coverage (include/dynamic-graph/eigen-io.h -> 100%) Make the matrix test, but 36 Warnings
* Increase the coverage (include/dynamic-graph/time-dependency.h -> 100%)
* Add test to follow multi-threading.
* Fix priority problem when logging + add CPU load introspection.
* Merge branch 'devel' into origin-2019-07-12
* Increase the coverage (include/dynamic-graph/Eigen-io.h)
  Add on test/signal-cast-registerer lot of try - catch for the signalException error
* Another useless function find, maybe delete?
  Try to make test for signal-ptr.h
* Useless function find, maybe delete?
* Try to find a way to coverage more files.
  Focus on signal-array (not Boost test).
  Try to understand a way to make boost test and analyze signal-array.
* Increase the coverage (include/dynamic-graph/Entity.h -> 100%)
  Add on test/Entity.h the virtual function (Entity::getClassName)
* Fix following Address Sanitizer
* Merge tag v3.4.0
* [tests] Add cmake tests.
* [doc] Update documentation for real-time-logger.
* [debug] Logger - Fix race condition
  Race condition if the value reach zero, then the time sample
  has to be reset to timeSamplePeriod.
* [entity] Add set/getTimeSample and set/getStreamPrintPeriod.
  This should be externalized as it has an impact on output
  messages.
* [cmake] Synchronize
* [js/doc] Add display using a browser and documentation
* Minor fix. Remove trailing whitespace
* [doc] Add documentation on macros for the entities.
* Merge tag 'v3.3.0'
  Release of version 3.3.0.
* Merge pull request `#39 <https://github.com/Rascof/dynamic-graph/issues/39>`_ from nim65s/devel
  Packaging for the v3.3.0 Release
* Merge branch 'devel' of github.com:stack-of-tasks/dynamic-graph into devel
* Merge branch 'devel' of github.com:stack-of-tasks/dynamic-graph into devel
* [doc] Add macros explanations.
* [test] Add missing tests/debug-logger-winit.cpp
* [tests] Add tests on sendMsgs without initialization.
  Working. Fix issue `#37 <https://github.com/Rascof/dynamic-graph/issues/37>`_
* [doc] Add documentation on using macros
* Remove GPL Headers
* [CMake] Update & remove CPack
* [tests] Improves the unit test of the debug-logger
  TODO: Right now this is only a simple coverage.
  The tests is always sending back True.
* [doc] Improve documentation on logger and real-time-logger
  Mostly give a sample on how to use the logger member inside the entities.
* [tests] Add test for logger.h
* [signal] Fix the macros declaring signals
  DECLARE_SIGNAL_IN, CONSTRUCT_SIGNAL_IN, DECLARE_SIGNAL_OUT and CONSTRUCT_SIGNAL_OUT
  The macros were duplicated in various SoT packages.
  Unify them by:
  * Adding prefix m\_ and suffix SIN for input signals
  declared as fields of entity classes.
  * Adding prefix m\_ and suffix SOUT for output signals
  declared as fields of entity classes.
* [tests] Add debug-tracer.cpp
* [tests] Remove std::cout in debu-trace.cpp
  Renamed tracer.cpp in debug-tracer.cpp
* [tests] entity add test for signalDeregistration.
* [tests] Improve tests on pool.cpp with respect to exceptions.
  Add entity test to check writeCompletionList + license modification.
* [tests] debug-trace.cpp: detect robustly trace output.
  pool.cpp: test exception catching.
* [tests] Improves unitary test on pool.cpp
* [tests] Improve test on pool
  Testing and checking output of writeGraph.
* [tests] Increase the coverage of unit tests for pool object.
* [doc] Improve documentation of entity + signals.
* [doc] Update documentation on debugging.
  Structural modification of the documentation
  to make it clearer.
* [doc] Rewamping the documentation structure.
  TODO: Homogeneous relationship between debugging information.
  Giving more python examples.
* [cmake] Synchronize
* [cmake] Remove install of contiifstream.h
* [cmake] Remove installation of plugin contiifstream.
* Remove contiffstream class
* [doc] Add Logger documentation
* [tests/entity.cpp] Clean code
  Remove useless usleep
  Remove useless array.
* [unittest] Add missing '#define ENABLE_RT_LOG
* Clean code.
* [topic/logger] Add Logger to all entities.
  It stream messages on a shared file.
  Each entity has a different verbosity level.
  Uses a non real time thread to perform logging.
  No yet working.
* [topic/logger_sigHelper] added logger.h and modified signal-helper.h from sot-torque-control
* Allow entity to be instanciated, for testing purposes
* [Doc] There is no more "Built-in scripting language"
* [CMake] Remove share
* [CMake] clean headers
* remove COPYING.LESSER, cf LICENSE
* remove debian packaging, use robotpkg
* [CI] include conf from dashboard
* sync submodule
* Fixed RealTimeLogger scheduler and priority
* [cmake] Add suffix for the cmake modules
* [README.md] Fix license from LGPL to BSD-clause 2
* [travis] Synchronize
* Fix `#30 <https://github.com/Rascof/dynamic-graph/issues/30>`_
* [cmake] Synchronize
* Revert "Fix PoolStorage destructor"
  This reverts commit 4c3d4c828e47d56eaaac38f6f835cc4447d82d60.
* Synchronize cmake module
* Fix RealTimeLogger
* In RealTimeLogger, add thread safety for writting + add doc.
* Add macros to use real time logs.
* Add real time logger
* Add __null_stream function to avoid compilation error
* Remove unused inclusion of iostream
* Fix PoolStorage destructor
* Add missing license file.
* Change license from LGPL to BSD-v2 only for .cpp files.
* [pool] Reintroduce the fact than in the destructor we should go at the
  beginning of the map.
* remove .version file
* sync submodules
* [CMake] Doxygen w/ MathJax
* remove shell
  ref https://github.com/stack-of-tasks/sot-core/issues/58
* Fix mistake in freeing object in the Pool destructor.
  Spotted by M. Naveau.
* [cmake] Synchronize
* [travis] Synchronize
* Merge pull request `#25 <https://github.com/Rascof/dynamic-graph/issues/25>`_ from nim65s/master
  update badges
* update badges
* Merge pull request `#24 <https://github.com/Rascof/dynamic-graph/issues/24>`_ from nim65s/master
  [CI] add .gitlab-ci.yml
* [CI] add .gitlab-ci.yml
* [doc] Add reference to tutorial.
* [doc] Improve documentation.
* Merge pull request `#20 <https://github.com/Rascof/dynamic-graph/issues/20>`_ from stack-of-tasks/devel
  Move Master to v3
* [debian] Correct error from previous commit
  Remove 'Nosoname true' for plugins
* [codespell] Correct minor spelling errors
* [debian] remove pedantic errors
  * add gpg public key for package releases
  * update watch file to look for key signature
  * remove typos from library plugins
  * change copyright to match dep5 format
  * remove call of ldconfig in package maintainence scripts
* [debian] debian changes for ubuntu + change copyright based on format
* [cmake] move cmake to current head
* Add version file for current stable version+ Edit debian/watch file
* debianize the package
* [c++] fix bug in matrix istream input operator
* [c++] update the ostream output format for MatrixHomogeneous to [M,N]((,,,),(,,,),(,,,))
* Patch for inputing Eigen::Transform as Matrix4d
* [eigen] add ostream and istream operators for Eigen/Geoemetry classes.
  modify dg::Vector and dg::Matrix istream operators.
* [eigen] Replace jrl-mal with eigen
* [cmake] Synchronize
* Merge pull request `#17 <https://github.com/Rascof/dynamic-graph/issues/17>`_ from andreadelprete/master
  Fix bug in signal-array (max number of signals was 20)
* Initialize signal array with size 20 even when constructing it from a single signal.
* Merge branch 'master' of github.com:andreadelprete/dynamic-graph
* Fix bug in signal-array that was limiting the number of signals in a signal-array to 20. Moreover I replaced the C array with an std::vector.
* Contributors: Alexis Nicolin, Andrea Del Prete, Bergé, Florent Lamiraux, Guilhem Saurel, Joseph Mirabel, Noëlie Ramuzat, Olivier Stasse, Rohan Budhiraja, andreadelprete, corentinberge, fbailly, ostasse@laas.fr, rascof

2.5.6 (2014-08-01)
------------------
* Merge pull request `#16 <https://github.com/Rascof/dynamic-graph/issues/16>`_ from gergondet/topic/FixVisibilityIssue
  Fix visibility issues
* [Win32] Remove template specialization declaratn, export symbols.
* Don't export/import template functions
* Move definition of template instatiation to cpp
* Fix some issues with the export
  - No need to export template functions
  - Expose some template instantiations
* Fix visibility issues
  - Replace DYNAMIC_GRAPH_DLLEXPORT by DYNAMIC_GRAPH_DLLAPI
  - Remove an unnecessary attribute
* [Travis] Synchronize.
* Merge pull request `#15 <https://github.com/Rascof/dynamic-graph/issues/15>`_ from francois-keith/master
  Add a method to check if a signal with the given name exists.
* Add a method to check if a signal with the given name exists.
* [Travis] Synchronize
* [cmake] Synchronize
* Merge pull request `#14 <https://github.com/Rascof/dynamic-graph/issues/14>`_ from francois-keith/master
  Correct the name of the method isPlugged.
* Correct the name of the method isPlugged.
  The old one is kept, but with a warning message
  (for now).
* Update ChangeLog to release v2.5.5
* [travis] Add missing dependencies
* [travis] Synchronize
* Get rid of the Debian directory
* Update README.md [skip ci]
* Add missing newline at end of file :lipstick: [skip ci]
* [travis] Synchronize
* [travis] Use jrl-travis
* [cmake] Synchronize
* Synchronize
* Fix plugindir in dynamic-graph.pc
* [travis] Enhance build script
* Update README
  [skip ci]
* Remove unwanted files
* [travis] Fix push permissions for gh-pages
* Synchronize
* [travis] Add multiarch support
* [travis] Add missing dependencies.
* [travis] Add Travis and coveralls.io support.
* Allow to access class name of a signal.
* Update lib installation path (multiarch portability).
* Synchronize
* Update changelog
* Synchronize.
  Fix MANDIR problem.
* Synchronize.
* Fix -Wcast-qual pulled by -Werror of gcc-4.7
  Pointed out by A. Mallet.
* Update ChangeLog.
* Synchronization
* IVIGIT, added signal-helper (macros to ease the declaration of signals) and entity helper (macros to make the macros of signal-helper possible).
* IVIGIT, added signal-helper (macros to ease the declaration of signals) and entity helper (macros to make the macros of signal-helper possible).
* Fix bug in method PoolStorage::writeGraph.
* Synchronize.
* MacOSX compatibility: avoid dangerous TARGET_LINK_LIBRARIES
  This crashes the linkage because of the particular link
  with the frameworks (here the framework Acceleration).
* Fix portability issues with Ubuntu 12.04 (64 bits).
* Do not try to delete void*, behavior is undefined.
* Avoid memory loss when calling Value::operator =
* Passing and returning elmt by const reference.
* Add boost::posix_time::ptime as a basic type for signals.
* Add missing include.
* Provide const access to entity map in the pool.
* Add method Entity::getDocString.
* Synchronize.
* Fix build for binutils > 2.22
  Patch reported by Anthony Mallet.
* Synchronize cmake submodule.
* Merge remote branch 'origin/master'
* Added a caster for signal<bool>.
* Update NEWS.
* Do not hardcode dl, use ${CMAKE_DL_LIBS}.
* Make destructor of TimeDependency virtual.
* Added the cmake to compile signal-ptr test.
* Modify the policy for plugin a sigptr in input.
* ivigit.
* Disable a test for mac.
* Revert "Move the definition of some ValueHelper<...>::TypeId to avoid link issues"
  This solution worked only for win32 systems, and
  created link errors for unix systems...
  This reverts commit e7e487ebdf9c550742d4c9525bbb151e25437393.
* Move the definition of some ValueHelper<...>::TypeId to avoid link issues
* Add missing header inclusion
* Win32: Correct a wrong macro
* Win32: Correct the importation/exportation of symbols
* For compatibility, added a static function to bind the singleton.
* Merge branch 'topic/proto-command' into topic/singleton
  Conflicts:
  tests/entity.cpp
* Entity has no more CLASS_NAME static member.
* Merge branch 'topic/proto-command' of github.com:jrl-umi3218/dynamic-graph into topic/proto-command
  Conflicts:
  include/dynamic-graph/entity.h
* Account for the Entity::getClassName becoming pure virtual.
* Pass the getclassName to pure virtual.
* Make package pass tests successfully.
* Cosmetic change.
* SignalCaster class is now a singleton.
  Call to g_caster function have been replaced by call to
  SignalCaster::getInstance.
* FactoryStorage and PoolStorage are now singletons.
  g_pool and g_factory global variables have been replaced by static methods
  getInstance in each class.
  getInstance returns a pointer to the unique instance of the class and
  creates it if needed.
* Add helper macro for entity declaration. Use it when possible.
* Rewrite an error message.
* Win32: Correct a link issue
  The var EXECUTABLE_NAME does not exist, so the command creates
  a wrong linkage between the two libraries.
* A cleaner way to define the suffix of the dynamic libraries according to each OS
* Add missing link directory command (f-kiss).
* Make getClassName method const
  This virtual methods is defined in Entity class. Changing the prototype
  breaks the overloading mecanism.
* Correct a bug in previous commit.
* Added command 4.
* Problem of automatic typing with templates.
* Merge branch 'topic/proto-command' of github.com:jrl-umi3218/dynamic-graph into topic/proto-command
* Add nullptr class.
* Corrected function to inline to avoid multiple symbole definition.
* Added a function to test for the existence of a named entity.
* Code cleaning after rewriting (marginal) of sig-cast. Introduction of a specific tracer behaviour for matrix and vector.
* Corrected a segfault problem in the test pool. The problem at the global level remains. At least, the test passes now.
* Modify the g_caster object with a singleton design.
* Reforge the caster for signals.
* Remove a duplicated command.
* IVIGIT: transfert some code and automatic-code-generation macros from sot-core to dg.
* Merge branch 'topic/proto-command' of github.com:jrl-umi3218/dynamic-graph into topic/proto-command
* Added new-style command for the two entities.
* Put back the dirty removal of Florent.
* Revert "Set version number as 1.99."
  This reverts commit 4513ebe960d8014e8d916f67f8c759f896fa5153.
* Value::operator== does nothing when a = a.
* Do not deregister entity in entity destructor. It is deregistered by the pool.
* Use getClassName() instead of CLASS_NAME in Entity constructor.
* Set version number as 1.99.
* Modify deallocation of poolStorage object in order to fix a memory bug.
* Rename poolStorage::entity -> poolStorage::entityMap.
* Added command 3 params.
* Remove unnecessary virtual keyword.
* Added a function to access directly a given command, with protection.
* Merge remote branch 'origin/topic/proto-command' into topic/tmpsafe-proto-command
* Added the command 2 args (what a funny code to write).
* Put back mistakenly removed method pool::getSignal.
* Added all-commnds in the cmake list.
* IVIGIT.
* Clean up: remove interpreter.
  Remove pool::getSignal().
* Revert "Add method to interprete a string as a python command in class Interpreter."
  This reverts commit 8fca0b1a7053beeb48eac0287ae2d62f0261bc87.
  Move python interpreter in dynamic-graph-python.
* Revert "Link libdynamicgraph.so with libpython"
  This reverts commit 0b9f9528b0c2fc0c57bc433b035babddd2f611ca.
  Move python interpreter in dynamic-graph-python.
* Added a bind for commands on std::ostream.
* Added missing include.
* Merge remote branch 'origin/topic/proto-command' into proto-command
* Added an alias for EMPTY_ARG for readibility.
* IVIGIT.
* Add read access to Entity::signalMap.
* IVIGIT.
* Added an helper to get the Value::TypeID directly from the type.
* Merge remote branch 'origin/topic/proto-command' into proto-command
* Link libdynamicgraph.so with libpython
  * src/CMakeLists.txt,
  * tools/CMakeLists.txt.
* Corrected a warning uninitialized value.
* Add method to interprete a string as a python command in class Interpreter.
  * include/dynamic-graph/interpreter.h,
  * src/CMakeLists.txt,
  * src/dgraph/interpreter.cpp,
  * tests/CMakeLists.txt,
  * tools/dg-python.cpp: new,
  * tools/CMakeLists.txt: this adds a dependency to  python in dynamic-graph.
* Merge back master branch.
* Synchronize.
* Add tracer test.
* Add a method returning the list of type names registered for signals.
  * include/dynamic-graph/signal-caster.h,
  * src/signal/signal-caster.cpp.
* Fix method documentation in factory.h.
* Comment classes.
* Complete forward declarations list in fwd.hh.
* Remove obsolete documentation.
* Fix warnings detected by clang.
* Update man pages.
* Update dg-shell-plugin to match new dg-shell usage.
* Remove warnings found by Clang.
* Install dynamic-graph scripts.
* Add dynamic-graph scripts.
* Enhance dg-shell.
* Use modern C++ comment style for headers.
* Improve error management in import.
* Enhance import to avoid importing a module twice.
* Enhance import to support plug-in, use Boost.Filesytem.
* Clean code.
* Cosmetic changes.
  * src/signal/signal-caster.cpp: cut long lines for function
  SignalCaster::registerCast.
* Make exception messages more explicit.
  * src/signal/signal-caster.cpp.
* When registering a type, store pointer to type_info in a map
  * include/dynamic-graph/signal-caster.h,
  * src/signal/signal-caster.cpp: if a typename is registered several times,
  throw only if pointers to type_info differ. When loading python modules,
  for some reason, global variables are constructed several times.
* Add tests for plug-in loading/unloading.
* Fix bad exception rethrow.
* Enhance run command error message.
* Clean interpreter test.
* Add tests for interpreter.
* Fix cppcheck errors.
* Make cast registerer more robust.
* Clean signal-caster.h.
* Convert test_signalcast into unit test.
* Convert test_depend example into depend unit test.
* Fix factory test.
* Convert test_pool.cpp example in pool.cpp unit test.
* Test FactoryStorage.
* Make tests more robust.
* Clean and document factoryStorage.
* Make ExceptionAbstract::getCode() const.
* Clean DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro.
* Add custom entity test.
* Make DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN more robust.
  The previous implementation was not namespace independent.
  Fix this by specifying types properly.
* Remove wrong comment.
* Enhance Entity test case.
* Add unit test for Entity class.
* Clean unit tests compilation.
* Remove unwanted debug call.
* Synchronize.
* Add cast registerer for maal matrix and vector types
  * src/signal/signal-caster.cpp.
* Make error message more explicit.
  * src/signal/signal-caster.cpp.
* Synchronize cmake submodule
  * cmake.
* Add a test to check Value assignment operator.
  * unitTesting/CMakeLists.txt,
  * unitTesting/test-value.cpp: new.
* Fix Value::operator=.
  * src/command/value.cpp: operator= should assign this.
* Add dependency to jrl-mal
  * CMakeLists.txt,
  * include/dynamic-graph/linear-algebra.h,
  * src/CMakeLists.txt.
  For homogeneity, we use jrl-mal for matrices and vectors.
* Command constructor takes a documentation string as input.
  * include/dynamic-graph/command-getter.h,
  * include/dynamic-graph/command-getter.t.cpp,
  * include/dynamic-graph/command-setter.h,
  * include/dynamic-graph/command-setter.t.cpp,
  * include/dynamic-graph/command.h,
  * src/command/command.cpp.
* Remove debug output
  * src/command/value.cpp.
* Fix double free error
  * include/dynamic-graph/value.h,
  * src/command/value.cpp.
* Remove debug output
  * src/command/command.cpp.
* Add support for vector and matrix
  * include/CMakeLists.txt,
  * include/dynamic-graph/command-setter.t.cpp,
  * include/dynamic-graph/value.h,
  * src/command/value.cpp.
  Types for vector and matrix are boost::numeric::ublas::vector<double>
  and boost::numeric::ublas::matrix<double>.
* Synchronize
  * cmake.
* Fix compilation error messages after rebase
  * include/dynamic-graph/command-getter.h,
  * include/dynamic-graph/command-setter.h,
  * include/dynamic-graph/command.h,
  * include/dynamic-graph/entity.h,
  * include/dynamic-graph/value.h,
  * src/command/command.cpp,
  * src/command/value.cpp.
* Re-introduce files in CMakeLists.txt that were lost in rebase
  * src/CMakeLists.txt.
* Fix bug in copy constructor of Value class
  * src/command/value.cpp.
* Support more type for command parameters
  * include/dynamic-graph/command-setter.h,
  * include/dynamic-graph/command-setter.t.cpp,
  * include/dynamic-graph/value.h,
  * src/command/value.cpp: support bool, unsigned and float.
* Add some documentation
  * include/dynamic-graph/command-getter.h,
  * include/dynamic-graph/command-setter.h,
  * include/dynamic-graph/command.h.
* Make function return values instead of reference to values.
  * include/dynamic-graph/command.h,
  * src/command/command.cpp.
* Fix memory issue and changed class name AnyType -> EitherType.
  * include/dynamic-graph/value.h,
  * src/command/value.cpp.
* Add template getter command
  * include/CMakeLists.txt,
  * include/dynamic-graph/command-getter.h: new,
  * include/dynamic-graph/command-getter.t.cpp: new,
  * include/dynamic-graph/parameter.h: deleted.
* Fix implementation of command-setter.
  * include/CMakeLists.txt,
  * include/dynamic-graph/command-setter.h: new,
  * include/dynamic-graph/command-setter.t.cpp: new,
  * include/dynamic-graph/value.h.
* Re-implement value getter in a nicer way.
  * include/dynamic-graph/value.h,
  * src/command/value.cpp.
* Fix several memory errors
  * include/dynamic-graph/command.h,
  * include/dynamic-graph/value.h
  * src/command/command.cpp,
  * src/command/value.cpp.
* Implement command
  * include/CMakeLists.txt,
  * include/dynamic-graph/command.h: new,
  * include/dynamic-graph/entity.h,
  * include/dynamic-graph/parameter.h: new,
  * include/dynamic-graph/value.h: new,
  * src/command/command.cpp: new,
  * src/command/value.cpp: new,
  * src/dgraph/entity.cpp.
* Synchronize
* Synchronize
* Synchronize
* Synchronize
* Synchronize.
* Synchronize
* Add missing SETUP_PROJECT_CPACK.
* Include cpack to geneate 8.04 Ubuntu package.
* Synchronize.
* Synchronize
* Merge branch 'master' of git://github.com/jrl-umi3218/dynamic-graph
* Synchronize
* Remove the macro BUILDING\_'PKG' and use the one defined by header.cmake instead
* Add missing Boost flags for the projects in tools
* Correct dependencies between projects
* Lighten CMakelists by removing win32 flags (now in cmake)
* Declare boost include directories for proper boost detection. Sometimes, the boost install path is *different* from the default include paths!
* Fix relative path in link_directories.
  To comply with CMake v2.8.2 policy.
* Fix warnings.
* Enable -Werror.
* Synchronize.
* Synchronize.
* Merge branch 'topic/submodule'
* Merge branch 'topic/warnings'
* Small documentation update.
* Synchronize.
* Synchronize.
* Synchronize.
* Fix pedantic, -Wextra warnings.
* Fix GCC warnings.
* Fix visibility management for all plug-ins.
* Synchronize.
* Switch to generic pthread detection macro.
* Synchronize.
* Switch to new Boost detection macro.
* Merge branch 'master' into topic/submodule
* Convert figure to png.
  Figures are to be converted to png so that all browsers can read them,
  however svg files are be kept to allow futher editions of the figures.
* Convert figure to png.
  Figures are to be converted to png so that all browsers can read them,
  however svg files are be kept to allow futher editions of the figures.
* Synchronize.
* Merge remote branch 'origin/master' into topic/submodule
* * Moved entity.png to doc/figures so that it can be found by doxygen (cannot use the svg version because it breaks the layout in my browser - Chrome).
  * Corrected some doxygen errors.
* Add extra image path for Doxygen documentation.
* Synchronize.
* Synchronize.
* Use generated config.hh header file for symbol visibility management.
* Set CUSTOM_HEADER_DIR in CMakeLists.txt.
* Synchronize.
* Merge branch 'master' of github.com:jrl-umi3218/dynamic-graph into topic/submodule
* Corrected doxygen errors. Complemented documentation on signals.
* Simplify CMakeLists.txt in unitTesting and add copyright header.
* Switch completely to submodule.
* Add doc as root CMake subdirectory, do not always generate the man page.
* Remove unwanted file.
* Switch documentation to submodule.
* Get rid of generated Makefile.
* Get rid of generated documentation.
* More documentation.
* Merge branch 'gh-pages' of github.com:jrl-umi3218/dynamic-graph
* Updated documentation
* Update HEAD Doxygen documentation.
  Source commit id: 241e52083f7159ba66f2866c931d51efefd5cfba
* Additional doc.
* Update HEAD Doxygen documentation.
  Source commit id: 241e52083f7159ba66f2866c931d51efefd5cfba
* Update HEAD Doxygen documentation.
  Source commit id: 241e52083f7159ba66f2866c931d51efefd5cfba
* Force generation of man pages.
* Modified documentation output dir.
* Added cmake submodule.
* Add a method that return the list of entity types registered in factory
  * include/dynamic-graph/factory.h,
  * src/dgraph/factory.cpp: method name is FactoryStorage::listEntities.
* Fix debian/watch file.
* Add Doxygen documentation for HEAD.
* github generated gh-pages branch
* Fixed bad rethrow.
* Fix import directive (default include path was wrong).
* Expanded documentation.
* Work on documentation.
* Remove trailing whitespaces.
* Merge branch 'master' of git+ssh://softs.laas.fr/git/jrl/frameworks/dynamicGraph
* More documentation stubs.
* Added documentation for most classes in dynamicgraph. Updated css.
* [doc] Files generated from templates are located in BUILD_DIR.
* Separated Tracer entity documentation.
* Updated doxygen configuration file; added documentation for entity Tracer.
* Added import functions for scripts to dynamic-graph (from Thomas Moulard)
* Added preliminary documentation on entities exposed by the package.
* Merge branch 'master' of git+ssh://softs.laas.fr/git/jrl/frameworks/dynamicGraph
* Corrected authors file.
* Release 1.1
* Fix man page list in CMakeLists.txt
* Add mailmap file.
* Add man pages to Debian pacakge.
* Add man pages.
* Rename tools dg- instead of sot-.
* Add plug-ins to package.
* Increment build number for the Debian package.
* Add missing build requirement in Debian package.
* Remove .sh extension to installed shell scripts.
  The old behavior triggers a lintian error.
* Use mktemp to create temporary file in sot-shell-plugin.sh.
* Install binaries in bin directory.
* Fix shell.sh.cmake.
  Add missing sheebang and generate file in a temporary directory.
  This is a quick hack as there is no reason that the current directory
  would be writeable for the user.
* Add newline at end of file.
* Fix Debian package (main binary pacakge was missing).
* Update project version to 1.0.0.99
* Debianize package.
* Add license header.
* Add LGPL-3 license.
* Rewrite AUTHORS file.
* Rewrite NEWS file.
* Rewrite README using Markdown syntax.
* Remove INSTALL file.
* Rename README into README.md.
* Getting rid of obsolete autogen.sh file.
* Revert "Added libraries rpath."
  It is a bad practice to put the libraries' path inside them.
  This reverts commit 088220ad54bb38a35c34c4bddd9690747092761c.
* Added libraries rpath.
* Removed automatic definition of VP_DEBUG.
* Merge branch 'master' of git+ssh://softs.laas.fr/git/jrl/frameworks/dynamicGraph
* Added more exception catch codes in interpreter. Should now link correctly with boost libraries. Small debug output changes.
* Correct the exception raised by signalTime
* Changed name of the traces file.
* Changed parts of documentation.
* Removed CMAKE_INSTALL_PREFIX in INSTALL directives.
* Re-added shell-functions plugin (exports functions defined in dynamic-graph to the shell).
* Added support for TOOLS exceptions
* Port Olivier's changes in SOT commit eacfd9544ca
* Enabled thread support (was disabled before because HAVE_LIBBOOST_THREAD was not defined)
* Added SignalCaster tests with shared libraries.
* Add message in exception when a signal type is already registered.
  * src/signal/signal-caster.cpp.
* Adding documentation to cmdPlug.
* Make exception message more explicit
  * src/signal/signal-caster.cpp: when type is not registered.
* Do not rethrow exception, since boost exceptions derive from std::exception
  * include/dynamic-graph/signal-caster.h,
  * include/dynamic-graph/signal.t.cpp.
* Make ExceptionAbstract derive from std::exception.
  * include/dynamic-graph/exception-abstract.h,
  * include/dynamic-graph/exception-factory.h,
  * include/dynamic-graph/exception-signal.h,
  * include/dynamic-graph/exception-traces.h,
  * include/dynamic-graph/signal-caster.h: This enables uses to catch
  a broader class of exceptions with messages using what() method.
* Use stringstream to pass values in set/get test.
  * unitTesting/test_signalcast.cpp.
* Add message in exception
  * src/signal/signal-caster.cpp.
* Make ExceptionAbstract::getStringMessage const
  * include/dynamic-graph/exception-abstract.h,
  * src/exception/exception-abstract.cpp.
* Added an example of caster construction for type vector (dummy cast, similar to the default one, just for the tuto).
* Made DefaultSignalCaster public (exported).
  Added boost::ublas::vector example to test_signalcast.
* Use the correct macro for import/export in functions.h
* Put shell functions firmly into dynamicgraph. Removed plugin. Corrected headers inclusion.
* Removed the EXTRA_LIBRARIES.
* Fix compilation of tests
  * src/CMakeLists.txt, link to dl,
  * unitTesting/CMakeLists.txt.
* Win32: Add some definitions to avoid some verbose warnings
* Win32: Correct API name for contiifstream
* Add the missing "int" in "int main (void)"
* The dependency in dl is unix-specific
* Win32: Correct exportation of g_caster
* Win32: Add the exportation of SignalCast
* Add a missing dependency between tracer-RT and tracer
  tracer-real-time needs tracer.lib
* Win32: typo in the API
* Win32: Correct typo in the macro name for the API
* Add uninstall target to dynamic-graph.
* Changed linking of exceptions and traces.
  The correct .so files for the plugins are now generated.
* Removed default definition of VP_DEBUG_MODE in cmake (already in header).
* Added tracer plugin to source.
* Reinforce the dependant->dependent policy.
  * include/dynamic-graph/signal-base.h,
  * include/dynamic-graph/signal-ptr.h,
  * include/dynamic-graph/signal-ptr.t.cpp,
  * include/dynamic-graph/time-dependency.h,
  * include/dynamic-graph/time-dependency.t.cpp,
  * src/dgraph/entity.cpp, dependancy -> dependency,
  * unitTesting/test_depend.cpp: makes the test_depend.cpp test works.
* Reinforce the dependant->dependent policy.
  Merge the florent branch.
  Makes the test_depend.cpp test works.
* Reintroduce file that mysteriously disappeared in commit 41e11cc3776c29c91ef
  * signal-time-dependent.h: new.
* Cosmetic changes
  * contiifstream.h: re-indent and remove trailing white spaces.
* Move class Contiifstream into namespace dynamicgraph.
  * include/dynamic-graph/contiifstream.h,
  * src/debug/contiifstream.cpp.
  Otherwise, the package does not compile in debug mode.
* First version for the interpreter-helper.
  API to access framework functions without sotInterpreter.
  Should ease the port to more complex interpreters without favoring
  the internal interpreter. The latter one is here for backward
  compatibility. It will be removed.
  This version compile but might not work yet.
  (not tested).
* Add -DVP_DEBUG in debug mode.
  * src/CMakeLists.txt.
* Corrected typo 'dependant' -> 'dependent'
* Added auto-generated script to start shell with default plugins loaded.
* Migrate code of plugin shell-functions to main library
  * src/CMakeLists.txt,
  * src/plugins/shell-functions.cpp,
  * src/plugins/shell-functions.cpp -> src/shell/functions.cpp.
* Remove trailing white spaces
  * src/CMakeLists.txt.
* Activate debug tracing by configuration option CMAKE_VERBOSITY_LEVEL
  * src/CMakeLists.txt.
* Fix documentation generation
  * CMakeLists.txt: build doc by default,
  * doc/package.dox.cmake: headers are now in include directory.
* Added UNIX guard for inclusion of dynamic load library in CMakeFiles.
* Fix boost detection
* Add detection of boost.
* Removed all using namespace directives from headers. Some minor resulting bugfixes.
* Separated unit tests and tools. Added ADD_TEST cmake command.
* Added README information. Read me! Removed the test test_factory (need to design a new one)
* Re-added shell functions and procedures.
* Removed all 'sot' references.
* Cosmetic change in CMakeLists.txt
* Now only generated documentation if GENERATE_DOC is ON.
* Added forgotten file all-signals.h
* Added forgotten test file test_signalcast.cpp
* Used signal exceptions for the exceptions thrown by SignalCast.
* Forgot to include some code in the last commit.
* Cast: Unit testing
  Added unit test of the signal casting facility. Not exporting the SignalCastRegister class anymore (inlined in the .h).
* Bug fixes following the reintroduction of casts in dynamicgraph.
* Implemented dynamic casting facility from signals to std::streams
  * New class SignalCaster
  * Re-added the get, set and trace commands to signals
* Added two unit tests.
  - signals dependency testing
  - factory testing (needs some SOT shared libraries to work though)
* Added a new test (test_pool).
* Changed CMakeLists to cope for the new headers location.
  Renamed dynamic-graph-API.h to dynamic-graph-api.h in every header file.
* Rename file according to standard
  * dynamic-graph-API.h -> dynamic-graph-api.h.
* Added forgotten header dynamic-graph-API.h
* The package name is now dynamic-graph (this still needs to be changed in the git repository). Updated source files to reflect that change. Clarified CMakeLists, explicited cpp source files for libraries.
* Added two unit tests.
  Shell (ok)
  Factory (currently needs sotGainAdaptive to compile, will change to another entity soon)
* Added documentation, renamed header paths, corrected CMakeLists accordingly.
  (dynamicGraph/ -> dynamic-graph/)
* Renamed pkg-config definition file to adhere standards.
* Removed dependencies on all other packages.
  For example, maal, was not needed.
* Removed debug hack left in the last commit.
* Type in symbol dgDEBUGFLOW resulted in not being able to load shared libraries. Fixed. Preliminary documentation changes. Re-added INSTALL file.
* dynamicGraph now compiles shared libraries.
* Removed SOT reference (<sot/*.h> --> <dynamicGraph/*.h>)
* Changed version number to 1.0.0
* Wrong file added to git repository (corrected).
* Added forgotten files.
* Configured dynamicGraph package and cmake files. Last-minute adjustment to code for SOT separation (SOT Break)
* Contributors: Aurelie Clodic, Florent Lamiraux, Francois Bleibel, Francois Keith, François Keith, Keith François, Nicolas Mansard, Olivier Stasse, Pierre Gergondet, Thomas Moulard, olivier-stasse, thomas-moulard
