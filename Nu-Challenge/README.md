Thanks to Clément Gachod for having shared his solution (injection-Clement.py) with me so I could debug mine.

## attack

The program sends a string that consists of:
- Shellcode: instructions the program will execute after the execution pointer has been redirected there.
- Padding (A NOP-slide that nobody slides on): characters to fill the string, `\x90` (NOP) in this case, but could be anything.
- Return instruction pointer: a return instruction pointer to overwrite the real one so that at the end of the function the program starts executing the shellcode.
- An end of line character so that `fgets` will return.

The shellcode we use here opens a shell that we can interact with since `stdin` and `stdout` were redirected to the TCP socket.

From here, all we have to do is send shell commands and receive answers.

Please note that for the connection to remain uninterrupted, all commands must return something. For example one can add `ls` at the end of a command.

## selfAttack

`selfAttack` was an attempt at reproducing the attack locally, but even with `-z execstack` and `-fno-stack-protector`, gdb seems to indicate that the system figures out the return instruction pointer was overwritten and returns a segfault.
