#include "incl/affine.h"
#include "incl/compare.hpp"
#include "incl/element.hpp"
#include "incl/fingerprint.hpp"
#include "i3s.h"

extern "C" {
  Element* Element_new() {
    return new Element();
  }

  FingerPrint* FingerPrint_new(double *ref, double *data, int nr) {
    return new FingerPrint(ref, data, nr);
  }

  double FingerPrint_getScore(FingerPrint *fgp) {
    return fgp->getScore();
  }

  double Compare_two(FingerPrint* unknown, FingerPrint* fp) {
    Pair *pairs = new Pair[100];
    int paircnt = 0;
    Compare::compareTwo(*unknown, *fp, pairs, &paircnt);
    // printf("score %f", fp.getScore());
    return fp->getScore();
  }
}

