/* The C library names the game calls: Sony's BIOS functions (A0 table) behind
 * stubs in the game's library, replaced here by host routines.
 *
 * Host names are `port_h_NAME`, so that no game or host library name meets
 * another. The game's code and the stubs: `addiu t2, 0xA0; jr t2; addiu t1, N`
 * (the listing), where N is the A0 call number in the comment of each. */
#include "port.h"

#include <stdarg.h>
#include <stdlib.h>

/* A0 2Ah memcpy(dst, src, n): copies n bytes forward, one at a time as the BIOS does, and
 * returns dst (PSX-SPX). Overlap behaves as a forward byte copy, not as memmove. */
void *port_h_memcpy(void *dst, const void *src, unsigned n);
void *port_h_memcpy(void *dst, const void *src, unsigned n)
{
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

/* A0 2Bh memset(dst, c, n): stores the low byte of c n times; returns dst (PSX-SPX). */
void *port_h_memset(void *dst, int c, unsigned n);
void *port_h_memset(void *dst, int c, unsigned n)
{
    unsigned char *d = dst;
    while (n--) *d++ = (unsigned char)c;
    return dst;
}

/* A0 19h strcpy(dst, src): copies up to and including the terminator; returns dst (PSX-SPX). */
char *port_h_strcpy(char *dst, const char *src);
char *port_h_strcpy(char *dst, const char *src)
{
    char *d = dst;
    while ((*d++ = *src++) != 0) {
    }
    return dst;
}

/* A0 15h strcat(dst, src): appends src at the end of dst; returns dst (PSX-SPX). */
char *port_h_strcat(char *dst, const char *src);
char *port_h_strcat(char *dst, const char *src)
{
    char *d = dst;
    while (*d) d++;
    while ((*d++ = *src++) != 0) {
    }
    return dst;
}

/* A0 17h strcmp(a, b): 0 when equal, else the difference of the first unequal bytes (unsigned chars).
 * The game only compares the result with 0 (slot0f), so the sign convention is not tested by it. */
int port_h_strcmp(const char *a, const char *b);
int port_h_strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}

/* A0 2Fh rand(): the generator of PSY-Q's documentation and of the BIOS (PSX-SPX): next = next * 1103515245 + 12345;
 * returns (next >> 16) & 0x7fff, with next starting at 1. The game's image holds only the A0 stub, not
 * these constants (the BIOS ROM has the code), so the constants are from the documentation, not from the listing. */
static unsigned rand_next = 1;
int port_h_rand(void);
int port_h_rand(void)
{
    rand_next = rand_next * 1103515245u + 12345u;
    return (int)((rand_next >> 16) & 0x7fffu);
}

/* A0 3Fh printf(fmt, ...): the BIOS prints to its debug console; here to standard error, so
 * that standard output keeps only the program's own lines. */
int port_h_printf(const char *fmt, ...);
int port_h_printf(const char *fmt, ...)
{
    va_list ap;
    int n;
    va_start(ap, fmt);
    n = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return n;
}

const struct port_library port_c_library[] = {
    { "memcpy", (void *)port_h_memcpy, NULL },
    { "memset", (void *)port_h_memset, NULL },
    { "strcpy", (void *)port_h_strcpy, NULL },
    { "strcat", (void *)port_h_strcat, NULL },
    { "strcmp", (void *)port_h_strcmp, NULL },
    { "rand", (void *)port_h_rand, NULL },
    { "printf", (void *)port_h_printf, NULL },
    { NULL, NULL, NULL }
};
