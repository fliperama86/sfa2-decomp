#include "../resources.h"
#include <assert.h>
#include <string.h>
static uint8_t data[128];
static void fixture(void)
{
    memset(data,0,sizeof data);ip32(data,2);ip32(data+4,1);
    ip32(data+8,0);ip32(data+12,0x1234);ip32(data+16,6);ip32(data+20,52);
    ip32(data+24,1);ip32(data+28,0x4567);ip32(data+32,6);ip32(data+36,58);
    ip32(data+40,0x08000003);ip32(data+44,64);ip32(data+48,64);
    memset(data+52,0xa5,6);memset(data+58,0x5a,6);memset(data+64,15,64);
}
int main(void)
{
    struct intro_resources r;unsigned i;
    fixture();assert(intro_resources_init(&r,data,sizeof data)==0);
    assert(intro_resources_read(&r,0,0x1234,6)==data+52);assert(intro_resources_read(&r,0,0x1237,3)==data+55);
    assert(intro_resources_read(&r,1,0x4567,6)==data+58);
    assert(!intro_resources_read(&r,0,0x1233,1));assert(!intro_resources_read(&r,0,0x1239,2));
    assert(!intro_resources_read(&r,0,0x4567,1));assert(!intro_resources_read(&r,2,0x1234,1));
    assert(!intro_resources_read(&r,1,0x4567,(size_t)-1));
    assert(intro_resources_tile(&r,8,1,1)==data+64);assert(!intro_resources_tile(&r,8,1,0));
    assert(!intro_resources_tile(&r,16,1,1));assert(!intro_resources_tile(&r,8,0x800001,1));
    for(i=0;i<sizeof data;i++) {fixture();assert(intro_resources_init(&r,data,i)==-1 && !r.payload);}
    fixture();ip32(data,32769);assert(intro_resources_init(&r,data,sizeof data)==-1);
    fixture();ip32(data+24,0);ip32(data+28,0x1235);assert(intro_resources_init(&r,data,sizeof data)==-1);
    fixture();ip32(data+20,53);assert(intro_resources_init(&r,data,sizeof data)==-1);
    fixture();ip32(data+16,0x300000);assert(intro_resources_init(&r,data,sizeof data)==-1);
    fixture();ip32(data+40,0x10000003);assert(intro_resources_init(&r,data,sizeof data)==-1);
    fixture();data[127]=16;assert(intro_resources_init(&r,data,sizeof data)==-1);
    return 0;
}
