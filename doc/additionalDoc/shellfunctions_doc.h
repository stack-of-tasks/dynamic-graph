/**
\ingroup gshellfunctions
\page shellfunctions_doc ShellFunctions
\note Note: this documentation covers specific aspects of the in-house
scripting language currently used by the dynamic-graph script. Unless
otherwise specified, it is only relevant for programmers using this
particular language. At the time of writing, there is talk of
replacing the language with python, so check if this information is still
relevant before reading.

\section description Description
The \b ShellFunctions plugin file adds common shell functions to the
dynamic-graph shell.
\section commands Commands
The supplementary \b commands provided (you can also type 'help' at the
dynamic-graph shell command line to see them and a brief description):
\code
  LoadPlugins
  Try
  DisplayPlugins
  DisplayFactory
  Commentary
  Unplug
  ClearPlugin
  SignalTime
  SynchroSignal
  Echo
  Copy
  Freeze
  Squeeze
  EnableTrace
  SetPrompt
  Sleep
  Beep
  FlagSet
  CompletionList
\endcode

\section addi Additional information
See doxygen documentation for the class ShellFunctions

\section generates Generated plugin file
shell-functions.dll or shell-functions.so.

**/
