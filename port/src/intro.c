/* First testable scene in the normal port, no original-code execution. */
#include "intro.h"
#include "port.h"
#include "gpu.h"
#include "../intro/scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *pack;
static struct intro_scene initial;
static int fail(char *err,size_t size,const char *why)
{
    if(err && size) snprintf(err,size,"arcade intro: %s",why);
    return -1;
}
int port_intro_configure(int enabled,const char *path,char *err,size_t size)
{
    FILE *file;
    uint8_t *next,digest[32];
    struct intro_scene scene;
    size_t got;
    int extra,bad;
    if(!enabled) { free(pack);pack=0;return 0; }
    file=fopen(path,"rb");
    if(!file) return fail(err,size,"cannot open data pack (use --intro-data FILE)");
    next=malloc(INTRO_PARTICLE_PACK_BYTES);
    if(!next) { fclose(file);return fail(err,size,"out of memory"); }
    got=fread(next,1,INTRO_PARTICLE_PACK_BYTES,file);extra=fgetc(file);bad=ferror(file);
    if(fclose(file)) bad=1;
    if(bad || extra!=EOF ||
       !((got==INTRO_PACK_BYTES && !memcmp(next,"SFA2I001",8)) ||
         (got==INTRO_PARTICLE_PACK_BYTES && !memcmp(next,"SFA2I002",8)))) {
        free(next);return fail(err,size,"invalid data pack header or length");
    }
    port_sha256(next+40,got-40,digest);
    if(memcmp(digest,next+8,32) || intro_scene_init(&scene,next+40,got-40)) {
        free(next);return fail(err,size,"corrupt or unsupported scene data");
    }
    free(pack);pack=next;initial=scene;
    return 0;
}
int port_intro_program(const char *name)
{
    struct intro_scene scene;
    uint16_t *pixels;
    unsigned char pad[PORT_PAD_FRAME];
    if(!pack || strcmp(name,"z2o")) return 0;
    pixels=malloc(INTRO_W*INTRO_H*sizeof *pixels);
    if(!pixels) port_halt(PORT_EXIT_PROGRAM,"arcade intro: no memory for frame");
    if(port_gpu_intro_begin()) port_halt(PORT_EXIT_GRAPHICS,"arcade intro: needs the patched PsyZ graphics backend");
    scene=initial;
    port_pad_host_init();
    port_clock_frame_drawn(PORT_CLOCK_RICH_PACKETS);
    printf("arcade intro: first scene slice, 192 updates; Start skips (%s; fades, audio and later scenes not yet implemented)\n",
           scene.embers.data ? "native embers enabled" : "legacy pack, no embers");
    fflush(stdout);
    for(;;) {
        int result=intro_scene_frame(&scene,pixels);
        if(result<0) port_halt(PORT_EXIT_PROGRAM,"arcade intro: invalid scene state");
        if(!result) break;
        unsigned frame=port_frames();
        port_gpu_intro_frame(pixels);
        do { port_tick(); } while(port_frames()==frame);
        port_pad_host_read(pad,sizeof pad);
        port_input_apply(pad,port_frames());
        if(pad[0]==0 && pad[1]!=0xff && !(pad[2]&8)) break;
    }
    port_gpu_intro_end();
    free(pixels);
    printf("arcade intro: slice returned after %u updates\n",scene.tick);
    return 1;
}
