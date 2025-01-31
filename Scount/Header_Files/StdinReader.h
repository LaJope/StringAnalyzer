#include "IReader.h"

namespace sc {

class StdinReader : public IReader {
public:
  std::string Read() override;
};

} // namespace sc
