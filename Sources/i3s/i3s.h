
#ifndef __I3S_H
#define __I3S_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Element Element;
typedef struct FingerPrint FingerPrint;

Element* Element_new();
FingerPrint* FingerPrint_new(double *ref, double *data, int nr);
double FingerPrint_getScore(FingerPrint *fgp);
double Compare_two(FingerPrint* unknown, FingerPrint* fp);

#ifdef __cplusplus
}
#endif
#endif
