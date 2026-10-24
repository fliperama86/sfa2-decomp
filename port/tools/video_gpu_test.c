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
/* Widescreen (port_video_api 5, port/widescreen.md step 1): the wide canvas of the frame area at (0,8). Primitives and clears are mirrored
 * with the canvas 64 columns wider on each side; a LoadImage into the area makes the next present not wide. Halfwords are RGB 5551. */
#define WIDE_RED   0x001f
#define WIDE_GREEN 0x03e0
#define WIDE_BLUE  0x7c00
#define WIDE_YELLOW 0x03ff
static void tile(int x,int y,int w,int h,int red,int green,int blue)
{
    TILE p;setTile(&p);setXY0(&p,x,y);setWH(&p,w,h);setRGB0(&p,red,green,blue);DrawPrim(&p);DrawSync(0);
}
static void area_clear(int y,int h,int red,int green,int blue)
{
    RECT r={0,8+y,384,h};ClearImage(&r,red,green,blue);DrawSync(0);
}
static int read_wide(uint16_t *canvas)
{
    int w=0,h=0,status;
    Psyz_VideoVSync(0);
    status=Psyz_Sfa2ReadWide(canvas,PORT_WIDE_W,PORT_ARCADE_H,&w,&h);
    if(!status)assert(w==PORT_WIDE_W && h==PORT_ARCADE_H);
    return status;
}
static void expect(const uint16_t *canvas,int x,int y,int color)
{
    if((canvas[y*PORT_WIDE_W+x]&0x7fff)!=color) {
        fprintf(stderr,"wide canvas (%d,%d): wanted 0x%04x, got 0x%04x\n",x,y,color,canvas[y*PORT_WIDE_W+x]&0x7fff);
        exit(1);
    }
}
static void two_tiles(uint16_t *canvas)
{
    int x,y;
    area_clear(0,PORT_ARCADE_H,0,0,0);
    /* A rectangle from 32 columns left of the area to 32 inside it, and one wholly inside. */
    tile(-32,40,64,16,255,0,0);tile(100,100,40,16,0,255,0);
    assert(read_wide(canvas)==0);
    for(y=0;y<PORT_ARCADE_H;y++)for(x=0;x<PORT_WIDE_W;x++) {
        int color=0;
        if(y>=40 && y<56 && x>=32 && x<96)color=WIDE_RED;
        if(y>=100 && y<116 && x>=164 && x<204)color=WIDE_GREEN;
        expect(canvas,x,y,color);
    }
}
static void wide_case(DRAWENV *draw)
{
    uint16_t canvas[PORT_WIDE_W*PORT_ARCADE_H],data[4]={0x7fff,0x7fff,0x7fff,0x7fff};
    unsigned char *frame;RECT r;int w,h,x,y;
    /* Many presents in a row: no frame limiter, since a compositor may hold the frame callbacks of a window nobody watches for a second each. */
    assert(!Psyz_VideoSetVsyncMode(PSYZ_VSYNC_LIMITLESS));
    /* Off, the default: nothing is wide. */
    assert(read_wide(canvas)==-1);
    Psyz_Sfa2SetWide(PORT_WIDE_EXTRA);PutDrawEnv(draw);
    /* Before the area was cleared whole there is no good canvas. */
    assert(read_wide(canvas)==-1);
    two_tiles(canvas);
    /* The 384 frame is the same picture, 64 columns in. */
    frame=Psyz_VideoAllocCapturedFrame(&w,&h);assert(frame && w==PORT_ARCADE_W && h==PORT_ARCADE_H);
    for(y=0;y<h;y++)for(x=0;x<w;x++) {
        const unsigned char *p=frame+(y*w+x)*3;
        int color=(p[0]>=230?WIDE_RED:0)|(p[1]>=230?WIDE_GREEN:0)|(p[2]>=230?WIDE_BLUE:0);
        expect(canvas,x+PORT_WIDE_EXTRA,y,color);
    }
    free(frame);
    /* A clear of the area's whole width, 10 rows high, spans the canvas's whole width. */
    area_clear(60,10,255,255,0);
    assert(read_wide(canvas)==0);
    for(x=0;x<PORT_WIDE_W;x++) {
        expect(canvas,x,59,x>=32 && x<96?0:0);
        expect(canvas,x,60,WIDE_YELLOW);expect(canvas,x,69,WIDE_YELLOW);expect(canvas,x,70,0);
    }
    /* The whole area, blue: every column of the canvas. */
    area_clear(0,PORT_ARCADE_H,0,0,255);
    assert(read_wide(canvas)==0);
    for(y=0;y<PORT_ARCADE_H;y++)for(x=0;x<PORT_WIDE_W;x++)expect(canvas,x,y,WIDE_BLUE);
    /* A LoadImage into the area leaves the video memory the only truth: not wide until the area is cleared whole again. */
    r=(RECT){16,24,2,2};LoadImage(&r,(u_long*)data);DrawSync(0);
    assert(read_wide(canvas)==-1);
    r=(RECT){0,8,384,224};ClearImage(&r,0,0,0);DrawSync(0);
    assert(read_wide(canvas)==0);expect(canvas,0,0,0);expect(canvas,PORT_WIDE_W-1,PORT_ARCADE_H-1,0);
    /* A MoveImage into the area does the same; one out of it does not. */
    r=(RECT){600,300,4,4};Psyz_Sfa2SetWide(PORT_WIDE_EXTRA);MoveImage(&r,600,310);DrawSync(0);
    assert(read_wide(canvas)==0);
    MoveImage(&r,20,40);DrawSync(0);
    assert(read_wide(canvas)==-1);
    area_clear(0,PORT_ARCADE_H,0,0,0);
    assert(read_wide(canvas)==0);
    /* A different canvas size or off again: nothing is read, the default presentation returns. */
    Psyz_Sfa2SetWide(0);
    assert(read_wide(canvas)==-1);
    Psyz_Sfa2SetWide(PORT_WIDE_EXTRA);
    assert(read_wide(canvas)==-1);
    /* At twice the internal resolution the canvas is twice as large and the readback samples every second pixel: the same picture. */
    assert(!Psyz_VideoSetInternalResolution(2));
    Psyz_Sfa2SetWide(PORT_WIDE_EXTRA);
    Psyz_VideoVSync(0);PutDrawEnv(draw);
    two_tiles(canvas);
    assert(!Psyz_VideoSetInternalResolution(1));
    Psyz_VideoVSync(0);
    Psyz_Sfa2SetWide(0);
    printf("GPU wide: canvas mirrors primitives left of the area and inside it, spans clears, goes stale on LoadImage/MoveImage into the area, off by default\n");
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
    wide_case(&draw);
    Psyz_Sfa2SetArcadeCanvas(0);
    pixels=Psyz_VideoAllocCapturedFrame(&w,&h); assert(pixels && w==368 && h==240); free(pixels);
    SetDefDispEnv(&display,0,0,320,240); PutDispEnv(&display);
    assert(!port_video_canvas_allowed(320,240,0,1));
    pixels=Psyz_VideoAllocCapturedFrame(&w,&h); assert(pixels && w==320 && h==240); free(pixels);
    printf("GPU checker: all4096 fixed pixels pass, 4bpp/direct, all mirrors, UV240 boundary; unmarked negative control differs at %d pixels\n",bad);
    return 0;
}
