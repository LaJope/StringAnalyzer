#pragma once

const char *HelpMessage = R""""(
usage: scount [-h | --help] [-v | --verbose] [-j | --to-json]
              [-o | --outfile <json-filename>] [-p | --port <port>]
              [-s | --sprint-path <path>]

Ensure that sprint program is either in the same folder from which you call
scount or in PATH. Optionally you can specify the path to sprint youself.

Default:
  Prompts user to type in string (<=64 characters),
  prints how many unique characters in the string there are,
  prints the set of all characters in ascending order of their count.
  Repeats that until user exits.

--to-json     - Redirect output to json file
--outfile     - Set output filename (.json)
--port        - Set port for scount and sprint to communicate
                (in case default port is occupied)
--sprint-path - Set path to sprint program

)"""";
