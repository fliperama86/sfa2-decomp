/* Pure generated UI-policy fixtures, no SDK/game/GPU/private input. */
#include "ui_layout.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
    static const struct { unsigned code,len,n;unsigned xy[4]; } shapes[]={
        {0x20,4,3,{1,2,3}}, {0x24,7,3,{1,3,5}}, {0x28,5,4,{1,2,3,4}},
        {0x2c,9,4,{1,3,5,7}}, {0x30,6,3,{1,3,5}}, {0x34,9,3,{1,4,7}},
        {0x38,8,4,{1,3,5,7}}, {0x3c,12,4,{1,4,7,10}},
        {0x40,3,2,{1,2}}, {0x50,4,2,{1,3}},
        {0x60,3,1,{1}}, {0x64,4,1,{1}}, {0x68,2,1,{1}}, {0x6c,3,1,{1}},
        {0x70,2,1,{1}}, {0x74,3,1,{1}}, {0x78,2,1,{1}}, {0x7c,3,1,{1}}
    };
    unsigned s,flag,i,j;uint32_t p[12],before[12],expected[12];
    assert(port_ui_center_offset(1,1,384)==8);assert(port_ui_center_offset(1,1,512)==72);
    assert(!port_ui_center_offset(0,1,384));assert(!port_ui_center_offset(1,0,384));
    assert(!port_ui_center_offset(1,1,368));assert(!port_ui_center_offset(1,1,INT_MIN));
    for(s=0;s<sizeof shapes/sizeof shapes[0];s++)for(flag=0;flag<4;flag++) {
        for(i=0;i<12;i++)p[i]=0xa5b6c7d8u+i;
        p[0]=(shapes[s].code+flag)<<24|0x314159;
        for(i=0;i<shapes[s].n;i++)p[shapes[s].xy[i]]=0x0040ffecu; /* x=-20,y64 */
        memcpy(before,p,sizeof p);memcpy(expected,p,sizeof p);
        assert(!port_ui_translate(p,shapes[s].len,0,1));assert(!memcmp(before,p,sizeof p));
        for(i=0;i<shapes[s].n;i++)expected[shapes[s].xy[i]]=0x0040fff4u; /* x=-12 */
        assert(port_ui_translate(p,shapes[s].len,8,0)==1);assert(!memcmp(expected,p,sizeof p));
        memcpy(p,before,sizeof p);assert(port_ui_translate(p,shapes[s].len-1,8,0)==-1);
        assert(!memcmp(before,p,sizeof p));
        p[shapes[s].xy[shapes[s].n-1]]=1023;
        memcpy(before,p,sizeof p);assert(port_ui_translate(p,shapes[s].len,8,0)==-1);
        assert(!memcmp(before,p,sizeof p));
    }
    { uint32_t life[3]={0x61ffff00u,18u<<16|216u,5u<<16|144u};
      assert(port_ui_translate(life,3,8,1)==1);
      assert(life[0]==0x61ffff00u && life[1]==(17u<<16|224u) && life[2]==(6u<<16|144u));
      life[1]=18u<<16|216u;life[2]=5u<<16|144u;
      assert(port_ui_translate(life,3,8,0)==1);assert(life[1]==(18u<<16|224u) && life[2]==(5u<<16|144u)); }
    for(i=0;i<6;i++) { p[0]=(0xe1u+i)<<24;memcpy(before,p,sizeof p);
        assert(!port_ui_translate(p,1,8,1));assert(!memcmp(before,p,sizeof p)); }
    p[0]=0x2c808080;
    assert(port_ui_packet_kind(0x80186004,p,9,17)==1);
    assert(port_ui_packet_kind(0x80188d04-40,p,9,7)==1);
    assert(!port_ui_packet_kind(0x80184984,p,9,14));
    assert(!port_ui_packet_kind(0x80188d04,p,9,14));
    assert(!port_ui_packet_kind(0x80186005,p,9,14));
    assert(!port_ui_packet_kind(0x80186004,p,9,-1));
    assert(!port_ui_packet_kind(0x80186004,p,8,14));
    p[0]=0x60808080;
    for(i=0;i<28;i++) {
        uint32_t addr=0x801ac6ac+i*16;int list=i<4?14:(i<8?15:(i>=24?7:8));
        int kind=i<8?2:(i>=12?1:0);
        assert(port_ui_packet_kind(addr,p,3,list)==kind);
        assert(!port_ui_packet_kind(addr,p,3,list+1));
    }
    /* Twelve meter fill records, three levels a side; their neighbours and other lists are none. */
    for(i=0;i<28;i++) {
        uint32_t addr=0x801ac6ac+i*16;int level=i>=12 && i<24?(int)((i-12)/2%3)+1:0;
        assert(port_ui_super_fill_level(addr,p,3,8)==level);
        assert(port_ui_super_fill_level(addr&0xffffffu,p,3,8)==level);
        assert(!port_ui_super_fill_level(addr,p,3,7) && !port_ui_super_fill_level(addr,p,3,14));
        assert(!port_ui_super_fill_level(addr+4,p,3,8) && !port_ui_super_fill_level(addr,p,4,8));
    }
    p[0]=0x64808080;assert(!port_ui_super_fill_level(0x801ac76c,p,3,8));
    for(i=1;i<=3;i++)for(j=0;j<=48;j++) {
        uint32_t fill[3]={0x6249c9f3u,213u<<16|(150u+i),(i==1?3u:4u)<<16|j},old[3];
        memcpy(old,fill,sizeof fill);
        assert(!port_ui_super_fill(fill,3,0) && !port_ui_super_fill(fill,3,4) && !port_ui_super_fill(fill,2,(int)i));
        assert(!memcmp(old,fill,sizeof fill));
        assert(port_ui_super_fill(fill,3,(int)i)==1);
        assert(fill[0]==old[0] && fill[1]==(212u<<16|(150u+i)) && fill[2]==((i+2)<<16|j));
        assert(!port_ui_super_fill(fill,3,(int)i)); /* Its own result is not a second input. */
    }
    { static const uint32_t bad[][3]={{0x60000000u,212u<<16|150,3u<<16|16},{0x60000000u,214u<<16|150,3u<<16|16},
          {0x60000000u,213u<<16|150,4u<<16|16},{0x60000000u,213u<<16|150,3u<<16|49},
          {0x60000000u,213u<<16|150,3u<<16|0xffff},{0x64000000u,213u<<16|150,3u<<16|16}};
      for(i=0;i<sizeof bad/sizeof bad[0];i++) { uint32_t q[3];memcpy(q,bad[i],sizeof q);
          assert(!port_ui_super_fill(q,3,1));assert(!memcmp(q,bad[i],sizeof q)); }
      { uint32_t q[3]={0x60000000u,213u<<16|150,3u<<16|16};assert(!port_ui_super_fill(q,3,2) && !port_ui_super_fill(q,3,3));
        q[2]=4u<<16|16;assert(!port_ui_super_fill(q,3,1));assert(port_ui_super_fill(q,3,3) && q[2]==(5u<<16|16)); } }
    p[0]=0xe100001f;
    for(i=0;i<12;i++)assert(port_ui_packet_kind(0x80188d6c+i*28,p,6,14)==1);
    for(i=0;i<16;i++)assert(port_ui_packet_kind(0x8018ff34+i*28,p,6,14)==1);
    assert(!port_ui_packet_kind(0x80188ebc,p,6,14));
    assert(!port_ui_packet_kind(0x801900f4,p,6,14));
    assert(!port_ui_packet_kind(0x80188d6c,p,6,13));
    p[0]=0x64808080;
    for(i=0;i<512;i++)for(j=0;j<30;j++)assert(port_ui_packet_kind(0x8018a4bc+i*20,p,4,(int)j)==1);
    assert(!port_ui_packet_kind(0x8018ccbc,p,4,14));
    assert(!port_ui_packet_kind(0x8018a4bd,p,4,14));
    assert(!port_ui_packet_kind(0x80100000,p,4,14)); /* A world sprite at the same depth. */
    puts("UI layout: 72 geometry forms, transactional edges, life composition, meter fill rows, exact buffers and canvas gates pass");
    return 0;
}
