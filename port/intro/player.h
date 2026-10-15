/* Full native intro driver, independent of the host window and audio backend. */
#ifndef PORT_INTRO_PLAYER_H
#define PORT_INTRO_PLAYER_H
#include "presentation.h"
struct intro_player {
    struct intro_world world; /* First member: the presentation callback's owner. */
    struct intro_presentation display;
    unsigned ticks;
};
/* Fixed supplied-region profile, normalized initial RNG. No captured state. */
void intro_player_init(struct intro_player *,intro_table_reader,void *,uint16_t seed);
/* One native logical update, including the palette task. Returns -1 on error,
 * 0 while active, 1 when finished. Sound commands are consumed by the host. */
int intro_player_step(struct intro_player *);
int intro_player_render(struct intro_player *,intro_video_tile,void *,uint16_t *,size_t);
#endif
