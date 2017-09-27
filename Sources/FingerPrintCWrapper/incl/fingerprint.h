#ifndef FINGERPRINTWRAPPER_H
#define FINGERPRINTWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FingerPrint FingerPrint;

FingerPrint* FingerPrint_new(double *ref, double *data, int nr);
double FingerPrint_getScore(FingerPrint *fgp);

#ifdef __cplusplus
}
#endif
#endif


