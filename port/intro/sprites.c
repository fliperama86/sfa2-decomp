/* Readable reconstruction of the intro's object consumers at 0x129ce..0x13120.
 * Input formats have been parsed and bounded by the importer, not interpreted
 * as instructions. See sprites-contract.md for wrapping and supported paths. */
#include "sprites.h"
#include "video.h"

static int32_t signed_word(uint16_t v) { return v<32768u ? (int32_t)v : (int32_t)v-65536; }
static int32_t floor_shift(int32_t v,unsigned bits)
{
    int32_t divisor=(int32_t)(1u<<bits);
    return v>=0 ? v/divisor : -((-v+divisor-1)/divisor);
}
static void put(uint8_t *p,unsigned value) { p[0]=(uint8_t)(value>>8); p[1]=(uint8_t)value; }

static uint16_t expanded(uint16_t position,int16_t amount)
{
    int32_t centered=signed_word((uint16_t)(position+8u));
    int32_t low=signed_word((uint16_t)(centered*(int32_t)amount));
    return (uint16_t)(position+floor_shift(low,4));
}
static void transform(const struct intro_sprite_style *s,uint16_t *x,uint16_t *y)
{
    if(s->expand_x) *x=expanded(*x,s->expand_x);
    if(s->expand_y) *y=expanded(*y,s->expand_y);
    if(s->rotate) {
        int32_t cx=signed_word((uint16_t)(*x+8u)),cy=signed_word((uint16_t)(*y+8u));
        *x=(uint16_t)(floor_shift(cx*s->matrix[0],8)+floor_shift(cy*s->matrix[1],8)-8);
        *y=(uint16_t)(floor_shift(cx*s->matrix[2],8)+floor_shift(cy*s->matrix[3],8)-8);
    }
}
static unsigned palette(unsigned attributes,const struct intro_sprite_style *s,unsigned mask)
{
    unsigned color=s->palette;
    if(!s->palette_mode) return attributes;
    if(s->palette_mode&128u) color+=attributes;
    return (attributes&mask)|(color&31u);
}

int intro_sprites_append(struct intro_sprite_list *list,
                         const struct intro_sprite_art *art,
                         const struct intro_sprite_style *s)
{
    unsigned i;
    if(!list || !art || !s || art->mode>10 || (art->mode&1u) ||
       list->capacity>INTRO_VIDEO_OBJECTS || list->count>list->capacity ||
       list->cells_left>INTRO_VIDEO_OBJECTS || (list->capacity && !list->objects) ||
       (art->mode==8 && s->bypass)) return -1;
    if(art->mode==10) return 0;
    if(!art->block || !art->cells || art->cells>INTRO_VIDEO_OBJECTS ||
       !art->blocks || art->blocks>INTRO_VIDEO_OBJECTS ||
       (art->mode==0 && art->cells!=art->blocks)) return -1;
    if(art->cells>list->cells_left) return 0;
    if(art->blocks>list->capacity-list->count) return -1;
    for(i=0;i<art->blocks;i++) {
        const struct intro_sprite_block *b=&art->block[i];
        uint8_t *out=list->objects+8u*(list->count+i);
        uint16_t x=(uint16_t)b->x,y=(uint16_t)b->y;
        unsigned attr=b->attributes,flip=(s->flip<<5)&96u;
        unsigned code=b->code,width=((attr>>8)&15u)*16u+16u,height=(attr>>12)*16u+16u;
        if(!s->bypass) {
            if(art->mode==6 || art->mode==8) transform(s,&x,&y);
            if(art->mode==0) {
                attr=palette(attr|flip,s,96u);
                flip=attr&96u; width=height=16;
            } else if(art->mode==6) attr=palette(attr|flip,s,0xff60u);
            else attr=palette(attr^flip,s,art->mode==8 ? 96u : 0xff60u);
            if(flip&32u) x=(uint16_t)(0u-x-width);
            if(flip&64u) y=(uint16_t)(0u-y-height);
        }
        x=(uint16_t)(x+s->x); y=(uint16_t)(y+s->y);
        if(art->mode==6 || art->mode==8) {
            if(signed_word(x)<-576 || signed_word(x)>448) x=448;
            if(signed_word(y)<-784 || signed_word(y)>240) y=240;
        }
        if(art->mode>=4) code=(code+0xa400u)&65535u;
        put(out,(x&1023u)|s->x_flags);
        put(out+2,(y&1023u)|(art->mode>=4 ? 0x2000u : s->y_flags));
        put(out+4,code); put(out+6,attr);
    }
    list->count+=art->blocks; list->cells_left-=art->cells;
    return 1;
}
