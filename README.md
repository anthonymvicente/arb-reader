arb-reader
-----------
Dev: Anthony Vicente

arb-reader is a small C program used to output random sections of text from a random text file. The user will provide a file containing paths to text files they wish to be part of the pool of potential source texts. The list file is expected to be delineated by new lines for each text file. arb-reader was created to be used in conjunction with the xscreensaver Phosphor to let a user compile a list of possible sources for Phosphor to display.

INSTALL:

simply run ```make``` in the download directory to generate the binary file.

USAGE, COMMAND LINE:

    arb-reader [list file]

where [list file] is a file containing a list of text files, including their path, from which a random entry will be selected. Each file must be on a separate line.

    arb-reader -d [directory]

where [directory] is a path to a directory containing text files to be read from.

add a `g` flag to cause 'glitch' like characters to be randomly inserted into lines of the text.

USAGE, PHOSPHOR:

I've found the best way to use arb-reader with Phosphor is to add the executable to your path (ie ~/bin). Open xscreensaver-demo and select Phosphor, then go to the Advanced tab. Under the Text Manipulation section choose program, and use the following format:

    arb-reader [list file]

where [list file] is a file containing a list of text files, including their path, from which a random entry will be selected. Each file must be on a separate line.

ADVANCED:

In order to ensure that the random selection of text doesn't chose the last line to start reading from, or chose 0 lines to read, I limit the random selection by offsetting the range by a ratio determined by the number of lines in the file and the PADDING_RATIO. This ratio can be changed if desired by changing the value of PADDING_RATIO in arb-reader.c

TODO:

- [x] make PADDING_RATIO change-safe
- [x] change 'excerpt from:' text to only display file name instead of full path
- [x] implement option to point arb-reader at a directory of text files
