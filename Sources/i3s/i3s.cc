#include "incl/affine.h"
#include "incl/compare.hpp"
#include "incl/element.hpp"
#include "incl/fingerprint.hpp"
#include "i3s.h"

extern "C" {
  Element* Element_new() {
    return new Element();
  }

  void Element_delete(Element *element) {
    delete element;
  }

  FingerPrint* FingerPrint_new(const double *ref, const double *data, int nr) {
    /* it's safe to cast away const: ref and data are not actually modified */
    return new FingerPrint((double *)ref, (double *)data, nr);
  }

  void FingerPrint_delete(FingerPrint *fgp) {
    delete fgp;
  }

  double FingerPrint_getScore(FingerPrint *fgp) {
    return fgp->getScore();
  }

  double Compare_two(FingerPrint* unknown, FingerPrint* fp) {
    Pair *pairs = new Pair[100];
    int paircnt = 0;
    Compare::compareTwo(*unknown, *fp, pairs, &paircnt);
    // printf("score %f", fp.getScore());
    delete[] pairs;

    return fp->getScore();
  }
}

