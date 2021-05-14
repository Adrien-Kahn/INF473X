### OverflowAttack

Syntax: `./OverflowAttack "[command to run on the attacked server]"`

The program will send the input command padded with 116 'a' before so that the command will overwrite the `command` string in the app. It is then executed by the app in the call `system(command)`.

In order to write in the html file, one can for example use the command `sed -i '/condition/a <p>Hello world</p>' index.html` to append the line <p>Hello world</p> at the end of any line that contains the string "condition".

### itest

The purpose of this script is to try to figure out the order in which strings are located in memory and how it relates to the order of declaration.

Based on some experimentation, it seems that without any compilation option, the longest strings are located in memory after the shortest ones, regardless of declaration order.

If we try to reproduce the code in the app, we indeed find out that `command` is located before `greeting_text` which would not allow for the sort of attack we are doing.

This suggest that some special option was indeed used to compile the app and change the order of memory allocation.
