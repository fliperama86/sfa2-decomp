/* Project-authored bounded scene slice. Assets are supplied privately. */
#ifndef PORT_INTRO_SCENE_H
#define PORT_INTRO_SCENE_H
#include "sequence.h"
#define INTRO_W 384
#define INTRO_H 224
#define INTRO_FORE_H 144
#define INTRO_SEQUENCE_BYTES 102u
#define INTRO_PAYLOAD_BYTES (102u+576u*2u+6u*384u*144u+4u*256u*256u*2u+512u*256u)
#define INTRO_PACK_BYTES (40u+INTRO_PAYLOAD_BYTES)
#define INTRO_SCENE_TICKS 192u
struct intro_scene {
    const uint8_t *data;
    struct intro_sequence foreground;
    unsigned tick;
};
/* Input must remain alive and unmodified. Invalid input leaves state unchanged. */
int intro_scene_init(struct intro_scene *scene,const uint8_t *data,size_t size);
/* Draw the current state and advance once. Returns 1, or 0 after 192 updates.
 * pixels is a nonaliasing 384*224 RGB555 buffer, never touched after completion. */
int intro_scene_frame(struct intro_scene *scene,uint16_t *pixels);
#endif
