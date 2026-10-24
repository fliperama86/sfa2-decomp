#include "full_sets.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define STEP 12u

static int fail(char *err, size_t errsize, const char *fmt, ...)
{
    if (err && errsize) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(err, errsize, fmt, ap);
        va_end(ap);
    }
    return -1;
}

static uint32_t get32(const uint8_t *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

static int inside(uint64_t at, uint64_t n, uint64_t size)
{
    return at <= size && n <= size - at;
}

static void write(uint8_t *out, uint8_t *changed, uint32_t at, const uint8_t *bytes, uint32_t n)
{
    memcpy(out + at, bytes, n);
    memset(changed + at, 1, n);
}

int arcade_full_build(const struct arcade_full_set *s, const uint8_t *module, uint32_t size, const uint8_t *source,
                      uint32_t source_size, uint32_t lists_address, uint8_t *out, uint8_t *changed, char *err, size_t errsize)
{
    uint32_t steps_size = (uint32_t)s->step_count * STEP, source_table = s->source_steps + steps_size, k;
    uint32_t delta = lists_address - (s->load + s->lists);   /* added to a source list offset: the same list, read from the new place */
    uint32_t first_step = s->source_load + s->source_steps, end_step = first_step + steps_size;
    unsigned r;

    if (!inside(s->source_positions, s->positions_size, source_size) || !inside(s->source_lists, s->lists_size, source_size) ||
        !inside(s->source_steps, steps_size, source_size) || !inside(source_table, 4u * (uint64_t)s->source_table_entries, source_size))
        return fail(err, errsize, "%s: a block lies outside the chunk of %s", s->file, s->source);
    if (!inside(s->positions, s->positions_size, size) || !inside(s->steps, steps_size, size) || !inside(s->table, 4u * (uint64_t)s->table_entries, size))
        return fail(err, errsize, "%s: a block lies outside its module chunk", s->file);
    if ((uint64_t)s->positions + s->positions_size > s->steps || (uint64_t)s->steps + steps_size > s->table)
        return fail(err, errsize, "%s: the position runs and steps do not fit before the step table", s->file);
    if (delta & 1u) return fail(err, errsize, "%s: the lists' place is odd (bit 0 of a list offset is a flag)", s->file);

    memcpy(out, module, size);
    memset(changed, 0, size);
    write(out, changed, s->positions, source + s->source_positions, s->positions_size);
    for (k = 0; k < s->step_count; k++) {
        uint8_t step[STEP];
        memcpy(step, source + s->source_steps + k * STEP, STEP);
        put32(step + 4, get32(step + 4) + delta);
        write(out, changed, s->steps + k * STEP, step, STEP);
    }
    for (r = 0; r < s->run_count; r++) {
        const struct arcade_full_run *u = &s->runs[r];
        if ((uint32_t)u->first + u->count > s->table_entries || (uint32_t)u->source_first + u->count > s->source_table_entries)
            return fail(err, errsize, "%s: run %u lies outside a step table", s->file, r);
        for (k = 0; k < u->count; k++) {
            uint32_t p = get32(source + source_table + 4u * (u->source_first + k)), v;
            uint8_t word[4];
            if (p < first_step || p >= end_step || (p - first_step) % STEP)
                return fail(err, errsize, "%s: entry %u of the step table of %s is not a step", s->file, u->source_first + k, s->source);
            v = s->load + s->steps + (p - first_step);
            put32(word, v);
            write(out, changed, s->table + 4u * (u->first + k), word, 4);
        }
    }
    for (k = 0; k < s->outside_count; k++) {
        const struct arcade_full_step *o = &s->outside[k];
        uint8_t word[4];
        if (!inside(o->source, STEP, source_size) || !inside(o->target, STEP, size))
            return fail(err, errsize, "%s: step %u outside the step block lies outside a chunk", s->file, k);
        put32(word, get32(source + o->source + 4) + delta);
        write(out, changed, o->target + 4, word, 4);
    }
    return 0;
}

int arcade_full_tiles(const uint8_t *source, uint32_t source_size, uint32_t address, uint32_t base, uint8_t *out, uint32_t room,
                      uint32_t *count, char *err, size_t errsize)
{
    uint32_t n, i, first;
    *count = 0;
    if (source_size < 4) return fail(err, errsize, "the source's sprite chunk is empty");
    first = get32(source);
    if (first == 0 || first % 4u || first > source_size) return fail(err, errsize, "the source's sprite chunk has no offset table");
    n = first / 4u;
    if ((uint64_t)n * 4u > room) return fail(err, errsize, "%u tiles do not fit in %u bytes", n, room);
    for (i = 0; i < n; i++) {
        uint32_t e = get32(source + 4u * i);
        if (e < first || e >= source_size) return fail(err, errsize, "entry %u of the source's sprite chunk lies outside it", i);
        put32(out + 4u * i, address + e - base);
    }
    *count = n;
    return 0;
}
