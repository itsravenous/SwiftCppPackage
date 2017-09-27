#include "fingerprint.hpp"
#include "incl/fingerprint.h"

extern "C" {
  FingerPrint* FingerPrint_new(double *ref, double *data, int nr) {
    return new FingerPrint(ref, data, nr);
  }

  double FingerPrint_getScore(FingerPrint *fgp) {
    return fgp->getScore();
  }
}

