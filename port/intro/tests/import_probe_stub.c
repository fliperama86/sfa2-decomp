/* Stand-in native player for testing importer bookkeeping without game inputs. */
#include "../player.h"
#include <string.h>
void intro_player_init(struct intro_player *p,intro_table_reader read,void *context,uint16_t seed)
{(void)seed;memset(p,0,sizeof *p);p->world.read=read;p->world.read_context=context;}
int intro_player_step(struct intro_player *p)
{
    if(!p->world.read(p->world.read_context,0,10,5) || !p->world.read(p->world.read_context,1,20,3)) return -1;
    return ++p->ticks==2;
}
int intro_player_render(struct intro_player *p,intro_video_tile tile,void *context,uint16_t *pixels,size_t count)
{
    const uint8_t *a,*b,*c,*d;(void)p;
    a=tile(context,8,0,0);b=tile(context,8,0,1);c=tile(context,16,1,0);d=tile(context,32,2,0);
    if(!a || !b || !c || !d || a!=tile(context,8,0,0)) return -1;
    memset(pixels,0,count*sizeof *pixels);pixels[0]=a[0];pixels[1]=b[0];pixels[2]=c[0];pixels[3]=d[0];return 0;
}
