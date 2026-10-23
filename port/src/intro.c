/* Complete native intro in the ordinary port. No original-code execution. */
#include "intro.h"
#include "intro_audio.h"
#include "port.h"
#include "gpu.h"
#include "../intro/player.h"
#include "../intro/resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint8_t *pack;
static struct intro_resources resources;
static int fail(char *err,size_t size,const char *why)
{if(err && size) snprintf(err,size,"arcade intro: %s",why);return -1;}
int port_intro_configure(int enabled,const char *path,char *err,size_t size)
{
    FILE *file;uint8_t *next,digest[32];struct intro_resources parsed;long length;size_t got;int bad;
    if(!enabled) {free(pack);pack=NULL;memset(&resources,0,sizeof resources);return 0;}
    if(!path || !(file=fopen(path,"rb"))) return fail(err,size,"cannot open data pack (use --intro-data FILE)");
    if(fseek(file,0,SEEK_END) || (length=ftell(file))<48 || length>(long)INTRO_RESOURCE_LIMIT+40 || fseek(file,0,SEEK_SET)) {
        fclose(file);return fail(err,size,"invalid data pack length");
    }
    next=malloc((size_t)length);if(!next) {fclose(file);return fail(err,size,"out of memory");}
    got=fread(next,1,(size_t)length,file);bad=ferror(file);if(fclose(file)) bad=1;
    if(bad || got!=(size_t)length) {free(next);return fail(err,size,"cannot read data pack");}
    if(memcmp(next,"SFA2I003",8)) {free(next);return fail(err,size,"full-intro data pack required; re-import the legacy scene pack");}
    port_sha256(next+40,got-40,digest);
    if(memcmp(digest,next+8,32) || intro_resources_init(&parsed,next+40,got-40)) {
        free(next);return fail(err,size,"corrupt or unsupported full-intro resources");
    }
    free(pack);pack=next;resources=parsed;return 0;
}
int port_intro_program(const char *name)
{
    struct intro_player *player;uint16_t *pixels;unsigned char pad[PORT_PAD_FRAME];char error[256];int ended=0,skipped=0;
    if(!pack || !name || strcmp(name,"z2o")) return 0;
    player=malloc(sizeof *player);pixels=malloc(INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT*sizeof *pixels);
    if(!player || !pixels) port_halt(PORT_EXIT_PROGRAM,"arcade intro: no memory for native state/frame");
    if(port_intro_audio_prepare(error,sizeof error)) port_halt(PORT_EXIT_PROGRAM,"%s",error);
    if(port_gpu_intro_begin()) port_halt(PORT_EXIT_GRAPHICS,"arcade intro: needs the patched PsyZ graphics backend");
    intro_player_init(player,intro_resources_read,&resources,0x1234);port_pad_host_init();
    port_clock_frame_drawn(PORT_CLOCK_RICH_PACKETS);
    printf("arcade intro: full native sequence, PS1-disc soundtrack; Start skips\n");fflush(stdout);
    while(player->ticks<6000) {
        unsigned frame;int result=intro_player_step(player);
        if(result<0 || intro_player_render(player,intro_resources_tile,&resources,pixels,INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT))
            port_halt(PORT_EXIT_PROGRAM,"%s",player->world.error ? player->world.error : "arcade intro: native state failure");
        if(player->world.sound_command==6) port_intro_audio_start();
        /* The PS1 soundtrack already contains its title cue. This is a stream
         * adaptation, not an interpretation of the original QSound command. */
        if(gw(&player->world,IG_SCENE)==14 && gw(&player->world,IG_STEP)==10)
            port_intro_audio_gain((iw16(player->world.video+0xc000)>>12)*65536u/15u);
        frame=port_frames();port_gpu_intro_frame(pixels);
        do {port_tick();} while(port_frames()==frame);
        port_pad_host_read(pad,sizeof pad);port_input_apply(pad,port_alive());
        if(pad[0]==0 && pad[1]!=0xff && !(pad[2]&8)) {skipped=1;break;}
        if(result) {ended=1;break;}
    }
    port_intro_audio_stop();port_gpu_intro_end();free(pixels);
    if(!ended && !skipped) port_halt(PORT_EXIT_PROGRAM,"arcade intro: native update budget exceeded");
    printf("arcade intro: %s after %u updates\n",skipped ? "skipped" : "complete",player->ticks);fflush(stdout);free(player);return 1;
}
