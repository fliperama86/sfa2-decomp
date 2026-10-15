#ifndef PORT_INTRO_ACTORS_H
#define PORT_INTRO_ACTORS_H
#include "world.h"
int intro_actor_select(struct intro_world *,uint8_t *,uint32_t table,unsigned index);
int intro_actor_tick(struct intro_world *,uint8_t *);
void intro_actor_collect(struct intro_world *,uint8_t *,int cull);
void intro_actor_strips(struct intro_world *,uint8_t *);
void intro_actor_portrait(struct intro_world *,uint8_t *);
int intro_actor_select24(struct intro_world *,uint8_t *,uint32_t,unsigned);
void intro_actor_tick24(struct intro_world *,uint8_t *);
void intro_actor_montage(struct intro_world *,uint8_t *);
void intro_actor_secondary(struct intro_world *,uint8_t *);
void intro_actor_fighter(struct intro_world *,uint8_t *);
int intro_actor_step_native(struct intro_world *,unsigned pool,uint16_t actor);
#endif
