/* Native palette-script actor. Script entries are timed palette commands and
 * sprite choices, never original executable instructions. */
#include "actors.h"

static void palette_record(struct intro_world *w,uint8_t *a,int next_pattern)
{
    const uint8_t *p;uint32_t record,list,source;unsigned index,command,destination,selector,brightness,i;
    if(next_pattern) {
        index=(iw16(a+0x48)+1u)&63u;ip16(a+0x48,index);
        p=intro_world_table(w,INTRO_DATA,0x7a0e8u+index,1);if(!p) return;
        index=(unsigned)(*p<128 ? *p : (int)*p-256);
        p=intro_world_table(w,INTRO_DATA,0x7a128u+index,2);if(!p) return;
        record=(uint32_t)(0x7a128+is16(iw16(p)));
    } else record=iw32(a+0x40)+8u;
    p=intro_world_table(w,INTRO_DATA,record,8);if(!p) return;
    ip32(a+0x40,record);ip32(a+0x44,iw32(p));list=iw32(p+4);brightness=iw16(w->video+0xc000)&0xf000u;
    for(i=0;i<128;i++,list+=6) {
        p=intro_world_table(w,INTRO_DATA,list,6);if(!p) return;
        destination=iw16(p);selector=iw16(p+2);command=iw16(p+4);
        p=intro_world_table(w,INTRO_INLINE_DATA,0x7a0d8u+selector,4);if(!p) return;source=iw32(p)+(command&0xffe0u);
        intro_world_palette_copy(w,source,0x90c000u+destination,16,(int)brightness);
        if(command&1u) return;
    }
    w->error="intro: unterminated palette command list";
}
static unsigned palette_kind(uint8_t *a)
{unsigned value=a[0x46]&127u;a[IA_SUBTYPE]=(uint8_t)value;return value;}
static void palette_sprite(struct intro_world *w,uint8_t *a,unsigned kind)
{
    const uint8_t *p;unsigned r;
    if(kind>3) {w->error="intro: palette sprite subtype";return;}
    p=intro_world_table(w,INTRO_INLINE_DATA,0x7a028u+kind*4u,4);if(!p) return;
    ip16(a+IA_PRIORITY,iw16(p));ip16(a+IA_Y,iw16(p+2));r=intro_world_random(w);
    ip16(a+IA_X,kind==3 ? 192u : (r&255u)+64u);
    p=intro_world_table(w,INTRO_INLINE_DATA,0x7a038u+kind*4u+(r&3u),1);if(!p) return;
    intro_actor_select(w,a,0x21b992,*p);
}
void intro_actor_palette(struct intro_world *w,uint8_t *a)
{
    unsigned type;
    switch(a[IA_STATE]) {
    case 0:
        a[IA_STATE]+=2;a[IA_LAYER]=14;a[IA_BYPASS]=1;a[IA_FLIP]=a[IA_CAMERA]=a[IA_SHAKE]=0;
        ip16(a+IA_PALETTE_MODE,0);ip16(a+0x48,0);ip16(a+IA_BANK,0x2000);
        palette_record(w,a,1);if(!w->error) palette_sprite(w,a,palette_kind(a));return;
    case 2:
        ip16(a+0x44,iw16(a+0x44)-1u);
        if(!iw16(a+0x44)) palette_record(w,a,!!(a[0x46]&128u));
        if(w->error) return;
        type=a[IA_SUBTYPE];
        if(!type) {
            type=palette_kind(a);if(!type) return;palette_sprite(w,a,type);
        } else if(type<=3) {
            if(a[IA_FLAGS]&128u) palette_sprite(w,a,palette_kind(a));
            if(w->error) return;
            intro_actor_tick(w,a);
            if(type==1 && a[IA_FLAGS+1]) ip16(a+IA_X,iw16(a+IA_X)+8u);
        } else {w->error="intro: palette actor subtype";return;}
        intro_actor_collect(w,a,0);return;
    case 4:case 6:intro_world_delete(w,0,(uint16_t)(a-w->ram));return;
    default:w->error="intro: palette actor state";
    }
}
