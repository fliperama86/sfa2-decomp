#include "player.h"
#include "actors.h"
#include <string.h>
static int present(struct intro_world *world)
{
    struct intro_player *p=(struct intro_player *)world;
    *ig(world,0x95)=0; /* Consume the previous publication at the display boundary. */
    return intro_present_objects(world,&p->display)<0 ? -1 : 0;
}
void intro_player_init(struct intro_player *p,intro_table_reader read,void *context,uint16_t seed)
{
    static const uint16_t registers[][2]={
        {0x18,0x9000},{0x1a,0x9040},{0x1c,0x9080},{0x1e,0x90e0},
        {0x20,0x90c0},{0x30,15},{0x32,0x1b02},{0x34,63},
        {0x36,0x7000},{0x38,0x807d},{0x3a,0x2461},{0x3c,64},{0x3e,16}
    };
    unsigned i;memset(p,0,sizeof *p);
    p->world.read=read;p->world.read_context=context;p->world.actor_step=intro_actor_step_native;
    p->world.present=present;p->world.title=intro_title_write;
    for(i=0;i<sizeof registers/sizeof *registers;i++) sw(&p->world,registers[i][0],registers[i][1]);
    sw(&p->world,IG_SEED,seed);
    /* Native equivalent of the white/black flash palette initialization.
     * Ordinary palette pages are loaded by the opening's first state. */
    memset(p->world.video+0x28000,255,0x2000);
    p->world.clear_objects=1;intro_present_clear(&p->world,&p->display);
}
int intro_player_step(struct intro_player *p)
{
    int result;
    if(!p || p->world.error) return -1;
    if(gw(&p->world,IG_EXIT)) return 1;
    p->world.sound_command=0;
    result=intro_director_step(&p->world);
    intro_present_clear(&p->world,&p->display);
    if(!result) {intro_fade_tick(&p->world);intro_world_random(&p->world);}
    p->ticks++;
    return p->world.error ? -1 : result;
}
int intro_player_render(struct intro_player *p,intro_video_tile tile,void *context,uint16_t *pixels,size_t count)
{
    struct intro_video video;
    if(!p || p->world.error) return -1;
    intro_present_video(&p->world,&p->display,&video,tile,context);
    if(intro_video_render(&video,pixels,count)) {p->world.error="intro: missing or invalid graphics resource";return -1;}
    return 0;
}
