/* Reading the members of the user's arcade archive (a ZIP file).
 *
 * Project-authored, plain C99: the central directory, stored and deflated
 * members (RFC 1951), and the CRC-32 of each member. No ZIP64, no
 * encryption, no other method. Every offset and length is checked against
 * the file before it is used; a fault is -1 with a line in err. */
#ifndef ARCADE_ZIP_H
#define ARCADE_ZIP_H

#include <stddef.h>
#include <stdint.h>

struct arcade_zip_member {
    char name[64];      /* as stored, at most 63 bytes */
    uint16_t method;    /* 0 stored, 8 deflated */
    uint32_t crc;       /* of the uncompressed bytes */
    uint32_t csize;     /* compressed bytes */
    uint32_t usize;     /* uncompressed bytes */
    uint32_t data;      /* offset in the file of the member's compressed bytes (after its local header) */
};

struct arcade_zip {
    unsigned char *file;    /* the whole archive */
    size_t size;
    struct arcade_zip_member *members;
    unsigned count;
};

/* Read the archive at path and its central directory. Refused: a missing end
 * record, a directory outside the file, ZIP64 values, a name of 64 bytes or
 * more, two members of one name, an encrypted member, a method other than 0
 * and 8, a local header that does not match its directory entry, compressed
 * bytes outside the file, more than 4096 members. */
int arcade_zip_open(const char *path, struct arcade_zip *z, char *err, size_t errsize);
void arcade_zip_close(struct arcade_zip *z);
/* The member of exactly this name, or NULL. */
const struct arcade_zip_member *arcade_zip_find(const struct arcade_zip *z, const char *name);
/* The member's uncompressed bytes in a new buffer (free() it) of m->usize
 * bytes (at least 1 allocated). Refused: deflate data that is malformed, that
 * ends early or gives another length, and a CRC-32 that differs. */
int arcade_zip_read(const struct arcade_zip *z, const struct arcade_zip_member *m, unsigned char **out, char *err, size_t errsize);

/* Raw deflate (no zlib header) of in[0..insize) into exactly outsize bytes.
 * 0 when the final block ends with out full; -1 on any malformed or
 * truncated input, a distance before the start of out, or output that would
 * run past outsize or stop short of it. */
int arcade_inflate(const unsigned char *in, size_t insize, unsigned char *out, size_t outsize);
uint32_t arcade_crc32(const unsigned char *p, size_t n);

#endif
