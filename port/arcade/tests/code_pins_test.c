/* Driver of code_pins.c (port/arcade/code_pins.py --compare-c and port/tools/test_arcade_code_pins.py): it links whatever table it is given.
 *   list                      "count N", then per row: name size pin data_pin range_count (hex strings for the hashes)
 *   ranges NAME               per range of the row: offset size
 *   find NAME                 exit 0 when the row exists, 1 when not
 *   zero NAME IN OUT [short]  arcade_code_zero on the bytes of file IN (with `short`: one byte less than their size); writes OUT;
 *                             exit 3 when the call refuses, 4 when there is no row, 2 on a file error */
#include "code_pins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void hex(const unsigned char *b)
{
    int i;
    for (i = 0; i < 32; i++) printf("%02x", b[i]);
}

int main(int argc, char **argv)
{
    unsigned i;
    const struct arcade_code_pin *p;
    if (argc >= 2 && !strcmp(argv[1], "list")) {
        printf("count %u\n", arcade_code_pin_count);
        for (i = 0; i < arcade_code_pin_count; i++) {
            p = &arcade_code_pins[i];
            printf("%s %u ", p->image, p->size);
            hex(p->pin);
            printf(" ");
            hex(p->data_pin);
            printf(" %u\n", p->count);
        }
        return 0;
    }
    if (argc >= 3 && !strcmp(argv[1], "find")) return arcade_code_pin_find(argv[2]) ? 0 : 1;
    if (argc >= 3 && !strcmp(argv[1], "ranges")) {
        p = arcade_code_pin_find(argv[2]);
        if (!p) return 4;
        for (i = 0; i < p->count; i++) printf("%u %u\n", (unsigned)p->ranges[i].offset, (unsigned)p->ranges[i].size);
        return 0;
    }
    if (argc >= 5 && !strcmp(argv[1], "zero")) {
        FILE *f = fopen(argv[3], "rb");
        unsigned char *buf;
        long n;
        int r;
        if (!f) return 2;
        fseek(f, 0, SEEK_END);
        n = ftell(f);
        fseek(f, 0, SEEK_SET);
        buf = malloc(n ? (size_t)n : 1);
        if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n) return 2;
        fclose(f);
        p = arcade_code_pin_find(argv[2]);
        if (!p) {
            free(buf);
            return 4;
        }
        r = arcade_code_zero(p, buf, (size_t)n - (argc >= 6 && !strcmp(argv[5], "short") ? 1 : 0));
        if (r) {
            free(buf);
            return 3;
        }
        f = fopen(argv[4], "wb");
        if (!f || fwrite(buf, 1, (size_t)n, f) != (size_t)n) return 2;
        fclose(f);
        free(buf);
        return 0;
    }
    fprintf(stderr, "usage: code_pins_test list | find NAME | ranges NAME | zero NAME IN OUT [short]\n");
    return 2;
}
