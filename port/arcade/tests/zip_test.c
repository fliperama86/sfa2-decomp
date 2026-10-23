/* Driver for port/tools/test_arcade_zip.py.
 *   zip_test list ARCHIVE [OUTDIR]   per member "name size crc32hex" after reading
 *                                    and verifying it; the bytes go to OUTDIR/<index>.bin
 *   zip_test inflate RAW SIZE OUT    arcade_inflate of a raw deflate file into SIZE bytes
 *   zip_test crc FILE                crc32 of a file, in hex
 * A fault prints one line to stderr and exits 1. */
#include "../zip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *slurp(const char *path, size_t *n)
{
    FILE *f = fopen(path, "rb");
    unsigned char *p;
    long len;
    if (!f || fseek(f, 0, SEEK_END) || (len = ftell(f)) < 0 || fseek(f, 0, SEEK_SET))
        return NULL;
    p = malloc(len ? (size_t)len : 1);
    if (!p || fread(p, 1, (size_t)len, f) != (size_t)len)
        return NULL;
    fclose(f);
    *n = (size_t)len;
    return p;
}

static int spit(const char *path, const unsigned char *p, size_t n)
{
    FILE *f = fopen(path, "wb");
    int ok = f && (n == 0 || fwrite(p, 1, n, f) == n);
    if (f && fclose(f))
        ok = 0;
    return ok;
}

static int list(const char *path, const char *outdir)
{
    struct arcade_zip z;
    char err[256];
    unsigned i;
    if (arcade_zip_open(path, &z, err, sizeof err)) {
        fprintf(stderr, "%s\n", err);
        return 1;
    }
    for (i = 0; i < z.count; i++) {
        const struct arcade_zip_member *m = &z.members[i];
        unsigned char *data;
        if (arcade_zip_find(&z, m->name) != m) {
            fprintf(stderr, "find failed for %s\n", m->name);
            return 1;
        }
        if (arcade_zip_read(&z, m, &data, err, sizeof err)) {
            fprintf(stderr, "%s\n", err);
            arcade_zip_close(&z);
            return 1;
        }
        printf("%s %u %08x\n", m->name, (unsigned)m->usize, (unsigned)arcade_crc32(data, m->usize));
        if (outdir) {
            char name[512];
            snprintf(name, sizeof name, "%s/%u.bin", outdir, i);
            if (!spit(name, data, m->usize)) {
                fprintf(stderr, "cannot write %s\n", name);
                return 1;
            }
        }
        free(data);
    }
    if (arcade_zip_find(&z, "no/such/member") != NULL) {
        fprintf(stderr, "find invented a member\n");
        return 1;
    }
    arcade_zip_close(&z);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc >= 3 && !strcmp(argv[1], "list") && argc <= 4)
        return list(argv[2], argc == 4 ? argv[3] : NULL);
    if (argc == 5 && !strcmp(argv[1], "inflate")) {
        size_t n, size = (size_t)strtoul(argv[3], NULL, 10);
        unsigned char *in = slurp(argv[2], &n), *out = malloc(size ? size : 1);
        int bad;
        if (!in || !out) {
            fprintf(stderr, "cannot read %s\n", argv[2]);
            return 1;
        }
        bad = arcade_inflate(in, n, out, size);
        if (bad)
            fprintf(stderr, "inflate refused\n");
        else if (!spit(argv[4], out, size))
            bad = 1;
        free(in);
        free(out);
        return bad ? 1 : 0;
    }
    if (argc == 3 && !strcmp(argv[1], "crc")) {
        size_t n;
        unsigned char *p = slurp(argv[2], &n);
        if (!p)
            return 1;
        printf("%08x\n", (unsigned)arcade_crc32(p, n));
        free(p);
        return 0;
    }
    fprintf(stderr, "usage: zip_test list ARCHIVE [OUTDIR] | inflate RAW SIZE OUT | crc FILE\n");
    return 2;
}
