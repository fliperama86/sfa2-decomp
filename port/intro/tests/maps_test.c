#include "../world.h"
#include <assert.h>
#include <string.h>
static struct intro_world w;
static uint8_t input[30];
static const uint8_t *read_block(void *context,unsigned plane,uint32_t address,size_t n)
{(void)context;return plane==INTRO_DATA && address>=0x200000 && address<=0x20001e && n<=0x20001e - address ? input+address-0x200000 : NULL;}
static void reset(void)
{memset(&w,0,sizeof w);w.read=read_block;sw(&w,0x18,0x9000);sw(&w,0x1a,0x9040);sw(&w,0x1c,0x9080);}
int main(void)
{
    static const uint32_t cells[3][6]={
        {0x90007c,0x902000,0x902004,0x9000fc,0x902080,0x902084},
        {0x90403c,0x905000,0x905004,0x90407c,0x905040,0x905044},
        {0x90801c,0x908800,0x908804,0x90803c,0x908820,0x908824}
    };
    static const unsigned bias[]={0,0xa400,0x900},last[]={0x1f80,0xfc0,0x7e0};unsigned layer,i;
    ip16(input,0x1234);ip16(input+2,2);ip16(input+4,1);
    for(i=0;i<6;i++) {ip16(input+6+4*i,i+1);ip16(input+8+4*i,0x101+i);}
    for(layer=0;layer<3;layer++) {
        reset();intro_map_write(&w,layer,0x200000,cells[layer][0],1);assert(!w.error);
        for(i=0;i<6;i++) {const uint8_t *p=w.video+cells[layer][i]-0x900000;assert(iw16(p)==bias[layer]+i+1);assert(iw16(p+2)==0x101+i);}
        intro_map_write(&w,layer,0x200000,cells[layer][0],0);assert(!w.error);
        for(i=0;i<6;i++) assert(iw16(w.video+cells[layer][i]-0x900000)==bias[layer]+(layer==2 ? 0 : 32));
    }
    ip16(input+2,0);ip16(input+4,1);
    for(layer=0;layer<3;layer++) {
        uint32_t base=0x900000+layer*0x4000;
        reset();intro_map_write(&w,layer,0x200000,base+last[layer],1);assert(!w.error);
        assert(iw16(w.video+base-0x900000+last[layer])==bias[layer]+1);
        assert(iw16(w.video+base-0x900000)==bias[layer]+2);
    }
    reset();assert(intro_map_position(&w,0,0,0)==0x90207c);assert(intro_map_position(&w,1,0,0)==0x904000);assert(intro_map_position(&w,2,0,0)==0x908000);
    assert(intro_map_position(&w,0,8,8)==0x9020f8);assert(intro_map_position(&w,1,16,16)==0x90707c);assert(intro_map_position(&w,2,32,32)==0x90b83c);
    reset();ip16(input+2,65535);intro_map_write(&w,0,0x200000,0x900000,1);assert(w.error);
    return 0;
}
