raw.txt is each eye message preceeded by their name and a , on a separate line
(The first line is all messages in a single line)

prog.c is the code itself with comments that hopefully explain how everything works.
(There's also a prebuilt executable for Windows which should work, but even if it doesn't or you're not on Windows compiling it yourself shouldn't be too complicated)
When compiled the program by default will try to open a file called "raw.txt" and print whatever output it generates to the standard output,
you can override this by passing it arguments that start with "i:" or "o:" to override its input or output file respectively with the file name/path after the colon.

The various translated_.txt files contain the cypertext "translated" into the rotationally agnostic representation in various formats(see the definition of the RepresentationFormat enum in prog.c)
Theese also follow the same conventions as raw.txt