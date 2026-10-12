/* The game's memory and code, as the runtime reaches them.
 *
 * In the Windows program the game's addresses are host addresses (the PS1's memory is mapped at its own addresses and the game's C
 * runs natively), so everything below is the identity or a plain call: the objects of the runtime are the same as before this
 * header was used (port/tools/objcheck.py compares them). In the macOS build (PORT_GUEST_VM, port/mac/README.md) the game runs in a
 * virtual machine and its addresses are not host addresses. The rules there:
 *
 *  - A pointer that the game gives to a host routine, or that a game structure holds, is a GUEST ADDRESS in a pointer's type. It is
 *    never dereferenced as it is. port_g (or port_gs for a span of known size) turns it into the host pointer to the same bytes; the
 *    result is used for the access and not kept. A routine that only passes the pointer on, or compares it, may use it as it is.
 *  - A host pointer into the game's memory (one that port_g gave, or one into a buffer of the runtime that the game is to see) becomes a
 *    guest address again with port_a, before it is stored in the game's memory, shown in a line or given back to the game.
 *  - A function of the game is a port_fn: a pointer in the Windows program, a guest address on the Mac. It is called with port_call0 to
 *    port_call4 (the arguments are words: integers or guest addresses; the results of these macros are not used) or port_callr0 to
 *    port_callr4 (the result is an unsigned word).
 *  - A host routine that returns a pointer to the game returns a guest address. Memory of the runtime that the game is given the address
 *    of (a cell, a buffer) lives in the game's memory: port_cell.
 */
#ifndef PORT_GUEST_H
#define PORT_GUEST_H

#include <stddef.h>
#include <stdint.h>

#ifdef PORT_GUEST_VM

typedef uint32_t port_fn;
#define PORT_FN_NULL 0u
#define PORT_FN_ADDR(f) ((const void *)(uintptr_t)(f))   /* for port_target_check and for lines that name a function */
#define PORT_FN_PTR(f)  ((void *)(uintptr_t)(f))         /* a function of the game given back to the game as a pointer */

/* port/mac/guest.c */
void    *port_gs(const void *guest, size_t size);        /* NULL when [guest, guest + size) is not memory of the game */
unsigned port_a(const void *host);                       /* 0 when `host` is not in the game's memory */
uint32_t port_guest_call(port_fn f, unsigned nargs, ...);   /* each argument a 32-bit word */
/* A cell of the game's memory, `size` bytes, made once for the name `key` and never freed: its guest address as a pointer (see above). */
void    *port_cell(const char *key, size_t size);
static inline void *port_g(const void *guest) { return port_gs(guest, 1); }

#define port_call0(f)             ((void)port_guest_call((f), 0))
#define port_call1(f, a)          ((void)port_guest_call((f), 1, (uint32_t)(uintptr_t)(a)))
#define port_call2(f, a, b)       ((void)port_guest_call((f), 2, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b)))
#define port_call3(f, a, b, c)    ((void)port_guest_call((f), 3, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b), (uint32_t)(uintptr_t)(c)))
#define port_call4(f, a, b, c, d) ((void)port_guest_call((f), 4, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b), (uint32_t)(uintptr_t)(c), (uint32_t)(uintptr_t)(d)))
#define port_callr0(f)             port_guest_call((f), 0)
#define port_callr1(f, a)          port_guest_call((f), 1, (uint32_t)(uintptr_t)(a))
#define port_callr2(f, a, b)       port_guest_call((f), 2, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b))
#define port_callr3(f, a, b, c)    port_guest_call((f), 3, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b), (uint32_t)(uintptr_t)(c))
#define port_callr4(f, a, b, c, d) port_guest_call((f), 4, (uint32_t)(uintptr_t)(a), (uint32_t)(uintptr_t)(b), (uint32_t)(uintptr_t)(c), (uint32_t)(uintptr_t)(d))

#else

typedef void (*port_fn)(void);
#define PORT_FN_NULL ((port_fn)0)
#define PORT_FN_ADDR(f) ((const void *)(f))
#define PORT_FN_PTR(f)  ((void *)(f))

static inline void    *port_gs(const void *guest, size_t size) { (void)size; return (void *)guest; }
static inline void    *port_g(const void *guest) { return (void *)guest; }
static inline unsigned port_a(const void *host) { return (unsigned)(uintptr_t)host; }
/* The cell is the runtime's own static storage: there is no other memory to put it in. */
#define port_cell(key, size) ((void *)0)

#define port_call0(f)              ((f)())
#define port_call1(f, a)           (((void (*)(unsigned))(f))((unsigned)(uintptr_t)(a)))
#define port_call2(f, a, b)        (((void (*)(unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b)))
#define port_call3(f, a, b, c)     (((void (*)(unsigned, unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b), (unsigned)(uintptr_t)(c)))
#define port_call4(f, a, b, c, d)  (((void (*)(unsigned, unsigned, unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b), (unsigned)(uintptr_t)(c), (unsigned)(uintptr_t)(d)))
#define port_callr0(f)             (((unsigned (*)(void))(f))())
#define port_callr1(f, a)          (((unsigned (*)(unsigned))(f))((unsigned)(uintptr_t)(a)))
#define port_callr2(f, a, b)       (((unsigned (*)(unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b)))
#define port_callr3(f, a, b, c)    (((unsigned (*)(unsigned, unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b), (unsigned)(uintptr_t)(c)))
#define port_callr4(f, a, b, c, d) (((unsigned (*)(unsigned, unsigned, unsigned, unsigned))(f))((unsigned)(uintptr_t)(a), (unsigned)(uintptr_t)(b), (unsigned)(uintptr_t)(c), (unsigned)(uintptr_t)(d)))

#endif

#endif
