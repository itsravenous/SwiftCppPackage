
#ifndef __I3S_H
#define __I3S_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Element Element;
typedef struct FingerPrint FingerPrint;

Element* Element_new();
void Element_delete(Element *element);
FingerPrint* FingerPrint_new(const double *ref, const double *data, int nr);
void FingerPrint_delete(FingerPrint* fgp);
double FingerPrint_getScore(FingerPrint *fgp);
double Compare_two(FingerPrint* unknown, FingerPrint* fp);

#ifdef __cplusplus
}
#endif
#endif
