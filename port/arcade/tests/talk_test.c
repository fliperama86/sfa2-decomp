/* Driver of test_arcade_talk.py and talk.py --compare-c.
 *   talk_test table                       print the linked table (arcade_talk): `page_table page_slot page_ordinal module_slot count`, then a
 *                                         line per page: `file text boxes b0 b1 b2 b3 script records`
 *   talk_test page FILE TEXT BOXES B0 B1 B2 B3 SCRIPT RECORDS PROGRAM GFX OUT
 *                                         arcade_talk_page_build for the page the numbers describe; prints `ok` and writes the 32,768 bytes
 *                                         to OUT, or `refused MESSAGE` and exits 3
 *   talk_test script FILE TEXT BOXES B0 B1 B2 B3 SCRIPT RECORDS PROGRAM MODULE OUTDIR
 *                                         arcade_talk_script_build on the module file; prints `ok` and writes OUTDIR/module and OUTDIR/changed,
 *                                         or `refused MESSAGE` and exits 3
 * Guard bytes after each output are checked: a write past them exits 4. Numbers may be written with 0x. Any other failure exits 1. */
#include "../talk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUARD 64u

static uint8_t *slurp(const char *path, size_t *n)
{
    FILE *f = fopen(path, "rb");
    uint8_t *p;
    long len;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = malloc((size_t)len + 1);
    if (!p || fread(p, 1, (size_t)len, f) != (size_t)len) {
        fclose(f);
        free(p);
        return NULL;
    }
    fclose(f);
    *n = (size_t)len;
    return p;
}

static int dump(const char *dir, const char *name, const void *p, size_t n)
{
    char path[1024];
    FILE *f;
    int ok;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f) return -1;
    ok = fwrite(p, 1, n, f) == n;
    return fclose(f) == 0 && ok ? 0 : -1;
}

static unsigned long num(const char *s)
{
    return strtoul(s, NULL, 0);
}

static int guard_ok(const uint8_t *p, size_t n)
{
    size_t i;
    for (i = 0; i < GUARD; i++)
        if (p[n + i] != 0xa5) return 0;
    return 1;
}

static void fill(struct arcade_talk_page *p, char **a)
{
    unsigned i;
    memset(p, 0, sizeof *p);
    p->file = a[0];
    p->text = (uint32_t)num(a[1]);
    p->boxes = (uint8_t)num(a[2]);
    for (i = 0; i < ARCADE_TALK_BOXES; i++)
        p->box_of[i] = (uint8_t)num(a[3 + i]);
    p->script = (uint32_t)num(a[7]);
    p->records = (uint16_t)num(a[8]);
}

static int page_cmd(char **a)
{
    struct arcade_talk_page p;
    size_t psize = 0, tsize = 0;
    uint8_t *program = slurp(a[9], &psize), *tiles = slurp(a[10], &tsize), *out = malloc(ARCADE_TALK_PAGE + GUARD);
    char err[256];
    int rc, status = 1;
    if (!program || !tiles || !out) goto done;
    fill(&p, a);
    memset(out, 0xee, ARCADE_TALK_PAGE);
    memset(out + ARCADE_TALK_PAGE, 0xa5, GUARD);
    rc = arcade_talk_page_build(&p, program, psize, tiles, tsize, out, err, sizeof err);
    if (!guard_ok(out, ARCADE_TALK_PAGE)) {
        printf("a write past an end\n");
        status = 4;
    } else if (rc) {
        printf("refused %s\n", err);
        status = 3;
    } else if (dump(a[11], "page", out, ARCADE_TALK_PAGE)) {
        status = 1;
    } else {
        printf("ok\n");
        status = 0;
    }
done:
    free(program);
    free(tiles);
    free(out);
    return status;
}

static int script_cmd(char **a)
{
    struct arcade_talk_page p;
    size_t psize = 0, msize = 0;
    uint8_t *program = slurp(a[9], &psize), *mod_in = slurp(a[10], &msize), *module = NULL, *changed = NULL;
    char err[256];
    int rc, status = 1;
    if (!program || !mod_in) goto done;
    module = malloc(msize + GUARD);
    changed = calloc(msize + GUARD, 1);
    if (!module || !changed) goto done;
    fill(&p, a);
    memcpy(module, mod_in, msize);
    memset(module + msize, 0xa5, GUARD);
    memset(changed + msize, 0xa5, GUARD);
    rc = arcade_talk_script_build(&p, program, psize, module, changed, msize, err, sizeof err);
    if (!guard_ok(module, msize) || !guard_ok(changed, msize)) {
        printf("a write past an end\n");
        status = 4;
    } else if (rc) {
        printf("refused %s\n", err);
        status = 3;
        /* a refusal must leave the module as it was */
        if (memcmp(module, mod_in, msize)) {
            printf("the module changed\n");
            status = 4;
        }
    } else if (dump(a[11], "module", module, msize) || dump(a[11], "changed", changed, msize)) {
        status = 1;
    } else {
        printf("ok\n");
        status = 0;
    }
done:
    free(program);
    free(mod_in);
    free(module);
    free(changed);
    return status;
}

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "table")) {
        unsigned i;
        printf("%u %u %u %u %u\n", arcade_talk.page_table, arcade_talk.page_slot, arcade_talk.page_ordinal, arcade_talk.module_slot, arcade_talk.count);
        for (i = 0; i < arcade_talk.count; i++) {
            const struct arcade_talk_page *p = &arcade_talk.pages[i];
            printf("%s %#x %u %u %u %u %u %#x %u\n", p->file, p->text, p->boxes, p->box_of[0], p->box_of[1], p->box_of[2], p->box_of[3], p->script, p->records);
        }
        return 0;
    }
    if (argc == 14 && !strcmp(argv[1], "page"))
        return page_cmd(argv + 2);
    if (argc == 14 && !strcmp(argv[1], "script"))
        return script_cmd(argv + 2);
    fprintf(stderr, "usage: see the head of this file\n");
    return 1;
}
