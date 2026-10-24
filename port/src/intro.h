/* Default-off normal-port intro integration. */
#ifndef PORT_INTRO_H
#define PORT_INTRO_H
#include <stddef.h>
int port_intro_configure(int enabled,const char *path,char *err,size_t size);
/* 1: opening replaced and returned; 0: disabled or another program. */
int port_intro_program(const char *name);
/* Whether the native intro has its data (a pack was loaded): port_intro_program("z2o") then plays it. */
int port_intro_available(void);
#endif
