#include "../presentation.h"
#include <assert.h>
#include <string.h>
static struct intro_world w;
static struct intro_presentation output;
static uint8_t resources[0xa0000];
static const uint8_t *read_data(void *context,unsigned plane,uint32_t address,size_t size)
{(void)context;return plane==INTRO_DATA && address<=sizeof resources && size<=sizeof resources-address ? resources+address : NULL;}
static uint8_t *setup(unsigned mode)
{
    uint16_t id;uint8_t *a,*p=resources+0x2100;unsigned header;
    memset(&w,0,sizeof w);memset(&output,0,sizeof output);memset(resources,0,sizeof resources);w.read=read_data;
    intro_world_reset_actors(&w);id=intro_world_alloc(&w,0);assert(id);a=w.ram+id;
    a[IA_ACTIVE]=a[IA_VISIBLE]=1;ip16(a+IA_PRIORITY,0xe000);ip16(a+IA_BANK,0x2000);ip32(a+IA_SEQUENCE,0x2000);
    ip32(resources+0x2004,0x2100);ip16(p,mode);ip16(p+2,1);
    ip16(resources+0x2200,5);ip16(resources+0x2202,0xffec);ip32(resources+0x9f264,0x2200);
    if(mode==0) {ip16(p+4,9);ip16(p+6,4);header=8;}
    else if(mode==2) {ip16(p+6,4);header=8;}
    else if(mode==4) header=6;
    else if(mode==6) {ip16(p+6,9);ip32(p+8,0x2200);header=12;}
    else {ip32(p+6,0x2200);header=10;}
    ip16(p+header,17);
    if(mode==2 || mode==4 || mode==8) ip16(p+header+2,9);
    if(mode==4) {ip16(p+header+4,5);ip16(p+header+6,0xffec);}
    sw(&w,0x62dc,id);*ig(&w,0x611e)=1;sw(&w,0x6104,0xe2dc);
    return a;
}
int main(void)
{
    unsigned mode;uint8_t *a;uint16_t id;struct intro_video v;
    for(mode=0;mode<=8;mode+=2) {
        a=setup(mode);
        assert(intro_present_objects(&w,&output)==1 && !w.error);
        assert(output.count==5 && iw16(output.objects+32)==0xe045 && iw16(output.objects+34)==0x20ec);
        assert(iw16(output.objects+36)==(mode>=4 ? 0xa411 : 17));assert(iw16(output.objects+38)==9);
        assert(iw32(output.objects+40)==0x8000);assert(gw(&w,0x11a)==1 && *ig(&w,0x611e)==0);
        assert(intro_present_objects(&w,&output)==0);
    }
    a=setup(6);ip16(a+IA_SCALE_X,16);ip16(a+IA_ANGLE,1);ip16(resources+0x1312a,256);ip16(resources+0x13130,256);
    assert(intro_present_objects(&w,&output)==1);assert(iw16(output.objects+32)==0xe052);
    a=setup(2);a[IA_CAMERA]=8;a[IA_SHAKE]=1;sw(&w,0x290,100);sw(&w,0x294,200);sw(&w,0x14e,3);
    assert(intro_present_objects(&w,&output)==1);assert(iw16(output.objects+32)==0xe3e1);assert(iw16(output.objects+34)==0x21b1);
    a=setup(4);id=intro_world_alloc(&w,1);assert(id);memcpy(w.ram+id,a,128);w.ram[id+IA_CLASS]=20;ip16(w.ram+id+IA_X,100);
    ip16(a+0x2e,id);ip16(a+0x28,id);
    assert(intro_present_objects(&w,&output)==1 && output.count==7);
    assert(iw16(output.objects+32)==0xe0a9 && iw16(output.objects+40)==0xe045 && iw16(output.objects+48)==0xe0a9);
    a=setup(4);*ig(&w,0x611e)=0;id=intro_world_alloc(&w,2);assert(id);a=w.ram+id;a[0]=a[1]=1;
    ip16(a+IA_Y,256);ip16(a+IA_PRIORITY,0xe000);ip16(a+IA_PALETTE_MODE,0x011e);ip32(a+IA_SEQUENCE,0xffff0000u+id+128u);
    memcpy(a+128,resources+0x2100,14);
    assert(intro_present_objects(&w,&output)==1 && output.count==5);assert(iw16(output.objects+38)==30);
    assert(iw16(output.objects+34)==0x23ec);
    w.clear_objects=1;intro_present_clear(&w,&output);assert(output.count==4 && iw32(output.objects+32)==0x80008000u);
    sw(&w,0x18,0x9000);sw(&w,0x1a,0x9040);sw(&w,0x1c,0x9080);sw(&w,0x20,0x90c0);sw(&w,0x1e,0x90e0);
    sw(&w,0x22,64);sw(&w,0x24,256);sw(&w,0x26,256);sw(&w,0x28,768);sw(&w,0x2a,64);sw(&w,0x2c,1792);
    intro_present_video(&w,&output,&v,NULL,NULL);assert(v.map_base[1]==0x4000 && v.palette_base==0xc000 && v.row_base==0xe000);
    assert(v.scroll_x[0]==0 && v.scroll_y[0]==0 && v.scroll_x[1]==0 && v.scroll_y[1]==0 && v.scroll_x[2]==0 && v.scroll_y[2]==0 && v.object_count==0 && v.objects==output.objects+32);
    a=setup(4);ip32(resources+0x2004,0xa0000);assert(intro_present_objects(&w,&output)==-1 && w.error);
    a=setup(4);a[IA_CAMERA]=8;ip16(a+IA_ROW,1);assert(intro_present_objects(&w,&output)==-1 && w.error);
    a=setup(4);*ig(&w,0x611e)=45;assert(intro_present_objects(&w,&output)==-1 && w.error);
    a=setup(4);ip16(resources+0x2104,65535);assert(intro_present_objects(&w,&output)==-1 && w.error);
    return 0;
}
