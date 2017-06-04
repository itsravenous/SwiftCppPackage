#include "shark.h"
#include "cwrapper.h"

extern "C" {
  Shark* Shark_new() {
    return new Shark();
  }

  void Shark_set_id(Shark* shark, int i) {
    shark->set_id(i);
  }

  int Shark_get_id(Shark* shark) {
    return shark->get_id();
  }

  void Shark_delete(Shark* shark) {
    delete shark;
  }
}

