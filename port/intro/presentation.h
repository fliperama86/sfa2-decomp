/* Native display-list assembly. Private imported resources are data only. */
#ifndef PORT_INTRO_PRESENTATION_H
#define PORT_INTRO_PRESENTATION_H
#include "world.h"
struct intro_presentation {
    /* Four original blank entries, up to 896 generated entries, terminator. */
    uint8_t objects[(INTRO_VIDEO_OBJECTS+5u)*8u];
    size_t count;
};
/* 1: new list, 0: original busy/disabled skip, -1: unsupported/bad data.
 * The host consumes the busy flag at a display boundary, not inside here. */
void intro_present_clear(struct intro_world *,struct intro_presentation *);
int intro_present_objects(struct intro_world *,struct intro_presentation *);
/* Resolve the original display-register shadows for the native video adapter.
 * Four priority-zero blank objects are omitted, with no visual effect. */
void intro_present_video(struct intro_world *,const struct intro_presentation *,
                         struct intro_video *,intro_video_tile,void *);
#endif
