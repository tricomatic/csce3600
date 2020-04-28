# **Major 2: The Shell and System Calls** *(Group 29)*

This program provides a command-line interface for users to run commands. It accepts user input (Interactive mode) or file (batch mode).

## Section Leaders

**(James) Colton Nicholas:** Path and Redirect\
**Yafet Kubrom:** Pipe and Myhistory\
**Michael Rakestraw:** cd and Alias\
**Brandon Vojtkofsky** Exit and Signal

## Build and Run
**Compile:** 'make'\
**Run batch mode** 'make run file'\
**Run interactive mode** 'make run'\
**Clean Build Directory:** 'make clean'

## Runtime
Run the program with either 'make run' or 'make run file'.\
Interactive mode will run multiple commands separated by semicolons(;) multiple times.\
When finished, enter <quit>.\
Batch mode will run commands line by line from a file. The file of commands must be present on the current directory.\
It will exit at EOF unless there is exit command in file.\
Have fun!!
