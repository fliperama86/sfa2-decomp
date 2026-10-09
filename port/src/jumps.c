/* The jumps at the game's resident functions, and the stop routine.
 *
 * A function with C gets `E9 rel32` to its host implementation. A function
 * without C gets `E8 rel32` to port_stop_entry, which finds out from its
 * return address which function was meant. Nothing is interpreted or emulated. */
#include "port.h"
#include "port_tables.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int by_address(const void *a, const void *b)
{
    unsigned x = *(const unsigned *)a, y = *(const unsigned *)b;
    return x < y ? -1 : x > y;
}

static int in_ram(unsigned address)
{
    return address >= PORT_RAM_BASE && address <= PORT_RAM_BASE + PORT_RAM_SIZE - 5;
}

static void put(unsigned char *ram, unsigned address, unsigned char opcode, const void *target)
{
    unsigned char *site = ram + (address - PORT_RAM_BASE);
    uint32_t rel = (uint32_t)((uintptr_t)target - ((uintptr_t)site + 5));
    site[0] = opcode;
    memcpy(site + 1, &rel, 4);
}

int port_jumps_write(unsigned char *ram, unsigned *with_c, unsigned *without_c, char *err, size_t errsize)
{
    unsigned i, n = 0, *addresses;

    if (sizeof(void *) != 4) {
        snprintf(err, errsize, "jumps: the port is a 32-bit program (a 5-byte jump reaches only 32-bit addresses)");
        return -1;
    }
    *with_c = *without_c = 0;
    addresses = malloc(((size_t)port_function_count + port_absent_count + 1) * sizeof *addresses);
    if (!addresses) {
        snprintf(err, errsize, "jumps: out of memory");
        return -1;
    }
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].image == -1) {
            if (!in_ram(port_functions[i].address)) {
                snprintf(err, errsize, "jumps: function %s at 0x%08x is outside RAM", port_functions[i].name, port_functions[i].address);
                free(addresses);
                return -1;
            }
            if (!port_functions[i].impl) {
                snprintf(err, errsize, "jumps: function %s at 0x%08x has no implementation", port_functions[i].name, port_functions[i].address);
                free(addresses);
                return -1;
            }
            addresses[n++] = port_functions[i].address;
        }
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == -1) {
            if (!in_ram(port_absents[i].address)) {
                snprintf(err, errsize, "jumps: function %s at 0x%08x is outside RAM", port_absents[i].name, port_absents[i].address);
                free(addresses);
                return -1;
            }
            addresses[n++] = port_absents[i].address;
        }
    qsort(addresses, n, sizeof *addresses, by_address);
    for (i = 1; i < n; i++)
        if (addresses[i] == addresses[i - 1]) {
            snprintf(err, errsize, "jumps: two entries at 0x%08x", addresses[i]);
            free(addresses);
            return -1;
        }
    free(addresses);
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].image == -1) {
            put(ram, port_functions[i].address, 0xe9, port_functions[i].impl);
            ++*with_c;
        }
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == -1) {
            put(ram, port_absents[i].address, 0xe8, (const void *)port_stop_entry);
            ++*without_c;
        }
    return 0;
}

static void finish(int status)
{
    fflush(stdout);
    exit(status);
}

/* Called by port_stop_entry with the return address of the 5-byte call. */
void port_stop(unsigned returned);
void port_stop(unsigned returned)
{
    unsigned address = returned - 5, i;
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == -1 && port_absents[i].address == address) {
            if (port_absents[i].library) {
                printf("stop: library function %s (0x%08x) has no host routine yet\n", port_absents[i].name, address);
                finish(4);
            }
            printf("stop: no C yet for %s (0x%08x)\n", port_absents[i].name, address);
            finish(3);
        }
    printf("stop: unknown function at 0x%08x\n", address);
    finish(5);
}

void port_stop_main_returned(void)
{
    printf("stop: main returned\n");
    finish(0);
}

/* The entry is a few instructions of assembly, not C, because it must read
 * the word at the top of the stack as it was on entry (the address after the
 * 5-byte call), and a C function cannot promise that at -O1: the compiler may
 * have moved the stack pointer or built a frame before any C statement runs,
 * and __builtin_return_address(0) would then depend on the frame it chose.
 * The stub passes that word as the argument of port_stop, with the stack
 * aligned, and never returns. */
#if defined(__i386__)
#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define PORT_US PORT_STR(__USER_LABEL_PREFIX__)
__asm__(".text\n"
        ".globl " PORT_US "port_stop_entry\n" PORT_US "port_stop_entry:\n"
        "\tmovl (%esp), %eax\n"
        "\tandl $-16, %esp\n"
        "\tsubl $12, %esp\n"
        "\tpushl %eax\n"
        "\tcall " PORT_US "port_stop\n"
        "\thlt\n");
#else
void port_stop_entry(void)
{
    port_stop(0);
}
#endif
