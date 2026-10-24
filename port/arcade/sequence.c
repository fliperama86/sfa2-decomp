#include "sequence.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The conversion of the arcade's sequence records to the PS1's animation steps, frame records and pointer table. The
 * rules, and why, are in sequences.py (the docstring of the tool and model_build, which this follows step by step).
 * In short, for the records of a run:
 *  - a step is the record's duration and flags, the cell list offset of its art record, the distance in steps to
 *    its jump target's step (when bit 15 is set), and the number of its frame record;
 *  - the frame record is the record's 16 frame bytes with each group of 4 reversed; frames are numbered from 1 in the
 *    order of first use, an all-zero frame is number 0, a repeated frame keeps its number;
 *  - the pointer table holds the addresses of steps, in the order of the arcade's tables. */

#define REC_SIZE 24u          /* an arcade record; 28 with a jump target */
#define JUMP 0x8000u
#define HOLD 4095u            /* the longest countdown: a hold */
#define ART_LO 0x100000u      /* where the arcade's art records lie (a plausibility bound for a pointer) */
#define ART_HI 0x400000u
#define MAX_CELLS 512u        /* a list longer than this is not a list */
#define NO_STEP 0xffffffffu

struct record {
    uint32_t address, art, target;
    uint16_t duration, flags;
    const uint8_t *frame;      /* 16 bytes in the program */
    int has_target;
};

struct seen {
    uint8_t key[ARCADE_SEQUENCE_FRAME];
    uint16_t number;
    int used;
};

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

static uint16_t be16(const uint8_t *p)
{
    return (uint16_t)(p[0] << 8 | p[1]);
}

static uint32_t be32(const uint8_t *p)
{
    return (uint32_t)be16(p) << 16 | be16(p + 2);
}

static uint16_t le16(const uint8_t *p)
{
    return (uint16_t)(p[0] | p[1] << 8);
}

static void put16(uint8_t *p, unsigned v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

static void put32(uint8_t *p, uint32_t v)
{
    put16(p, v & 0xffffu);
    put16(p + 2, v >> 16);
}

const struct arcade_sequence *arcade_sequence_find(const char *file)
{
    unsigned i;
    for (i = 0; i < arcade_sequence_count; i++)
        if (!strcmp(arcade_sequences[i].file, file)) return &arcade_sequences[i];
    return NULL;
}

/* ---- list offsets from the fighter's output */

/* The end of the list at offset o of the lists block, or 0 when it is not a list (the reader of the PS1 module's lists, as
 * fighters.py parse_list reads them: a header of 5 words, then the body of the mode). */
static size_t list_end(const uint8_t *lists, size_t size, size_t o)
{
    unsigned n, mode, count = 0, step;
    size_t p;
    if (o > size || size - o < 10) return 0;
    n = le16(lists + o);
    mode = le16(lists + o + 2);
    if (n < 1 || n > MAX_CELLS || (mode != 0 && mode != 2 && mode != 4 && mode != 6) || le16(lists + o + 6)) return 0;
    p = o + 10;
    if (mode == 0 || mode == 2) {
        step = mode == 0 ? 2u : 4u;
        if ((size_t)step * n > size - p) return 0;
        return p + (size_t)step * n;
    }
    step = mode == 4 ? 4u : 6u;
    while (count < n) {
        unsigned run;
        if (step > size - p) return 0;
        run = le16(lists + p + (mode == 4 ? 2u : 4u));
        count += run + 1;
        if (count > n) return 0;
        p += step;
    }
    return p;
}

/* The end of the art record at a: mode 2 has a block count, mode 0 a cell count (the layouts of fighter.c). */
static int record_end(const uint8_t *prog, size_t size, uint32_t a, uint32_t *end)
{
    unsigned mode;
    if ((a & 1) || a > size || size - a < 8) return -1;
    mode = be16(prog + a);
    if (mode == 2) *end = a + 8 + 4u * (be16(prog + a + 4) + 1u);
    else if (mode == 0) *end = a + 8 + 2u * be16(prog + a + 2);
    else return -1;
    return *end <= size ? 0 : -1;
}

static int cmp_list(const void *pa, const void *pb)
{
    const struct arcade_sequence_list *a = pa, *b = pb;
    return a->record < b->record ? -1 : a->record > b->record;
}

int arcade_sequence_lists(const uint8_t *program, size_t program_size, const struct arcade_fighter *f,
                          const struct arcade_fighter_out *fo, struct arcade_sequence_lists *out, char *err, size_t errsize)
{
    struct arcade_sequence_list *items = NULL;
    size_t cap = 0, n = 0, o = 0, i, w = 0, li = 0;
    unsigned s, c;

    out->item = NULL;
    out->count = 0;
    for (s = 0; s < f->segment_count; s++) cap += f->segments[s].count;
    items = malloc((cap ? cap : 1) * sizeof *items);
    if (!items) return fail(err, errsize, "out of memory");
    for (s = 0; s < f->segment_count; s++) {
        const struct arcade_fighter_segment *seg = &f->segments[s];
        uint32_t a = seg->address;
        for (c = 0; c < seg->count; c++) {
            size_t e;
            if (fo->list_at && li < fo->list_count) o = fo->list_at[li];   /* the PS1's lists may be padded: the builder says where each starts */
            e = list_end(fo->lists, fo->lists_size, o);
            li++;
            if (!e) {
                free(items);
                return fail(err, errsize, "%s: no list at +%#x of the lists", f->file, (unsigned)o);
            }
            if (seg->address) {
                uint32_t next;
                if (record_end(program, program_size, a, &next)) {
                    free(items);
                    return fail(err, errsize, "%s: no art record at %#x", f->file, (unsigned)a);
                }
                items[n].record = a;
                items[n].offset = (uint32_t)o;
                n++;
                a = next;
            }
            o = e;
        }
    }
    /* sorted by record; of equal records the later list is kept (qsort is not stable: order them by offset too) */
    for (i = 1; i < n; i++) {
        size_t k = i;
        struct arcade_sequence_list t = items[i];
        while (k && items[k - 1].record > t.record) {
            items[k] = items[k - 1];
            k--;
        }
        items[k] = t;
    }
    for (i = 0; i < n; i++) {
        if (w && items[w - 1].record == items[i].record) items[w - 1] = items[i];
        else items[w++] = items[i];
    }
    out->item = items;
    out->count = w;
    return 0;
}

void arcade_sequence_lists_free(struct arcade_sequence_lists *lists)
{
    free(lists->item);
    lists->item = NULL;
    lists->count = 0;
}

static int list_offset(const struct arcade_sequence_lists *lists, uint32_t record, uint32_t *offset)
{
    struct arcade_sequence_list key, *hit;
    key.record = record;
    key.offset = 0;
    hit = lists->count ? bsearch(&key, lists->item, lists->count, sizeof *lists->item, cmp_list) : NULL;
    if (!hit) return 0;
    *offset = hit->offset;
    return 1;
}

/* ---- the arcade's records */

static int valid_record(const uint8_t *prog, size_t size, uint32_t a)
{
    unsigned dur;
    uint32_t art;
    if ((a & 1) || a > size || size - a < REC_SIZE) return 0;
    dur = be16(prog + a);
    art = be32(prog + a + 4);
    return dur >= 1 && dur <= HOLD && !(art & 1) && art >= ART_LO && art < ART_HI;
}

static unsigned record_total(const struct arcade_sequence *s)
{
    unsigned g, n = 0;
    for (g = 0; g < s->group_count; g++) n += s->groups[g].count;
    return n;
}

/* The records of the groups, one group after the other. */
static int read_records(const uint8_t *prog, size_t size, const struct arcade_sequence *s, struct record *recs, char *err, size_t errsize)
{
    unsigned k = 0, g, c;
    int quirks = (s->rules & ARCADE_SEQUENCE_JUMP_WITHOUT_BIT15) != 0;
    for (g = 0; g < s->group_count; g++) {
        uint32_t a = s->groups[g].address;
        for (c = 0; c < s->groups[g].count; c++, k++) {
            struct record *r = &recs[k];
            uint32_t len = REC_SIZE;
            if (!valid_record(prog, size, a)) return fail(err, errsize, "%s: no sequence record at %#x", s->file, (unsigned)a);
            r->address = a;
            r->duration = be16(prog + a);
            r->flags = be16(prog + a + 2);
            r->art = be32(prog + a + 4);
            r->frame = prog + a + 8;
            r->has_target = 0;
            r->target = 0;
            if (r->flags & JUMP) len = REC_SIZE + 4;
            else if (quirks && !valid_record(prog, size, a + REC_SIZE) && valid_record(prog, size, a + REC_SIZE + 4)) {
                len = REC_SIZE + 4;
                r->flags |= JUMP;
            }
            if (len > REC_SIZE) {
                if (size - a < len) return fail(err, errsize, "%s: sequence record at %#x runs past the program", s->file, (unsigned)a);
                r->target = be32(prog + a + REC_SIZE);
                r->has_target = 1;
            }
            a += len;
        }
    }
    return 0;
}

/* The index of the record at address a, or -1 (the records are in ascending order). */
static long record_index(const struct record *recs, unsigned count, uint32_t a)
{
    unsigned lo = 0, hi = count;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if (recs[mid].address == a) return (long)mid;
        if (recs[mid].address < a) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}

/* ---- frames */

static void convert_frame(const uint8_t *fr, uint8_t *out)
{
    unsigned g, i;
    for (g = 0; g < ARCADE_SEQUENCE_FRAME; g += 4)
        for (i = 0; i < 4; i++) out[g + i] = fr[g + 3 - i];
}

static int all_zero(const uint8_t *p)
{
    unsigned i;
    for (i = 0; i < ARCADE_SEQUENCE_FRAME; i++)
        if (p[i]) return 0;
    return 1;
}

static size_t frame_slot(const struct seen *tab, size_t mask, const uint8_t *key, int *found)
{
    uint32_t h = 2166136261u;
    size_t i;
    unsigned k;
    for (k = 0; k < ARCADE_SEQUENCE_FRAME; k++) h = (h ^ key[k]) * 16777619u;
    i = h & mask;
    while (tab[i].used) {
        if (!memcmp(tab[i].key, key, ARCADE_SEQUENCE_FRAME)) {
            *found = 1;
            return i;
        }
        i = (i + 1) & mask;
    }
    *found = 0;
    return i;
}

/* ---- the build */

static uint16_t scale_flags(uint16_t fl)
{
    unsigned low = fl & 0xffu;
    if (!low || low % 0x18u) return fl;
    return (uint16_t)((fl & 0x8100u) | low / 0x18u);
}

void arcade_sequence_free(struct arcade_sequence_out *out)
{
    free(out->steps);
    free(out->steps_built);
    free(out->frames);
    free(out->frames_built);
    free(out->table);
    free(out->table_built);
    memset(out, 0, sizeof *out);
}

int arcade_sequence_build(const uint8_t *program, size_t program_size, const struct arcade_sequence *s,
                          const struct arcade_sequence_lists *lists, struct arcade_sequence_out *out, char *err, size_t errsize)
{
    struct record *recs = NULL;
    uint32_t *step_of = NULL;
    uint32_t *number = NULL;
    int32_t *override = NULL;
    struct seen *tab = NULL;
    uint32_t *entries = NULL;
    size_t nentries = 0, mask, cap;
    unsigned r, k, t, top = 0, nrec = record_total(s);
    int rc = -1;

    memset(out, 0, sizeof *out);
    if (err && errsize) err[0] = 0;
    out->step_count = s->step_count;
    out->frame_count = s->frame_count;
    out->table_entries = s->table_entries;
    out->steps = calloc((size_t)s->step_count * ARCADE_SEQUENCE_STEP + 1, 1);
    out->steps_built = calloc((size_t)s->step_count + 1, 1);
    out->frames = calloc((size_t)s->frame_count * ARCADE_SEQUENCE_FRAME + 1, 1);
    out->frames_built = calloc((size_t)s->frame_count + 1, 1);
    out->table = calloc((size_t)s->table_entries * 4 + 1, 1);
    out->table_built = calloc((size_t)s->table_entries + 1, 1);
    recs = calloc((size_t)nrec + 1, sizeof *recs);
    step_of = malloc(((size_t)nrec + 1) * sizeof *step_of);
    number = calloc((size_t)nrec + 1, sizeof *number);
    override = malloc(((size_t)s->step_count + 1) * sizeof *override);
    for (cap = 2; cap < 2 * ((size_t)nrec + 1); cap *= 2) {}
    mask = cap - 1;
    tab = calloc(cap, sizeof *tab);
    if (!out->steps || !out->steps_built || !out->frames || !out->frames_built || !out->table || !out->table_built || !recs || !step_of ||
        !number || !override || !tab) {
        fail(err, errsize, "out of memory");
        goto done;
    }
    for (r = 0; r < nrec; r++) step_of[r] = NO_STEP;
    for (k = 0; k < s->step_count; k++) override[k] = -1;
    if (read_records(program, program_size, s, recs, err, errsize)) goto done;
    for (r = 0; r < s->run_count; r++) {
        const struct arcade_sequence_run *run = &s->runs[r];
        if ((unsigned)run->record + run->count > nrec || (unsigned)run->step + run->count > s->step_count) {
            fail(err, errsize, "%s: a run outside the records or the steps", s->file);
            goto done;
        }
        for (k = 0; k < run->count; k++) {
            if (step_of[run->record + k] != NO_STEP) {
                fail(err, errsize, "%s: runs overlap", s->file);
                goto done;
            }
            step_of[run->record + k] = (uint32_t)run->step + k;
        }
    }
    for (k = 0; k < s->override_count; k++) {
        if (s->overrides[k].step >= s->step_count || s->overrides[k].number >= s->frame_count) {
            fail(err, errsize, "%s: a frame override outside the steps or the frame records", s->file);
            goto done;
        }
        override[s->overrides[k].step] = s->overrides[k].number;
    }

    /* frame records, numbered in order of first use down the runs */
    if (s->frame_count) out->frames_built[0] = 1;
    for (r = 0; r < s->run_count; r++) {
        const struct arcade_sequence_run *run = &s->runs[r];
        for (k = 0; k < run->count; k++) {
            uint8_t conv[ARCADE_SEQUENCE_FRAME];
            unsigned num;
            int found;
            size_t slot;
            convert_frame(recs[run->record + k].frame, conv);
            slot = frame_slot(tab, mask, conv, &found);
            if (override[run->step + k] >= 0) {
                num = (unsigned)override[run->step + k];
                if (num > top) top = num;
                if (!all_zero(conv) && !found) {
                    memcpy(tab[slot].key, conv, sizeof conv);
                    tab[slot].number = (uint16_t)num;
                    tab[slot].used = 1;
                }
            } else if (all_zero(conv)) {
                num = 0;
            } else if (found) {
                num = tab[slot].number;
            } else {
                top++;
                num = top;
                memcpy(tab[slot].key, conv, sizeof conv);
                tab[slot].number = (uint16_t)num;
                tab[slot].used = 1;
            }
            if (num >= s->frame_count) {
                fail(err, errsize, "%s: frame %u beyond the %u frame records", s->file, num, (unsigned)s->frame_count);
                goto done;
            }
            memcpy(out->frames + (size_t)num * ARCADE_SEQUENCE_FRAME, conv, sizeof conv);
            out->frames_built[num] = 1;
            number[run->record + k] = num;
        }
    }

    /* steps */
    for (r = 0; r < s->run_count; r++) {
        const struct arcade_sequence_run *run = &s->runs[r];
        for (k = 0; k < run->count; k++) {
            const struct record *rec = &recs[run->record + k];
            unsigned jj = (unsigned)run->step + k, fl = rec->flags;
            uint32_t dp;
            int32_t lo = 0;
            uint8_t *st;
            if (run->record + k >= s->scale_first && run->record + k < (unsigned)s->scale_first + s->scale_count) fl = scale_flags((uint16_t)fl);
            if (!list_offset(lists, rec->art, &dp)) continue;
            if (rec->flags & JUMP) {
                long ti = rec->has_target ? record_index(recs, nrec, rec->target) : -1;
                int64_t d;
                if (ti < 0 || step_of[ti] == NO_STEP) continue;
                d = (int64_t)step_of[ti] - (int64_t)jj;
                if (d < -32768 || d > 32767) continue;
                lo = (int32_t)d;
            }
            st = out->steps + (size_t)jj * ARCADE_SEQUENCE_STEP;
            put16(st, rec->duration);
            put16(st + 2, fl);
            put32(st + 4, dp);
            put16(st + 8, (unsigned)lo & 0xffffu);
            put16(st + 10, number[run->record + k]);
            out->steps_built[jj] = 1;
        }
    }

    /* the pointer table: the entries of the arcade's tables, in order */
    for (t = 0; t < s->table_count; t++) nentries += s->tables[t].count;
    entries = malloc((nentries + 1) * sizeof *entries);
    if (!entries) {
        fail(err, errsize, "out of memory");
        goto done;
    }
    {
        size_t e = 0;
        for (t = 0; t < s->table_count; t++) {
            const struct arcade_sequence_table *tb = &s->tables[t];
            for (k = 0; k < tb->count; k++) {
                size_t at = (size_t)tb->address + 2u * k;
                if (at > program_size || program_size - at < 2) {
                    fail(err, errsize, "%s: sequence table at %#x runs past the program", s->file, (unsigned)tb->address);
                    goto done;
                }
                entries[e++] = tb->address + (uint32_t)(int32_t)(int16_t)be16(program + at);
            }
        }
    }
    for (k = 0; k < s->slice_count; k++) {
        const struct arcade_sequence_slice *sl = &s->slices[k];
        if ((size_t)sl->entry + sl->count > nentries || (unsigned)sl->slot + sl->count > s->table_entries) {
            fail(err, errsize, "%s: a table slice outside the arcade tables or the PS1 table", s->file);
            goto done;
        }
        for (t = 0; t < sl->count; t++) {
            long ri = record_index(recs, nrec, entries[sl->entry + t]);
            if (ri < 0 || step_of[ri] == NO_STEP) continue;
            put32(out->table + 4u * (sl->slot + t), s->load + s->steps + ARCADE_SEQUENCE_STEP * step_of[ri]);
            out->table_built[sl->slot + t] = 1;
        }
    }
    rc = 0;
done:
    free(recs);
    free(step_of);
    free(number);
    free(override);
    free(tab);
    free(entries);
    if (rc) arcade_sequence_free(out);
    return rc;
}
