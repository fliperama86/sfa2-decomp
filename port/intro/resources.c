#include "resources.h"
#include <string.h>
int intro_resources_init(struct intro_resources *out,const uint8_t *p,size_t n)
{
    struct intro_resources r;size_t cursor;unsigned i;uint64_t previous=0,end=0;uint32_t last_key=0;
    if(!out) return -1;
    memset(out,0,sizeof *out);
    if(!p || n<8 || n>INTRO_RESOURCE_LIMIT) return -1;
    memset(&r,0,sizeof r);r.payload=p;r.size=n;r.span_count=iw32(p);r.tile_count=iw32(p+4);
    if(!r.span_count || r.span_count>32768 || !r.tile_count || r.tile_count>65536) return -1;
    cursor=8u+(size_t)r.span_count*16u+(size_t)r.tile_count*12u;
    if(cursor>n) return -1;
    r.spans=p+8;r.tiles=r.spans+r.span_count*16u;
    for(i=0;i<r.span_count;i++) {
        const uint8_t *entry=r.spans+i*16u;
        unsigned plane=iw32(entry);uint32_t address=iw32(entry+4),size=iw32(entry+8),offset=iw32(entry+12);
        uint64_t key=((uint64_t)plane<<32)|address;
        if(plane>1 || address>=0x300000u || !size || size>0x300000u-address || offset!=cursor ||
           size>n-cursor || (i && (key<=previous || key<end))) return -1;
        previous=key;end=key+size;cursor+=size;
    }
    for(i=0;i<r.tile_count;i++) {
        const uint8_t *entry=r.tiles+i*12u;uint32_t key=iw32(entry),size=iw32(entry+4),offset=iw32(entry+8);
        unsigned side=key>>24,half=key&1u,code=(key&0xffffffu)>>1;size_t j;
        if((side!=8 && side!=16 && side!=32) || (side!=8 && half) ||
           code>=0x1400000u/(side==8 ? 64u : side*side/2u) || size!=side*side ||
           offset!=cursor || size>n-cursor || (i && key<=last_key)) return -1;
        for(j=0;j<size;j++) if(p[cursor+j]>15) return -1;
        last_key=key;cursor+=size;
    }
    if(cursor!=n) return -1;
    *out=r;return 0;
}
const uint8_t *intro_resources_read(void *context,unsigned plane,uint32_t address,size_t n)
{
    const struct intro_resources *r=context;unsigned low=0,high;uint64_t key;
    const uint8_t *entry;uint32_t start,size;
    if(!r || !r->payload || plane>1 || !n) return NULL;
    key=((uint64_t)plane<<32)|address;high=r->span_count;
    while(low<high) {
        unsigned mid=low+(high-low)/2;entry=r->spans+mid*16u;
        if((((uint64_t)iw32(entry)<<32)|iw32(entry+4))<=key) low=mid+1;else high=mid;
    }
    if(!low) return NULL;
    entry=r->spans+(low-1u)*16u;start=iw32(entry+4);size=iw32(entry+8);
    if(iw32(entry)!=plane || address<start || address-start>size || n>size-(address-start)) return NULL;
    return r->payload+iw32(entry+12)+(address-start);
}
const uint8_t *intro_resources_tile(void *context,unsigned side,uint32_t code,unsigned half)
{
    const struct intro_resources *r=context;unsigned low=0,high;uint32_t key;
    if(!r || !r->payload || (side!=8 && side!=16 && side!=32) || code>=0x800000u || half>1 || (side!=8 && half)) return NULL;
    key=(side<<24)|(code<<1)|half;high=r->tile_count;
    while(low<high) {
        unsigned mid=low+(high-low)/2;const uint8_t *entry=r->tiles+mid*12u;uint32_t found=iw32(entry);
        if(found==key) return r->payload+iw32(entry+8);
        if(found<key) low=mid+1;else high=mid;
    }
    return NULL;
}
