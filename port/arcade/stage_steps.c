/* The animation steps of the stage modules, built from the arcade's sequence records (stage_steps.h; the specification is build_steps in stage_steps.py).
 * Project-authored, plain C99. No byte of either game is here. */
#include "stage_steps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULE_BASE 0x801e8000u   /* where the stage module is linked */
#define STEP 12u
#define RECORD 16u                /* an arcade record; 20 with a jump */
#define JUMP 0x8000u
#define HOLD 4095u
#define ART_LO 0x100000u
#define ART_HI 0x400000u

static unsigned be16(const uint8_t *p) { return (unsigned)p[0] << 8 | p[1]; }
static uint32_t be32(const uint8_t *p) { return (uint32_t)be16(p) << 16 | be16(p + 2); }

struct record { uint32_t addr, art, target; unsigned countdown, flags, size; };

const struct arcade_stage_steps *arcade_stage_steps_find(const char *file)
{
    unsigned i;
    if (!file) return NULL;
    for (i = 0; i < arcade_stage_steps_count; i++)
        if (!strcmp(arcade_stage_steps[i].file, file)) return &arcade_stage_steps[i];
    return NULL;
}

static int msg(char *err, size_t errsize, const char *what, unsigned a, unsigned b)
{
    if (err && errsize) snprintf(err, errsize, what, a, b);
    return -1;
}

/* The record at a (valid_record in stage_steps.py): an even address, a countdown 1..4095, an even art address in range, the jump address when bit 15 is set. */
static int read_record(const uint8_t *prog, size_t n, uint32_t a, struct record *r)
{
    if ((a & 1u) || a > n || n - a < RECORD) return -1;
    r->addr = a;
    r->countdown = be16(prog + a);
    r->flags = be16(prog + a + 2);
    r->art = be32(prog + a + 4);
    if (r->countdown < 1 || r->countdown > HOLD || (r->art & 1u) || r->art < ART_LO || r->art >= ART_HI) return -1;
    r->size = RECORD;
    r->target = 0;
    if (r->flags & JUMP) {
        if (n - a < RECORD + 4u) return -1;
        r->target = be32(prog + a + RECORD);
        r->size = RECORD + 4u;
    }
    return 0;
}

int arcade_stage_steps_build(const uint8_t *program, size_t program_size, const struct arcade_stage_steps *row,
                             uint8_t *module, uint8_t *known, size_t module_size, char *err, size_t errsize)
{
    unsigned r, k, total = 0, n = 0, i, rc = 0;
    uint32_t *addr = NULL, *off = NULL;
    if (err && errsize) err[0] = 0;
    if (!row || !row->run_count) return 0;
    for (r = 0; r < row->run_count; r++) total += row->runs[r].count;
    addr = malloc(((size_t)total + 1u) * sizeof *addr);
    off = malloc(((size_t)total + 1u) * sizeof *off);
    if (!addr || !off) {
        free(addr);
        free(off);
        return msg(err, errsize, "out of memory in the steps (%u)", total, 0);
    }
    for (r = 0; r < row->run_count; r++) {         /* where each record's step lies: the jumps find their targets here */
        uint32_t a = row->runs[r].record;
        for (k = 0; k < row->runs[r].count; k++) {
            struct record rec;
            if (read_record(program, program_size, a, &rec)) { rc = (unsigned)msg(err, errsize, "run %u: no record at %#x", r, a); break; }
            addr[n] = a;
            off[n] = row->runs[r].offset + STEP * k;
            n++;
            a += rec.size;
        }
        if (rc) break;
    }
    if (rc) {
        free(addr);
        free(off);
        return -1;
    }
    n = 0;
    for (r = 0; r < row->run_count; r++) {
        const struct arcade_stage_step_run *run = &row->runs[r];
        uint32_t a = run->record;
        unsigned first = n, bad = 0;
        uint8_t *tmp;
        if (run->offset > module_size || (size_t)STEP * run->count > module_size - run->offset) {
            rc = (unsigned)msg(err, errsize, "run %u does not fit the module (%u)", r, run->offset);
            n += run->count;
            continue;
        }
        tmp = malloc((size_t)STEP * run->count + 1u);
        if (!tmp) { rc = (unsigned)msg(err, errsize, "out of memory in the steps (%u)", r, 0); n += run->count; continue; }
        for (k = 0; k < run->count && !bad; k++) {
            struct record rec;
            uint8_t *dst = tmp + STEP * k;
            uint32_t grid = 0;
            int loop = 0, found = 0;
            read_record(program, program_size, a, &rec);        /* read above */
            for (i = 0; i < row->art_count; i++)
                if (row->arts[i].art == rec.art) { grid = MODULE_BASE + row->arts[i].grid; found = 1; break; }
            if (!found) { rc = (unsigned)msg(err, errsize, "run %u: the art record at %#x has no grid", r, rec.art); bad = 1; break; }
            if (rec.flags & JUMP) {
                found = 0;
                for (i = 0; i < total; i++)
                    if (addr[i] == rec.target) { loop = ((int)off[i] - (int)(run->offset + STEP * k)) / (int)STEP; found = 1; break; }
                if (!found) { rc = (unsigned)msg(err, errsize, "run %u: the jump to %#x leads to no record of a run", r, rec.target); bad = 1; break; }
            }
            dst[0] = (uint8_t)rec.countdown; dst[1] = (uint8_t)(rec.countdown >> 8);
            dst[2] = (uint8_t)rec.flags; dst[3] = (uint8_t)(rec.flags >> 8);
            dst[4] = (uint8_t)grid; dst[5] = (uint8_t)(grid >> 8); dst[6] = (uint8_t)(grid >> 16); dst[7] = (uint8_t)(grid >> 24);
            dst[8] = (uint8_t)loop; dst[9] = (uint8_t)((unsigned)loop >> 8);
            dst[10] = 0; dst[11] = 0;
            a += rec.size;
        }
        n = first + run->count;
        if (!bad) {
            memcpy(module + run->offset, tmp, (size_t)STEP * run->count);
            if (known) memset(known + run->offset, 1, (size_t)STEP * run->count);
        }
        free(tmp);
    }
    free(addr);
    free(off);
    return rc ? -1 : 0;
}
