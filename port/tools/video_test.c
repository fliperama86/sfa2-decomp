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
    for(i=0;i<30;i++) {
        assert(port_video_ot_index(0x801fc000u+i*4)==i);
        assert(port_video_ot_index(0xa01fc0e8u+i*4)==i);
        assert(port_video_ot_index(0x1fc000u+i*4)==i);
    }
    assert(port_video_ot_index(0x801987e8)==-1); /* Pointer storage is not the table. */
    assert(port_video_ot_index(0x801fc001)==-1);
    assert(port_video_ot_index(0x801fc078)==-1);
    assert(port_video_ot_index(0x801fc160)==-1);
    for(base=0;base<=240;base+=16)for(flip=0;flip<4;flip++) {
        struct port_video_corner p[4];unsigned char raw[40],old[40];
        for(i=0;i<4;i++)p[i]=(struct port_video_corner){
            (short)((((i&1)!=0)^((flip&1)!=0))*16),
            (short)((((i&2)!=0)^((flip&2)!=0))*16),
            (unsigned short)(base+(i&1?15:0)),(unsigned short)(base+(i&2?15:0))};
        assert(port_video_pixel_grid(p));
        for(i=0;i<4;i++) { assert(p[i].u==base+(i&1?16:0));assert(p[i].v==base+(i&2?16:0)); }
        packet(raw,flip,base);memcpy(old,raw,40);
        assert(!port_video_tag_tiles(0x801fc020,0x80186004,raw,0));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_tiles(0x801fc03c,0x80186004,raw,1));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_tiles(0x801fc020,0x80186005,raw,1));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_tiles(0x801fc020,0x80188d04,raw,1));assert(!memcmp(raw,old,40));
        assert(port_video_tag_tiles(0x801fc020,0x80186004,raw,1));old[23]|=32;assert(!memcmp(raw,old,40));
        packet(raw,flip,base);assert(port_video_tag_tiles(0x1fc020,0x186004,raw,1));
        packet(raw,flip,base);assert(port_video_tag_tiles(0x801fc108,0x80187684,raw,1));
        packet(raw,flip,base);memcpy(old,raw,40);
        assert(!port_video_tag_tiles(0x801987e8,0x80186004,raw,1));assert(!memcmp(raw,old,40));
        assert(!port_video_tag_tiles(0x801fc020,0x80184984,raw,1));assert(!memcmp(raw,old,40));
    }
    { unsigned char p[40],old[40];
      packet(p,0,0);p[7]=0x24;memcpy(old,p,40);assert(!port_video_tag_tiles(0x801fc020,0x80186004,p,1));assert(!memcmp(p,old,40));
      packet(p,0,0);p[3]=8;assert(!port_video_tag_tiles(0x801fc020,0x80186004,p,1));
      packet(p,0,0);p[16]=32;assert(!port_video_tag_tiles(0x801fc020,0x80186004,p,1)); }
    { struct port_video_corner p[4]={{0,0,0,0},{15,0,15,0},{0,16,0,15},{15,16,15,15}},old[4];
      memcpy(old,p,sizeof p);assert(!port_video_pixel_grid(p));assert(!memcmp(p,old,sizeof p)); }
    /* Life cap/body tiles include mixed 16/15 UV spans. Only grow the short axis. */
    for(flip=0;flip<4;flip++) {
        unsigned char p[40],old[40];struct port_video_corner c[4];
        packet(p,flip,16);p[20]=p[36]=32;memcpy(old,p,40);
        assert(port_video_tag_tiles(0x801fc038,0x80186004,p,1));
        old[23]|=32;assert(!memcmp(p,old,40));
        for(i=0;i<4;i++)c[i]=(struct port_video_corner){
            (short)((((i&1)!=0)^((flip&1)!=0))*16),
            (short)((((i&2)!=0)^((flip&2)!=0))*16),
            (unsigned short)(16+(i&1?16:0)),(unsigned short)(16+(i&2?15:0))};
        assert(port_video_pixel_grid(c));
        /* The full U side of a mirrored tile is sampled one texel on, as on the console. */
        for(i=0;i<4;i++)assert(c[i].u==16+(flip&1)+(i&1?16:0) && c[i].v==16+(i&2?16:0));
    }
    /* Both sides full and mirrored: both one on. Mirrored but 15 texels: grown only. */
    for(flip=0;flip<4;flip++)for(base=0;base<=224;base+=16) {
        struct port_video_corner c[4];
        for(i=0;i<4;i++)c[i]=(struct port_video_corner){
            (short)((((i&1)!=0)^((flip&1)!=0))*16),(short)((((i&2)!=0)^((flip&2)!=0))*16),
            (unsigned short)(base+(i&1?16:0)),(unsigned short)(base+(i&2?16:0))};
        assert(port_video_pixel_grid(c));
        for(i=0;i<4;i++)assert(c[i].u==base+(flip&1)+(i&1?16:0) && c[i].v==base+(flip&2?1:0)+(i&2?16:0));
    }
    /* Texels that fall with a falling coordinate are no mirror; rising against it they are. */
    { struct port_video_corner c[4]={{16,16,48,48},{0,16,32,48},{16,0,48,32},{0,0,32,32}};
      assert(port_video_pixel_grid(c));assert(c[0].u==48 && c[1].u==32 && c[0].v==48 && c[2].v==32); }
    { struct port_video_corner c[4]={{0,16,32,32},{16,16,48,32},{0,0,32,48},{16,0,48,48}};
      assert(port_video_pixel_grid(c));assert(c[0].u==32 && c[1].u==48 && c[0].v==33 && c[2].v==49); }
    /* A mirrored full side at the end of the page has no texel one on: the original path draws it. */
    { struct port_video_corner c[4]={{16,0,240,0},{0,0,256,0},{16,16,240,16},{0,16,256,16}},old[4];
      memcpy(old,c,sizeof c);assert(!port_video_pixel_grid(c));assert(!memcmp(old,c,sizeof c)); }
    /* All eight exact records, full/partial/empty widths, and no RAM geometry edits. */
    for(i=0;i<8;i++)for(base=0;base<=144;base++) {
        unsigned char p[16]={0},old[16];int right=(i&2)!=0;
        uint32_t address=0x801ac6acu+(unsigned)i*16,ot=i<4?0x801fc038u:0x801fc03cu;
        int x=right?216:152-base,y=18,h=5;
        p[3]=3;p[4]=240;p[5]=i<4?240:48;p[7]=0x60;
        p[8]=(unsigned char)x;p[9]=(unsigned char)(x>>8);p[10]=18;p[12]=(unsigned char)base;p[14]=5;
        memcpy(old,p,16);
        assert(!port_video_tag_life_fill(ot,address,p,0));assert(!memcmp(p,old,16));
        assert(!port_video_tag_life_fill(ot^4,address,p,1));assert(!memcmp(p,old,16));
        assert(!port_video_tag_life_fill(ot,address+1,p,1));assert(!memcmp(p,old,16));
        assert(port_video_tag_life_fill(ot,address,p,1));old[7]|=1;assert(!memcmp(p,old,16));
        assert(port_video_tag_life_fill(ot+0xe8,address,p,1));assert(!memcmp(p,old,16));
        assert(port_video_tag_life_fill(ot,address,p,1));assert(!memcmp(p,old,16));
        assert(port_video_life_fill(x,&y,base,&h));assert(y==17 && h==6);
        assert(!port_video_life_fill(x,&y,base,&h));assert(y==17 && h==6);
    }
    /* Damage starts at the live-health endpoint, not just the outside edge. */
    for(base=0;base<=144;base++)for(i=base;i<=144;i++) {
        unsigned char p[16]={0};int x=216+base;
        p[3]=3;p[7]=0x60;p[8]=(unsigned char)x;p[9]=(unsigned char)(x>>8);
        p[10]=18;p[12]=(unsigned char)(i-base);p[14]=5;
        assert(port_video_tag_life_fill(0x801fc03c,0x801ac70c,p,1));assert(p[7]==0x61);
    }
    { int y=18,h=5;assert(!port_video_life_fill(8,&y,-1,&h));assert(y==18 && h==5);
      assert(!port_video_life_fill(216,&y,INT_MAX,&h));assert(y==18 && h==5); }
    { unsigned char p[16]={0},old[16];
      p[3]=3;p[7]=0x60;p[8]=8;p[10]=18;p[12]=144;p[14]=5;
      for(i=0;i<6;i++) {
          unsigned char q[16];memcpy(q,p,16);
          switch(i) { case 0:q[7]=0x64;break;case 1:q[3]=4;break;
              case 2:q[10]=17;break;case 3:q[14]=6;break;case 4:q[12]=145;break;case 5:q[8]=7;break; }
          memcpy(old,q,16);assert(!port_video_tag_life_fill(0x801fc038,0x801ac6ac,q,1));assert(!memcmp(q,old,16));
      }
      assert(!port_video_tag_life_fill(0x801fc038,0x801ac72c,p,1));
      assert(!port_video_tag_life_fill(0x801fc020,0x801ac6ac,p,1));
      p[7]=0x62;assert(port_video_tag_life_fill(0x1fc038,0x1ac6ac,p,1));assert(p[7]==0x63);
      p[7]=0x60;assert(!port_video_tag_life_fill(0x801fc038,0x801ac6cc,p,1)); /* Wrong side. */
    }
    assert(port_video_arcade_fit(1280,960,&view));assert(view.x==0 && view.y==0 && view.w==1280 && view.h==960);
    assert(port_video_arcade_fit(1920,1080,&view));assert(view.x==240 && view.y==0 && view.w==1440 && view.h==1080);
    assert(!port_video_arcade_fit(383,288,&view));assert(!port_video_arcade_fit(384,287,&view));
    assert(port_video_arcade_fit(INT_MAX,INT_MAX,&view));assert((long long)view.w*3==(long long)view.h*4);
    assert(!port_video_canvas_allowed(384,240,0,0));assert(port_video_canvas_allowed(384,240,0,1));
    assert(!port_video_canvas_allowed(320,240,0,1));assert(!port_video_canvas_allowed(384,240,1,1));
    assert(!port_video_canvas_allowed(384,224,0,1));
    puts("video: mirrors/UV256/idempotence, super/life selection, eight fill records, flag-off packet identity, aspect/mode gates pass");
    return 0;
}
