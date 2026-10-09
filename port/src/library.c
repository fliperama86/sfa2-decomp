/* The library layer's mechanism: the tables of host routines, their install
 * over the stop calls, the overrides of game functions, the listing, the
 * trace, and the stop line for a host routine that meets something it cannot
 * serve.
 *
 * Nothing here knows a library function by name; the domains' tables do
 * (kernel.c, threads.c, clib.c, sound.c, card.c, overrides.c), registered in domains.c. */
#include "port.h"
#include "port_tables.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void port_halt(int status, const char *fmt, ...)
{
    va_list ap;

    printf("stop: ");
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
    fflush(stdout);
    exit(status);
}

/* ---- the trace ---- */

struct traced {
    unsigned site;      /* the library function's address */
    const char *name;
    void *host;         /* NULL: it has none, the call ends in the stop line */
};

static FILE *trace_file;
static struct traced *traced;
static unsigned traced_count;

void port_trace_set(FILE *f)
{
    trace_file = f;
}

void port_trace_line(const char *fmt, ...)
{
    va_list ap;
    if (!trace_file) return;
    va_start(ap, fmt);
    vfprintf(trace_file, fmt, ap);
    va_end(ap);
    fputc('\n', trace_file);
    fflush(trace_file);
}

void port_stop(unsigned returned);  /* jumps.c */

/* Called by port_trace_entry with the address after the 5-byte call and a
 * pointer to the call's arguments. Returns the host routine to continue in. */
void *port_trace_log(unsigned returned, const unsigned *args);
void *port_trace_log(unsigned returned, const unsigned *args)
{
    unsigned i, site = returned - 5;

    for (i = 0; i < traced_count; i++)
        if (traced[i].site == site) {
            fprintf(trace_file, "%s 0x%x 0x%x 0x%x 0x%x\n", traced[i].name, args[0], args[1], args[2], args[3]);
            fflush(trace_file);
            if (!traced[i].host) {
                fflush(stdout);
                port_stop(returned);
            }
            return traced[i].host;
        }
    port_halt(PORT_EXIT_UNKNOWN, "unknown traced call at 0x%08x", site);
    return NULL;
}

#if defined(__i386__)
#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define PORT_US PORT_STR(__USER_LABEL_PREFIX__)
/* The traced library function's site holds `call port_trace_entry`, so the
 * word at the top of the stack is the site's address plus 5, then come the
 * game's return address and the arguments. The entry logs, drops that word
 * and continues in the host routine as if the game had called it directly. */
__asm__(".text\n"
        ".globl " PORT_US "port_trace_entry\n" PORT_US "port_trace_entry:\n"
        "\tpushl %ebp\n"
        "\tmovl %esp, %ebp\n"
        "\tleal 12(%ebp), %eax\n"
        "\tandl $-16, %esp\n"
        "\tsubl $8, %esp\n"
        "\tpushl %eax\n"
        "\tpushl 4(%ebp)\n"
        "\tcall " PORT_US "port_trace_log\n"
        "\tmovl %ebp, %esp\n"
        "\tpopl %ebp\n"
        "\taddl $4, %esp\n"
        "\tjmp *%eax\n");
void port_trace_entry(void);
#else
void port_trace_entry(void)
{
}
#endif

/* ---- the install ---- */

static void put(unsigned char *ram, unsigned address, unsigned char opcode, const void *target)
{
    unsigned char *site = ram + (address - PORT_RAM_BASE);
    uint32_t rel = (uint32_t)((uintptr_t)target - ((uintptr_t)site + 5));
    site[0] = opcode;
    memcpy(site + 1, &rel, 4);
}

/* A table entry names its function by the build's name, or as `@0xADDRESS`
 * (the function at that address, whatever the build calls it). */
static const struct port_absent *find_library(const char *name)
{
    unsigned i, address = 0;
    char *end = NULL;

    if (name[0] == '@') address = (unsigned)strtoul(name + 1, &end, 16);
    for (i = 0; i < port_absent_count; i++) {
        if (port_absents[i].image != -1 || !port_absents[i].library) continue;
        if (name[0] == '@' ? (*end == 0 && port_absents[i].address == address) : strcmp(port_absents[i].name, name) == 0) return &port_absents[i];
    }
    return NULL;
}

static const struct port_function *find_function(const char *name)
{
    unsigned i;
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].image == -1 && strcmp(port_functions[i].name, name) == 0) return &port_functions[i];
    return NULL;
}

static unsigned override_total(void)
{
    unsigned s, n = 0;
    for (s = 0; s < port_override_set_count; s++) {
        unsigned j;
        for (j = 0; port_override_sets[s][j].name; j++) n++;
    }
    return n;
}

static const struct port_override *override_at(unsigned k)
{
    unsigned s, j;
    for (s = 0; s < port_override_set_count; s++)
        for (j = 0; port_override_sets[s][j].name; j++)
            if (k-- == 0) return &port_override_sets[s][j];
    return NULL;
}

/* Check every table against itself and the build's tables. */
static int check(char *err, size_t errsize)
{
    unsigned d, d2, j, j2, k;

    for (d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++) {
            const struct port_library *e = &port_domains[d].table[j];
            if (!e->host) {
                snprintf(err, errsize, "library: %s (%s) is listed with no routine", e->name, port_domains[d].name);
                return -1;
            }
            for (d2 = d; d2 < port_domain_count; d2++)
                for (j2 = d2 == d ? j + 1 : 0; port_domains[d2].table[j2].name; j2++)
                    if (find_library(port_domains[d2].table[j2].name) && find_library(e->name) == find_library(port_domains[d2].table[j2].name)) {
                        snprintf(err, errsize, "library: %s is listed twice (%s and %s, as %s)", e->name, port_domains[d].name, port_domains[d2].name, port_domains[d2].table[j2].name);
                        return -1;
                    }
            if (!find_library(e->name)) {
                snprintf(err, errsize, "library: %s (%s) is listed but this build has no library function of that name without C", e->name, port_domains[d].name);
                return -1;
            }
        }
    for (k = 0; k < override_total(); k++) {
        const struct port_override *o = override_at(k);
        if (!o->host || !o->note) {
            snprintf(err, errsize, "overrides: %s has no routine or no note", o->name);
            return -1;
        }
        for (j = k + 1; j < override_total(); j++)
            if (strcmp(o->name, override_at(j)->name) == 0) {
                snprintf(err, errsize, "overrides: %s is listed twice", o->name);
                return -1;
            }
        if (!find_function(o->name)) {
            snprintf(err, errsize, "overrides: %s is listed but this build has no function of that name with C", o->name);
            return -1;
        }
    }
    return 0;
}

static const struct port_library *host_for(const struct port_absent *a)
{
    unsigned d, j;
    for (d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++)
            if (find_library(port_domains[d].table[j].name) == a) return &port_domains[d].table[j];
    return NULL;
}

int port_library_install(unsigned char *ram, struct port_install *out, char *err, size_t errsize)
{
    unsigned i, k;

    memset(out, 0, sizeof *out);
    if (check(err, errsize) != 0) return -1;
    if (trace_file) {
        free(traced);
        traced = malloc(((size_t)port_absent_count + 1) * sizeof *traced);
        traced_count = 0;
        if (!traced) {
            snprintf(err, errsize, "library: out of memory");
            return -1;
        }
    }
    for (i = 0; i < port_absent_count; i++) {
        const struct port_absent *a = &port_absents[i];
        const struct port_library *e;
        if (a->image != -1 || !a->library) continue;
        e = host_for(a);
        if (e) {
            out->host++;
            if (e->note) out->noop++;
        } else
            out->stops++;
        if (trace_file) {
            traced[traced_count].site = a->address;
            traced[traced_count].name = a->name;
            traced[traced_count].host = e ? e->host : NULL;
            traced_count++;
            put(ram, a->address, 0xe8, (const void *)port_trace_entry);
        } else if (e)
            put(ram, a->address, 0xe9, e->host);
    }
    for (k = 0; k < override_total(); k++) {
        put(ram, find_function(override_at(k)->name)->address, 0xe9, override_at(k)->host);
        out->overrides++;
    }
    return 0;
}

int port_library_list(char *err, size_t errsize)
{
    unsigned i, d, j, shown;

    if (check(err, errsize) != 0) return -1;
    for (shown = 0, d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++)
            if (!port_domains[d].table[j].note) shown++;
    printf("host routines (%u):\n", shown);
    for (d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++)
            if (!port_domains[d].table[j].note) printf("  %s 0x%08x (%s)\n", port_domains[d].table[j].name, find_library(port_domains[d].table[j].name)->address, port_domains[d].name);
    for (shown = 0, d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++)
            if (port_domains[d].table[j].note) shown++;
    printf("routines that do nothing on purpose (%u):\n", shown);
    for (d = 0; d < port_domain_count; d++)
        for (j = 0; port_domains[d].table[j].name; j++)
            if (port_domains[d].table[j].note) printf("  %s 0x%08x (%s): %s\n", port_domains[d].table[j].name, find_library(port_domains[d].table[j].name)->address, port_domains[d].name, port_domains[d].table[j].note);
    for (shown = 0, i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == -1 && port_absents[i].library && !host_for(&port_absents[i])) shown++;
    printf("left that stop (%u):\n", shown);
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == -1 && port_absents[i].library && !host_for(&port_absents[i]))
            printf("  %s 0x%08x\n", port_absents[i].name, port_absents[i].address);
    printf("overrides of game functions (%u):\n", override_total());
    for (i = 0; i < override_total(); i++) printf("  %s: %s\n", override_at(i)->name, override_at(i)->note);
    {
        unsigned host = 0, stops = 0;
        for (i = 0; i < port_absent_count; i++)
            if (port_absents[i].image == -1 && port_absents[i].library) {
                if (host_for(&port_absents[i])) host++;
                else stops++;
            }
        printf("library: %u host routines, %u left that stop\n", host, stops);
    }
    fflush(stdout);
    return 0;
}
