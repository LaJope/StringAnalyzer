#pragma once

const char *HelpMessage = R""""(
usage: scount [-h | --help] [-v | --verbose] [-j | --to-json]
              [-S | --start-sprint] [-o | --outfile <json-filename>]
              [-p | --port <port>] [-s | --sprint-path <path>] 

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

)"""";
