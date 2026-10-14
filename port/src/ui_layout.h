/* Project-authored common-UI layout policy. No assets, SDK or game execution. */
#ifndef PORT_UI_LAYOUT_H
#define PORT_UI_LAYOUT_H
#include "video.h"
/* Center the original 368-wide coordinate space, never stretch it. */
static inline int port_ui_center_offset(int enabled,int canvas_active,int width)
{
    return enabled && canvas_active && width>368?(width-368)/2:0;
}
/* Exact resident producer buffers, not broad OT depth or texture-page rules.
 * Return 2 for the health fills, 1 for other common UI, 0 for unselected packets.
 * Overlay-specific art is deliberately not covered by this first increment. */
static inline int port_ui_packet_kind(uint32_t address,const uint32_t *words,unsigned len,int list)
{
    uint32_t p=address&0x1fffffffu,offset;unsigned code;
    if(list<0 || list>=30 || !len)return 0;
    code=words[0]>>24;
    if(p>=0x186004u && p<0x188d04u && (p-0x186004u)%40==0 &&
       len==9 && (code&0xfc)==0x2c)return 1;
    if(p>=0x1ac6acu && p<0x1ac86cu && (p-0x1ac6acu)%16==0 &&
       len==3 && (code&0xfc)==0x60) {
        offset=p-0x1ac6acu;
        if(offset<0x80)return list==(offset<64?14:15)?2:0;
        if(offset>=0xc0)return list==(offset>=0x180?7:8)?1:0;
    }
    if(list==14 && len==6 && code==0xe1 &&
       ((p>=0x188d6cu && p<0x188ebcu && (p-0x188d6cu)%28==0) ||
        (p>=0x18ff34u && p<0x1900f4u && (p-0x18ff34u)%28==0)))return 1;
    if(p>=0x18a4bcu && p<0x18ccbcu && (p-0x18a4bcu)%20==0 &&
       len==4 && (code&0xfc)==0x64)return 1;
    return 0;
}
static inline int port_ui_s11(unsigned value) { return (int)((value&2047)^1024)-1024; }
/* The six super-meter fill records (one per level and side, two display buffers
 * each) on their list: 1 to 3 for the level, counted from the middle of the screen
 * outward, 0 for every other packet. Address and list before any packet read. */
static inline int port_ui_super_fill_level(uint32_t address,const uint32_t *words,unsigned len,int list)
{
    uint32_t p=address&0x1fffffffu;
    if(list!=8 || len!=3 || p<0x1ac76cu || p>=0x1ac82cu || (p-0x1ac76cu)%16 ||
       ((words[0]>>24)&0xfc)!=0x60)return 0;
    return (int)((p-0x1ac76cu)/32%3)+1;
}
/* The meter frame's 16-pixel tiles carry 15 texels on the original path, which
 * shows their rows from 8 down one pixel lower and never shows the last row. The
 * game's fills (Y 213; 3, 4 and 4 rows by level) fit that picture. One-to-one
 * tiles show the artwork's interior from Y 212, and 3, 4 and 5 rows tall. Inferred
 * from the frame's tile rows and the fill records' constants. Y and height of
 * the copied command only: X, width, colour and the game's record stay. */
static inline int port_ui_super_fill(uint32_t *words,unsigned len,int level)
{
    int w,h;
    if(len!=3 || level<1 || level>3 || ((words[0]>>24)&0xfc)!=0x60)return 0;
    w=(int16_t)words[2];h=(int16_t)(words[2]>>16);
    if(port_ui_s11(words[1]>>16)!=213 || h!=(level==1?3:4) || w<0 || w>48)return 0;
    words[1]=(words[1]&0xffffu)|212u<<16;
    words[2]=(words[2]&0xffffu)|(uint32_t)(level+2)<<16;
    return 1;
}
/* One already validated GP0 command, not a whole packet. Transactional on bad
 * length/coordinate range. X only, apart from composing the selected life fix.
 * UV/color/control words are never changed, nor the command/marker bits. */
static inline int port_ui_translate(uint32_t *words,unsigned len,int dx,int fix_life)
{
    unsigned code,verts,stride,need,i,index;uint32_t next[12];
    if(!dx || !len)return 0;
    code=words[0]>>24;
    if(code>=0x20 && code<=0x3f) {
        verts=code&8?4:3;stride=1+!!(code&4)+!!(code&16);
        need=1+verts+(code&4?verts:0)+(code&16?verts-1:0);
    } else if((code>=0x40 && code<=0x43) || (code>=0x50 && code<=0x53)) {
        verts=2;stride=code&16?2:1;need=code&16?4:3;
    } else if(code>=0x60 && code<=0x7f) {
        verts=1;stride=1;need=2+!!(code&4)+(((code>>3)&3)==0);
    } else return 0; /* Draw state, transfers and no-ops are not UI geometry. */
    if(len!=need || len>12)return -1;
    for(i=0;i<len;i++)next[i]=words[i];
    if(fix_life && (code&0xfc)==0x60 && (code&PORT_VIDEO_LIFE_FILL)) {
        int x=port_ui_s11(next[1]),y=port_ui_s11(next[1]>>16);
        int w=(int16_t)next[2],h=(int16_t)(next[2]>>16);
        if(port_video_life_fill(x,&y,w,&h)) {
            next[1]=(next[1]&0xffffu)|((uint32_t)(uint16_t)y<<16);
            next[2]=(next[2]&0xffffu)|((uint32_t)(uint16_t)h<<16);
        }
    }
    for(i=0;i<verts;i++) {
        long long x;index=1+i*stride;
        x=(long long)port_ui_s11(next[index])+dx;
        if(x<-1024 || x>1023)return -1;
        next[index]=(next[index]&0xffff0000u)|(uint16_t)(int16_t)x;
    }
    for(i=0;i<len;i++)words[i]=next[i];
    return 1;
}
#endif
