/* Native scene sequencing, recovered from the eight original intro controllers.
 * Actor, data and presentation dependencies are explicit, never interpreted CPU
 * code or recorded frame playback. See director-contract.md. */
#include "world.h"
#include "particles.h"
#include <string.h>

static void step(struct intro_world *w) {sw(w,IG_STEP,gw(w,IG_STEP)+2);}
static void order(struct intro_world *w,unsigned layers,unsigned priorities)
{sw(w,IG_LAYERS,layers);sw(w,IG_PRIORITY,priorities);}
static void palette_bank(struct intro_world *w,unsigned bank,int both)
{sw(w,IG_PALETTE,bank);if(both) sw(w,0x48,bank);}
static void scroll(struct intro_world *w,unsigned x1,unsigned y1,unsigned x2,unsigned y2,unsigned x3,unsigned y3)
{sw(w,0x22,x1);sw(w,0x24,y1);sw(w,0x26,x2);sw(w,0x28,y2);sw(w,0x2a,x3);sw(w,0x2c,y3);}
static void map(struct intro_world *w,unsigned layer,unsigned index,uint32_t destination)
{intro_world_load_map(w,layer,index,destination);}
static void pal(struct intro_world *w,uint32_t source,uint32_t destination,unsigned count,int brightness)
{intro_world_palette_copy(w,source,destination,count,brightness);}
static uint8_t *spawn(struct intro_world *w,unsigned kind)
{
    uint16_t id=intro_world_alloc(w,0);uint8_t *a;
    if(!id) return NULL;
    a=w->ram+id;a[IA_ACTIVE]++;ip16(a+IA_KIND,kind);return a;
}
static void activate(struct intro_world *w,uint16_t id,unsigned kind)
{w->ram[id]++;ip16(w->ram+id+IA_KIND,kind);}
static int group(struct intro_world *w,unsigned n,uint16_t *ids)
{
    unsigned i,j;
    if(*ig(w,IG_FREE)<n) return 0;
    for(i=0;i<n;i++) {ids[i]=intro_world_alloc(w,0);if(!ids[i]) return 0;}
    for(i=1;i<n;i++) for(j=i;j && ids[j]<ids[j-1];j--) {uint16_t v=ids[j];ids[j]=ids[j-1];ids[j-1]=v;}
    return 1;
}
static void frame(struct intro_world *w,int secondary)
{
    intro_world_update_actors(w,0);
    if(secondary) intro_world_update_actors(w,1);
    if(!w->error) {
        if(!w->present) w->error="intro: native presentation missing";
        else if(w->present(w)) {if(!w->error) w->error="intro: presentation failed";}
        else w->rendered++;
    }
}
static void next_scene(struct intro_world *w)
{sw(w,IG_SCENE,gw(w,IG_SCENE)+2);sw(w,IG_STEP,0);sl(w,IG_AUX,0);palette_bank(w,0x92a0,1);}
static void begin(struct intro_world *w)
{step(w);intro_world_reset_actors(w);intro_world_clear_maps(w);}
static void full_palette(struct intro_world *w)
{palette_bank(w,0x90c0,0);sw(w,IG_PALETTE_SET,0);intro_world_palettes(w,1);}
static void fade(struct intro_world *w,unsigned mode,unsigned target)
{
    if(*ig(w,IG_FADE_ACTIVE)) return;
    *ig(w,IG_FADE_WAIT)=255;*ig(w,IG_FADE_ACTIVE)=255;
    w->fade_mode=mode;w->fade_target=target;w->fade_delay=0;
}
static void embers(struct intro_world *w,unsigned phase)
{
    struct intro_particle_spawn p;uint16_t interval=(uint16_t)gw(w,IG_INTERVAL),seed=(uint16_t)gw(w,IG_SEED);
    int result=intro_particle_spawn(&interval,&seed,phase,*ig(w,IG_FREE),&p);uint8_t *a;
    sw(w,IG_INTERVAL,interval);sw(w,IG_SEED,seed);
    if(result>0 && (a=spawn(w,0x8000u+p.subtype))) {a[IA_CAMERA]=0;a[IA_LAYER]=(uint8_t)p.layer;ip16(a+IA_PRIORITY,p.priority);}
}
static void wind(struct intro_world *w,unsigned vertical,unsigned color)
{
    unsigned r,subtype;uint8_t *a;
    if(!countdown(w,IG_INTERVAL)) return;
    r=intro_world_random(w);sw(w,IG_INTERVAL,(r&(vertical ? 15u : 7u))+(vertical ? 4u : 1u));
    subtype=(r&3u)+(vertical ? 3u : 0u);if(subtype==(vertical ? 6u : 3u)) subtype=vertical ? 4u : 1u;
    if(*ig(w,IG_FREE)<=(vertical ? 7u : 6u)) return;
    a=spawn(w,0x8400u+subtype);if(a) {ip16(a+IA_PALETTE_MODE,color);a[IA_DELAY]=0;}
}
static void wind_initial(struct intro_world *w,unsigned vertical,unsigned color)
{
    unsigned n=(intro_world_random(w)&(vertical ? 3u : 7u))+(vertical ? 5u : 9u),i;
    for(i=0;i<n;i++) {
        uint16_t id=intro_world_alloc(w,0);unsigned kind;uint8_t *a;if(!id) return;
        kind=(intro_world_random(w)&3u)+(vertical ? 3u : 0u);if(kind==(vertical ? 6u : 3u)) kind=vertical ? 4u : 1u;
        a=w->ram+id;a[IA_ACTIVE]++;ip16(a+IA_KIND,0x8400u+kind);ip16(a+IA_PALETTE_MODE,color);a[IA_DELAY]=1;
    }
}
static void fire_rows(struct intro_world *w) {sw(w,IG_ROW,(gw(w,IG_ROW)-1u)&511u);}
static void fire_background(struct intro_world *w)
{if(countdown(w,0x2ba)) {sw(w,0x28,(gw(w,0x28)-256u)&768u);sw(w,0x2ba,6);}}
static void fire_palette(struct intro_world *w)
{
    static const unsigned phases[]={0,32,64,32};unsigned p=(gw(w,IG_SIGNAL)+1u)&3u;
    sw(w,IG_SIGNAL,p);pal(w,0x2959c0u+phases[p],0x90cc00,16,-1);
}
static void fire_tick(struct intro_world *w,unsigned particles,int cycle)
{fire_rows(w);if(cycle) fire_palette(w);embers(w,particles);frame(w,0);}
static void fire_scene(struct intro_world *w)
{
    unsigned i,sub;uint8_t *a;uint16_t ids[3];
    switch(gw(w,IG_STEP)) {
    case 0:
        if(w->ram[0xc68]&2u) {sw(w,IG_SCENE,14);memset(ig(w,8),0,16);return;}
        begin(w);w->sound_command=6;scroll(w,0,0,512,768,512,0x710);sw(w,IG_ROW,0);
        intro_world_fill_map(w,0,0,2,0x902000);intro_world_fill_map(w,0,0,2,0x903000);
        for(i=0;i<4;i++) {map(w,1,0x8a+i,0x904000+i*0x1000);map(w,1,0x8a+i,0x904400+i*0x1000);}
        map(w,2,0x6a,0x908000);map(w,2,0x6b,0x908100);
        for(i=0;i<512;i++) {unsigned row=i&7u;ip16(w->video+0xe400+2*i,192u+(row<4 ? row : 7u-row));}
        sw(w,IG_TIMER,192);sw(w,IG_INTERVAL,4);order(w,0x390e,0x6421);
        palette_bank(w,0x90c0,0);sw(w,IG_PALETTE_SET,0);intro_world_palettes(w,0);
        for(i=0x1c000;i<0x1d000;i+=2) ip16(w->video+i,0xff00);
        fade(w,0xc01,0xffff);return;
    case 2:
        if(countdown(w,IG_TIMER)) {step(w);sw(w,IG_TIMER,1);sw(w,IG_AUX,0);palette_bank(w,0x9280,1);sw(w,0x26,0);sw(w,0x2a,0);}
        fire_tick(w,0,0);return;
    case 4:
        if(countdown(w,IG_TIMER)) {
            sub=gw(w,IG_AUX)/2;
            if(sub==14) {
                step(w);sw(w,IG_TIMER,192);sw(w,IG_AUX,0);palette_bank(w,0x90c0,0);
                sw(w,0x210,0);sw(w,0x214,240);sw(w,0x2ba,6);sw(w,0x22,0);sw(w,0x24,240);sw(w,0x26,0);
                a=spawn(w,0x8000);if(a) {a[IA_LAYER]=6;a[IA_CAMERA]=4;ip16(a+IA_PRIORITY,0xe000);sw(w,IG_LINK,(unsigned)(a-w->ram));}
                pal(w,0x281200,0x90c440,16,-1);pal(w,0x2959c0,0x90cc00,16,-1);
            } else if(sub<14) {
                sw(w,IG_AUX,gw(w,IG_AUX)+2);sw(w,IG_TIMER,2);
                if(sub&1u) {palette_bank(w,0x9280,0);sw(w,0x26,512);}
                else {
                    palette_bank(w,0x90c0,0);
                    if(sub) for(i=0;i<16;i++) {
                        unsigned first=(sub&2u) ? 0xf00 : 0;
                        ip16(w->video+0xc440+2*i,(iw16(w->video+0xc440+2*i)&0xf000u)|first);
                        ip16(w->video+0xcc00+2*i,(iw16(w->video+0xcc00+2*i)&0xf000u)|(first ? 0 : 0xf00));
                    }
                }
            } else {w->error="intro: fire flash state";return;}
        }
        fire_tick(w,1,0);return;
    case 6:
        fire_background(w);
        if(countdown(w,IG_TIMER)) {
            static const unsigned banks[]={0x9280,0x90c0,0x91c0,0x90c0,0x9280,0x90c0,0x92a0};
            sub=gw(w,IG_AUX)/2;
            if(sub<7) {sw(w,IG_AUX,gw(w,IG_AUX)+2);sw(w,IG_TIMER,2);palette_bank(w,banks[sub],1);order(w,(sub&1u)?0x390e:0x270e,(sub&1u)?0x6421:0x7771);}
            else if(sub==7) {
                step(w);sw(w,IG_TIMER,144);sw(w,IG_AUX,0);palette_bank(w,0x90c0,1);order(w,0x390e,0x6421);
                if(group(w,3,ids)) for(i=0;i<3;i++) {activate(w,ids[2-i],0x8100+i);w->ram[ids[2-i]+IA_VARIANT]=4;}
            } else {w->error="intro: fire exit flash state";return;}
        }
        fire_tick(w,2,1);return;
    case 8:
        fire_background(w);
        if(countdown(w,IG_TIMER)) {
            step(w);sw(w,IG_TIMER,2);palette_bank(w,0x9280,1);ip16(w->video+0xc440,0xfffd);
            a=intro_world_actor(w,0,(uint16_t)gw(w,IG_LINK));if(!a) return;a[IA_LAYER]=0;
        }
        fire_tick(w,2,1);return;
    case 10:
        sub=gw(w,IG_TIMER);sw(w,IG_TIMER,sub-1);
        if(!sub) {
            sw(w,IG_TIMER,0);
            if(!*ig(w,IG_FADE_ACTIVE)) {step(w);palette_bank(w,0x90c0,0);order(w,0x1e0e,0x4271);sl(w,0x240,0x30000);sl(w,0x24c,0x3000);fade(w,0x1e03,15);}
        }
        fire_tick(w,2,1);return;
    case 12:
        if(!*ig(w,IG_FADE_WAIT)) {next_scene(w);sl(w,IG_STEP,0);return;}
        fire_background(w);sl(w,0x214,gl(w,0x214)-gl(w,0x244));sl(w,0x244,gl(w,0x244)+gl(w,0x24c));
        sw(w,0x22,gw(w,0x210));sw(w,0x24,gw(w,0x214));fire_rows(w);frame(w,0);return;
    default:w->error="intro: fire director state";
    }
}
static void portrait_trail(struct intro_world *w,unsigned leader,unsigned copies)
{
    uint16_t ids[5];unsigned i;if(!group(w,5,ids)) return;
    activate(w,ids[0],leader);
    for(i=1;i<5;i++) {uint8_t *a=w->ram+ids[5-i];activate(w,ids[5-i],copies);a[IA_DELAY]=(uint8_t)i;ip16(a+IA_LINK,ids[0]);}
}
static void strip_trail(struct intro_world *w,unsigned side)
{
    uint16_t ids[4];unsigned i;
    spawn(w,0x8200u+side);if(!group(w,4,ids)) return;
    activate(w,ids[0],0x8300u+16u*side);
    for(i=1;i<4;i++) {uint8_t *a=w->ram+ids[4-i];activate(w,ids[4-i],0x8300u+16u*side+i);ip16(a+IA_TIMER,i*8u);ip16(a+IA_LINK,ids[0]);}
}
static void strips_scene(struct intro_world *w)
{
    unsigned i;uint8_t *a;
    switch(gw(w,IG_STEP)) {
    case 0:
        /* Clear-map yield leaves the original scalar result at one here. */
        begin(w);sw(w,0x290,1);sw(w,0x294,768);scroll(w,1,1,1,768,1,0x700);sw(w,IG_INTERVAL,4);
        for(i=0;i<3;i++) map(w,1,0x93+i,0x904400+0x400*i);
        map(w,2,0x6c,0x908000);map(w,2,0x6c,0x908100);order(w,0x2d0e,0x4621);full_palette(w);
        if(!spawn(w,0x8202) || !spawn(w,0x8203)) {frame(w,0);return;}
        for(i=0;i<2;i++) {a=spawn(w,0x8b0b);if(!a) {frame(w,0);return;}a[IA_VARIANT]=(uint8_t)i;}
        a=spawn(w,0x0114);if(a) {a[IA_CAMERA]=8;ip16(a+IA_X,0x2d0);ip16(a+IA_Y,0x7c0);wind_initial(w,0,0);}
        frame(w,0);return;
    case 2:if(*ig(w,IG_FREE)>=5) {step(w);sw(w,IG_TIMER,0);strip_trail(w,0);}break;
    case 4:if(gw(w,IG_TIMER)==0x8283) step(w);break;
    case 6:
        sw(w,0x290,gw(w,0x290)+3);
        if(gw(w,0x290)>=0x158 && *ig(w,IG_FREE)>=5) {step(w);sw(w,IG_TIMER,0);strip_trail(w,1);}
        sw(w,0x26,gw(w,0x290));break;
    case 8:
        sw(w,0x290,gw(w,0x290)+3);if(gw(w,0x290)>=0x280) sw(w,0x290,0x280);sw(w,0x26,gw(w,0x290));
        if(gw(w,IG_TIMER)==0x8283) {next_scene(w);return;}break;
    default:w->error="intro: strips director state";return;
    }
    wind(w,0,0);frame(w,1);
}
static void portraits_scene(struct intro_world *w)
{
    unsigned i,j;
    switch(gw(w,IG_STEP)) {
    case 0:
        begin(w);scroll(w,0,256,0,768,512,0x700);
        for(i=0;i<4;i++) intro_world_fill_map(w,2,i<2?0:8,i<2?3:0,0x908000+i*256);
        order(w,0x1b0e,0x2461);full_palette(w);
        pal(w,0x29c8e0,0x90c020,48,-1);pal(w,0x29c960,0x90c080,16,-1);pal(w,0x29cb20,0x90c0a0,16,-1);pal(w,0x29cb80,0x90c0c0,48,-1);
        for(i=0;i<5;i++) for(j=0;j<15;j++) ip16(w->video+0xc120+i*32+j*2,iw16(w->video+0xc2c0+i*2));
        return;
    case 2:
        i=gw(w,IG_AUX)/2;
        if(i<3) {sw(w,IG_AUX,gw(w,IG_AUX)+2);sw(w,0x2a,i&1u?512:0);return;}
        if(i!=3) {w->error="intro: portrait flash state";return;}
        step(w);sw(w,IG_TIMER,0);sl(w,IG_AUX,0);sw(w,0x2a,0);if(spawn(w,0x8500)) spawn(w,0x8501);break;
    case 4:case 8:
        if(gw(w,IG_INTERVAL)==0x8585) {step(w);sw(w,IG_INTERVAL,0);palette_bank(w,0x9280,0);}break;
    case 6:
        if(*ig(w,IG_FREE)>=10) {step(w);palette_bank(w,0x90c0,0);portrait_trail(w,0x8502,0x8509);portrait_trail(w,0x8503,0x850a);}break;
    case 10:step(w);sw(w,IG_TIMER,40);palette_bank(w,0x90c0,0);break;
    case 12:if(countdown(w,IG_TIMER)) {next_scene(w);return;}break;
    default:w->error="intro: portraits director state";return;
    }
    frame(w,1);
}
static void fighters_scene(struct intro_world *w)
{
    unsigned index;uint8_t *a;
    switch(gw(w,IG_STEP)) {
    case 0:
        begin(w);scroll(w,0,0,0,768,0,0x700);sw(w,IG_INTERVAL,4);
        map(w,2,0x6c,0x908000);map(w,2,0x6c,0x908100);order(w,0x1b0e,0x2461);full_palette(w);
        pal(w,0x2a1180,0x90c000,16,-1);pal(w,0x2acd80,0x90cc60,16,-1);wind_initial(w,0,256);frame(w,0);return;
    case 2:step(w);sw(w,IG_TIMER,1);sw(w,IG_SIGNAL,3);sw(w,IG_LINK,0);wind(w,0,256);frame(w,0);return;
    case 4:
        if(countdown(w,IG_TIMER)) {
            sw(w,IG_TIMER,1);index=gw(w,IG_LINK);
            if((*ig(w,IG_SIGNAL+1)&(1u<<(index&7u))) && *ig(w,0x6117)>=2) {
                a=spawn(w,0x8600u+((index<<4)&255u));
                if(a) {if(index>=4) step(w);sw(w,IG_TIMER,index>=4?112:54);sw(w,IG_LINK,index+1);}
            }
        }break;
    case 6:if(countdown(w,IG_TIMER)) {next_scene(w);return;}break;
    default:w->error="intro: fighters director state";return;
    }
    wind(w,0,256);frame(w,1);
}
static void headshots_scene(struct intro_world *w)
{
    static const unsigned kinds[]={0x8505,0x8506,0x8504,0x8507},colors[]={6,11,1,16};unsigned i;uint8_t *a;
    switch(gw(w,IG_STEP)) {
    case 0:
        begin(w);scroll(w,0,256,0,768,0,0x700);intro_world_fill_map(w,2,0,3,0x908000);intro_world_fill_map(w,2,0,3,0x908100);
        order(w,0x1b0e,0x2461);full_palette(w);pal(w,0x29cca0,0x90c020,240,-1);return;
    case 2:
        if(*ig(w,IG_FREE)>=4) {step(w);sw(w,IG_TIMER,44);sl(w,IG_FLAGS,2);for(i=0;i<4;i++) {a=spawn(w,kinds[i]);if(a) a[IA_TIMER]=(uint8_t)colors[i];}}
        frame(w,0);return;
    case 4:
        if(countdown(w,IG_TIMER)) {sw(w,IG_TIMER,1);if(gl(w,IG_FLAGS)==0x85858585u && spawn(w,0x8508)) {step(w);sw(w,IG_TIMER,48);sl(w,IG_FLAGS,0);}}break;
    case 6:if(countdown(w,IG_TIMER)) {next_scene(w);sl(w,0x10,0);return;}break;
    default:w->error="intro: headshots director state";return;
    }
    frame(w,1);
}

static void camera_scroll(struct intro_world *w)
{
    unsigned i;
    for(i=0;i<3;i++) {
        sw(w,IG_SCROLL+i*4,gw(w,IG_CAMERA1+i*128+IA_X));
        sw(w,IG_SCROLL+i*4+2,gw(w,IG_CAMERA1+i*128+IA_Y));
    }
}
static void montage_camera(struct intro_world *w)
{
    unsigned phase,row,i;const uint8_t *p;
    if(countdown(w,0x2b2)) {
        phase=(gw(w,0x2b4)+1u)&7u;sw(w,0x2b4,phase);
        p=intro_world_table(w,INTRO_INLINE_DATA,0x4902u+phase*8u,6);if(!p) return;
        sw(w,0x2b2,iw16(p));sw(w,0x290,iw16(p+2));sw(w,0x294,iw16(p+4));
    }
    sl(w,0x210,gl(w,0x210)-0x7000u);sl(w,0x310,gl(w,0x310)-0x2000u);sl(w,0x390,gl(w,0x390)-0x4000u);
    camera_scroll(w);row=((gw(w,0x390)+128u)&255u)+64u;
    for(i=0;i<86;i++) ip16(w->video+0xe000+2*i,row);
}
static void montage_brightness(struct intro_world *w)
{
    unsigned layer,i,j,brightness,offset;const uint8_t *p;uint8_t *dst;
    for(layer=0;layer<2;layer++) {
        uint8_t *value=ig(w,layer ? IG_LINK : IG_SIGNAL);
        brightness=value[0]&240u;if(brightness==value[1]) continue;
        value[1]=(uint8_t)brightness;
        /* The palette-offset lists are indirect data reads, not PC operands. */
        for(i=0;i<32;i++) {
            p=intro_world_table(w,INTRO_DATA,(layer ? 0x4476u : 0x446au)+2*i,2);if(!p) return;
            offset=iw16(p);if(offset&32768u) break;
            dst=intro_world_video(w,0x90c400u+layer*1024u+offset,32);if(!dst) return;
            for(j=0;j<16;j++) ip16(dst+2*j,(iw16(dst+2*j)&4095u)|(brightness<<8));
        }
        if(i==32) {w->error="intro: unterminated palette offset list";return;}
    }
}
static void montage_flash(struct intro_world *w)
{
    unsigned index=*ig(w,IG_TIMER);uint8_t *a;
    const uint8_t *p=intro_world_table(w,INTRO_INLINE_DATA,0x4480u+index,2);if(!p) return;
    if(gw(w,0x294)>=iw16(p) && !*ig(w,IG_TIMER+1)) {
        a=spawn(w,0x8e00u+(index>>1));
        if(a) {*ig(w,IG_TIMER)=(uint8_t)(index+2);*ig(w,IG_TIMER+1)=255;}
    }
    palette_bank(w,*ig(w,IG_TIMER+1) && gw(w,0x294)!=0x1a0 ? 0x91c0 : 0x90c0,0);
}
static void montage_scene(struct intro_world *w)
{
    unsigned i,y;uint8_t *a;
    switch(gw(w,IG_STEP)) {
    case 0:
        begin(w);scroll(w,0xa8,0x50,0xc0,0,0x40,0x700);sw(w,0x294,0);sw(w,IG_INTERVAL,4);
        map(w,0,0x76,0x901000);map(w,1,0x92,0x906400);
        map(w,1,0x4a,0x904000);map(w,1,0x4a,0x904400);map(w,1,0x53,0x904800);map(w,1,0x53,0x904c00);
        map(w,1,0x55,0x905c00);map(w,1,0x56,0x906c00);
        map(w,2,0x6d,0x908000);map(w,2,0x6e,0x908100);map(w,2,0x22,0x908200);map(w,2,0x23,0x908300);
        intro_world_fill_map(w,2,8,0,0x908400);intro_world_fill_map(w,2,8,0,0x908500);
        for(i=0;i<8;i++) map(w,2,0x48+(i&1),0x90b800+256*i);
        order(w,0x1b0e,0x2461);palette_bank(w,0x90c0,0);sw(w,IG_PALETTE_SET,0);
        pal(w,0x2811c0,0x90c400,512,0);pal(w,0x28b5c0,0x90c800,512,0);pal(w,0x2959c0,0x90cc00,512,0xf000);
        pal(w,0x29c9a0,0x90c000,144,0);pal(w,0x2a11a0,0x90c120,32,0);pal(w,0x2a1260,0x90c160,16,-1);
        for(i=0;i<4096;i+=2) ip16(w->video+0x1c000+i,iw16(w->video+0xc000+i)&4095u);
        pal(w,0x29ca00,0x91c180,64,0xf000);pal(w,0x29c9a0,0x91c200,32,0xf000);
        pal(w,0x29cac0,0x91c240,48,0xf000);pal(w,0x29cbe0,0x91c2a0,16,0xf000);ip16(w->video+0x1c2c0,65535);
        if(spawn(w,0x8702) && spawn(w,0x8701) && spawn(w,0x8700)) wind_initial(w,1,0x10b);
        break;
    case 2:
        step(w);sw(w,IG_TIMER,0);sw(w,IG_SIGNAL,0);sw(w,IG_LINK,0);wind(w,1,0x10b);break;
    case 4:
        y=(gw(w,0x294)+1u)&65535u;
        if(y>0x120) {step(w);y=0x120;}
        sw(w,0x294,y);sw(w,0x28,y);sw(w,0x24,y+0x50);
        if(is16(gw(w,0x24))>=0xe0) {i=*ig(w,IG_SIGNAL)+8u;*ig(w,IG_SIGNAL)=(uint8_t)(i>255u ? 240u : i);}
        if(is16(gw(w,0x28))>=0x100) {i=*ig(w,IG_LINK)+8u;*ig(w,IG_LINK)=(uint8_t)(i>255u ? 240u : i);}
        montage_brightness(w);montage_flash(w);wind(w,1,0x10b);break;
    case 6:
        y=(gw(w,0x294)+1u)&65535u;
        if(y>0x1a0) {
            step(w);sw(w,IG_TIMER,3);sw(w,IG_AUX,0);palette_bank(w,0x9280,1);order(w,0x2d0e,0x4621);y=0x1a0;
            map(w,0,0x38,0x902000);map(w,0,0x39,0x903000);
        }
        sw(w,0x294,y);sw(w,0x24,y+0x50);montage_flash(w);break;
    case 8:
        if(countdown(w,IG_TIMER)) switch(gw(w,IG_AUX)) {
        case 0:
            sw(w,IG_AUX,2);sw(w,IG_TIMER,3);intro_world_reset_actors(w);palette_bank(w,0x90c0,0);
            sw(w,0x2a,0x440);sw(w,0x2c,0x700);
            for(i=0;i<64;i++) ip16(w->video+0xcac0+2*i,0xf000);
            break;
        case 2:
            sw(w,IG_AUX,4);sw(w,IG_TIMER,3);sw(w,IG_INTERVAL,0);scroll(w,0,0,0,0,0x240,0x700);palette_bank(w,0x92a0,0);
            if(spawn(w,0x8800)) for(i=0;i<16;i++) if(!spawn(w,0x8801)) break;
            break;
        case 4:
            step(w);map(w,1,0x54,0x905000);map(w,1,0x54,0x905400);map(w,1,0x55,0x905800);
            for(i=0;i<3;i++) map(w,1,0x56,0x906000+1024*i);
            palette_bank(w,0x90c0,0);order(w,0x390e,0x6421);pal(w,0x2a11e0,0x90c0e0,64,-1);break;
        default:w->error="intro: montage flash state";return;
        }
        break;
    case 10:
        if(gw(w,IG_INTERVAL)==0x8800) {
            step(w);
            for(i=0;i<4;i++) if(!spawn(w,0x8900+i)) break;
            if(i!=4) break;
            /* These original writes change the kind byte only. */
            a=spawn(w,0x8c00);if(!a) break;
            a=spawn(w,0x8d00);if(!a) break;
            sl(w,0x210,0x400000);sl(w,0x214,0);sl(w,0x290,0x400000);sl(w,0x294,0x3000000);
            sl(w,0x310,0x400000);sl(w,0x314,0);sl(w,0x390,0);sl(w,0x394,0);
            camera_scroll(w);sw(w,0x2b2,5);sw(w,0x2b4,0);
        }
        break;
    case 12:
        step(w);sw(w,IG_TIMER,80);order(w,0x1b0e,0x2461);sw(w,IG_STAGE,0x24);
        pal(w,0x26d0e0,0x90c160,16,0xf000);pal(w,0x26d4a0,0x90c180,16,0xf000);
        pal(w,0x275c40+0x24*112,0x90c000,112,0xf000);
        pal(w,0x27c1c0+0x24*512,0x90c400,512,0xf000);pal(w,0x2865c0+0x24*512,0x90c800,512,0xf000);
        pal(w,0x2909c0+0x24*512,0x90cc00,512,0xf000);return;
    case 14:
        if(countdown(w,IG_TIMER)) {
            sw(w,IG_TIMER,1);
            if(*ig(w,IG_FREE)>=2) {
                sw(w,IG_SCENE,gw(w,IG_SCENE)+2);sw(w,IG_DELAY,32);memset(ig(w,IG_STEP),0,16);
                if(spawn(w,0x8a00) && spawn(w,0x8a01)) pal(w,0x2a1280,0x90c1a0,160,0xf000);
            }
        }
        montage_camera(w);frame(w,0);return;
    default:w->error="intro: montage director state";return;
    }
    frame(w,1);
}

static void duel_particles(struct intro_world *w,int sparks)
{
    unsigned r,i;uint8_t *a;if(!countdown(w,IG_INTERVAL)) return;
    r=intro_world_random(w);sw(w,IG_INTERVAL,(r&3u)+1u);
    if(sparks) {spawn(w,0x8b06);return;}
    if(*ig(w,IG_FREE)<=6) return;
    for(i=0;i<2;i++) {a=spawn(w,0x8b00);if(!a) return;a[IA_DELAY]=(uint8_t)(r&31u);a[IA_VARIANT]=(uint8_t)i;}
}
static void duel_scene(struct intro_world *w)
{
    unsigned i,j;uint16_t ids[4];uint8_t *a;
    switch(gw(w,IG_STEP)) {
    case 0:
        if(gw(w,IG_INTERVAL)!=0x8a8a || !countdown(w,IG_DELAY)) {montage_camera(w);frame(w,0);return;}
        step(w);sw(w,IG_TIMER,16);sw(w,IG_INTERVAL,4);intro_world_reset_actors(w);
        for(i=0;i<3;i++) {sw(w,0x210+i*128,64);sw(w,0x214+i*128,i==0?256:i==1?768:0x700);}
        camera_scroll(w);memcpy(ig(w,0x4a),ig(w,IG_SCROLL),12);order(w,0x2d0e,0x4621);
        intro_world_fill_map(w,0,15,0,0x900000);map(w,0,0x3d,0x901000);
        map(w,1,0x8e,0x904000);map(w,1,0x8f,0x904400);map(w,1,0x90,0x905000);map(w,1,0x91,0x905400);
        map(w,1,0x96,0x906000);map(w,1,0x97,0x906400);
        map(w,2,0x45,0x908000);intro_world_fill_map(w,2,15,0,0x908100);map(w,2,0x71,0x908800);map(w,2,0x72,0x908900);
        sw(w,IG_PALETTE_SET,0);pal(w,0x2811c0,0x90c400,512,0xf000);pal(w,0x2959c0,0x90cc00,512,0xf000);
        if(spawn(w,0x8b0a)) {pal(w,0x2a1440,0x90c2e0,64,0xf000);pal(w,0x2ab500,0x90c860,240,0xf000);}return;
    case 2:
        if(countdown(w,IG_TIMER)) {
            sw(w,IG_TIMER,1);
            if(*ig(w,IG_FREE)>=2) {step(w);sw(w,IG_SIGNAL,0);for(i=0;i<2;i++) {a=spawn(w,0x8b01);if(a) a[IA_VARIANT]=(uint8_t)i;}}
        }
        duel_particles(w,0);break;
    case 4:
        if(gw(w,IG_SIGNAL)==0x8b8b && spawn(w,0x8b03)) {step(w);sw(w,IG_SIGNAL,0);}
        duel_particles(w,0);break;
    case 6:
        if(*ig(w,IG_SIGNAL)==0x8b && *ig(w,IG_FREE)>=4) {
            step(w);sw(w,IG_TIMER,72);sw(w,0x290,64);sw(w,0x294,512);sw(w,0x310,64);sw(w,0x314,0x600);
            sw(w,0x26,64);sw(w,0x28,512);sw(w,0x2a,64);sw(w,0x2c,0x600);
            for(i=0;i<4;i++) {a=spawn(w,0x8b04+i/2);if(a) a[IA_VARIANT]=(uint8_t)(i&1u);}
        }
        duel_particles(w,0);break;
    case 8:
        if(countdown(w,IG_TIMER) && *ig(w,IG_FREE)>=8) {
            step(w);sw(w,IG_TIMER,256);sw(w,IG_SIGNAL,0);sw(w,0x290,64);sw(w,0x294,256);sw(w,0x26,64);sw(w,0x28,256);
            for(i=0;i<2;i++) if(group(w,4,ids)) {
                activate(w,ids[0],0x8b07);w->ram[ids[0]+IA_VARIANT]=(uint8_t)i;
                for(j=1;j<4;j++) {activate(w,ids[4-j],0x8b08);w->ram[ids[4-j]+IA_VARIANT]=(uint8_t)i;w->ram[ids[4-j]+IA_DELAY]=(uint8_t)j;}
            }
            pal(w,0x2a1ae0,0x90c000,64,0xf000);
        }
        duel_particles(w,1);break;
    case 10:if(gw(w,IG_SIGNAL)==0x8b8b) {step(w);fade(w,0x1e01,15);}break;
    case 12:
        if(!*ig(w,IG_FADE_WAIT)) {sw(w,IG_SCENE,gw(w,IG_SCENE)+2);sw(w,IG_STEP,0);memset(ig(w,IG_AUX),0,12);palette_bank(w,0x9280,1);}break;
    default:w->error="intro: duel director state";return;
    }
    frame(w,0);
}

static void logo_color(struct intro_world *w)
{
    unsigned i,shift,threshold=w->ram[0xc76]&15u;
    const uint8_t *p=intro_world_table(w,INTRO_DATA,(uint32_t)(0x295a60+is16(gw(w,IG_PALETTE_SET)*512u)),32);
    if(!p) return;
    for(i=0;i<16;i++) {
        unsigned current=iw16(w->video+0xcca0+2*i),target=iw16(p+2*i),result=current&0xf000u;
        for(shift=0;shift<12;shift+=4) {
            unsigned channel=(current>>shift)&15u;
            if(channel>threshold && channel>((target>>shift)&15u)) channel=threshold;
            result|=channel<<shift;
        }
        ip16(w->video+0xcca0+2*i,result);
    }
}
static void logo_scene(struct intro_world *w)
{
    unsigned i,variant;uint16_t ids[4],id;uint8_t *a;const uint8_t *p;
    switch(gw(w,IG_STEP)) {
    case 0:
        begin(w);scroll(w,0,256,0,0,0,0x700);sw(w,IG_PALETTE_SET,0);
        map(w,2,0x6f,0x908000);map(w,2,0x70,0x908100);order(w,0x1b0e,0x2461);palette_bank(w,0x90c0,0);
        memset(w->video+0xc000,255,4096);ip16(w->ram+0xc76,0xf00);return;
    case 2:
        ip16(w->ram+0xc76,iw16(w->ram+0xc76)-0x30);
        if(!iw16(w->ram+0xc76)) {
            step(w);sl(w,IG_FLAGS,0);
            if(group(w,4,ids)) {
                static const unsigned slots[]={3,2,0,1};
                p=intro_world_table(w,INTRO_INLINE_DATA,0x429c+(w->ram[0xc68]&6u),2);if(!p) return;variant=iw16(p);
                for(i=0;i<4;i++) {activate(w,ids[slots[i]],0x8100+i);w->ram[ids[slots[i]]+IA_VARIANT]=(uint8_t)variant;}
                intro_world_palettes(w,1);return;
            }
        }
        logo_color(w);return;
    case 4:if(gl(w,IG_FLAGS)==0x81818181u) {step(w);sw(w,IG_TIMER,8);palette_bank(w,0x9280,1);}break;
    case 6:
        if(countdown(w,IG_TIMER)) {
            step(w);sw(w,IG_TIMER,256);sl(w,IG_FLAGS,0);
            if(!w->title) {w->error="intro: native title lettering missing";return;}
            if(w->title(w)) {if(!w->error) w->error="intro: title lettering failed";return;}
            id=intro_world_alloc(w,2);
            if(!id) {w->error="intro: title actor pool exhausted";return;}
            a=w->ram+id;a[IA_ACTIVE]++;a[IA_KIND]=8;
            a[IA_SUBTYPE]=(uint8_t)(*ig(w,0x9c) ? 12 : *ig(w,0x9d) ? 13 : 13+*ig(w,0x72));ip16(a+IA_PALETTE_MODE,0x11e);
            palette_bank(w,0x90c0,0);
        } else palette_bank(w,*ig(w,IG_TIMER+1)&1u ? 0x9280 : 0x92a0,0);
        break;
    case 8:
        if(gw(w,IG_LINK)==0x8181 && countdown(w,IG_TIMER)) {step(w);fade(w,1,0);}
        if(gw(w,IG_TIMER)==255) w->sound_command=0x100; /* Title sound cue (original queue command 0xbd, parameter 0x1000). */
        intro_world_update_actors(w,0);intro_world_update_actors(w,2);goto present;
    case 10:
        if(!*ig(w,IG_FADE_WAIT)) {sw(w,IG_EXIT,gw(w,IG_EXIT)+2);memset(ig(w,IG_SCENE),0,20);palette_bank(w,0x92a0,1);}
        intro_world_update_actors(w,0);intro_world_update_actors(w,2);goto present;
    default:w->error="intro: logo director state";return;
    }
    frame(w,0);return;
present:
    if(w->error) return;
    if(!w->present) w->error="intro: native presentation missing";
    else if(w->present(w)) {if(!w->error) w->error="intro: presentation failed";}
    else w->rendered++;
}

int intro_director_step(struct intro_world *w)
{
    if(!w || w->error) return -1;
    if(gw(w,IG_EXIT)) return 1;
    switch(gw(w,IG_SCENE)) {
    case 0:fire_scene(w);break;
    case 2:strips_scene(w);break;
    case 4:portraits_scene(w);break;
    case 6:fighters_scene(w);break;
    case 8:headshots_scene(w);break;
    case 10:montage_scene(w);break;
    case 12:duel_scene(w);break;
    case 14:logo_scene(w);break;
    default:w->error="intro: director scene index";
    }
    return w->error ? -1 : gw(w,IG_EXIT) ? 1 : 0;
}
