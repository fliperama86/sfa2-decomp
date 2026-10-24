/* Generated RAM, real GPU packet-copy seam, mock SDK sink, no game/GPU. */
#define _GNU_SOURCE
#include "gpu.h"
#include "settings.h"
#include "ui_layout.h"
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
static int pixel_perfect_ui,center_ui,ui_canvas_active,wide_on;
struct port_settings port_settings;   /* the walk reads wide_ends */
typedef unsigned long OT_TYPE;
#define LINK_END 0x00ffffffu
static void stop(const char *fmt,...)
{
    va_list args;va_start(args,fmt);vfprintf(stderr,fmt,args);va_end(args);abort();
}
static void DrawOTag(OT_TYPE *words);
void port_clock_frame_drawn(unsigned packets) { (void)packets; }   /* the clock's hook that the list walker calls */
unsigned port_frames(void) { return 1; }   /* the frame count of the line that the anchor prints at a camera end (never 120 here) */
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

/* Widescreen's bridge (port/widescreen.md): the host's queue per entry and the anchor of the world's packets. */
static int x_of(const uint32_t *w) { return (int16_t)w[1]; }
static int y_of(const uint32_t *w) { return (int16_t)(w[1]>>16); }
static void fight_layer(int active,int cam,int min,int max)
{
    unsigned char *l=(unsigned char *)(uintptr_t)0x801aa5d4u;
    l[0]=(unsigned char)active;*(int16_t *)(l+0x12)=(int16_t)cam;*(int16_t *)(l+0x44)=(int16_t)min;*(int16_t *)(l+0x46)=(int16_t)max;
}
static void clear_ot(uint32_t base)
{
    unsigned i;
    memset((void *)(uintptr_t)PORT_RAM_BASE,0,PORT_RAM_SIZE);
    for(i=0;i<30;i++)*at(base+i*4)=i?(base+(i-1)*4)&LINK_END:LINK_END;
}
static void wide_cases(void)
{
    uint32_t base=0x801fc000u;
    uint32_t world[]={0x600000ff,100u<<16|300,8u<<16|8};
    uint32_t far_x[]={0x600000ff,100u<<16|1020,8u<<16|8};   /* +64 leaves the 11 bits: sent as it is */
    uint32_t hud[]={0x2c808080,60u<<16|20,0x78070000,60u<<16|36,0x000d000f,76u<<16|20,0x00000f00,76u<<16|36,0x00000f0f};
    uint32_t qa[]={0x601111ff,11u<<16|200,8u<<16|8},qb[]={0x602222ff,22u<<16|200,8u<<16|8},qc[]={0x603333ff,33u<<16|200,8u<<16|8};
    static const int cams[3]={0,256,512};
    static const int shift[3]={-64,0,64};
    unsigned k,mode,i;
    int dx;
    ui_canvas_active=1;pixel_perfect_ui=0;
    for(mode=0;mode<7;mode++) {
        /* 0 centred HUD, 1 HUD in place, 2 widescreen off, 3 layer inactive, 4 no canvas, 5 empty range, 6 the view centred (wide_ends
         * centred, the default): nothing moves in 2..6 */
        center_ui=mode!=1;wide_on=mode!=2;ui_canvas_active=mode!=4;port_settings.wide_ends=mode!=6?PORT_WIDE_ANCHORED:PORT_WIDE_CENTRED;
        dx=port_ui_center_offset(center_ui,ui_canvas_active,PORT_ARCADE_W);
        for(k=0;k<3;k++) {
            int a=mode<2?shift[k]:0;
            clear_ot(base);
            fight_layer(mode!=3,cams[k],0,mode==5?0:512);
            append(base,14,0x80100200,world,3);
            append(base,14,0x80100240,far_x,3);
            append(base,17,0x80186004,hud,9);
            assert(!port_gpu_queue_packet(14,qa,3));assert(!port_gpu_queue_packet(14,qb,3));assert(!port_gpu_queue_packet(13,qc,3));
            captured_count=0;send_list(at(base+29*4));
            /* HUD (entry 17), then entry 14: its queued packets in the order queued, then the game's (last linked first), then 13's. */
            assert(captured_count==6);
            assert(captured[0].len==9 && captured[0].words[1]==(60u<<16|(unsigned)(20+dx)));
            assert(captured[1].len==3 && y_of(captured[1].words)==11 && x_of(captured[1].words)==200+a);
            assert(captured[2].len==3 && y_of(captured[2].words)==22 && x_of(captured[2].words)==200+a);
            assert(x_of(captured[3].words)==(1020+a>1023?1020:1020+a));   /* past the 11 bits (a = 64): untouched, the program goes on */
            assert(captured[4].len==3 && y_of(captured[4].words)==100 && x_of(captured[4].words)==300+a);
            assert(captured[5].len==3 && y_of(captured[5].words)==33 && x_of(captured[5].words)==200+a);
            for(i=0;i<3;i++)assert(captured[1].words[i]==(i==1?(qa[1]&0xffff0000u)|(uint16_t)(200+a):qa[i]));
            /* Only once: the queue is empty after the walk. */
            captured_count=0;send_list(at(base+29*4));assert(captured_count==3);
            assert(x_of(captured[2].words)==300+a);
        }
    }
    port_settings.wide_ends=PORT_WIDE_ANCHORED;   /* the rest of the cases are of the anchored view */
    /* An entry that the walk does not reach drops its packets at the end of that walk; they do not reach a later one. */
    wide_on=1;ui_canvas_active=1;center_ui=1;
    clear_ot(base);fight_layer(1,0,0,512);
    assert(!port_gpu_queue_packet(3,qa,3));
    captured_count=0;send_list(at(base+2*4));assert(captured_count==0);
    assert(!port_gpu_queue_packet(2,qb,3));
    captured_count=0;send_list(at(base+2*4));assert(captured_count==1 && y_of(captured[0].words)==22 && x_of(captured[0].words)==200-64);
    captured_count=0;send_list(at(base+3*4));assert(captured_count==0);
    /* Refusals: an entry out of range, no words, no length, a packet too long, a full queue (0x10000 words a frame). */
    assert(port_gpu_queue_packet(-1,qa,3)==-1 && port_gpu_queue_packet(30,qa,3)==-1);
    assert(port_gpu_queue_packet(0,NULL,3)==-1 && port_gpu_queue_packet(0,qa,0)==-1 && port_gpu_queue_packet(0,qa,256)==-1);
    for(k=0;!port_gpu_queue_packet(5,qa,3);k++)assert(k<0x10000);
    assert(k==0x10000/3);
    assert(port_gpu_queue_packet(5,qa,3)==-1);
    assert(!port_gpu_queue_packet(5,qa,1) && port_gpu_queue_packet(5,qa,1)==-1);   /* the last word fits, then no more */
    captured_count=0;send_list(at(base+2*4));assert(captured_count==0);   /* ends the frame: the queue is empty again */
    assert(!port_gpu_queue_packet(2,qb,3));
    captured_count=0;send_list(at(base+2*4));assert(captured_count==1);
    wide_on=0;
}
/* The planes kept inside their painting and anchored at their own speed (port/widescreen.md, steps 5 and 6): port_gpu_layer_view and the
 * shift of a registered plane's packets. */
/* The plane furthest back, at a stage end only, moved toward 0 from the anchor as far as its wide view, -64 - s to 448 - s, needs to
 * stay inside art_lo..art_hi, never past 0; in the middle of a stage, and for a plane in front of another, the anchor. */
static int expected_shift(int a,int art_lo,int art_hi)
{
    int lo=448-art_hi,hi=-64-art_lo;
    if(a>0)return a<(hi>0?hi:0)?a:(hi>0?hi:0);
    if(a<0)return a>(lo<0?lo:0)?a:(lo<0?lo:0);
    return 0;
}
static void plane_cases(void)
{
    uint32_t base=0x801fc000u;
    uint32_t plane[]={0x600000ff,100u<<16|300,8u<<16|8},other[]={0x602222ff,50u<<16|300,8u<<16|8},plane2[]={0x604444ff,70u<<16|310,8u<<16|8};
    uint32_t hud[]={0x2c808080,60u<<16|20,0x78070000,60u<<16|36,0x000d000f,76u<<16|20,0x00000f00,76u<<16|36,0x00000f0f};
    uint32_t qp[]={0x601111ff,11u<<16|200,8u<<16|8},qo[]={0x603333ff,33u<<16|200,8u<<16|8},qp2[]={0x605555ff,55u<<16|220,8u<<16|8};
    static const int cams[3]={0,256,512},anchor[3]={-64,0,64};
    static const int art[5][2]={{-256,640},{-100,500},{0,384},{-10,420},{-64,448}};
    unsigned k,v,mode,i;
    int dx,s,s2;
    ui_canvas_active=1;pixel_perfect_ui=0;center_ui=1;
    dx=port_ui_center_offset(center_ui,ui_canvas_active,PORT_ARCADE_W);
    /* modes: 0 widescreen on in a fight, 1 widescreen off, 2 the main layer inactive (no fight): the views do not apply in 1 and 2 */
    for(mode=0;mode<3;mode++)for(v=0;v<5;v++)for(k=0;k<3;k++) {
        int a=mode?0:anchor[k];
        wide_on=mode!=1;
        s=mode?0:expected_shift(a,art[v][0],art[v][1]);
        s2=a;                                    /* the second plane (entry 13) is in front of the first (14): the anchor */
        clear_ot(base);
        fight_layer(mode!=2,cams[k],0,512);
        append(base,14,0x80100400,plane,3);      /* in the registered pool */
        append(base,14,0x80100500,other,3);      /* in no pool: keeps the anchor */
        append(base,13,0x80100600,plane2,3);     /* in the second plane's pool */
        append(base,17,0x80186004,hud,9);
        port_gpu_layer_view(0x801aa544u,0x80100400,0x80100440,14,art[v][0],art[v][1]);
        port_gpu_layer_view(0x801aa664u,0x80100600,0x80100640,13,-100,500);
        assert(!port_gpu_queue_packet(14,qp,3));assert(!port_gpu_queue_packet(13,qp2,3));assert(!port_gpu_queue_packet(12,qo,3));
        captured_count=0;send_list(at(base+29*4));
        /* HUD (17), entry 14: its queued packet, then the game's last-linked first (other, plane); entry 13: queued, plane2; entry 12: queued */
        assert(captured_count==7);
        assert(captured[0].len==9 && captured[0].words[1]==(60u<<16|(unsigned)(20+dx)));            /* the HUD keeps its own shift */
        assert(x_of(captured[1].words)==200+s && y_of(captured[1].words)==11);                       /* the plane's entry: its shift */
        assert(x_of(captured[2].words)==300+a && y_of(captured[2].words)==50);                       /* outside any pool: the anchor */
        assert(x_of(captured[3].words)==300+s && y_of(captured[3].words)==100);                      /* in the pool: the shift */
        assert(x_of(captured[4].words)==220+s2 && y_of(captured[4].words)==55);                      /* the second plane's entry */
        assert(x_of(captured[5].words)==310+s2 && y_of(captured[5].words)==70);
        assert(x_of(captured[6].words)==200+a && y_of(captured[6].words)==33);                       /* an entry without a plane: the anchor */
        /* negative control: where the bounds clamp (not the wide painting) the plane's shift is not the anchor, so a walk that moved it by
         * the anchor would fail the checks above */
        if(!mode && v==1 && k!=1)assert(s!=a);
        if(!mode && v==2)assert(s==0);                                                               /* a painting of 384: the view of 0 at the ends */
        if(!mode && k==1)assert(s==0);                                                               /* the middle of a stage: nothing moves */
        if(!mode && v==0)assert(s==a);                                                               /* a wide painting: nothing changes */
        /* The views are for one walk: without a new call the next walk moves the plane by the anchor. */
        assert(!port_gpu_queue_packet(14,qp,3));
        captured_count=0;send_list(at(base+29*4));
        assert(captured_count==5 && x_of(captured[1].words)==200+a && x_of(captured[3].words)==300+a && x_of(captured[4].words)==310+a);   /* hud, qp, other, plane, plane2 */
    }
    /* Refusals: an empty range, an entry out of range, a painting that does not hold the view's own 0..384 are ignored. */
    wide_on=1;clear_ot(base);fight_layer(1,0,0,512);
    port_gpu_layer_view(0x801aa544u,0x80100400,0x80100400,14,-100,500);
    port_gpu_layer_view(0x801aa544u,0x80100400,0x80100440,30,-100,500);
    port_gpu_layer_view(0x801aa544u,0x80100400,0x80100440,14,10,500);
    port_gpu_layer_view(0x801aa544u,0x80100400,0x80100440,14,-100,300);
    append(base,14,0x80100400,plane,3);
    captured_count=0;send_list(at(base+29*4));assert(captured_count==1 && x_of(captured[0].words)==300-64);
    /* At most eight views a frame: the ninth is ignored. */
    clear_ot(base);fight_layer(1,512,0,512);
    for(i=0;i<9;i++) { port_gpu_layer_view(0x801aa700u+i*0x90,0x80100400+i*0x100,0x80100440+i*0x100,14,-100,500); append(base,14,0x80100400+i*0x100,plane,3); }
    captured_count=0;send_list(at(base+29*4));assert(captured_count==9);
    for(i=0;i<9;i++)assert(x_of(captured[8-i].words)==(i<8?300+36:300+64));    /* the first eight clamp to 36, the ninth keeps the anchor */
    wide_on=0;
}
/* ---- step 6: each plane's speed against the camera's ---- */
#define PLANES 4
struct plane_spec {
    uint32_t record;      /* the layer's Chan record: scroll at +0x12, ot_index at +0x8a */
    int ot, entry;        /* its own entry (ot_index) and the entry of its side cells */
    int art_lo, art_hi;
    int step, drift;      /* its scroll moves by step per frame while the camera moves by 4 (a speed of step / 4), and by drift on its own */
    int start;            /* where its scroll starts (modulo 512) */
};
static int tp_scroll[PLANES];
static uint32_t tp_pool(int i) { return 0x80100400u+(uint32_t)i*0x100u; }

static void tp_set_stage(unsigned stage) { *(uint16_t *)at(0x80190148u)=(uint16_t)stage; }
static int find_x(unsigned y)
{
    unsigned i;
    for(i=0;i<captured_count;i++)if(captured[i].len==3 && (unsigned)y_of(captured[i].words)==y)return x_of(captured[i].words);
    assert(!"no packet at that y");
    return 0;
}

/* One walk of a frame: the camera at `cam`, the scroll of each plane in tp_scroll[], every plane registered with a packet in its pool (y 100 + i), a
 * side cell queued for its entry (y 10 + i) and another packet of the game on its own entry that is in no pool (y 50 + i); also a packet in no plane's
 * entry (y 70) and the HUD. fight 0: the main layer off. */
static void tp_walk(uint32_t base,unsigned stage,int fight,int cam,const struct plane_spec *sp,int n)
{
    uint32_t pool[3]={0x600000ff,0,8u<<16|8},other[3]={0x602222ff,0,8u<<16|8},cell[3]={0x601111ff,0,8u<<16|8},world[3]={0x600000ff,70u<<16|300,8u<<16|8};
    uint32_t hud[]={0x2c808080,60u<<16|20,0x78070000,60u<<16|36,0x000d000f,76u<<16|20,0x00000f00,76u<<16|36,0x00000f0f};
    int i;
    clear_ot(base);
    tp_set_stage(stage);
    fight_layer(fight,cam,0,512);
    for(i=0;i<n;i++) {
        unsigned char *l=(unsigned char *)(uintptr_t)sp[i].record;
        *(int16_t *)(l+0x12)=(int16_t)tp_scroll[i];l[0x8a]=(unsigned char)sp[i].ot;
        pool[1]=(unsigned)(100+i)<<16|300;other[1]=(unsigned)(50+i)<<16|300;cell[1]=(unsigned)(10+i)<<16|200;
        append(base,(unsigned)sp[i].ot,tp_pool(i),pool,3);
        append(base,(unsigned)sp[i].ot,0x80100800u+(uint32_t)i*0x40u,other,3);
        port_gpu_layer_view(sp[i].record,tp_pool(i),tp_pool(i)+0x40u,sp[i].entry,sp[i].art_lo,sp[i].art_hi);
        assert(!port_gpu_queue_packet(sp[i].entry,cell,3));
    }
    append(base,5,0x80100c00u,world,3);
    append(base,17,0x80186004,hud,9);
    captured_count=0;send_list(at(base+29*4));
}

/* The camera at 200 for 250 frames (the planes drift on their own), then ramped to `to` in steps of 4 (the planes move by their steps and drifts). */
static void tp_ramp(uint32_t base,unsigned stage,int to,struct plane_spec *sp,int n)
{
    int i,cam,dir=to>200?1:-1;
    for(i=0;i<n;i++)tp_scroll[i]=sp[i].start&511;
    tp_walk(base,stage,1,200,sp,n);
    for(i=0;i<250;i++) {
        int j;
        for(j=0;j<n;j++)tp_scroll[j]=(tp_scroll[j]+sp[j].drift)&511;
        tp_walk(base,stage,1,200,sp,n);
    }
    for(cam=200;cam!=to;) {
        int j;
        cam+=4*dir;
        for(j=0;j<n;j++)tp_scroll[j]=(tp_scroll[j]+dir*sp[j].step+sp[j].drift)&511;   /* wraps at 512: 511 -> 0 is a step of 1 */
        tp_walk(base,stage,1,cam,sp,n);
    }
}

static int round_half(int num,int den) { return num>=0?(2*num+den)/(2*den):-((2*-num+den)/(2*den)); }

static void estimator_cases(void)
{
    uint32_t base=0x801fc000u;
    /* A moves at 0.25 of the camera's speed (and wraps), B at 2, C at 0.5 with a drift of 1 a frame of its own, D (entry 20, furthest back) at 0.5.
     * A and C are behind the camera's plane (own entries 12 and 14), B in front (13). */
    struct plane_spec sp[PLANES]={
        {0x801aa544u,12,12,-256,640,1,0,505},{0x801aa664u,13,13,-256,640,8,0,3},{0x801aa700u,14,14,-256,640,2,1,0},{0x801aa790u,20,20,-256,640,2,0,100}};
    static const int ends[2]={480,16};              /* a = 32 at the right end, -48 at the left */
    unsigned e;
    int a,hud_x;
    ui_canvas_active=1;pixel_perfect_ui=0;center_ui=1;wide_on=1;
    hud_x=20+port_ui_center_offset(center_ui,ui_canvas_active,PORT_ARCADE_W);
    for(e=0;e<2;e++) {
        int sa,sb,sc,sd;
        tp_ramp(base,7+e,ends[e],sp,PLANES);
        a=e?-48:32;
        sa=round_half(a,4);sb=2*a;sc=round_half(a,2);sd=round_half(a,2);
        /* per packet kind. Pool and queued cells: the layer's own shift (f times a); D, the furthest back, within 0..a (inside its painting). */
        assert(find_x(100)==300+sa && find_x(10)==200+sa);                  /* A: f = 0.25 */
        assert(find_x(101)==300+sb && find_x(11)==200+sb);                  /* B: f = 2 */
        assert(abs(find_x(102)-300-sc)<=1 && abs(find_x(12)-200-sc)<=1);    /* C: f = 0.5 with a drift of its own (the estimate is within a pixel) */
        assert(find_x(103)==300+sd && find_x(13)==200+sd);                  /* D: f = 0.5, wide painting: round(f * a) */
        /* the other packets of the game on a plane's own entry: moved with a plane behind the camera's (f < 1), not with one in front, and the world's */
        assert(find_x(50)==300+sa);                                         /* A's entry 12: a prop */
        assert(find_x(51)==300+a);                                          /* B's entry 13: f >= 1, the anchor */
        assert(abs(find_x(52)-300-sc)<=1);                                  /* C's entry 14 */
        assert(find_x(53)==300+sd);                                         /* D's entry 20 */
        assert(find_x(70)==300+a);                                          /* in no plane's entry: the anchor */
        { unsigned i;for(i=0;i<captured_count;i++)if(captured[i].len==9)assert(captured[i].words[1]==(60u<<16|(unsigned)hud_x));}   /* the HUD */
        /* negative control: a wrong factor, or none, would not pass the checks above */
        assert(sa!=a && sa!=round_half(a,2) && sb!=a && sd!=a);
    }
    /* A new stage starts the estimates over (f = 1: every plane moves by the anchor, D within 0..a); so does the main layer off. */
    tp_walk(base,99,1,480,sp,PLANES);
    assert(find_x(100)==300+32 && find_x(101)==300+32 && find_x(50)==300+32 && find_x(51)==300+32);
    tp_ramp(base,20,480,sp,PLANES);                                     /* known again */
    tp_walk(base,20,1,480,sp,PLANES);assert(find_x(100)==300+8);
    tp_walk(base,20,0,480,sp,PLANES);                                   /* the main layer off: no fight, nothing moves, the estimates go */
    assert(find_x(100)==300 && find_x(101)==300 && find_x(50)==300);
    tp_walk(base,20,1,480,sp,PLANES);assert(find_x(100)==300+32 && find_x(101)==300+32);
    /* Samples that do not count: a camera step over 64, a layer step 64 over its own, leave f where it was. */
    tp_ramp(base,21,480,sp,PLANES);
    tp_scroll[0]=(tp_scroll[0]+300)&511;tp_walk(base,21,1,300,sp,PLANES);                                  /* the camera 180 back, the layer far off */
    tp_scroll[0]=(tp_scroll[0]+100)&511;tp_walk(base,21,1,302,sp,PLANES);                                  /* a step of 100 for 2 */
    tp_walk(base,21,1,480,sp,PLANES);assert(find_x(100)==300+8);
    /* A scroll that wraps (511 to 0) is a step of 1, and counts: with five steps of the camera, the fourth across the wrap, the estimate is known
     * at the fifth only if the wrapped step was taken. */
    {
        struct plane_spec one[1]={{0x801aa544u,12,12,-256,640,1,0,507}};
        int c;
        tp_scroll[0]=508;
        tp_walk(base,60,1,456,one,1);
        for(c=460;c<=476;c+=4) {
            tp_scroll[0]=(tp_scroll[0]+1)&511;
            tp_walk(base,60,1,c,one,1);
        }
        assert(tp_scroll[0]==(508+5)%512 && find_x(100)==300+round_half(28,4));          /* 508 -> 509, 510, 511, 0, 1: f = 0.25 at a = 28 */
    }
    /* A layer registered twice in a walk (two pools) is sampled once: its drift is not pulled to 0 by the second look. */
    {
        struct plane_spec two[2]={{0x801aa700u,14,14,-256,640,2,1,0},{0x801aa700u,14,15,-256,640,2,1,0}};
        tp_ramp(base,61,480,two,2);
        assert(abs(find_x(100)-300-round_half(32,2))<=1 && abs(find_x(101)-300-round_half(32,2))<=1);
    }
    /* The plane furthest back inside its painting and no further than the anchor: f = 0.5 and f = 2, at the right and the left end. */
    {
        static const int art[5][2]={{-256,640},{-100,500},{0,384},{-10,420},{-300,640}};
        static const int steps[2]={2,8};
        unsigned v,k,m;
        for(m=0;m<2;m++)for(k=0;k<2;k++)for(v=0;v<4;v++) {
            struct plane_spec one[1]={{0x801aa790u,20,20,art[v][0],art[v][1],steps[m],0,100}};
            int s,lo=448-art[v][1],hi=-64-art[v][0],ak=k?-48:32,f_a=m?2*ak:round_half(ak,2);
            tp_ramp(base,30+m*4+k*2+v,k?16:480,one,1);
            s=f_a;
            if(s>0)s=s<(hi>0?hi:0)?s:(hi>0?hi:0);else if(s<0)s=s>(lo<0?lo:0)?s:(lo<0?lo:0);
            s=ak>0?(s<0?0:s>ak?ak:s):(s>0?0:s<ak?ak:s);
            assert(find_x(100)==300+s && find_x(10)==200+s && find_x(50)==300+(m?ak:s));       /* f = 2: no prop rule (f >= 1) */
            assert(s>=(ak<0?ak:0) && s<=(ak>0?ak:0));                                          /* within 0..a */
            if(m && v==0)assert(s==ak);                                                        /* f = 2 with a wide painting: no further than a */
            if(!m && v==2)assert(s==0);                                                        /* a painting of 384: back to 0 */
        }
    }
    wide_on=0;
}
int main(void)
{
    unsigned frame,flags,i,n;unsigned char *saved=malloc(PORT_RAM_SIZE);
    void *ram=mmap((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    assert(ram==(void *)(uintptr_t)PORT_RAM_BASE && saved);
    port_settings.wide_ends=PORT_WIDE_ANCHORED;   /* the anchored view, unless a case says otherwise */
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
    wide_cases();
    plane_cases();
    estimator_cases();
    free(saved);assert(!munmap(ram,PORT_RAM_SIZE));
    puts("UI seam: real walker/copy, host packet queue, stage anchor, planes kept inside their painting, both frame tables, eight flag states, compound sprite, meter fills, P2 life cap, RAM/world/single-packet identity pass");
    return 0;
}
