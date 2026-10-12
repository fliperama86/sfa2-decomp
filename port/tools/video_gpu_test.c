/* Generated port-backend native-raster fixture, no game/disc/assets/audio, exits immediately. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <psyz.h>
#include <libgpu.h>
#include "video.h"
static void quad(int x,int y,int base,int flip,int depth,int tagged,int u_span)
{
    POLY_FT4 p;
    setPolyFT4(&p); setRGB0(&p,128,128,128); setShadeTex(&p,1);
    setXY4(&p,x+(flip&1?16:0),y+(flip&2?16:0),
               x+(flip&1?0:16),y+(flip&2?16:0),
               x+(flip&1?16:0),y+(flip&2?0:16),
               x+(flip&1?0:16),y+(flip&2?0:16));
    setUV4(&p,base,base,base+u_span,base,base,base+15,base+u_span,base+15);
    p.tpage=getTPage(depth,0,640,depth==0?256:0)|(tagged?PORT_VIDEO_PIXEL_GRID:0);
    p.clut=getClut(0,480); DrawPrim(&p);
}
static void life_fill(int x,int w,int yellow,int tagged)
{
    TILE p;setTile(&p);setXY0(&p,x,18);setWH(&p,w,5);
    setRGB0(&p,255,yellow?255:0,0);
    if(tagged)p.code|=PORT_VIDEO_LIFE_FILL;
    DrawPrim(&p);
}
static void pixel(const unsigned char *pixels,int w,int x,int y,int red,int green,int blue)
{
    const unsigned char *p=pixels+(y*w+x)*3;
    assert((red?p[0]>=230:p[0]==0) && (green?p[1]>=230:p[1]==0) && (blue?p[2]>=230:p[2]==0));
}
int main(int argc,char **argv)
{
    DRAWENV draw; DISPENV display; TILE bg; RECT r;
    unsigned short direct[256], packed[64], palette[16]={0,0x7fff};
    unsigned char *pixels; int w,h,depth,base,flip,x,y,bad=0;
    ResetGraph(0);
    SetDefDrawEnv(&draw,0,8,384,224); SetDefDispEnv(&display,0,0,384,240);
    PutDrawEnv(&draw); PutDispEnv(&display);
    assert(Psyz_Sfa2ExperimentalVideoAvailable());
    pixels=Psyz_VideoAllocCapturedFrame(&w,&h); assert(pixels && w==368 && h==240); free(pixels);
    Psyz_Sfa2SetPixelPerfectUi(1);
    Psyz_Sfa2SetArcadeCanvas(1); SetDispMask(1);
    r=(RECT){0,480,16,1}; LoadImage(&r,(u_long*)palette);
    for(y=0;y<16;y++) for(x=0;x<16;x++) direct[y*16+x]=(x+y)&1?0:0x7fff;
    for(y=0;y<16;y++) for(x=0;x<4;x++) packed[y*4+x]=y&1?0x1010:0x0101;
    for(base=0;base<=240;base+=240) {
        r=(RECT){640+base,base,16,16}; LoadImage(&r,(u_long*)direct);
        r=(RECT){640+base/4,256+base,4,16}; LoadImage(&r,(u_long*)packed);
    }
    setTile(&bg); setXY0(&bg,0,0); setWH(&bg,384,224); setRGB0(&bg,0,255,0); DrawPrim(&bg);
    for(depth=0;depth<=2;depth+=2) for(base=0;base<=240;base+=240) for(flip=0;flip<4;flip++)
        quad(16+flip*24,16+(depth/2*2+base/240)*32,base,flip,depth,1,15);
    quad(16,160,0,0,0,0,15); quad(64,160,0,1,0,0,15);
    for(flip=0;flip<4;flip++)quad(16+flip*24,192,0,flip,0,1,16);
    DrawSync(0); Psyz_VideoVSync(0); pixels=Psyz_VideoAllocCapturedFrame(&w,&h);
    assert(pixels && w==384 && h==224);
    for(depth=0;depth<=2;depth+=2) for(base=0;base<=240;base+=240) for(flip=0;flip<4;flip++)
        for(y=0;y<16;y++) for(x=0;x<16;x++) {
            unsigned char *p=pixels+((16+(depth/2*2+base/240)*32+y)*w+16+flip*24+x)*3;
            int sx=flip&1?15-x:x, sy=flip&2?15-y:y;
            int white=p[0]>=230 && p[1]>=230 && p[2]>=230;
            int green=p[0]==0 && p[1]>=230 && p[2]==0;
            if(((sx+sy)&1)?!green:!white) {
                fprintf(stderr,"checker mismatch depth%d base%d flip%d at%d,%d: %d,%d,%d\n",depth,base,flip,x,y,p[0],p[1],p[2]);
                free(pixels); return 1;
            }
        }
    for(flip=0;flip<4;flip++)for(y=0;y<16;y++)for(x=0;x<16;x++) {
        int white=!(((flip&1?15-x:x)+(flip&2?15-y:y))&1);
        pixel(pixels,w,16+flip*24+x,192+y,white,1,white);
    }
    for(flip=0;flip<2;flip++) for(y=0;y<16;y++) for(x=0;x<16;x++) {
        unsigned char *p=pixels+((160+y)*w+(flip?64:16)+x)*3;
        int white=p[0]>=230 && p[1]>=230 && p[2]>=230;
        if(white!=!(((flip?15-x:x)+y)&1)) bad++;
    }
    assert(bad>0);
    /* Even a marked primitive keeps the old sampling when its feature is off. */
    Psyz_Sfa2SetPixelPerfectUi(0);
    quad(128,160,0,0,0,1,15);
    DrawSync(0);Psyz_VideoVSync(0);
    free(pixels);pixels=Psyz_VideoAllocCapturedFrame(&w,&h);assert(pixels);
    { int offbad=0;
      for(y=0;y<16;y++)for(x=0;x<16;x++) {
          unsigned char *p=pixels+((160+y)*w+128+x)*3;
          unsigned char *legacy=pixels+((160+y)*w+16+x)*3;
          assert(p[0]==legacy[0] && p[1]==legacy[1] && p[2]==legacy[2]);
          int white=p[0]>=230 && p[1]>=230 && p[2]>=230;
          if(white!=!((x+y)&1))offbad++;
      }
      assert(offbad>0);printf("GPU flag-off marked control: %d legacy-different pixels retained\n",offbad);
    }
    if(argc>1) { FILE *f=fopen(argv[1],"wb"); assert(f); fprintf(f,"P6\n%d %d\n255\n",w,h);
        assert(fwrite(pixels,3,(size_t)w*h,f)==(size_t)w*h); assert(!fclose(f)); }
    free(pixels);
    /* Full left, partial right with delayed damage, plus a zero-width fill. */
    Psyz_Sfa2SetPixelPerfectUi(1);DrawPrim(&bg);
    life_fill(8,144,1,1);life_fill(216,72,1,1);life_fill(288,72,0,1);life_fill(152,0,0,1);
    DrawSync(0);Psyz_VideoVSync(0);pixels=Psyz_VideoAllocCapturedFrame(&w,&h);assert(pixels);
    for(y=16;y<=23;y++)for(x=8;x<360;x++) {
        int inside=y>=17 && y<=22;
        int left=x<152, right=x>=216;
        pixel(pixels,w,x,y,inside&&(left||right),!(inside&&x>=288),0);
    }
    free(pixels);
    /* Unmarked enabled and marked disabled TILEs retain their five-row geometry. */
    DrawPrim(&bg);life_fill(8,144,1,0);life_fill(216,144,1,0);
    DrawSync(0);Psyz_VideoVSync(0);pixels=Psyz_VideoAllocCapturedFrame(&w,&h);assert(pixels);
    Psyz_Sfa2SetPixelPerfectUi(0);DrawPrim(&bg);life_fill(8,144,1,1);life_fill(216,144,1,1);
    DrawSync(0);Psyz_VideoVSync(0);
    { unsigned char *off=Psyz_VideoAllocCapturedFrame(&w,&h);assert(off);
      for(y=16;y<=23;y++)for(x=8;x<360;x++) {
          const unsigned char *a=pixels+(y*w+x)*3,*b=off+(y*w+x)*3;
          assert(a[0]==b[0] && a[1]==b[1] && a[2]==b[2]);
          pixel(off,w,x,y,y>=18&&y<=22&&(x<152||x>=216),1,0);
      }
      free(off); }
    free(pixels);
    puts("GPU life: mixed16/15 spans/all mirrors, six-row full/partial/damage, empty fill; unmarked/flag-off pixel identity pass");
    Psyz_Sfa2SetArcadeCanvas(0);
    pixels=Psyz_VideoAllocCapturedFrame(&w,&h); assert(pixels && w==368 && h==240); free(pixels);
    SetDefDispEnv(&display,0,0,320,240); PutDispEnv(&display);
    assert(!port_video_canvas_allowed(320,240,0,1));
    pixels=Psyz_VideoAllocCapturedFrame(&w,&h); assert(pixels && w==320 && h==240); free(pixels);
    printf("GPU checker: all4096 fixed pixels pass, 4bpp/direct, all mirrors, UV240 boundary; unmarked negative control differs at %d pixels\n",bad);
    return 0;
}
