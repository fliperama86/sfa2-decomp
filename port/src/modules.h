/* The modules package (L4): the jumps of an overlay module, written when the
 * module is first called.
 *
 * L1's main calls port_modules_init once, after the resident jumps are written
 * (port_jumps_write and port_library_install) and before the game starts. The
 * disc layer (cd.c) must have been started by port_cd_init. Everything else
 * is registered by this package: it sets port_cd_written_hook (cd.h) and, on
 * Windows, a vectored exception handler. */
#ifndef PORT_MODULES_H
#define PORT_MODULES_H

#include "port.h"

/* Make the pages the disc writes from now on non-executable and be ready to
 * place the module that a call into one of them meets. If the system cannot
 * give that (no handler), it ends the program with `refused: ...` and status 2. */
void port_modules_init(void);

/* One file of the disc's directory: the name without its version suffix, lower
 * case; the first sector; the length in bytes. */
struct port_disc_file {
    char name[32];
    unsigned sector, size;
};
/* disc.c: every file of the root directory and of the folders one level below
 * it, in directory order, into out[0..max); *count says how many. */
int port_disc_list(struct port_disc *d, struct port_disc_file *out, unsigned max, unsigned *count, char *err, size_t errsize);

#endif
