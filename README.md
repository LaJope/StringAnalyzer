# String analyser

This repository contains 2 programs: scount and sprint. Scount act as a server, which prompts user to input some string,
counts letters in the string, connects to the client (sprint) and sends collected data to it via sockets.

## Help Scount

usage: scount [-h | --help] [-v | --verbose] [-j | --to-json]
              [-S | --start-sprint] [-o | --outfile \<json-filename\>]
              [-p | --port \<port\>] [-s | --sprint-path \<path\>] 

If using -S flag, ensure that sprint program is either in the same folder from
which you call scount or in PATH. Optionally you can specify the path to sprint
youself.

Default:
  Prompts user to type in string (<=64 characters of english alphabet),
  prints how many unique characters in the string there are,
  sends data at specified port (delfault 8060) to sprint client (if connected).
  Repeats that until user exits.

--to-json      - Redirect output to json file
--outfile      - Set output filename (.json)
--port         - Set port for scount and sprint to communicate
--start-sprint - Automatically start sprint alongside scount program
--sprint-path  - Set path to sprint program

## Help Sprint

usage: sprint [-h | --help] [-v | --verbose] [-p | --port \<port\>]

Default:
  Listens on port 8060 for incoming messages from scount. Prints recieved
  letters in ascending order depending on its frequency.

--port - Set port for scount and sprint to communicate

## Dependencies
This repository does not require you to install nothing, apart from gcc and cmake to build the project.
#### Ubuntu
```bash
sudo apt install gcc cmake
```
#### Arch linux
```bash
sudo pacman -S gcc cmake
```

## Build
To build the program in project root directory type:
```bash
cmake -S . -B build ;
cmake --build build
```
The executables will be placed in ./build/bin/
