/* What the shared runtime files (../src) need from the macOS build's platform layer (platform.c), under PORT_GUEST_VM. */
#ifndef PORT_MAC_PLATFORM_H
#define PORT_MAC_PLATFORM_H

#include <stddef.h>
#include <stdio.h>

#include "vm.h"

/* The numbers of the `hvc` stubs that the runtime writes in the game's memory (see README.md). A function without C: PORT_MAC_STOP_BASE plus the
 * row of port_absents. A host routine of the library or an override: PORT_MAC_HOST_BASE plus the order of its registration. */
#define PORT_MAC_STOP_BASE 0x4000u     /* up to PORT_MAC_BOUND_BASE */
#define PORT_MAC_HOST_BASE 0x8000u
/* The stubs of the guest image for the routines of the runtime that the programs' library names are bound to (port_bounds): from this number up. */
#define PORT_MAC_BOUND_BASE 0x7000u

/* The range of the guest's own compiled code (the image's executable segments), for the checks of addresses that the game hands over. */
extern unsigned port_mac_text_begin, port_mac_text_end;

/* Write a stub at the PS1 address `address` of the RAM that starts at `ram` (a host pointer): a branch to the guest address `target`; an `hvc #number`;
 * or, for a host routine, an `hvc` that registers `host` (named `name`, traced to `trace` if not null) and returns the number used. */
void port_mac_put_branch(unsigned char *ram, unsigned address, unsigned target);
void port_mac_put_hvc(unsigned char *ram, unsigned address, unsigned number);
void port_mac_put_host(unsigned char *ram, unsigned address, const void *host, const char *name);
/* After the host wrote stubs: make the guest see them. */
void port_mac_sync(void);
/* Is the word at the PS1 address `address` a stub of this layer (a branch out of the RAM, or an `hvc`) that is still there? */
int  port_mac_stub_intact(unsigned address);

/* --trace: log each hostcall (called by port_trace_set). */
void port_mac_trace_calls(int on);

/* The path of the guest image (--guest FILE; null: guest.elf in the folder of the program), read by port_map. */
extern const char *port_mac_image_path;

#endif
