#pragma once

const char *HelpMessage = R""""(
usage: sprint [-h | --help] [-v | --verbose] [-p | --port <port>]

Default:
  Listens on port 8060 for incoming messages from scount. Prints recieved
  letters in ascending order depending on its frequency.

--port         - Set port for scount and sprint to communicate

)"""";

