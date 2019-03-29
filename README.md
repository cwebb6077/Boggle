# Boggle

This repository contains C code to implement Boggle. 

## Getting Started

You will need to get a copy of the project up and running on your local machine in order to be able to play the game. You can do this by either cloning the repo or downloading a .zip file. See Instructions section for notes on how to set up the project on a live system.  

## Instructions

1. In your terminal, type the command <code>make textfile</code>.

    - If this obtains an error, you may not have the dictionary in the directory needed.
        - To find the directory, type <code>sudo find / -name "words" -print</code>.
        - The directory should be the first one found. You may have to <code>^C</code> to stop the command.
        - Copy the directory path and type <code>grep -v "^[A-Z]" /path/to/file | grep -v "'" | grep -v "é" | grep -v "Å"  > dict.txt </code>
        - You now have a working dictionary, and can move along to the next step.
    - If you do not have the dictionary it will need to be installed, type <code>sudo apt-get install dict</code> to do this.
        - Type <code>sudo find / -name "words" -print</code>
        - And then, <code>grep -v "^[A-Z]" /path/to/file | grep -v "'" | grep -v "é" | grep -v "Å"  > dict.txt </code>
        - You should have now a working dictionary, and can move along to the next step. 
    - **If you want to add your own dictionary file, it must be all lowercase and regular a-z Latin characters (lowercase is the most important!)** 

1. Next, type <code>make</code>. This will compile the program.

1. Finally, type <code>./boggle</code> to start the game. 

## Video Tutorials

- [Player vs. Computer Mode]()
- [Player vs. Player]()

## Authors

* **Connor Webb** - *Creator of Repo* - [cwebb6077](https://github.com/cwebb6077)

