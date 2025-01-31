#include <cstdint>

#include "IProcess.h"

namespace sc {

class StringProcess : public IProcess {
public:
  int Process(std::string, std::map<char, uint8_t> &) override;
};

} // namespace sc
