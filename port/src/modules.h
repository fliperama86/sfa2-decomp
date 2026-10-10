/* The modules layer: the jumps of an overlay module, written when the
 * module is first called, for pinned content only.
 *
 * main.c calls port_modules_init once, after the resident jumps are written
 * (port_jumps_write and port_library_install) and before the game starts. The
 * disc layer (cd.c) must have been started by port_cd_init. Everything else
 * is registered by this layer: it sets port_cd_written_hook (cd.h),
 * port_module_known (port.h) and, on Windows, a vectored exception handler. */
#ifndef PORT_MODULES_H
#define PORT_MODULES_H

#include "port.h"

/* Make the pages the disc writes from now on non-executable and be ready to
 * place the module that a call into one of them meets. The resident program's text range [text_address,
 * text_address + text_size) is never made non-executable and never a module's. If the system cannot
 * give that (no handler), it ends the program with `refused: ...` and status 2. */
void port_modules_init(unsigned text_address, unsigned text_size);

#endif
