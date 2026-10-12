/* Generated-RAM test of the real AddPrim bridge, no game/disc or GPU startup.
 * Link gpu.c and this fixture with an already built SDK. Exits before video. */
#include "gpu.h"
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif
void port_halt(int code,const char *fmt,...)
{
    va_list args;va_start(args,fmt);vfprintf(stderr,fmt,args);va_end(args);
    exit(code?code:1);
}
void port_tick(void) { assert(!"unexpected frame-clock call"); }
void port_callbacks_reset(void) { assert(!"unexpected callback reset"); }
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
static void quad(unsigned char p[40])
{
    int i;memset(p,0,40);p[3]=9;p[7]=0x2c;p[22]=13;
    for(i=0;i<4;i++) {
        p[8+i*8]=(i&1)?16:0;p[10+i*8]=(i&2)?16:0;
        p[12+i*8]=(i&1)?31:16;p[13+i*8]=(i&2)?31:16;
    }
}
int main(void)
{
    void (*add)(uint32_t *,uint32_t *)=library("AddPrim");
    int frame,enabled,index;
    void *ram;
#ifdef _WIN32
    ram=VirtualAlloc((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
#else
    ram=mmap((void *)(uintptr_t)PORT_RAM_BASE,PORT_RAM_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
#endif
    assert(ram==(void *)(uintptr_t)PORT_RAM_BASE);
    /* Initialization/frame-swap values read from unchanged producer C, not
     * calculated by the selection helper being tested. The pointer lives elsewhere. */
    for(frame=0;frame<2;frame++)for(enabled=0;enabled<2;enabled++) {
        uint32_t base=frame?0x801fc0e8u:0x801fc000u;
        unsigned char *q=(unsigned char *)(uintptr_t)(0x80186004u+frame*0x1680u);
        *(uint32_t *)(uintptr_t)0x801987c8u=base;
        assert(!port_gpu_set_video_options(enabled,0));
        for(index=7;index<=15;index++) {
            uint32_t *ot=(uint32_t *)(uintptr_t)(*(uint32_t *)(uintptr_t)0x801987c8u+index*4);
            quad(q);*ot=0x00ffffffu;add(ot,(uint32_t *)q);
            assert(q[23]==(enabled&&(index==8||index==14)?32:0));
            assert((*ot&0xffffffu)==((uintptr_t)q&0x1fffffu));
            assert(*(uint32_t *)q==0x09ffffffu);
            assert(q[16]==16 && q[20]==31); /* No original geometry/UV changes. */
        }
        for(index=0;index<8;index++) {
            unsigned char *p=(unsigned char *)(uintptr_t)(0x801ac6acu+index*16);
            uint32_t *ot=(uint32_t *)(uintptr_t)(base+(index<4?14:15)*4);
            int x=(index&2)?216:8;
            memset(p,0,16);p[3]=3;p[7]=0x60;p[8]=(unsigned char)x;
            p[10]=18;p[12]=144;p[14]=5;*ot=0x00ffffffu;add(ot,(uint32_t *)p);
            assert(p[7]==(enabled?0x61:0x60));assert(p[10]==18 && p[14]==5 && p[12]==144);
        }
        /* The old mistaken pointer-storage address is a deliberate rejection. */
        for(index=8;index<=15;index++) {
            uint32_t *wrong=(uint32_t *)(uintptr_t)(0x801987c8u+index*4);
            quad(q);*wrong=0x00ffffffu;add(wrong,(uint32_t *)q);assert(!q[23]);
        }
        { unsigned char *p=(unsigned char *)(uintptr_t)0x801ac6acu;
          memset(p,0,16);p[3]=3;p[7]=0x60;p[8]=8;p[10]=18;p[12]=144;p[14]=5;
          add((uint32_t *)(uintptr_t)0x80198800u,(uint32_t *)p);assert(p[7]==0x60); }
    }
    assert(!port_gpu_set_video_options(0,0));
#ifdef _WIN32
    assert(VirtualFree(ram,0,MEM_RELEASE));
#else
    assert(!munmap(ram,PORT_RAM_SIZE));
#endif
    puts("GPU bridge: actual two frame-table pointer values, super/life selection, default-off and old-address rejection pass");
    return 0;
}
