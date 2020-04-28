# **Major 2: The Shell and System Calls *(Group 29)***

This program provides a command-line interface for users to run commands.\
It accepts user input (Interactive mode) or file (batch mode).

## #Section-Leaders

**(James) Colton Nicholas:** Path and Redirect\
**Yafet Kubrom:** Pipe and MyHistory\
**Michael Rakestraw:** cd and Alias\
**Brandon Vojtkofsky:** Exit and Signal

## #Build-and-Run
**First Time Before Run:** **`chmod +x newshell.sh`**\
**Run Batch Mode:** `./newshell.sh <batch_file>`\
**Run Interactive Mode:** `./newshell.sh`\
**Clean Build Directory:** `make clean`

## #Runtime
Run the program however you wish (see **#Build-and-Run**).\
Interactive mode will run multiple commands separated by semicolons (`;`) multiple times.\
When finished (in interactive mode), enter **`exit`**.\
Batch mode will run commands line by line from a file.\
The file of commands must be present within the current directory.\
It will exit at EOF unless there is an exit command in file.\
Have fun!!

## #Custom-Commands
Many of the commands you will be using are simply supported\
using execvp() system calls, but some of our commands are\
custom. Here are a list of those:
 - `changeprompt [prompt]` - used to change the shell prompt
 - `alias alias_name='command'` - add custom command aliases
 - `path [[+|-] [pathname]]` - check path or add/remove
 - `myhistory [-e <command_number> | -c]` - command history
 - `exit` - exit newshell

## #Issues
If you ever find yourself stuck in the program somehow,\
you may not be able to exit with traditional exit signals\
from stdin. Open yourself another terminal, find the `PID` of\
the `newshell` process using `ps -u` and use **`kill -9 <PID>`**.

Sometimes the program will hang when a line of a batch script\
has lots of semicolon-separated commands. A fix for this is unknown.\
Using `Ctrl+C` should get you out of this situation, should you\
find yourself there.

If one of our custom commands (for ex: changepath, myhistory, etc...)\
is inside of user input (for ex: `prompt> xxmyhistoryss`), the associated\
function will still be called. Expected behavior would be to get a "command\
unknown" message. This is currently unfixed.

User is not given a new prompt when a signal is handled.\
However, you may still enter commands on this single\
unprompted line.

## #Design-and-Organization
#### User Input
User input was a little bit tricky. Essentially, what happens first is a check if\
the user would like to exit. If not, the input is then passed on.\
Input is then tokenized by semicolons (whether there are any or not)\
and stored into a string array of commands. For example, `ls -al ; cd`\
would be stored as `ls -al` and `cd`. Then, each command is tokenized\
by spaces, and replaced back into their string separated by single spaces.\
This effectively removes any additional whitespace, leading, in the middle\
of, or following the command.

Now, the array of commands is checked to see if it contains one of the\
custom commands. If it does, the command is passed on to the function\
and if not, then the array is passed onto a system call, where it will be run\
inside of a child process.

Finally, input is reset and the programs loops once more.

#### Path
Possible Usage:\
`path` - this will print the current value of PATH\
`path + ./bin`  - this will add ./bin from your PATH\
`path - ./bin` - this will remove ./bin from your PATH

When user input contains "path", the associated path() function\
will then be called. This fucntion, below main(), will process\
user input, decide what the user is trying to do, and continue\
to complete their request.

#### Redirect
First, everything left of the angle bracket (`<` or `>`) is saved into an array\
and then the filename to the right is stored into a temporary variable.

Taking advantage of dup2() function calls to redirect stdin and stdout and\
execvp() Linux system calls, we are able to re-create Bash's redirection\
functionality.

All that is dont at this point is determining if we are modifying stdin or\
stdout (eseentially, if `<` or `>` is specified), and then the appropriate\
dup2() calls can be made.

#### Pipe
ex. cmd1 | cmd2\
ex. cmd1 | cmd2 | cmd3\
This command connects output of cmd1 to input of cmd2\
and output of cmd2 to input of cmd3.\
It uses the pipe() system call (unnamed) to implement this command.

#### MyHistory
*myhistory* lists the history of previous commands run in your shell.\
myhistory supports two flags.\
*myhistory -c* clears your history and \
*myhistory -e <myhistory_number>* executes the specific command\
associated with that number. \
myhistory keeps history of 20 commands at a time. \
The 21st command will overwrite the 1st command.

#### cd
cd command accepts cd with 1 optional argument \
For cd with a given destination I just copied all but the 'cd '\
of the given command and passed it to chdir() \
For cd without a destination, I passed 'getenv("HOME")' to chdir(),\
 which gets the home directory of the user

#### Alias
alias accepts either alias, alias -c,\
alias -r alias_name, alias alias_name='command' \
for alias as a whole, I created 2 arrays, 1 for storing alias\
names & 1 for storing the corresponding commands that were aliased,\
and set all entries in both to "" \
for alias, I looped through the alias_names array and output all aliased\
commands, and if the array had no aliased command\
I output "No commands have been aliased" \
for alias -c, I looped through the arrays and set all values back to "" \
for alias -r alias_name, I looped throught the alias_names array,\
and if there was a match for the given name, I set that value & its\
corresponding command to "" \
for alias alias_name='command', I parse through the string\
and store the alias_name in the alias_names array & the command\
in the alias_com array

#### Exit
[Description goes here.]

#### Signal
[Description goes here.]

#### ChangePrompt
User command input is taken in and checked if the command was executed\
properly. If only `changeprompt` was sent, then a usage statment will be\
sent, but if used correctly (for ex: `changeprompt coolprompt`), then the\
global variable containing prompt will be properly updated to reflect the\
user's request.
