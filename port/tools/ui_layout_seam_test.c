/* Generated RAM, real GPU packet-copy seam, mock SDK sink, no game/GPU. */
#define _GNU_SOURCE
#include "gpu.h"
#include "ui_layout.h"
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
static int pixel_perfect_ui,center_ui,ui_canvas_active;
typedef unsigned long OT_TYPE;
#define LINK_END 0x00ffffffu
static void stop(const char *fmt,...)
{
    va_list args;va_start(args,fmt);vfprintf(stderr,fmt,args);va_end(args);abort();
}
static void DrawOTag(OT_TYPE *words);
void port_clock_frame_drawn(unsigned packets) { (void)packets; }   /* the clock's hook that the list walker calls */
/* REAL_GPU_SEAM */
static unsigned captured_count;
static struct { unsigned len;uint32_t words[24]; } captured[16];
static void DrawOTag(OT_TYPE *node)
{
    for(;;) {
        unsigned i;assert(captured_count<16 && node[1]<=24);
        captured[captured_count].len=(unsigned)node[1];
        for(i=0;i<node[1];i++)captured[captured_count].words[i]=(uint32_t)node[2+i];
        captured_count++;
        if(node[0]==LINK_END)break;
        node=(OT_TYPE *)(uintptr_t)node[0];
    }
}
static uint32_t *at(uint32_t a) { return (uint32_t *)(uintptr_t)a; }
static void append(uint32_t base,unsigned index,uint32_t address,const uint32_t *words,unsigned len)
{
    uint32_t *p=at(address),*ot=at(base+index*4);unsigned i;
    p[0]=len<<24|(*ot&LINK_END);for(i=0;i<len;i++)p[1+i]=words[i];*ot=address&LINK_END;
}
int main(void)
{
    unsigned frame,flags,i,n;unsigned char *saved=malloc(PORT_RAM_SIZE);
    void *ram=mmap((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    assert(ram==(void *)(uintptr_t)PORT_RAM_BASE && saved);
    for(frame=0;frame<2;frame++)for(flags=0;flags<8;flags++) {
        uint32_t base=frame?0x801fc0e8u:0x801fc000u;
        int dx=(flags&2)&&(flags&4)?8:0;
        uint32_t font[]={0x64808080,40u<<16|40,0x78070000,8u<<16|8};
        uint32_t world[]={0x600000ff,100u<<16|300,8u<<16|8};
        uint32_t life[]={0x61ffff00,18u<<16|8,5u<<16|144};
        uint32_t super1[]={0x6049c9f3,213u<<16|150,3u<<16|16};
        /* P2's left life cap as the tile-map drawer writes it, marked for the pixel grid. */
        uint32_t cap[]={0x2c808080,7u<<16|224,0x7a470000u|48u<<8|192,7u<<16|208,0x002d0000u|PORT_VIDEO_PIXEL_GRID<<16|48u<<8|207,
            23u<<16|224,63u<<8|192,23u<<16|208,63u<<8|207};
        uint32_t super3[]={0x60f0f020,213u<<16|294,4u<<16|48};
        uint32_t hud[]={0x2c808080,60u<<16|20,0x78070000,60u<<16|36,0x000d000f,
            76u<<16|20,0x00000f00,76u<<16|36,0x00000f0f};
        uint32_t strip[]={0xe100001f,0xe2000000,0x03000000,0x7c808080,104u<<16|120,0x78070000};
        memset(ram,0,PORT_RAM_SIZE);
        for(i=0;i<30;i++)*at(base+i*4)=i?(base+(i-1)*4)&LINK_END:LINK_END;
        append(base,14,0x80186004+40*3+frame*0x1680,cap,9);
        append(base,13,0x80186004+40*5+frame*0x1680,cap,9); /* the same cap on another list stays whole */
        append(base,14,0x80100200,world,3);
        append(base,14,0x8018a4bc+frame*0x1400,font,4);
        append(base,14,0x801ac6ac,life,3);
        append(base,14,0x80188d6c+frame*0x38,strip,6);
        append(base,17,0x80186004+frame*0x1680,hud,9);
        append(base,8,0x801ac76c+frame*16,super1,3);
        append(base,8,0x801ac80c+frame*16,super3,3);
        append(base,8,0x801ac82c,super3,3); /* The record after the six: same words, not a meter fill. */
        append(base,9,0x801ac78c,super3,3); /* A meter record on another list is not one either. */
        memcpy(saved,ram,PORT_RAM_SIZE);
        pixel_perfect_ui=!!(flags&1);ui_canvas_active=!!(flags&2);center_ui=!!(flags&4);
        captured_count=0;send_list(at(base+29*4));assert(captured_count==11);
        assert(!memcmp(saved,ram,PORT_RAM_SIZE));
        assert(captured[0].len==9);assert(captured[0].words[1]==(60u<<16|(unsigned)(20+dx)));
        for(i=0;i<9;i++)if(i!=1 && i!=3 && i!=5 && i!=7)assert(captured[0].words[i]==hud[i]);
        assert(captured[1].len==5);assert(captured[1].words[0]==strip[0] && captured[1].words[1]==strip[1]);
        assert(captured[1].words[2]==strip[3] && captured[1].words[3]==(104u<<16|(unsigned)(120+dx)));
        n=dx&&pixel_perfect_ui?17:18;
        assert(captured[2].words[0]==life[0] && captured[2].words[1]==(n<<16|(unsigned)(8+dx)));
        assert(captured[2].words[2]==((dx&&pixel_perfect_ui?6u:5u)<<16|144));
        assert(captured[3].words[1]==(40u<<16|(unsigned)(40+dx)));
        assert(captured[3].words[0]==font[0] && captured[3].words[2]==font[2] && captured[3].words[3]==font[3]);
        assert(captured[4].len==3 && !memcmp(captured[4].words,world,sizeof world));
        if(pixel_perfect_ui) {
            static const unsigned xs[8]={7u<<16|216,7u<<16|208,23u<<16|216,23u<<16|208,7u<<16|224,7u<<16|216,23u<<16|224,23u<<16|216};
            assert(captured[5].len==18);
            for(i=0;i<2;i++) { const uint32_t *q=captured[5].words+9*i;
                assert(q[0]==cap[0] && q[2]>>16==0x7a47 && q[4]>>16==0x002d);
                assert(q[1]==xs[4*i]+(unsigned)dx && q[3]==xs[4*i+1]+(unsigned)dx && q[5]==xs[4*i+2]+(unsigned)dx && q[7]==xs[4*i+3]+(unsigned)dx);
                assert((q[2]&0xffff)==(48u<<8|(i?200u:199u)) && (q[4]&0xffff)==(48u<<8|(i?192u:207u)));
                assert(q[6]==(64u<<8|(i?200u:199u)) && q[8]==(64u<<8|(i?192u:207u))); }
        } else {
            assert(captured[5].len==9);
            for(i=0;i<9;i++)assert(captured[5].words[i]==((i&1)?cap[i]+(unsigned)dx:cap[i]));
        }
        assert(captured[6].len==9);
        for(i=0;i<9;i++)assert(captured[6].words[i]==((i&1)?cap[i]+(unsigned)dx:cap[i]));
        for(i=7;i<9;i++)assert(captured[i].len==3 && !memcmp(captured[i].words,super3,sizeof super3));
        n=pixel_perfect_ui?212:213;
        assert(captured[9].len==3 && captured[9].words[0]==super3[0]);
        assert(captured[9].words[1]==(n<<16|(unsigned)(294+dx)) && captured[9].words[2]==((pixel_perfect_ui?5u:4u)<<16|48));
        assert(captured[10].len==3 && captured[10].words[0]==super1[0]);
        assert(captured[10].words[1]==(n<<16|(unsigned)(150+dx)) && captured[10].words[2]==(3u<<16|16));
        captured_count=0;
        { uint32_t single[]={0x03000000,0x600000ff,20u<<16|20,4u<<16|4};
          send_single(single);assert(captured_count==1 && captured[0].words[1]==single[2]); }
    }
    free(saved);assert(!munmap(ram,PORT_RAM_SIZE));
    puts("UI seam: real walker/copy, both frame tables, eight flag states, compound sprite, meter fills, P2 life cap, RAM/world/single-packet identity pass");
    return 0;
}
