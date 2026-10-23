#include "program.h"
#include <string.h>

/* Found by comparing the PS1 program's data with the arcade program, field
 * byte order aside: first runs of at least 32 bytes with at least 16 that are
 * not zero, in 16-bit fields; then (port/arcade/program_map.py) runs of at
 * least 32 bytes with 32-bit fields too, kept by the amount of evidence of the
 * run and the table it lies in. Each piece's layout is what its bytes show: a
 * field whose bytes are the same either way is given the layout of the rest.
 * `--arcade-check` compares every piece with the user's disc. The method and
 * its limits are in port/arcade/README.md. */
const struct arcade_piece arcade_program_pieces[] = {
    {0x8016e5c4u,   118u, 0x00d8dcu, "lbbbbbblbbbbbblbbbbbblbbbbbblbbbbbblbbbbbblbbbbbblb", "score_rank_table"},
    {0x8016ea70u,  2688u, 0x099500u, "b", "data_8016ea70+0x0"},
    {0x8016f6a8u,   912u, 0x0971aau, "l", "table_8016f658+0x50"},
    {0x8016fa8cu,   160u, 0x098870u, "l", "motion_speed_table"},
    {0x8016fb2cu,  1220u, 0x098928u, "l", "motion_speed_table+0xa0"},
    {0x8016fff4u,   128u, 0x098df0u, "l", "motion_recs+0x4a0"},
    {0x80170122u,    64u, 0x024ffau, "bbbbbbbbblbbbbbbbbbbbbbbbbbbbbb", "bytes_801700b0+0x72"},
    {0x80170172u,    54u, 0x025024u, "bbbbbbbbblbbbbbbbbbbbbbbbb", "bytes_801700b0+0xc2"},
    {0x801701bcu,    84u, 0x02508eu, "b", "bytes_801700b0+0x10c"},
    {0x8017021eu,    36u, 0x025002u, "bbbbblbbbbbbbbbbb", "bytes_801700b0+0x16e"},
    {0x80170274u,  1172u, 0x024098u, "l", "vec_table_80170274"},
    {0x8017070cu,  1752u, 0x024530u, "l", "vec_table_80170514+0x1f8"},
    {0x80170de6u,   502u, 0x024c0au, "bbbllbbbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbbllbbbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbblllbb", "vec_table_80170cf4+0xf2"},
    {0x80170ff0u,   388u, 0x024e14u, "l", "vec_table_80170f34+0xbc"},
    {0x80171174u,   672u, 0x098150u, "w", "table_80171174+0x0"},
    {0x80171414u,   260u, 0x0977f0u, "b", "kind_table_248"},
    {0x8017151au,    50u, 0x0978f6u, "b", "kind_table_248+0x106"},
    {0x8017155au,   170u, 0x097966u, "b", "kind_table_248+0x146"},
    {0x8017161cu,   152u, 0x097a28u, "b", "kind_table_29b+0xb8"},
    {0x801716b4u,    72u, 0x09772cu, "w", "data_801716b4+0x0"},
    {0x801716fcu,    76u, 0x0977a4u, "w", "data_801716b4+0x48"},
    {0x80176da0u,   512u, 0x01fc5eu, "b", "data_80176da0+0x0"},
    {0x80176fa0u,    32u, 0x010332u, "b", "data_80176da0+0x200"},
    {0x8017711cu,   352u, 0x011494u, "b", "table_8017711c+0x0"},
    {0x801777d4u,  1024u, 0x09c048u, "b", "table_801777d4+0x0"},
    {0x80177fd4u,   208u, 0x09c968u, "b", "table_80177fd4+0x0"},
    {0x801780a4u,    80u, 0x09c918u, "b", "table_80177fd4+0xd0"},
    {0x801780f4u,   100u, 0x09ca88u, "b", "table_80177fd4+0x120"},
    {0x80178174u,   100u, 0x09cb08u, "b", "table_80177fd4+0x1a0"},
    {0x801781f4u,   144u, 0x09cb88u, "b", "table_80177fd4+0x220"},
    {0x80178284u,    80u, 0x09c858u, "b", "table_80178274+0x10"},
    {0x801782d4u,   112u, 0x09cc68u, "b", "table_80178274+0x60"},
    {0x80178344u,   144u, 0x09c918u, "b", "table_80178274+0xd0"},
    {0x801783d4u,   132u, 0x09cd68u, "b", "table_80178274+0x160"},
    {0x80178458u,    92u, 0x09ca2cu, "b", "table_80178274+0x1e4"},
    {0x801784b4u,    64u, 0x09ce08u, "b", "table_80178274+0x240"},
    {0x801784f4u,    32u, 0x09cac8u, "b", "table_80178274+0x280"},
    {0x80178514u,  4904u, 0x09cea8u, "b", "table_80178514+0x0"},
    {0x8017984cu,  4224u, 0x09e1e0u, "b", "table_8017984c+0x0"},
    {0x8017ac7cu,    96u, 0x0270d2u, "b", "table_8017ac7c+0x0"},
    {0x8017acf4u,    96u, 0x0270d2u, "b", "quad_table+0x0"},
    {0x8017ae84u,    32u, 0x068812u, "w", "table_8017ad5c+0x128"},
    {0x8017aeb4u,    32u, 0x011a96u, "w", "table_8017ad5c+0x158"},
    {0x8017c9f0u,    46u, 0x05ca82u, "wbwwwwwwwwwwwwwwbbwb", "table_8017c9f0+0x0"},
    {0x8017cac4u,    32u, 0x05ca82u, "w", "table_8017cac4+0x0"},
    {0x8017cc28u,   256u, 0x05d722u, "l", "offsets_cc28"},
    {0x8017cd54u,   352u, 0x05db04u, "w", "table_8017cd54+0x0"},
    {0x8017ceccu,    32u, 0x05ca82u, "w", "data_8017cecc+0x0"},
    {0x8017cf7cu,    32u, 0x098f30u, "b", "data_8017cf7c"},
    {0x8017d0acu,   464u, 0x028bbeu, "b", "table_8017d0ac+0x0"},
    {0x8017d450u,    74u, 0x09af4au, "w", "table_8017d450+0x0"},
    {0x8017d49cu,    52u, 0x09af94u, "l", "table_8017d450+0x4c"},
    {0x8017d598u,    86u, 0x09bc4au, "w", "table_8017d598+0x0"},
    {0x8017d5eeu,    42u, 0x09bc9cu, "w", "table_8017d598+0x56"},
    {0x8017d618u,    94u, 0x09bccau, "w", "table_8017d618+0x0"},
    {0x8017d676u,    34u, 0x09bd24u, "w", "table_8017d618+0x5e"},
    {0x8017d698u,    82u, 0x09bd4au, "w", "data_8017d698+0x0"},
    {0x8017d6eeu,    42u, 0x09bd9cu, "w", "data_8017d698+0x56"},
    {0x8017d718u,    86u, 0x09bdcau, "w", "table_8017d718+0x0"},
    {0x8017d76eu,    42u, 0x09be1cu, "w", "table_8017d718+0x56"},
    {0x8017d7b6u,    98u, 0x09be64u, "w", "table_8017d798+0x1e"},
    {0x8017d818u,   102u, 0x09becau, "w", "data_8017d818+0x0"},
    {0x80181380u,    78u, 0x027be4u, "w", "table_80181380"},
    {0x801813d0u,    38u, 0x027c34u, "w", "table_80181380+0x50"},
    {0x801813f8u,    38u, 0x027c5cu, "w", "table_80181380+0x78"},
    {0x80181420u,    38u, 0x027c84u, "w", "table_80181380+0xa0"},
    {0x80181448u,    38u, 0x027cacu, "w", "table_80181380+0xc8"},
    {0x80181470u,    38u, 0x027cd4u, "w", "table_80181380+0xf0"},
    {0x80181498u,    38u, 0x027cfcu, "w", "table_80181380+0x118"},
    {0x801814c0u,    38u, 0x027d24u, "w", "table_80181380+0x140"},
    {0x801814e8u,    38u, 0x027d4cu, "w", "table_80181380+0x168"},
    {0x80181510u,    32u, 0x011418u, "b", "table_80181510+0x0"},
};
const unsigned arcade_program_piece_count = sizeof arcade_program_pieces / sizeof arcade_program_pieces[0];

/* The bytes of one field of the layout: 2 for 'b' and 'w', 4 for 'l', 0 for anything else. */
static unsigned field_size(char c)
{
    return c == 'b' || c == 'w' ? 2u : c == 'l' ? 4u : 0u;
}

/* The layout is well formed and, repeated, ends exactly at the end of size bytes. */
static int layout_ok(const char *units, unsigned size)
{
    size_t i = 0, n = units ? strlen(units) : 0;
    unsigned at = 0;
    if (n == 0) return 0;
    for (i = 0; i < n; i++)
        if (!field_size(units[i])) return 0;
    for (i = 0; at < size; i = (i + 1) % n) at += field_size(units[i]);
    return at == size;
}

int arcade_piece_build(const unsigned char *program, size_t program_size, const struct arcade_piece *p, unsigned char *out)
{
    size_t i = 0, n;
    unsigned at = 0, k, w;
    const unsigned char *src;
    if (!program || !p || !out || (p->size & 1u) || (p->source & 1u) || !layout_ok(p->units, p->size)) return -1;
    if (p->source > program_size || p->size > program_size - p->source) return -1;
    n = strlen(p->units);
    src = program + p->source;
    while (at < p->size) {
        w = field_size(p->units[i]);
        for (k = 0; k < w; k++) out[at + k] = p->units[i] == 'b' ? src[at + k] : src[at + w - 1u - k];
        at += w;
        i = (i + 1) % n;
    }
    return 0;
}

int arcade_pieces_valid(const struct arcade_piece *p, unsigned count)
{
    unsigned i;
    for (i = 0; i < count; i++) {
        if (!layout_ok(p[i].units, p[i].size) || !p[i].name || p[i].size == 0 || (p[i].size & 1u) || (p[i].address & 1u) || (p[i].source & 1u)) return -1;
        if (p[i].address < ARCADE_PS1_DATA_START || p[i].address > ARCADE_PS1_DATA_END || p[i].size > ARCADE_PS1_DATA_END - p[i].address) return -1;
        if (i && p[i].address < p[i - 1].address + p[i - 1].size) return -1;
    }
    return 0;
}
