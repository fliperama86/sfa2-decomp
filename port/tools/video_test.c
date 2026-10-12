/* Synthetic policy fixtures, no game, dependency, GPU or disc input. */
#include "video.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
static void packet(unsigned char p[40],int flip,int base)
{
    int i;memset(p,0,40);p[3]=9;p[7]=0x2c;p[22]=13;
    for(i=0;i<4;i++) {
        p[8+i*8]=(unsigned char)((((i&1)!=0)^((flip&1)!=0))*16);
        p[10+i*8]=(unsigned char)((((i&2)!=0)^((flip&2)!=0))*16);
        p[12+i*8]=(unsigned char)(base+(i&1?15:0));
        p[13+i*8]=(unsigned char)(base+(i&2?15:0));
    }
}
int main(void)
{
    int base,flip,i;struct port_video_view view;
    for(base=0;base<=240;base+=16)for(flip=0;flip<4;flip++) {
        struct port_video_corner p[4];unsigned char raw[40],old[40];
        for(i=0;i<4;i++)p[i]=(struct port_video_corner){
            (short)((((i&1)!=0)^((flip&1)!=0))*16),
            (short)((((i&2)!=0)^((flip&2)!=0))*16),
            (unsigned short)(base+(i&1?15:0)),(unsigned short)(base+(i&2?15:0))};
        assert(port_video_pixel_grid(p));
        for(i=0;i<4;i++) { assert(p[i].u==base+(i&1?16:0));assert(p[i].v==base+(i&2?16:0)); }
        assert(port_video_pixel_grid(p));
        packet(raw,flip,base);memcpy(old,raw,40);
        assert(!port_video_tag_super(0x801987e8,0x80186004,raw,0));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_super(0x80198800,0x80186004,raw,1));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_super(0x801987e8,0x80186005,raw,1));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_super(0x801987e8,0x80188d04,raw,1));assert(!memcmp(raw,old,40));
        assert(port_video_tag_super(0x801987e8,0x80186004,raw,1));old[23]|=32;assert(!memcmp(raw,old,40));
        packet(raw,flip,base);assert(port_video_tag_super(0x1987e8,0x184984,raw,1));
    }
    { unsigned char p[40],old[40];
      packet(p,0,0);p[7]=0x24;memcpy(old,p,40);assert(!port_video_tag_super(0x801987e8,0x80186004,p,1));assert(!memcmp(p,old,40));
      packet(p,0,0);p[3]=8;assert(!port_video_tag_super(0x801987e8,0x80186004,p,1));
      packet(p,0,0);p[16]=32;assert(!port_video_tag_super(0x801987e8,0x80186004,p,1)); }
    { struct port_video_corner p[4]={{0,0,0,0},{15,0,15,0},{0,16,0,15},{15,16,15,15}},old[4];
      memcpy(old,p,sizeof p);assert(!port_video_pixel_grid(p));assert(!memcmp(p,old,sizeof p)); }
    assert(port_video_arcade_fit(1280,960,&view));assert(view.x==0 && view.y==0 && view.w==1280 && view.h==960);
    assert(port_video_arcade_fit(1920,1080,&view));assert(view.x==240 && view.y==0 && view.w==1440 && view.h==1080);
    assert(!port_video_arcade_fit(383,288,&view));assert(!port_video_arcade_fit(384,287,&view));
    assert(port_video_arcade_fit(INT_MAX,INT_MAX,&view));assert((long long)view.w*3==(long long)view.h*4);
    assert(!port_video_canvas_allowed(384,240,0,0));assert(port_video_canvas_allowed(384,240,0,1));
    assert(!port_video_canvas_allowed(320,240,0,1));assert(!port_video_canvas_allowed(384,240,1,1));
    assert(!port_video_canvas_allowed(384,224,0,1));
    puts("video: mirrors/UV256/idempotence, exact HUD selection, flag-off packet identity, aspect/mode gates pass");
    return 0;
}
