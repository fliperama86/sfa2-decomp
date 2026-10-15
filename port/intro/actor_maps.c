/* Native tile-plane animation actors. Saved routine identifiers are metadata,
 * never function pointers or executable dispatch. Camera selects native code. */
#include "actors.h"
#include <string.h>
static void paint(struct intro_world *w,uint8_t *a,unsigned layer,uint32_t record,unsigned x,unsigned y)
{
    const uint8_t *p=intro_world_table(w,INTRO_DATA,record,16);uint32_t destination;if(!p) return;
    destination=intro_map_position(w,layer,x+iw16(p+12),y+iw16(p+14));
    intro_map_write(w,layer,iw32(p+4),destination,a[IA_VISIBLE]);
}
static void background(struct intro_world *w,uint8_t *a)
{
    static const uint32_t tables[]={0x23efae,0x23f110,0x243910};
    static const uint32_t update_ids[]={0x18d68,0x18de6,0x18e64};
    unsigned camera=a[IA_CAMERA],layer;const uint8_t *p;
    if(a[IA_STATE]==4 || a[IA_STATE]==6) {intro_world_delete(w,0,(uint16_t)(a-w->ram));return;}
    if(!camera && !a[IA_STATE]) {a[IA_STATE]+=2;a[IA_VISIBLE]=255;return;}
    if(camera!=4 && camera!=8 && camera!=12) {w->error="intro: background camera";return;}
    layer=camera/4-1;
    if(a[IA_STATE]==0) {
        a[IA_STATE]+=2;a[IA_VISIBLE]=255;ip32(a+IA_VX,update_ids[layer]);
        if(intro_actor_select(w,a,tables[layer],a[IA_SUBTYPE])) return;
    } else if(a[IA_STATE]==2) {
        if(*ig(w,0x125) || *ig(w,0x134)) return;
        if(iw32(a+IA_VX)!=update_ids[layer]) {w->error="intro: background selector metadata";return;}
        if(iw16(a+IA_TICKS)!=1) {ip16(a+IA_TICKS,iw16(a+IA_TICKS)-1u);return;}
        if(intro_actor_tick(w,a)) return;
    } else {w->error="intro: background state";return;}
    p=intro_world_table(w,INTRO_DATA,iw32(a+IA_SEQUENCE),16);if(!p) return;memcpy(a+IA_SCALE_X,p+12,4);
    paint(w,a,layer,iw32(a+IA_SEQUENCE),iw16(a+IA_X),iw16(a+IA_Y));
}
static void tiled_panorama(struct intro_world *w,uint8_t *a)
{
    const uint8_t *p;unsigned copy,extra;uint32_t rec;
    if(a[IA_STATE]==4 || a[IA_STATE]==6) {intro_world_delete(w,0,(uint16_t)(a-w->ram));return;}
    if(a[IA_STATE]==0) {
        a[IA_STATE]+=2;a[IA_VISIBLE]=255;ip16(a+IA_X,64);ip16(a+IA_Y,160);ip32(a+IA_VY,0);
        p=intro_world_table(w,INTRO_DATA,0x243918,2);if(!p) return;ip32(a+0x50,(uint32_t)(0x243910+is16(iw16(p))));
        if(intro_actor_select(w,a,0x243910,3)) return;
    } else if(a[IA_STATE]==2) {
        if(iw16(a+IA_TICKS)!=1) {ip16(a+IA_TICKS,iw16(a+IA_TICKS)-1u);return;}
        if(intro_actor_tick(w,a)) return;
    } else {w->error="intro: tiled panorama state";return;}
    rec=iw32(a+IA_SEQUENCE);p=intro_world_table(w,INTRO_DATA,rec,16);if(!p) return;memcpy(a+IA_SCALE_X,p+12,4);
    for(copy=0;copy<4;copy++) {ip16(a+IA_VX,copy*512u);paint(w,a,2,rec,iw16(a+IA_X)+copy*512u,iw16(a+IA_Y)+iw16(a+IA_VY));}
    extra=a[IA_FLAGS+1];if(!extra) return;
    rec=iw32(a+0x50)+extra-1u;ip32(a+0x60,rec);
    for(copy=0;copy<4;copy++) {ip16(a+IA_VX,copy*512u);paint(w,a,2,rec,iw16(a+IA_X)+copy*512u,iw16(a+IA_Y)+iw16(a+IA_VY));}
}
void intro_actor_map(struct intro_world *w,uint8_t *a)
{
    if(a[IA_KIND]==1) background(w,a);
    else if(a[IA_KIND]==0x8d) tiled_panorama(w,a);
    else w->error="intro: tile-plane actor kind";
}
