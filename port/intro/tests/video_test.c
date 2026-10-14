#include "../video.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static uint8_t memory[INTRO_VIDEO_MEMORY],objects[INTRO_VIDEO_OBJECTS*8u];
static uint16_t pixels[INTRO_VIDEO_WIDTH*INTRO_VIDEO_HEIGHT];
static uint8_t cells[3][2][1024],transparent[1024],solid[1024];
static unsigned missing,mode,last_code,last_side,last_half;

static void put(unsigned at,unsigned value) { memory[at]=(uint8_t)(value>>8); memory[at+1]=(uint8_t)value; }
static void obj(unsigned i,unsigned x,unsigned y,unsigned code,unsigned attr)
{
    unsigned j,v[4]={x,y,code,attr};
    for(j=0;j<4;j++) { objects[i*8+j*2]=(uint8_t)(v[j]>>8); objects[i*8+j*2+1]=(uint8_t)v[j]; }
}
static const uint8_t *tile(void *context,unsigned side,uint32_t code,unsigned half)
{
    unsigned layer=side==8 ? 0 : side==16 ? 1 : 2;
    (void)context;
    last_code=code; last_side=side; last_half=half;
    if(missing) return NULL;
    if(mode==1) return solid;
    if(mode==2) return transparent;
    if(mode==3) { memset(solid,(int)(code%15u),sizeof solid); return solid; }
    return cells[layer][half];
}
static struct intro_video setup(void)
{
    struct intro_video v;
    unsigned i,j,k,side;
    memset(&v,0,sizeof v); memset(memory,0,sizeof memory); memset(objects,0,sizeof objects);
    memset(transparent,15,sizeof transparent); memset(solid,1,sizeof solid);
    missing=mode=0;
    for(i=0;i<3;i++) for(j=0;j<2;j++) {
        side=8u<<i;
        for(k=0;k<side*side;k++) cells[i][j][k]=(uint8_t)((k%side+3u*(k/side)+j*7u)%15u);
    }
    v.memory=memory; v.memory_size=sizeof memory; v.map_base[1]=0x4000; v.map_base[2]=0x8000;
    v.palette_base=0xc000; v.row_base=0xe000; v.objects=objects;
    v.object_x_offset=64; v.object_y_offset=16;
    v.layer_control=0x3900; v.priority_control=0x6421; v.video_control=15;
    v.tile=tile;
    for(i=0;i<2048;i++) put(v.palette_base+i*2u,0xf000u|(i&15u)*0x111u);
    return v;
}
static void render(struct intro_video *v) { assert(!intro_video_render(v,pixels,sizeof pixels/sizeof *pixels)); }
static uint16_t gray(unsigned pen) { return intro_video_color((uint16_t)(0xf000u+pen*0x111u)); }

static void maps(void)
{
    unsigned layer,c,r,i,color,seen[4096];
    for(layer=0;layer<3;layer++) {
        memset(seen,0,sizeof seen);
        for(c=0;c<64;c++) for(r=0;r<64;r++) {
            i=intro_video_map_index(layer,c,r); assert(i<4096); assert(!seen[i]); seen[i]=1;
        }
    }
    assert(intro_video_map_index(3,0,0)==4096);
    for(color=0;color<65536;color++) {
        unsigned brightness=15+2*(color>>12),expected=0;
        for(i=0;i<3;i++) expected|=((((color>>(8u-4u*i))&15u)*17u*brightness/45u)>>3)<<(5u*i);
        assert(intro_video_color((uint16_t)color)==expected);
    }
}
static void scroll(void)
{
    unsigned layer,flip,x,y;
    for(layer=0;layer<3;layer++) for(flip=0;flip<4;flip++) {
        struct intro_video v=setup();
        unsigned side=8u<<layer,i;
        v.layer_control|=(uint16_t)(2u<<layer); v.video_control=12;
        v.scroll_x[layer]=65529; v.scroll_y[layer]=65531;
        for(i=0;i<4096;i++) put(v.map_base[layer]+i*4+2,flip*32);
        render(&v);
        for(y=0;y<INTRO_VIDEO_HEIGHT;y++) for(x=0;x<INTRO_VIDEO_WIDTH;x++) {
            unsigned sx=x+64u+v.scroll_x[layer],sy=y+16u+v.scroll_y[layer];
            unsigned tx=sx%side,ty=sy%side,half=layer==0 ? (sx/side)&1u : 0;
            if(flip&1u) tx=side-1u-tx;
            if(flip&2u) ty=side-1u-ty;
            assert(pixels[y*INTRO_VIDEO_WIDTH+x]==gray((tx+3u*ty+half*7u)%15u));
        }
        assert(last_side==side);
        assert(last_code==(layer==0 ? 0x20000u : layer==1 ? 0x10000u : 0x4000u));
    }
    {
        struct intro_video v=setup(); unsigned i;
        v.layer_control|=4; v.row_offset=1010;
        for(i=0;i<1024;i++) put(v.row_base+2*i,i*3u+i/16u);
        render(&v);
        for(y=0;y<INTRO_VIDEO_HEIGHT;y++) for(x=0;x<INTRO_VIDEO_WIDTH;x++) {
            unsigned row=(y+16u+1010u)&1023u;
            unsigned dx=row*3u+row/16u;
            assert(pixels[y*INTRO_VIDEO_WIDTH+x]==gray(((x+64u+dx)%16u+3u*((y+16u)%16u))%15u));
        }
    }
}
static void sprites(void)
{
    struct intro_video v=setup();
    unsigned flip,x,y;
    for(flip=0;flip<4;flip++) {
        v=setup(); v.object_count=1; mode=3;
        obj(0,0xe000u+64,0x2000u+16,0x2f,0x1100u+32u*flip);
        render(&v);
        for(y=0;y<32;y++) for(x=0;x<32;x++) {
            unsigned cx=(flip&1u) ? 1u-x/16 : x/16;
            unsigned cy=(flip&2u) ? 1u-y/16 : y/16;
            unsigned code=flip ? 0x1002fu+cx+16u*cy : 0x10020u+((15u+cx)&15u)+16u*cy;
            assert(pixels[y*384+x]==gray(code%15u));
        }
        assert(pixels[32]==0 && pixels[32*384]==0);
    }
    v=setup(); v.object_count=1;
    obj(0,0xe000+64,16,7,96); render(&v);
    for(y=0;y<16;y++) for(x=0;x<16;x++)
        assert(pixels[y*384+x]==gray((15u-x+3u*(15u-y))%15u));
    v.object_x_offset=68; v.object_y_offset=18; mode=1; render(&v);
    assert(pixels[11]==gray(1) && pixels[12]==0 && pixels[14*384]==0);
    obj(0,0xe000,0,7,128); render(&v); /* offset bypass puts origin at (64,16). */
    assert(pixels[0]==gray(1) && pixels[15]==gray(1) && pixels[16]==0);
    obj(0,0xe000+1020,1020,7,0); render(&v); assert(pixels[0]==0);
}
static void priority(void)
{
    struct intro_video v;
    unsigned rank,back;
    for(back=0;back<16;back++) for(rank=0;rank<8;rank++) {
        v=setup(); mode=1; v.layer_control|=8;
        v.priority_control=(uint16_t)(back<<12); v.object_count=1;
        put(v.palette_base+2,0xff00); put(v.palette_base+1536u*2+2,0xf0f0);
        obj(0,(rank<<13)+64,16,1,0); render(&v);
        assert(pixels[0]==intro_video_color((uint16_t)(rank>back ? 0xff00 : 0xf0f0)));
    }
    v=setup(); mode=1; v.layer_control|=8; v.object_count=2;
    put(v.palette_base+2,0xff00); put(v.palette_base+1536u*2+2,0xf0f0);
    obj(0,0xe000+64,16,1,0); obj(1,0x2000+64,16,1,0); render(&v);
    assert(pixels[0]==intro_video_color(0xf0f0)); /* hidden front object still masks back object */
    v.object_count=1; render(&v); assert(pixels[0]==intro_video_color(0xff00));
    v.layer_control&=(uint16_t)~8u; mode=2; render(&v); assert(pixels[0]==0);
}
static void failures(void)
{
    struct intro_video v=setup();
    uint16_t marker=0x1234;
    pixels[0]=marker;
    assert(intro_video_render(NULL,pixels,384u*224u));
    assert(intro_video_render(&v,NULL,384u*224u));
    assert(intro_video_render(&v,pixels,1));
    v.map_base[0]=4; assert(intro_video_render(&v,pixels,384u*224u)); v.map_base[0]=0;
    v.row_base=INTRO_VIDEO_MEMORY; assert(intro_video_render(&v,pixels,384u*224u)); v.row_base=0xe000;
    v.palette_base=INTRO_VIDEO_MEMORY-2; assert(intro_video_render(&v,pixels,384u*224u)); v.palette_base=0xc000;
    v.object_count=897; assert(intro_video_render(&v,pixels,384u*224u)); v.object_count=0;
    v.video_control=0x8000; assert(intro_video_render(&v,pixels,384u*224u)); v.video_control=15;
    v.layer_control=0; assert(intro_video_render(&v,pixels,384u*224u)); assert(pixels[0]==marker);
    v.layer_control=0x3908; missing=1; assert(intro_video_render(&v,pixels,384u*224u));
    missing=0; mode=1; memset(solid,16,sizeof solid); assert(intro_video_render(&v,pixels,384u*224u));
}
int main(void)
{
    maps(); scroll(); sprites(); priority(); failures();
    puts("intro video: synthetic map, color, scroll, object, priority and bounds controls passed");
    return 0;
}
