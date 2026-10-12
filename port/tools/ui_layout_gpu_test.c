/* Generated pixels through the whole GPU bridge, no game/disc/audio.
 * Link this fixture with gpu.c and an already built patched SDL_GPU SDK. */
#include "gpu.h"
#include <psyz.h>
#include <libgpu.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#ifdef _WIN32
#include <minwindef.h>
#include <memoryapi.h>
#else
#include <sys/mman.h>
#endif
void port_halt(int code,const char *fmt,...)
{
    va_list args;va_start(args,fmt);vfprintf(stderr,fmt,args);va_end(args);exit(code?code:1);
}
void port_tick(void) {}
void port_callbacks_reset(void) {}
int port_game_span(const void *p,size_t n)
{
    uintptr_t a=(uintptr_t)p;
    return a>=PORT_RAM_BASE && a<PORT_RAM_BASE+PORT_RAM_SIZE && n<=PORT_RAM_BASE+PORT_RAM_SIZE-a;
}
static void *library(const char *name)
{
    const struct port_library *p;
    for(p=port_gpu_library;p->name;p++)if(!strcmp(p->name,name))return p->host;
    assert(!"missing GPU bridge entry");return NULL;
}
static uint32_t *at(uint32_t a) { return (uint32_t *)(uintptr_t)a; }
static void append(uint32_t base,unsigned index,uint32_t addr,const uint32_t *words,unsigned len)
{
    unsigned i;uint32_t *p=at(addr);
    void (*add)(uint32_t *,uint32_t *)=library("AddPrim");
    p[0]=len<<24;for(i=0;i<len;i++)p[1+i]=words[i];add(at(base+index*4),p);
}
int main(void)
{
    int (*reset)(int)=library("ResetGraph");
    void *(*drawdef)(void *,int,int,int,int)=library("SetDefDrawEnv");
    void *(*dispdef)(void *,int,int,int,int)=library("SetDefDispEnv");
    void (*drawput)(void *)=library("PutDrawEnv");
    void *(*dispput)(void *)=library("PutDispEnv");
    void (*drawlist)(void *)=library("DrawOTag");
    unsigned frame,flags,i,x,y;unsigned long checked=0;void *ram;
    unsigned short palette[16]={0,0x7fff},packed[64],pixels[384*224];
    unsigned char *saved=malloc(PORT_RAM_SIZE);RECT r;
#ifdef _WIN32
    ram=VirtualAlloc((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
#else
    ram=mmap((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
    assert(ram==(void *)(uintptr_t)PORT_RAM_BASE && saved);
    reset(0);drawdef(at(0x80100600),0,8,384,224);drawput(at(0x80100600));
    dispdef(at(0x80100700),0,0,384,240);
    r=(RECT){0,480,16,1};LoadImage(&r,(u_long *)palette);
    for(i=0;i<64;i++)packed[i]=0x1111;
    r=(RECT){640,256,4,16};LoadImage(&r,(u_long *)packed);
    for(frame=0;frame<2;frame++)for(flags=0;flags<8;flags++) {
        uint32_t base=frame?0x801fc0e8u:0x801fc000u;
        uint32_t page=getTPage(0,0,640,256),clut=getClut(0,480);
        uint32_t font[]={0x64808080,40u<<16|40,clut<<16,8u<<16|8};
        uint32_t world[]={0x60ff0000,100u<<16|300,8u<<16|8};
        uint32_t life[]={0x6000ffff,18u<<16|8,5u<<16|144};
        uint32_t hud[]={0x2c808080,80u<<16|80,clut<<16,80u<<16|96,page<<16|15,
            96u<<16|80,15u<<8,96u<<16|96,15u<<8|15};
        uint32_t strip[]={0xe1000000|page,0xe2000000,0x03000000,0x7c808080,104u<<16|120,clut<<16};
        uint32_t mode[]={0xe1000000|page,0xe2000000};
        int dx=(flags&2)&&(flags&4)?8:0,life_y=flags&1?17:18,life_h=flags&1?6:5;
        assert(!port_gpu_set_video_options(!!(flags&1),!!(flags&2)));
        assert(!port_gpu_set_ui_layout(!!(flags&4)));dispput(at(0x80100700));
        for(i=0;i<30;i++)*at(base+i*4)=i?(base+(i-1)*4)&0xffffffu:0xffffffu;
        append(base,14,0x80100200,world,3);
        append(base,14,0x801ac6ac,life,3);
        append(base,14,0x8018a4bc+frame*0x1400,font,4);
        append(base,14,0x80188d6c+frame*0x38,strip,6);
        append(base,14,0x8018ccbc,mode,2);
        append(base,17,0x80186004+frame*0x1680,hud,9);
        r=(RECT){0,8,384,224};ClearImage(&r,0,255,0);
        memcpy(saved,ram,PORT_RAM_SIZE);drawlist(at(base+29*4));DrawSync(0);
        StoreImage(&r,(u_long *)pixels);assert(!memcmp(saved,ram,PORT_RAM_SIZE));
        for(y=0;y<224;y++)for(x=0;x<384;x++) {
            unsigned short expected=0x03e0;
            if(x>=8u+(unsigned)dx && x<152u+(unsigned)dx && y>=(unsigned)life_y && y<(unsigned)(life_y+life_h))expected=0x03ff;
            if((x>=40u+(unsigned)dx && x<48u+(unsigned)dx && y>=40 && y<48) ||
               (x>=80u+(unsigned)dx && x<96u+(unsigned)dx && y>=80 && y<96) ||
               (x>=120u+(unsigned)dx && x<136u+(unsigned)dx && y>=104 && y<120))expected=0x7fff;
            if(x>=300 && x<308 && y>=100 && y<108)expected=0x7c00;
            /* This checks placement/color classes, not exact RGB quantization.
             * The existing renderer may reduce a full texel channel by a step. */
            { unsigned got=pixels[y*384+x]&0x7fff;
              int same=((expected&31)?(got&31)>=29:(got&31)==0) &&
                  ((expected&0x3e0)?((got>>5)&31)>=29:((got>>5)&31)==0) &&
                  ((expected&0x7c00)?((got>>10)&31)>=29:((got>>10)&31)==0);
            if(!same) {
                fprintf(stderr,"UI pixel mismatch frame%u flags%u at%u,%u: got%04x expected%04x\n",frame,flags,x,y,pixels[y*384+x],expected);
                return 1;
            }
            checked++;
            }
        }
    }
    /* The setting cannot move UI in a movie/other display mode. */
    dispdef(at(0x80100700),0,0,320,240);dispput(at(0x80100700));
    { uint32_t base=0x801fc000u,font[]={0x64808080,40u<<16|40,getClut(0,480)<<16,8u<<16|8};
      for(i=0;i<30;i++)*at(base+i*4)=i?(base+(i-1)*4)&0xffffffu:0xffffffu;
      append(base,14,0x8018a4bc,font,4);r=(RECT){0,8,384,224};ClearImage(&r,0,255,0);
      drawlist(at(base+29*4));DrawSync(0);StoreImage(&r,(u_long *)pixels);
      assert((pixels[40*384+40]&31)>=29 && ((pixels[40*384+40]>>10)&31)>=29);
      assert((pixels[40*384+48]&0x7fff)==0x03e0); }
    printf("UI GPU: %lu generated pixels pass, both frame tables/all8 flag states, HUD/font/compound sprite/life composition, RAM/world identity, other-mode gate\n",checked);
    free(saved);
#ifdef _WIN32
    assert(VirtualFree(ram,0,MEM_RELEASE));
#else
    assert(!munmap(ram,PORT_RAM_SIZE));
#endif
    return 0;
}
