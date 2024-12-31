raw.txt is each eye message glyph-by-glyph, in separate lines
alphabet.txt lists each unique letter of the rotationally agnostic representation(their trigram form, "magic index" in decimal, and the ASCII+32 equivalent of the "magic index")
The various translated_.txt files contain the ciphertext "translated" into the rotationally agnostic representation in various formats(see the definition of the RepresentationFormat enum in prog.c) Theese also follow the same conventions as raw.txt

prog.c is the code itself with comments that hopefully explain how everything works.
The compiled program expects at least 1 argument in the format "i:filename" which tells it what file to read the ciphertext from.
This file should be in the same format as raw.txt.
Additionally a file to write whatever output the program generates can be specified with an argument of the format "o:filename", if no such argument is provided, the program will write its output to stdout.
Any arguments that don't conform to either format will be ignored.