#include "i3s.h"
#include <stdio.h>

int main(int argc, char** argv) {
  double refs[] = { 0.0, 0.0, 100.0, 100.0, 0.0, 100.0 };
  double spots[] = {
    10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,
    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
    30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0, 30.0,
    40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0
  };
  double refs2[] = { 0.0, 0.0, 100.0, 100.0, 0.0, 100.0 };
  double spots2[] = {
    15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0, 15.0,
    20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0, 20.0,
    37.0, 37.0, 37.0, 37.0, 37.0, 37.0, 37.0, 37.0,
    40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0
  };
  int count = 4;
  struct FingerPrint* fgp = FingerPrint_new(&refs, &spots, count);
  struct FingerPrint* fgp2 = FingerPrint_new(&refs2, &spots2, count);
  Compare_two(fgp2, fgp);
  double score = FingerPrint_getScore(fgp);
  printf("Score: %f\n", score);
}
