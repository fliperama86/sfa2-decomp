#include "texts.h"
#include <string.h>

/* One character of the form at a: its ASCII code, or -1. */
static int character(unsigned form, const uint8_t *a)
{
    if (form == ARCADE_TEXT_PLAIN) return a[0] >= 0x20 && a[0] <= 0x7e ? a[0] : -1;
    if (a[0] == 0 && a[1] == 0) return ' ';
    if ((a[0] != 0x00 && a[0] != 0x80) || a[1] < 0xa1 || a[1] > 0xfe) return -1;
    return a[1] & 0x7f;
}

int arcade_text_build(const uint8_t *program, size_t program_size, const struct arcade_text_string *s, uint8_t *out)
{
    unsigned step, i;
    uint64_t span;
    if (!program || !s || !out || s->form > ARCADE_TEXT_PLAIN || (uint32_t)s->chars + 3u > s->size) return -1;
    step = s->form == ARCADE_TEXT_CODES ? 2u : 1u;
    if (step == 2u && (s->source & 1u)) return -1;
    span = (uint64_t)s->chars * step;
    if (s->source > program_size || span > program_size - s->source) return -1;
    for (i = 0; i < s->chars; i++) {
        int c = character(s->form, program + s->source + (size_t)i * step);
        if (c < 0) return -1;
        out[i] = (uint8_t)c;
    }
    out[s->chars] = '@';
    out[s->chars + 1u] = '@';
    memset(out + s->chars + 2u, 0, s->size - s->chars - 2u);
    return 0;
}

int arcade_text_module_valid(const struct arcade_text_module *m)
{
    unsigned i;
    if (!m || !m->file || (m->count && !m->strings)) return -1;
    for (i = 0; i < m->count; i++) {
        const struct arcade_text_string *s = &m->strings[i];
        if (s->form > ARCADE_TEXT_PLAIN || (uint32_t)s->chars + 3u > s->size || s->offset > m->size || s->size > m->size - s->offset) return -1;
        if (s->form == ARCADE_TEXT_CODES && (s->source & 1u)) return -1;
        if (i && s->offset < m->strings[i - 1].offset + m->strings[i - 1].size) return -1;
    }
    return 0;
}
