#include "../player.h"
#include <assert.h>
static struct intro_player p;
static const uint8_t *missing(void *context,unsigned plane,uint32_t at,size_t n)
{(void)context;(void)plane;(void)at;(void)n;return 0;}
int main(void)
{
    unsigned i;
    intro_player_init(&p,missing,0,0x1234);
    assert(!p.ticks && !p.world.error && gw(&p.world,IG_SEED)==0x1234);
    assert(gw(&p.world,0x18)==0x9000 && gw(&p.world,0x20)==0x90c0);
    for(i=0x28000;i<0x2a000;i++) assert(p.world.video[i]==255);
    for(i=0x2a000;i<0x2c000;i++) assert(!p.world.video[i]);
    assert(p.display.count==4 && iw32(p.display.objects+32)==0x80008000u);
    assert(intro_player_step(&p)==-1 && p.world.error && p.ticks==1);
    intro_player_init(&p,missing,0,7);sw(&p.world,IG_EXIT,2);
    assert(intro_player_step(&p)==1 && !p.ticks && !p.world.error);
    return 0;
}
