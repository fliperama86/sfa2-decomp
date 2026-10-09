/* Modules: the jumps of an overlay module, written when the module is first called, for pinned content only.
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
 *  3. Before any jump is written, the chunk is read from the user's image and its SHA-256 must be the one the
 *     build pins for the image (the build's [[image]] sha256 covers exactly the chunk's bytes, as they lie in the
 *     archive); the bytes in memory at each function start must still be the chunk's. A chunk that is not the
 *     pinned one ends the program with a line that names the image, and no jump of that image is written.
 *  4. The jumps of that image's functions go on the pages of the image that the
 *     disc wrote from that chunk (E9 to the C; E8 to the stop call without C),
 *     those pages become executable, one line is printed and the call resumes.
 *  5. A later disc write to those pages (hook) takes the permission away and
 *     forgets the owner; the next call goes through the handler again.
 *
 * Everything read from an archive is bounded before use: the count of chunks, each chunk's length and place
 * against the file's extent on the disc, the chunk against the PS1's RAM, the page and table indexes.
 *
 * Resident code is never a module's: a page that holds any byte of the resident program's text keeps its execute
 * permission whatever the disc copies into its other bytes (a data sector may end in the page where the resident
 * code begins), and a copy that reaches the first bytes of a resident function, where the start-up wrote a jump
 * to the C, ends the program with a line (the C stays and the bytes would not).
 *
 * Which faults the handler accepts: an execute fault, on the game's thread, whose address is the instruction
 * pointer, inside the PS1's RAM, on a page that this file made non-executable, whose bytes the disc layer wrote,
 * while no fault of this kind is being handled. Everything else goes on to the program's crash line.
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
static unsigned sys_thread(void);

/* ---- state ------------------------------------------------------------ */
static volatile unsigned char exec_page[NPAGES];    /* 1: the page is executable (read by the timer thread too) */
static short owner_page[NPAGES];           /* image index + 1 that placed its jumps on the page, 0: none */
static struct port_disc_file *files;
static unsigned file_count;
static int installed;
static int handling;                       /* a fault is being handled: a second one is not ours */
static unsigned game_thread;               /* the id of the thread that called port_modules_init */
static unsigned text_lo, text_hi;          /* the resident program's text range: [lo, hi) */

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
        if (sector >= files[i].sector && (unsigned long long)sector < (unsigned long long)files[i].sector + ((unsigned long long)files[i].size + PORT_DATA - 1) / PORT_DATA) return &files[i];
    return NULL;
}

struct chunk {
    unsigned index, slot, table, size;
    unsigned first_sector, sectors;   /* on the disc */
};

/* The chunk of the archive that holds `sector`. 0, or -1 with a reason. Every length is checked against the
 * file's extent on the disc before it is used, whichever chunk is asked for. */
static int chunk_of(const struct port_disc_file *f, unsigned sector, struct chunk *c, const char **why)
{
    unsigned char head[PORT_DATA];
    char err[PORT_ERR];
    unsigned count, i;
    unsigned long long offset = 0x800, extent = ((unsigned long long)f->size + PORT_DATA - 1) / PORT_DATA * PORT_DATA;
    int found = -1;

    if (f->size < PORT_DATA || port_disc_read(port_cd_disc(), f->sector, PORT_DATA, head, err, sizeof err) != 0) {
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
        unsigned long long size = le32(e + 4), padded = (size + PORT_DATA - 1) / PORT_DATA * PORT_DATA;
        unsigned long long first = (unsigned long long)f->sector + offset / PORT_DATA;
        if (offset + padded > extent) {
            *why = "its header gives a chunk that runs past the end of the file";
            return -1;
        }
        if (found < 0 && sector >= first && sector < first + padded / PORT_DATA) {
            found = (int)i;
            c->index = i;
            c->slot = le16(e);
            c->table = le16(e + 2);
            c->size = (unsigned)size;
            c->first_sector = (unsigned)first;
            c->sectors = (unsigned)(padded / PORT_DATA);
        }
        offset += padded;
    }
    if (found < 0) {
        *why = "its header has no chunk at that sector";
        return -1;
    }
    return 0;
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

/* A call that no module can be placed at. In `quiet` mode (the check of an address the game handed over) it only
 * reports the failure; otherwise it ends the program with the line. */
static int unplaced(int quiet, unsigned address, unsigned extent, int sector, const struct port_disc_file *f, const char *why)
{
    char at[512];
    if (quiet) return -1;
    images_at(address, extent, at, sizeof at);
    if (f)
        printf("stop: call to 0x%08x, which no module can be placed at: the page was written from sector %d of %s (%s); images the tables have at that address: %s\n",
               address, sector, f->name, why, at);
    else
        printf("stop: call to 0x%08x, which no module can be placed at: the page was written from sector %d, which is in no file of the disc (%s); images the tables have at that address: %s\n",
               address, sector, why, at);
    finish(PORT_EXIT_UNKNOWN);
    return -1;
}

/* ---- the identity of a chunk ---------------------------------------------- */

/* Read the chunk from the user's image and compare its SHA-256 with the one pinned for the image. Returns the
 * chunk's bytes (the caller frees them); a chunk that is not the pinned one ends the program, and so does an
 * image the build pinned nothing for. */
static unsigned char *verified_chunk(int index, const struct chunk *c, const struct port_disc_file *f)
{
    const struct port_image *im = &port_images[index];
    unsigned char *bytes, digest[32];
    char err[PORT_ERR];

    if (!im->sha256) {
        printf("refused: image %s has no pinned content in this build; no jump of it was written\n", im->name);
        finish(PORT_EXIT_REFUSED);
    }
    bytes = malloc(c->size ? c->size : 1);
    if (!bytes || (c->size && port_disc_read(port_cd_disc(), c->first_sector, c->size, bytes, err, sizeof err) != 0)) {
        printf("refused: image %s: the chunk at sector %u of %s cannot be read (%s); no jump of it was written\n", im->name, c->first_sector, f->name, bytes ? err : "out of memory");
        finish(PORT_EXIT_REFUSED);
    }
    port_sha256(bytes, c->size, digest);
    if (memcmp(digest, im->sha256, 32) != 0) {
        printf("refused: image %s: the chunk at sector %u of %s is not the pinned content (its SHA-256 differs from the build's); no jump of it was written\n",
               im->name, c->first_sector, f->name);
        finish(PORT_EXIT_REFUSED);
    }
    return bytes;
}

/* ---- placing a module -------------------------------------------------- */

#define WINDOW 16u   /* bytes at a function start that must still be the chunk's */

static int owned(int index, unsigned page_address)
{
    return owner_page[(page_address - PORT_RAM_BASE) / PAGE] == index + 1;
}

static void place(int index, const struct chunk *c, const unsigned char *bytes, unsigned fault, const struct port_disc_file *f)
{
    const struct port_image *im = &port_images[index];
    unsigned lo = im->address & ~(PAGE - 1), hi, p, i;
    unsigned long long end = (unsigned long long)im->address + c->size + PAGE - 1;
    unsigned with_c = 0, without_c = 0;
    int src;

    hi = end > PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE ? PORT_RAM_BASE + PORT_RAM_SIZE : (unsigned)end & ~(PAGE - 1);
    if (end >= (1ull << 32)) hi = PORT_RAM_BASE + PORT_RAM_SIZE;
#define OURS(page) ((src = port_cd_page_source(page)) >= 0 && (unsigned)src >= c->first_sector && (unsigned)src < c->first_sector + c->sectors)
#define NEW(a) (!owned(index, (a) & ~(PAGE - 1)) || !owned(index, ((a) + 4) & ~(PAGE - 1)))
#define SAME(a) do { \
        unsigned off_ = (a) - im->address, n_ = c->size - off_ < WINDOW ? c->size - off_ : WINDOW; \
        if (off_ >= c->size || memcmp((const void *)(size_t)(a), bytes + off_, n_) != 0) { \
            printf("refused: image %s: the memory at 0x%08x is not the pinned content any more (the chunk at sector %u of %s was changed after the disc wrote it); no jump of it was written\n", \
                   im->name, (a), c->first_sector, f->name); \
            finish(PORT_EXIT_REFUSED); \
        } \
    } while (0)
    /* first every function is checked against the pinned bytes, then the jumps are written */
    for (i = 0; i < port_function_count; i++) {
        unsigned a = port_functions[i].address;
        if (port_functions[i].image != index || a < im->address || a - im->address + 4 >= c->size || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1)) && NEW(a)) SAME(a);
    }
    for (i = 0; i < port_absent_count; i++) {
        unsigned a = port_absents[i].address;
        if (port_absents[i].image != index || a < im->address || a - im->address + 4 >= c->size || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1)) && NEW(a)) SAME(a);
    }
    for (i = 0; i < port_function_count; i++) {
        unsigned a = port_functions[i].address;
        if (port_functions[i].image != index || a < im->address || a - im->address + 4 >= c->size || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1)) && NEW(a)) {
            jump_site(a, 0xe9, port_functions[i].impl);
            with_c++;
        }
    }
    for (i = 0; i < port_absent_count; i++) {
        unsigned a = port_absents[i].address;
        if (port_absents[i].image != index || a < im->address || a - im->address + 4 >= c->size || a + 4 >= hi) continue;
        if (OURS(a & ~(PAGE - 1)) && OURS((a + 4) & ~(PAGE - 1)) && NEW(a)) {
            jump_site(a, 0xe8, (const void *)port_module_stop_entry);
            without_c++;
        }
    }
#undef SAME
#undef NEW
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

/* Place the module that `address` is in, when its page was written by the disc layer and is not executable.
 * 0: placed. -1: not ours (quiet mode: for whatever reason). Out of quiet mode a page of the disc that no module
 * fits ends the program with a line. */
static int resolve(unsigned address, int quiet)
{
    int src, found = -1;
    unsigned i;
    const struct port_disc_file *f;
    struct chunk c = { 0, 0, 0, 0, 0, 0 };
    const char *why = "";
    char ambiguous[160] = "";
    unsigned char *bytes;

    src = port_cd_page_source(address);
    if (src < 0) return -1;
    load_files();
    f = file_of_sector((unsigned)src);
    if (!f) return unplaced(quiet, address, 0, src, NULL, "no file");
    if (chunk_of(f, (unsigned)src, &c, &why) != 0) return unplaced(quiet, address, 0, src, f, why);
    for (i = 0; i < port_image_count; i++) {
        const struct port_image *im = &port_images[i];
        if (im->slot != c.slot || c.table != 0 || !carries(im, f->name)) continue;
        if (address < im->address || address - im->address >= c.size) continue;
        if (found >= 0) {
            snprintf(ambiguous, sizeof ambiguous, "images %s and %s both fit", port_images[found].name, im->name);
            return unplaced(quiet, address, c.size, src, f, ambiguous);
        }
        found = (int)i;
    }
    if (found < 0) return unplaced(quiet, address, c.size, src, f, "no image of the tables has that archive, slot and address");
    if ((unsigned long long)port_images[found].address + c.size > PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE)
        return unplaced(quiet, address, c.size, src, f, "the chunk would end outside the PS1's RAM");
    if (port_images[found].like && !has_entries(found)) {   /* a second placement the build has not made */
        if (quiet) return -1;
        printf("stop: call to 0x%08x in image %s, which is a like placement of %s; its C is not built yet\n", address, port_images[found].name, port_images[found].like);
        finish(PORT_EXIT_NO_C);
    }
    if (!is_function_start(found, address)) {
        char name[160];
        snprintf(name, sizeof name, "image %s is there but 0x%08x is no function start of it", port_images[found].name, address);
        return unplaced(quiet, address, c.size, src, f, name);
    }
    bytes = verified_chunk(found, &c, f);
    place(found, &c, bytes, address, f);
    free(bytes);
    return 0;
}

/* Handle an execute fault at `address`. 0: resumed. -1: not ours to handle. */
static int fault(unsigned address, unsigned eip, unsigned thread)
{
    int r;

    if (!installed || handling || thread != game_thread || eip != address) return -1;
    if (address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return -1;
    if (exec_page[(address - PORT_RAM_BASE) / PAGE]) return -1;   /* a page this layer did not make non-executable */
    handling = 1;
    r = resolve(address, 0);
    handling = 0;
    return r;
}

/* For the timer thread of interrupt.c: a thread whose instruction pointer is on a page that is not executable is in
 * the access fault of this layer; the vblank must not interrupt it there. */
static int blocked(unsigned address)
{
    return address >= PORT_RAM_BASE && address - PORT_RAM_BASE < PORT_RAM_SIZE && !exec_page[(address - PORT_RAM_BASE) / PAGE];
}

/* The check of an address that the game handed over (port_target_check): is it the start of a function of a
 * module that is placed, or whose page can be placed now? */
static int known(unsigned address)
{
    unsigned page;
    int owner;

    if (!installed || handling || address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return 0;
    page = (address - PORT_RAM_BASE) / PAGE;
    if (!owner_page[page] && !exec_page[page]) {
        handling = 1;
        resolve(address, 1);
        handling = 0;
    }
    owner = owner_page[page] - 1;
    return owner >= 0 && is_function_start(owner, address);
}

/* ---- the hook of the disc layer ---------------------------------------- */

/* The disc layer copied `bytes` bytes to `address`: does that reach the 5 bytes of a jump that the start-up wrote at
 * a resident function? If so the program ends. */
static void check_resident(unsigned address, unsigned bytes)
{
    unsigned long long end = (unsigned long long)address + bytes;
    unsigned i, best = 0;
    const char *name = NULL;

    if (end <= text_lo || address >= text_hi) return;
    for (i = 0; i < port_function_count; i++) {
        unsigned a = port_functions[i].address;
        if (port_functions[i].image == -1 && (unsigned long long)a + 5 > address && a < end && (!name || a < best)) {
            name = port_functions[i].name;
            best = a;
        }
    }
    for (i = 0; i < port_absent_count; i++) {
        unsigned a = port_absents[i].address;
        if (port_absents[i].image == -1 && (unsigned long long)a + 5 > address && a < end && (!name || a < best)) {
            name = port_absents[i].name;
            best = a;
        }
    }
    if (name) {
        printf("stop: the game overwrote resident code at 0x%08x (%s): a copy of %u bytes to 0x%08x reaches the jump written there, and the C stays while the bytes would not\n",
               best, name, bytes, address);
        finish(PORT_EXIT_DISC);
    }
}

static int resident_page(unsigned page)
{
    unsigned lo = PORT_RAM_BASE + page * PAGE;
    return text_hi > text_lo && lo < text_hi && (unsigned long long)lo + PAGE > text_lo;
}

static void written(unsigned address, unsigned bytes)
{
    unsigned first, last, p;
    if (!bytes || address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return;
    check_resident(address, bytes);
    first = (address - PORT_RAM_BASE) / PAGE;
    last = (unsigned)(((unsigned long long)address - PORT_RAM_BASE + bytes - 1) / PAGE);
    if (last >= NPAGES) last = NPAGES - 1;
    for (p = first; p <= last; p++) {
        if (resident_page(p)) continue;   /* resident code: the page keeps its permission and is never a module's */
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

void port_modules_init(unsigned text_address, unsigned text_size)
{
    unsigned p;
    text_lo = text_address;
    text_hi = text_address + text_size;
    if (text_hi < text_lo) text_hi = text_lo = 0;
    memset(owner_page, 0, sizeof owner_page);
    for (p = 0; p < NPAGES; p++) exec_page[p] = 1;
    if (sys_install() != 0) refuse("modules: no access fault handler for this system");
    game_thread = sys_thread();
    installed = 1;
    port_cd_written_hook = written;
    port_module_known = known;
    port_page_blocked = blocked;
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
    return fault((unsigned)r->ExceptionInformation[1], (unsigned)p->ContextRecord->Eip, GetCurrentThreadId()) == 0 ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}

static unsigned sys_thread(void)
{
    return GetCurrentThreadId();
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

static unsigned sys_thread(void)
{
    return 0;
}

#endif
