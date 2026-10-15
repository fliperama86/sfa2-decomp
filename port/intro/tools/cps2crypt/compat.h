/* Small project-authored compatibility surface for the offline BSD cipher. */
#ifndef INTRO_CPS2CRYPT_COMPAT_H
#define INTRO_CPS2CRYPT_COMPAT_H
#include <cstdint>
#define BIT(value,bit) (((value)>>(bit))&1u)
template<int N,typename... T> uint32_t bitswap(uint32_t value,T... indices)
{
    static_assert(sizeof...(indices)==N,"bit permutation width");
    const int bits[]={indices...};uint32_t result=0;
    for(int bit:bits) result=(result<<1)|BIT(value,bit);
    return result;
}
#endif
