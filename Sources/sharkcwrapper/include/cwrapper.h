#ifndef SHARKWRAPPER_H
#define SHARKWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Shark Shark;

Shark* Shark_new();
void Shark_set_id(Shark* shark, int i);
int Shark_get_id(Shark* shark);
void Shark_delete(Shark* shark);

#ifdef __cplusplus
}
#endif
#endif


