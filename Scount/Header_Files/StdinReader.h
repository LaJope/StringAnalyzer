#include "IReader.h"

namespace sc {

class StdinReader : public IReader {
public:
  std::string Read(bool &) override;
};

} // namespace sc
