/* Importer-only boundary. No CPU execution and no key or ROM is compiled in. */
#include <cstddef>
#include <cstdint>
#include <new>
#include <vector>
void cps2_decrypt(uint16_t *,uint16_t *,int,const uint32_t *,uint32_t,uint32_t);
extern "C" int intro_import_decrypt(const uint8_t *data,size_t size,const uint8_t *key,size_t key_size,uint8_t *out)
{
    if(!data || !key || !out || size!=0x300000u || key_size!=20) return -1;
    try {
        uint16_t decoded[10]={};
        for(unsigned bit=0;bit<160;bit++) {
            unsigned source=(317u-bit)%160u;
            if((key[source/8]>>((source^7u)%8))&1u) decoded[bit/16]|=(uint16_t)(0x8000u>>(bit%16));
        }
        uint32_t master[2]={(uint32_t)decoded[0]<<16|decoded[1],(uint32_t)decoded[2]<<16|decoded[3]};
        uint32_t low=0,high=(((~decoded[9]&0x3ffu)<<14)|0x3fffu)+1u;
        if(decoded[9]==0xffffu) {low=0xff0000u;high=0xffffffu;}
        std::vector<uint16_t> input(size/2),output(size/2);
        for(size_t i=0;i<input.size();i++) input[i]=(uint16_t)((unsigned)data[i*2]*256u+data[i*2+1]);
        cps2_decrypt(input.data(),output.data(),(int)size,master,low/2,high/2);
        for(size_t i=0;i<output.size();i++) {out[i*2]=(uint8_t)(output[i]>>8);out[i*2+1]=(uint8_t)output[i];}
        return 0;
    } catch(const std::bad_alloc &) {return -1;}
}
