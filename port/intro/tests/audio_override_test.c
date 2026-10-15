/* Isolate the real queue selector. Dead unrelated drive functions are unlinked. */
#include "../../src/cd.c"
#include <assert.h>
static size_t replacement(short *out,size_t n)
{assert(n==1);out[0]=111;out[1]=-222;return 1;}
int main(void)
{
    short out[2];xa.count=2;xa.q[0]=3;xa.q[1]=4;xa.q[2]=5;xa.q[3]=6;
    assert(!port_cd_audio_override && port_xa_pull(out,1)==1 && out[0]==3 && xa.count==1);
    port_cd_audio_override=replacement;
    assert(port_xa_pull(out,1)==1 && out[0]==111 && out[1]==-222 && xa.count==1);
    port_cd_audio_override=NULL;
    assert(port_xa_pull(out,1)==1 && out[0]==5 && out[1]==6 && !xa.count);
    return 0;
}
