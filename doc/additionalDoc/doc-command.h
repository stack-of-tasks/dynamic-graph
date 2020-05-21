/** \page subpage_command Commands

\section subpage_command_intro Quick introduction

Commands are allowing to expand the entities.
It has no real interest in C++, and is mostly targeted to scripting languages
used to manipulate the control graph.
dynamic-graph-python provides a mecanism which is automatically binding command
to python. The main interest for a programmer is that there is no additional
lines to add. It is realized through CMake rules provided by the submodule
jrl-cmakemodules.

\section subpage_command_using Extending entities

\subsection subpage_command_setters_and_getters Setters and getters

To modify a quantity with a special method of your class, it is recommanded to
use Setter.

For instance to specify the size of the state in one entity and calls the
method setStateSize it is possible to write:

\code
    docstring = "\n"
                "    Set size of state vector\n"
                "\n";
    addCommand("resize", new command::Setter<Device, unsigned int>(
                             *this, &Device::setStateSize, docstring));
\endcode

Getting the information of the member of the class can be realized with
the following snippet:

\code
    addCommand("getTimeStep",
               command::makeDirectGetter(
                   *this, &this->timestep_,
                   command::docDirectGetter("Time step", "double")));
\endcode

\subsubsection subsubpage_command_void_multiple_args Implementing a command

Templates are provided to create commands returning no value, but with up to
4 arguments.

In order to call the following method:
\code
  void four_args(const int &, const int &, const int &, const int &) {
    test_four_args_ = true;
  }
\endcode

It is sufficient to add in the constructor:
\code
    addCommand("4_args",
               makeCommandVoid4(
                   *this, &CustomEntity::four_args,
                   docCommandVoid4("four args", "int", "int", "int", "int")));
\endcode

The arguments are limited to the types provided by the class Value.

\subsection subpage_command_void_multiple_args Commands returning a value

The templates with the prefix makeCommandReturnType are allowing to return
a type.

For instance to add a command returning a type with two arguments:
\code
  std::string two_args_ret(const int &, const int &)
  { test_two_args_ = true; return std::string("return");}
\endcode

In the constructor, the following snippet shows to create the command:
\code
    addCommand("2_args_r",
               makeCommandReturnType2(
                 *this, &CustomEntity::two_args_ret,
                 docCommandVoid2("two args", "int","int")));
\endcode

\section section_calling_a_generic_command Calling a generic command

Of course calling in C++ a command amounts to call directly the method.
However in the context of writing a wrapper for another language,
one may wants to find a command and build the call.

All the commands of an entity are store in a map. The strings storing the
commands name are the map keys.

Therefore calling the previous command can be done with the following snippet:
\code

std::map<const std::string, Command *> aCommandMap =
 this->getNewStyleCommandMap();

std::string cmd_name = "4_args";

std::map<const std::string, Command *>::iterator it_map;

it_map = aCommandMap.find(cmd_name);
if (it_map == aCommandMap.end())

int first_arg = 1;
Value aValue(first_arg);
std::vector<Value> values;

for (unsigned int i = 0; i < 4; i++)
  values.push_back(aValue);

it_map->second->setParameterValues(values);
it_map->second->execute();
it_map->second->owner();
it_map->second->getDocstring();

\endcode

when returning a value the line with the call to execute is :
\code
Value aValue =it_map->second->execute();
\endcode

*/
