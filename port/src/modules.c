/* Modules: the jumps of an overlay module, written when the module is first called.
 *
 * The game loads modules from the disc into memory that several modules share
 * over time. The linked program holds the C of every module, and which C stands
 * at an address depends on what the disc put there. So:
 *
 *  1. Every page that CdGetSector writes loses its execute permission (the hook
 *     port_cd_written_hook of cd.c calls us), and the jumps that stood on it are
 *     gone with the bytes the disc wrote.
 *  2. A call into such a page is an access fault of the kind "execute". The
 *     handler takes the faulting address and asks the disc layer from which
 *     sector the page was last written. That sector is in a file of the disc's
 *     directory (an archive); the archive's own header (docs/overlays.md:
 *     u32 count, then 32-byte entries of u16 slot, u16 table, u32 size, chunk
 *     data from 0x800, each chunk padded to 2048 bytes) says which chunk holds
 *     the sector, hence its slot, table and exact length. The image is the one
 *     of the tables with that slot whose archives hold that file and whose
 *     address range [address, address + chunk length) holds the faulting address.
 *  3. The jumps of that image's functions go on the pages of the image that the
 *     disc wrote from that chunk (E9 to the C; E8 to the stop call without C),
 *     those pages become executable, one line is printed and the call resumes.
 *  4. A later disc write to those pages (hook) takes the permission away and
 *     forgets the owner; the next call goes through the handler again.
 *
 * The extent of an image is the length of its chunk in the archive (read at run
 * time), not something the tables carry: the tables know functions, not data.
 *
 * The system-specific part (page protection and the fault handler) is the last
 * section of this file. Nothing here interprets or emulates the game's code. */
#include "modules.h"
#include "cd.h"
#include "port_tables.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PAGE 0x1000u
#define NPAGES (PORT_RAM_SIZE / PAGE)
#define MAX_FILES 8192u

/* ---- the system part (defined at the end) ---------------------------- */
static int sys_protect(unsigned address, unsigned size, int executable);
static int sys_install(void);

/* ---- state ------------------------------------------------------------ */
static unsigned char exec_page[NPAGES];    /* 1: the page is executable */
static short owner_page[NPAGES];           /* image index + 1 that placed its jumps on the page, 0: none */
static struct port_disc_file *files;
static unsigned file_count;
static int installed;

static void finish(int status)
{
    fflush(stdout);
    exit(status);
}

static void refuse(const char *line)
{
    printf("refused: %s\n", line);
    finish(PORT_EXIT_REFUSED);
}

static unsigned le32(const unsigned char *p)
{
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

static unsigned le16(const unsigned char *p)
{
    return (unsigned)p[0] | (unsigned)p[1] << 8;
}

static int same_name(const char *a, const char *b)
{
    for (; *a && *b; a++, b++)
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
    return *a == *b;
}

static void jump_site(unsigned address, unsigned char opcode, const void *target)
{
    unsigned char *site = (unsigned char *)(size_t)address;
    uint32_t rel = (uint32_t)((uintptr_t)target - ((uintptr_t)site + 5));
    site[0] = opcode;
    memcpy(site + 1, &rel, 4);
}

/* ---- the stop call of a module function without C ---------------------- */

void port_module_stop(unsigned returned);
void port_module_stop(unsigned returned)
{
    unsigned address = returned - 5, i;
    int owner = address >= PORT_RAM_BASE && address - PORT_RAM_BASE < PORT_RAM_SIZE ? owner_page[(address - PORT_RAM_BASE) / PAGE] - 1 : -1;
    for (i = 0; owner >= 0 && i < port_absent_count; i++)
        if (port_absents[i].image == owner && port_absents[i].address == address) {
            printf("stop: no C yet for %s (0x%08x)\n", port_absents[i].name, address);
            finish(PORT_EXIT_NO_C);
        }
    printf("stop: unknown function at 0x%08x\n", address);
    finish(PORT_EXIT_UNKNOWN);
}

void port_module_stop_entry(void);
#if defined(__i386__)
#define PORT_STR2(x) #x
#define PORT_STR(x) PORT_STR2(x)
#define PORT_US PORT_STR(__USER_LABEL_PREFIX__)
/* As port_stop_entry in jumps.c: reads the return address of the 5-byte call. */
__asm__(".text\n"
        ".globl " PORT_US "port_module_stop_entry\n" PORT_US "port_module_stop_entry:\n"
        "\tmovl (%esp), %eax\n"
        "\tandl $-16, %esp\n"
        "\tsubl $12, %esp\n"
        "\tpushl %eax\n"
        "\tcall " PORT_US "port_module_stop\n"
        "\thlt\n");
#else
void port_module_stop_entry(void)
{
    port_module_stop(0);
}
#endif

/* ---- the disc's directory and the archive headers ---------------------- */

static void load_files(void)
{
    char err[PORT_ERR];
    struct port_disc *d = port_cd_disc();
    if (files) return;
    if (!d) {
        printf("stop: modules: the disc layer was not started\n");
        finish(PORT_EXIT_DISC);
    }
    files = malloc(MAX_FILES * sizeof *files);
    if (!files || port_disc_list(d, files, MAX_FILES, &file_count, err, sizeof err) != 0) {
        printf("stop: modules: cannot list the disc's files: %s\n", files ? err : "out of memory");
        finish(PORT_EXIT_DISC);
    }
}

static const struct port_disc_file *file_of_sector(unsigned sector)
{
    unsigned i;
    for (i = 0; i < file_count; i++)
        if (sector >= files[i].sector && sector < files[i].sector + (files[i].size + PORT_DATA - 1) / PORT_DATA) return &files[i];
    return NULL;
}

struct chunk {
    unsigned index, slot, table, size;
    unsigned first_sector, sectors;   /* on the disc */
};

/* The chunk of the archive that holds `sector`. 0, or -1 with a reason. */
static int chunk_of(const struct port_disc_file *f, unsigned sector, struct chunk *c, const char **why)
{
    unsigned char head[PORT_DATA];
    char err[PORT_ERR];
    unsigned count, i, offset = 0x800;
    if (port_disc_read(port_cd_disc(), f->sector, PORT_DATA, head, err, sizeof err) != 0) {
        *why = "its header cannot be read";
        return -1;
    }
    count = le32(head);
    if (count == 0 || count > (PORT_DATA - 0x20) / 32) {
        *why = "its header does not look like an archive";
        return -1;
    }
    for (i = 0; i < count; i++) {
        const unsigned char *e = head + 0x20 + 32 * i;
        unsigned size = le32(e + 4), padded = (size + PORT_DATA - 1) / PORT_DATA * PORT_DATA;
        unsigned first = f->sector + offset / PORT_DATA;
        if (sector >= first && sector < first + padded / PORT_DATA) {
            c->index = i;
            c->slot = le16(e);
            c->table = le16(e + 2);
            c->size = size;
            c->first_sector = first;
            c->sectors = padded / PORT_DATA;
            return 0;
        }
        offset += padded;
    }
    *why = "its header has no chunk at that sector";
    return -1;
}

/* ---- the tables ------------------------------------------------------- */

static int carries(const struct port_image *im, const char *archive)
{
    const char *const *a;
    for (a = im->archives; a && *a; a++)
        if (same_name(*a, archive)) return 1;
    return 0;
}

static int is_function_start(int image, unsigned address)
{
    unsigned i;
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].image == image && port_functions[i].address == address) return 1;
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == image && port_absents[i].address == address) return 1;
    return 0;
}

static int has_entries(int image)
{
    unsigned i;
    for (i = 0; i < port_function_count; i++)
        if (port_functions[i].image == image) return 1;
    for (i = 0; i < port_absent_count; i++)
        if (port_absents[i].image == image) return 1;
    return 0;
}

/* The names of the images whose range holds `address`: [address, address + extent) when the chunk (and so the
 * extent) is known, else [address, the last function the tables hold for the image]. */
static void images_at(unsigned address, unsigned extent, char *out, size_t size)
{
    unsigned i, j, used = 0;
    out[0] = 0;
    for (i = 0; i < port_image_count; i++) {
        unsigned last = port_images[i].address;
        int n;
        if (address < port_images[i].address) continue;
        for (j = 0; j < port_function_count; j++)
            if (port_functions[j].image == (int)i && port_functions[j].address > last) last = port_functions[j].address;
        for (j = 0; j < port_absent_count; j++)
            if (port_absents[j].image == (int)i && port_absents[j].address > last) last = port_absents[j].address;
        if (extent ? address - port_images[i].address >= extent : address > last) continue;
        n = snprintf(out + used, size - used, "%s%s", used ? ", " : "", port_images[i].name);
        if (n < 0 || (size_t)n >= size - used) break;
        used += (unsigned)n;
    }
    if (!used) snprintf(out, size, "none");
}

static void unplaced(unsigned address, unsigned extent, int sector, const struct port_disc_file *f, const char *why)
{
    char at[512];
    images_at(address, extent, at, sizeof at);
    if (f)
        printf("stop: call to 0x%08x, which no module can be placed at: the page was written from sector %d of %s (%s); images the tables have at that address: %s\n",
               address, sector, f->name, why, at);
    else
        printf("stop: call to 0x%08x, which no module can be placed at: the page was written from sector %d, which is in no file of the disc (%s); images the tables have at that address: %s\n",
               address, sector, why, at);
    finish(PORT_EXIT_UNKNOWN);
}

/* ---- placing a module -------------------------------------------------- */

static void place(int index, const struct chunk *c, unsigned fault)
{
    const struct port_image *im = &port_images[index];
    unsigned lo = im->address & ~(PAGE - 1), hi = (im->address + c->size + PAGE - 1) & ~(PAGE - 1), p, i;
    unsigned with_c = 0, without_c = 0;
    int src;

    if (hi > PORT_RAM_BASE + PORT_RAM_SIZE) hi = PORT_RAM_BASE + PORT_RAM_SIZE;
#define OURS(page) ((src = port_cd_page_source(page)) >= 0 && (unsigned)src >= c->first_sector && (unsigned)src < c->first_sector + c->sectors)
    for (i = 0; i < port_function_count; i++) {
        unsigned a = port_functions[i].address;
        if (port_functions[i].image != index || a < lo || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1))) {
            jump_site(a, 0xe9, port_functions[i].impl);
            with_c++;
        }
    }
    for (i = 0; i < port_absent_count; i++) {
        unsigned a = port_absents[i].address;
        if (port_absents[i].image != index || a < lo || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1))) {
            jump_site(a, 0xe8, (const void *)port_module_stop_entry);
            without_c++;
        }
    }
    /* the pages: owner and permission, in runs */
    for (p = lo; p < hi;) {
        unsigned q;
        if (!OURS(p)) {
            p += PAGE;
            continue;
        }
        for (q = p; q < hi && OURS(q); q += PAGE) owner_page[(q - PORT_RAM_BASE) / PAGE] = (short)(index + 1);
        if (sys_protect(p, q - p, 1) != 0) {
            printf("stop: modules: cannot make 0x%08x..0x%08x executable\n", p, q);
            finish(PORT_EXIT_OTHER);
        }
        for (; p < q; p += PAGE) exec_page[(p - PORT_RAM_BASE) / PAGE] = 1;
    }
#undef OURS
    printf("module: %s at 0x%08x, %u C jumps, %u without C\n", im->name, im->address, with_c, without_c);
    fflush(stdout);
    if (!exec_page[(fault - PORT_RAM_BASE) / PAGE]) {
        printf("stop: modules: the page of 0x%08x is still not executable after placing %s\n", fault, im->name);
        finish(PORT_EXIT_OTHER);
    }
}

/* Handle an execute fault at `address`. 0: resumed. -1: not ours to handle. */
static int fault(unsigned address)
{
    int src, found = -1;
    unsigned i;
    const struct port_disc_file *f;
    struct chunk c;
    const char *why = "";
    char ambiguous[160] = "";

    if (!installed || address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return -1;
    src = port_cd_page_source(address);
    if (src < 0) return -1;
    load_files();
    f = file_of_sector((unsigned)src);
    if (!f) unplaced(address, 0, src, NULL, "no file");
    if (chunk_of(f, (unsigned)src, &c, &why) != 0) unplaced(address, 0, src, f, why);
    for (i = 0; i < port_image_count; i++) {
        const struct port_image *im = &port_images[i];
        if (im->slot != c.slot || c.table != 0 || !carries(im, f->name)) continue;
        if (address < im->address || address - im->address >= c.size) continue;
        if (found >= 0) {
            snprintf(ambiguous, sizeof ambiguous, "images %s and %s both fit", port_images[found].name, im->name);
            unplaced(address, c.size, src, f, ambiguous);
        }
        found = (int)i;
    }
    if (found < 0) unplaced(address, c.size, src, f, "no image of the tables has that archive, slot and address");
    if (port_images[found].like && !has_entries(found)) {   /* a second placement the build has not made (L5) */
        printf("stop: call to 0x%08x in image %s, which is a like placement of %s; its C is not built yet\n", address, port_images[found].name, port_images[found].like);
        finish(PORT_EXIT_NO_C);
    }
    if (!is_function_start(found, address)) {
        char name[160];
        snprintf(name, sizeof name, "image %s is there but 0x%08x is no function start of it", port_images[found].name, address);
        unplaced(address, c.size, src, f, name);
    }
    place(found, &c, address);
    return 0;
}

/* ---- the hook of the disc layer ---------------------------------------- */

static void written(unsigned address, unsigned bytes)
{
    unsigned first, last, p;
    if (!bytes || address < PORT_RAM_BASE) return;
    first = (address - PORT_RAM_BASE) / PAGE;
    last = (address - PORT_RAM_BASE + bytes - 1) / PAGE;
    if (last >= NPAGES) last = NPAGES - 1;
    for (p = first; p <= last; p++) {
        owner_page[p] = 0;
        if (exec_page[p]) {
            if (sys_protect(PORT_RAM_BASE + p * PAGE, PAGE, 0) != 0) {
                printf("stop: modules: cannot take the execute permission from 0x%08x\n", PORT_RAM_BASE + p * PAGE);
                finish(PORT_EXIT_OTHER);
            }
            exec_page[p] = 0;
        }
    }
}

void port_modules_init(void)
{
    unsigned p;
    memset(owner_page, 0, sizeof owner_page);
    for (p = 0; p < NPAGES; p++) exec_page[p] = 1;
    if (sys_install() != 0) refuse("modules: no access fault handler for this system");
    installed = 1;
    port_cd_written_hook = written;
    for (p = 0; p < NPAGES; p++)
        if (port_cd_page_source(PORT_RAM_BASE + p * PAGE) >= 0) written(PORT_RAM_BASE + p * PAGE, 1);
}

/* ---- the system part ----------------------------------------------------- */

#ifdef _WIN32
#include <windows.h>

static int sys_protect(unsigned address, unsigned size, int executable)
{
    DWORD old;
    return VirtualProtect((void *)(size_t)address, size, executable ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE, &old) ? 0 : -1;
}

static LONG CALLBACK vectored(EXCEPTION_POINTERS *p)
{
    const EXCEPTION_RECORD *r = p->ExceptionRecord;
    if (r->ExceptionCode != EXCEPTION_ACCESS_VIOLATION || r->NumberParameters < 2 || r->ExceptionInformation[0] != 8) return EXCEPTION_CONTINUE_SEARCH;
    return fault((unsigned)r->ExceptionInformation[1]) == 0 ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}

static int sys_install(void)
{
    return AddVectoredExceptionHandler(1, vectored) ? 0 : -1;
}

#else

static int sys_protect(unsigned address, unsigned size, int executable)
{
    (void)address; (void)size; (void)executable;
    return 0;
}

static int sys_install(void)
{
    return -1;
}

#endif
