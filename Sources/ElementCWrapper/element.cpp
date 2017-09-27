#include "element.hpp"
#include "incl/element.h"

extern "C" {
  Element* Element_new() {
    return new Element();
  }
}

