#include "elf.h"
#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PT_LOAD 1
#define SHT_SYMTAB 2
#define SHN_LORESERVE 0xff00

static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static uint32_t rd32(const uint8_t *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24; }

static int fail(char *err, size_t errsize, const char *path, const char *what)
{
    snprintf(err, errsize, "%s: %s", path, what);
    return -1;
}

static int by_address(const void *a, const void *b)
{
    const struct elf_symbol *x = a, *y = b;
    if (x->address != y->address) return x->address < y->address ? -1 : 1;
    return strcmp(x->name, y->name);
}

int elf_load(const char *path, struct elf_image *image, char *err, size_t errsize)
{
    FILE *f = fopen(path, "rb");
    uint8_t *d;
    long length;
    unsigned i, phnum, shnum;
    uint32_t phoff, shoff;

    memset(image, 0, sizeof *image);
    if (!f) return fail(err, errsize, path, "cannot open");
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    d = length > 0 ? malloc((size_t)length) : NULL;
    if (!d || fread(d, 1, (size_t)length, f) != (size_t)length) {
        fclose(f);
        free(d);
        return fail(err, errsize, path, "cannot read");
    }
    fclose(f);
    if (length < 52 || memcmp(d, "\x7f" "ELF", 4) != 0 || d[4] != 1 || d[5] != 1 || rd16(d + 18) != 183) {
        free(d);
        return fail(err, errsize, path, "not a 32-bit little-endian AArch64 ELF file");
    }
    image->entry = rd32(d + 24);
    phoff = rd32(d + 28);
    shoff = rd32(d + 32);
    phnum = rd16(d + 44);
    shnum = rd16(d + 48);
    if (phoff + (uint64_t)phnum * 32 > (uint64_t)length || shoff + (uint64_t)shnum * 40 > (uint64_t)length) {
        free(d);
        return fail(err, errsize, path, "the headers run past the end");
    }
    for (i = 0; i < phnum; i++) {
        const uint8_t *h = d + phoff + 32 * i;
        uint32_t offset = rd32(h + 4), vaddr = rd32(h + 8), filesz = rd32(h + 16), memsz = rd32(h + 20), flags = rd32(h + 24);
        void *at;
        int perms;
        if (rd32(h) != PT_LOAD || memsz == 0) continue;
        if (vaddr < VM_IMAGE_BASE || (uint64_t)vaddr + memsz > VM_IMAGE_LIMIT || vaddr % 0x1000 || filesz > memsz || (uint64_t)offset + filesz > (uint64_t)length) {
            free(d);
            return fail(err, errsize, path, "a segment is outside the image's area, not on a page, or runs past the file");
        }
        at = vm_g(vaddr, memsz);
        if (!at) {
            free(d);
            return fail(err, errsize, path, "a segment is not in the guest's memory");
        }
        memcpy(at, d + offset, filesz);
        perms = (flags & 4 ? VM_R : 0) | (flags & 2 ? VM_W : 0) | (flags & 1 ? VM_X : 0);
        if (flags & 1) {
            if (image->text_end == 0 || vaddr < image->text_begin) image->text_begin = vaddr;
            if (vaddr + memsz > image->text_end) image->text_end = vaddr + memsz;
        }
        if (vm_protect(vaddr, ((size_t)memsz + 0xfff) & ~(size_t)0xfff, perms) != 0) {
            free(d);
            return fail(err, errsize, path, "cannot give a segment its permissions");
        }
    }
    for (i = 0; i < shnum; i++) {
        const uint8_t *s = d + shoff + 40 * i;
        uint32_t off, size, link;
        const uint8_t *str, *sym;
        size_t n, k;
        if (rd32(s + 4) != SHT_SYMTAB) continue;
        off = rd32(s + 16);
        size = rd32(s + 20);
        link = rd32(s + 24);
        if (link >= shnum || (uint64_t)off + size > (uint64_t)length) break;
        {
            const uint8_t *ls = d + shoff + 40 * link;
            uint32_t soff = rd32(ls + 16), ssize = rd32(ls + 20);
            if ((uint64_t)soff + ssize > (uint64_t)length) break;
            image->strings = malloc(ssize + 1);
            if (!image->strings) break;
            memcpy(image->strings, d + soff, ssize);
            image->strings[ssize] = 0;
            str = (const uint8_t *)image->strings;
            n = size / 16;
            image->symbols = malloc(n * sizeof *image->symbols);
            if (!image->symbols) break;
            sym = d + off;
            for (k = 1; k < n; k++) {
                const uint8_t *e = sym + 16 * k;
                uint32_t name = rd32(e);
                unsigned shndx = rd16(e + 14);
                if (name == 0 || name >= ssize || shndx == 0 || (shndx >= SHN_LORESERVE && shndx != 0xfff1)) continue;
                image->symbols[image->count].name = (const char *)str + name;
                image->symbols[image->count].address = rd32(e + 4);
                image->symbols[image->count].size = rd32(e + 8);
                image->count++;
            }
            qsort(image->symbols, image->count, sizeof *image->symbols, by_address);
        }
        break;
    }
    free(d);
    return 0;
}

void elf_free(struct elf_image *image)
{
    free(image->by_name);
    free(image->symbols);
    free(image->strings);
    memset(image, 0, sizeof *image);
}

static int by_name_cmp(const void *a, const void *b)
{
    return strcmp((*(const struct elf_symbol *const *)a)->name, (*(const struct elf_symbol *const *)b)->name);
}

int elf_find(const struct elf_image *image, const char *name, uint32_t *address)
{
    struct elf_image *mutable_image = (struct elf_image *)image;
    const struct elf_symbol key = { name, 0, 0 }, *k = &key, *const *hit;
    if (!image->by_name && image->count) {
        size_t i;
        mutable_image->by_name = malloc(image->count * sizeof *image->by_name);
        if (!mutable_image->by_name) return -1;
        for (i = 0; i < image->count; i++) mutable_image->by_name[i] = &image->symbols[i];
        qsort(mutable_image->by_name, image->count, sizeof *image->by_name, by_name_cmp);
    }
    if (!image->by_name) return -1;
    hit = bsearch(&k, image->by_name, image->count, sizeof *image->by_name, by_name_cmp);
    if (!hit) return -1;
    *address = (*hit)->address;
    return 0;
}

const char *elf_symbol_at(const struct elf_image *image, uint32_t address, uint32_t *offset)
{
    size_t lo = 0, hi = image->count;
    const struct elf_symbol *s;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (image->symbols[mid].address <= address) lo = mid + 1;
        else hi = mid;
    }
    if (lo == 0) return NULL;
    s = &image->symbols[lo - 1];
    if (s->address < VM_IMAGE_BASE || address - s->address > 0x100000u) return NULL;   /* the PS1's own names are absolute symbols, not code of the image */
    *offset = address - s->address;
    return s->name;
}
