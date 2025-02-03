#include "IReader.h"

namespace sc {

class StdinReader : public IReader {
public:
  StdinReader();

  std::string Read(bool &) override;
};

} // namespace sc
