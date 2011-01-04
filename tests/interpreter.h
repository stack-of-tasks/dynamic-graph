// Copyright 2010 Thomas Moulard.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_TESTS_INTERPRETER_H
# define DYNAMIC_GRAPH_TESTS_INTERPRETER_H

# define RUN_COMMAND(CMD, ARGS)			\
    output_test_stream output;			\
    std::stringstream ss;			\
    std::string cmd = CMD;			\
    ss << ARGS;					\
    std::istringstream args (ss.str ());	\
    shell.cmd (cmd, args, output)

# define RUN_COMMAND_NOARG(CMD)			\
    output_test_stream output;			\
    std::stringstream ss;			\
    std::string cmd = CMD;			\
    std::istringstream args (ss.str ());	\
    shell.cmd (cmd, args, output)

#endif //! DYNAMIC_GRAPH_TESTS_INTERPRETER_H
