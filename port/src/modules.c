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
 *     sector the WORD at that address was written (the disc layer records the
 *     origin of every word of RAM, not of every page: images begin and end in
 *     the middle of pages). Bytes that did not come from the disc are never a
 *     module's: the fault goes on to the crash line. That sector is in a file of the disc's
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
 *  4. A page that is about to become executable and holds the function start of
 *     another image, written from that image's chunk and not yet placed, is
 *     refused with a line naming both images and the page (its first call
 *     would not fault any more).
 *  5. The jumps of that image's functions go on the pages of the image that the
 *     disc wrote from that chunk (E9 to the C; E8 to the stop call without C),
 *     those pages become executable, one line is printed and the call resumes.
 *  6. A later disc write to those pages (hook) takes the permission away and
 *     forgets the owner; the next call goes through the handler again.
 *
 * Everything read from an archive is bounded before use: the count of chunks, each chunk's length and place
 * against the file's extent on the disc, the chunk against the PS1's RAM, the page and table indexes.
 *
 * Installation is per entry, not per page. Before a page becomes executable every declared entry of the image on
 * it must be installable (its five bytes inside the chunk and all written from it), else the placement is refused,
 * naming the first entry that is not, and nothing is written. An entry counts as installed while this layer's own
 * jump (or call to the stop) is there: checked by content at the moment of use (a call that faulted, a target handed
 * over for a later call). A disc write to the page takes its owner away, so its entries are no longer installed.
 * Stated limit: a write by the game's own code over an installed entry is not seen when it happens; it is seen when
 * the entry is next handed over as a target (the line says the entry's jump is no longer there), not when the game
 * calls it directly.
 *
 * Resident code is never a module's: a page that holds any byte of the resident program's text keeps its execute
 * permission whatever the disc copies into its other bytes (a data sector may end in the page where the resident
 * code begins), and a copy that reaches the first bytes of a resident function, where the start-up wrote a jump
 * to the C, ends the program with a line (the C stays and the bytes would not).
 *
 * Which faults the handler accepts: an execute fault, on the game's thread, whose address is the instruction
 * pointer, inside the PS1's RAM, on a page that this file made non-executable, at a word that the disc layer wrote.
 * Everything else goes on to the program's crash line.
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
static unsigned char *entry_state;         /* per row of the tables (functions, then absents): 1 once this layer wrote the entry's jump */
static short owner_page[NPAGES];           /* image index + 1 that placed its jumps on the page, 0: none */
static struct port_disc_file *files;
static unsigned file_count;
static int installed;
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
    if (page_address < PORT_RAM_BASE || page_address - PORT_RAM_BASE >= PORT_RAM_SIZE) return 0;
    return owner_page[(page_address - PORT_RAM_BASE) / PAGE] == index + 1;
}

/* Did any word of the page come from this chunk? */
static int page_ours(unsigned page, const struct chunk *c)
{
    unsigned a;
    for (a = page; a < page + PAGE; a += 4) {
        int src = port_cd_source_at(a);
        if (src >= 0 && (unsigned)src >= c->first_sector && (unsigned)src < c->first_sector + c->sectors) return 1;
    }
    return 0;
}

/* ---- the entries of an image, as one list ---------------------------------- */

/* Row i of the functions with C (i < port_function_count) and then of the functions without C. */
static int row_image(unsigned i)
{
    return i < port_function_count ? port_functions[i].image : port_absents[i - port_function_count].image;
}

static unsigned row_address(unsigned i)
{
    return i < port_function_count ? port_functions[i].address : port_absents[i - port_function_count].address;
}

static const char *row_name(unsigned i)
{
    return i < port_function_count ? port_functions[i].name : port_absents[i - port_function_count].name;
}

static unsigned char row_opcode(unsigned i)
{
    return i < port_function_count ? 0xe9 : 0xe8;   /* a jump to the C, or a call to the stop */
}

static const void *row_target(unsigned i)
{
    return i < port_function_count ? port_functions[i].impl : (const void *)port_module_stop_entry;
}

#define ROWS (port_function_count + port_absent_count)

/* Are the five bytes at `address` exactly the jump or call that jump_site writes for `target`? */
static int bytes_are(unsigned address, unsigned char opcode, const void *target)
{
    const unsigned char *site = (const unsigned char *)(size_t)address;
    uint32_t rel = (uint32_t)((uintptr_t)target - ((uintptr_t)site + 5));
    return site[0] == opcode && memcmp(site + 1, &rel, 4) == 0;
}

/* 1: the entry of image `index` at `address` is installed and its jump remains valid. 0: not installed (or its page
 * is no longer the image's: it will be placed anew on the next call). -1: it was installed, its page is still the
 * image's, and its five bytes are not what was written. Is the entry installed, and does its jump remain valid? Installed means: this layer
 * wrote the jump (or the call to the stop) of that very entry, the pages it lies on are still this image's, and the
 * five bytes are, now, exactly what was written. (A write by the game's own code to those bytes after the disc
 * layer's last write is seen here, at the moment of use, and only here.) */
static int entry_valid(int index, unsigned address)
{
    unsigned i;
    for (i = 0; i < ROWS; i++) {
        if (row_image(i) != index || row_address(i) != address) continue;
        if (!entry_state[i] || !owned(index, address & ~(PAGE - 1)) || !owned(index, (address + 4) & ~(PAGE - 1))) return 0;
        return bytes_are(address, row_opcode(i), row_target(i)) ? 1 : -1;
    }
    return 0;
}

static void place(int index, const struct chunk *c, const unsigned char *bytes, const struct port_disc_file *f)
{
    const struct port_image *im = &port_images[index];
    unsigned lo = im->address & ~(PAGE - 1), hi, p, i;
    unsigned long long end = (unsigned long long)im->address + c->size + PAGE - 1;
    unsigned with_c = 0, without_c = 0, worst = 0;
    int src, bad = 0;
    static unsigned char becoming[NPAGES];

    hi = end > PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE ? PORT_RAM_BASE + PORT_RAM_SIZE : (unsigned)end & ~(PAGE - 1);
    if (end >= (1ull << 32)) hi = PORT_RAM_BASE + PORT_RAM_SIZE;
#define IN_CHUNK(a) ((src = port_cd_source_at(a)) >= 0 && (unsigned)src >= c->first_sector && (unsigned)src < c->first_sector + c->sectors)
    /* the pages that become executable: those with a word of this chunk that are not this image's yet */
    memset(becoming, 0, sizeof becoming);
    for (p = lo; p < hi; p += PAGE)
        if (!owned(index, p) && page_ours(p, c)) becoming[(p - PORT_RAM_BASE) / PAGE] = 1;
#define ON_BECOMING(a) ((a) >= PORT_RAM_BASE && (a) - PORT_RAM_BASE < PORT_RAM_SIZE && (becoming[((a) - PORT_RAM_BASE) / PAGE] || \
                        ((a) + 4 - PORT_RAM_BASE < PORT_RAM_SIZE && becoming[((a) + 4 - PORT_RAM_BASE) / PAGE])))
    /* every declared entry of the image on those pages must be installable: its five bytes inside the chunk and all
     * written from it. Otherwise the page would be executable with an entry in it that nothing jumps from. */
    for (i = 0; i < ROWS; i++) {
        unsigned a = row_address(i);
        if (row_image(i) != index || !ON_BECOMING(a)) continue;
        if (a < im->address || a - im->address + 5 > c->size || !IN_CHUNK(a) || !IN_CHUNK(a + 4)) {
            if (!bad || a < worst) worst = a;
            bad = 1;
        }
    }
    if (bad) {
        const char *name = "?";
        for (i = 0; i < ROWS; i++)
            if (row_image(i) == index && row_address(i) == worst) name = row_name(i);
        printf("refused: image %s: the entry %s at 0x%08x cannot be installed: the bytes of this entry did not all come from the pinned chunk (at sector %u of %s), and the page 0x%08x would become executable without a jump there; no jump of the image was written\n",
               im->name, name, worst, c->first_sector, f->name, worst & ~(PAGE - 1));
        finish(PORT_EXIT_REFUSED);
    }
#define SAME(a) do { \
        unsigned off_ = (a) - im->address, n_ = c->size - off_ < WINDOW ? c->size - off_ : WINDOW; \
        if (off_ >= c->size || memcmp((const void *)(size_t)(a), bytes + off_, n_) != 0) { \
            printf("refused: image %s: the memory at 0x%08x is not the pinned content any more (the chunk at sector %u of %s was changed after the disc wrote it); no jump of it was written\n", \
                   im->name, (a), c->first_sector, f->name); \
            finish(PORT_EXIT_REFUSED); \
        } \
    } while (0)
    /* first every entry is checked against the pinned bytes, then the jumps are written */
    for (i = 0; i < ROWS; i++)
        if (row_image(i) == index && ON_BECOMING(row_address(i))) SAME(row_address(i));
    for (i = 0; i < ROWS; i++) {
        unsigned a = row_address(i);
        if (row_image(i) != index || !ON_BECOMING(a)) continue;
        jump_site(a, row_opcode(i), row_target(i));
        entry_state[i] = 1;
        if (i < port_function_count) with_c++;
        else without_c++;
    }
#undef SAME
#undef ON_BECOMING
    /* the pages: owner and permission, in runs */
    for (p = lo; p < hi;) {
        unsigned q;
        if (!becoming[(p - PORT_RAM_BASE) / PAGE]) {
            p += PAGE;
            continue;
        }
        for (q = p; q < hi && becoming[(q - PORT_RAM_BASE) / PAGE]; q += PAGE) owner_page[(q - PORT_RAM_BASE) / PAGE] = (short)(index + 1);
        if (sys_protect(p, q - p, 1) != 0) {
            printf("stop: modules: cannot make 0x%08x..0x%08x executable\n", p, q);
            finish(PORT_EXIT_OTHER);
        }
        for (; p < q; p += PAGE) exec_page[(p - PORT_RAM_BASE) / PAGE] = 1;
    }
#undef IN_CHUNK
    /* The call resumes only into a jump that this placement wrote: the faulting address is a function start of the
     * image (checked by the caller), its page is one of the becoming pages (its word came from the chunk), and every
     * entry on those pages was installed above or the placement was refused. */
    printf("module: %s at 0x%08x, %u C jumps, %u without C\n", im->name, im->address, with_c, without_c);
    fflush(stdout);
}

/* The image that the bytes at `address` came from, by the word record of the disc layer: the chunk that wrote the
 * word, its image by slot, archive and range. -1 when the bytes did not come from the disc, no image fits, or two
 * do. Out of `quiet` mode a failure ends the program with the line. Fills c and f for the image found. */
static int locate(unsigned address, int quiet, struct chunk *c, const struct port_disc_file **f_out, int *src_out)
{
    int src, found = -1;
    unsigned i;
    const struct port_disc_file *f;
    const char *why = "";
    char ambiguous[160] = "";

    src = port_cd_source_at(address);
    if (src < 0) return -1;
    load_files();
    f = file_of_sector((unsigned)src);
    if (!f) return unplaced(quiet, address, 0, src, NULL, "no file");
    if (chunk_of(f, (unsigned)src, c, &why) != 0) return unplaced(quiet, address, 0, src, f, why);
    for (i = 0; i < port_image_count; i++) {
        const struct port_image *im = &port_images[i];
        if (im->slot != c->slot || c->table != 0 || !carries(im, f->name)) continue;
        if (address < im->address || address - im->address >= c->size) continue;
        if (found >= 0) {
            snprintf(ambiguous, sizeof ambiguous, "images %s and %s both fit", port_images[found].name, im->name);
            return unplaced(quiet, address, c->size, src, f, ambiguous);
        }
        found = (int)i;
    }
    if (found < 0) return unplaced(quiet, address, c->size, src, f, "no image of the tables has that archive, slot and address");
    *f_out = f;
    *src_out = src;
    return found;
}

/* Before the pages of an image become executable: a page that holds the function start of ANOTHER image, written
 * from that image's chunk, which is not placed, would no longer fault on its first call (the page is executable
 * now, and the bytes there are not jumped). Refuse that, naming both images and the page. */
static void check_shared(int index, const struct chunk *c)
{
    const struct port_image *im = &port_images[index];
    unsigned lo = im->address & ~(PAGE - 1), i, n = 0;
    unsigned long long end = (unsigned long long)im->address + c->size + PAGE - 1;
    unsigned hi = end >= PORT_RAM_BASE + (unsigned long long)PORT_RAM_SIZE ? PORT_RAM_BASE + PORT_RAM_SIZE : (unsigned)end & ~(PAGE - 1);
    static unsigned char becoming[NPAGES];
    unsigned p;

    memset(becoming, 0, sizeof becoming);
    for (p = lo; p < hi; p += PAGE)
        if (!owned(index, p) && page_ours(p, c)) {
            becoming[(p - PORT_RAM_BASE) / PAGE] = 1;
            n++;
        }
    if (!n) return;
    for (i = 0; i < port_function_count + port_absent_count; i++) {
        int other = i < port_function_count ? port_functions[i].image : port_absents[i - port_function_count].image;
        unsigned a = i < port_function_count ? port_functions[i].address : port_absents[i - port_function_count].address;
        struct chunk oc = { 0, 0, 0, 0, 0, 0 };
        const struct port_disc_file *of = NULL;
        int osrc = 0, found;
        if (other < 0 || other == index || a < PORT_RAM_BASE || a - PORT_RAM_BASE >= PORT_RAM_SIZE || !becoming[(a - PORT_RAM_BASE) / PAGE]) continue;
        if (owned(other, a & ~(PAGE - 1))) continue;
        {
            int src = port_cd_source_at(a);
            if (src < 0 || ((unsigned)src >= c->first_sector && (unsigned)src < c->first_sector + c->sectors)) continue;   /* not from the disc, or from this chunk */
        }
        found = locate(a, 1, &oc, &of, &osrc);
        if (found != other) continue;   /* the bytes there are some other content: this start belongs to an image that is not loaded */
        printf("refused: images %s and %s share the page 0x%08x: the function of %s at 0x%08x lies in it, %s is not placed, and its first call would no longer fault once the page is executable\n",
               im->name, port_images[other].name, a & ~(PAGE - 1), port_images[other].name, a, port_images[other].name);
        finish(PORT_EXIT_REFUSED);
    }
}

/* Place the module that `address` is in, when its bytes came from the disc layer and its page is not executable.
 * 0: placed. -1: not ours (quiet mode: for whatever reason). Out of quiet mode a word of the disc that no module
 * fits ends the program with a line; bytes that did not come from the disc are never a module's: -1. */
static int resolve(unsigned address, int quiet)
{
    int src = 0, found;
    const struct port_disc_file *f = NULL;
    struct chunk c = { 0, 0, 0, 0, 0, 0 };
    unsigned char *bytes;

    found = locate(address, quiet, &c, &f, &src);
    if (found < 0) return -1;
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
    check_shared(found, &c);
    place(found, &c, bytes, f);
    free(bytes);
    return 0;
}

/* Handle an execute fault at `address`. 0: resumed. -1: not ours to handle. */
static int fault(unsigned address, unsigned eip, unsigned thread)
{
    if (!installed || thread != game_thread || eip != address) return -1;
    if (address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return -1;
    if (exec_page[(address - PORT_RAM_BASE) / PAGE]) return -1;   /* a page this layer did not make non-executable */
    return resolve(address, 0);
}

/* For the timer thread of interrupt.c: a thread whose instruction pointer is on a page that is not executable is in
 * the access fault of this layer; the vblank must not interrupt it there. */
static int blocked(unsigned address)
{
    return address >= PORT_RAM_BASE && address - PORT_RAM_BASE < PORT_RAM_SIZE && !exec_page[(address - PORT_RAM_BASE) / PAGE];
}

/* The check of an address that the game handed over (port_target_check): is it an entry of a module that this layer
 * installed (its own jump or stop was written and is still there, checked now by content), after placing the
 * module if the address's bytes came from the disc and the page is not placed yet? */
static int known(unsigned address)
{
    unsigned page;
    int owner;

    if (!installed || address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return 0;
    page = (address - PORT_RAM_BASE) / PAGE;
    if (!owner_page[page] && !exec_page[page]) resolve(address, 1);
    owner = owner_page[page] - 1;
    return owner >= 0 ? entry_valid(owner, address) : 0;
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

static int page_has_source(unsigned page)
{
    unsigned a;
    for (a = PORT_RAM_BASE + page * PAGE; a < PORT_RAM_BASE + (page + 1) * PAGE; a += 4)
        if (port_cd_source_at(a) >= 0) return 1;
    return 0;
}

void port_modules_init(unsigned text_address, unsigned text_size)
{
    unsigned p;
    text_lo = text_address;
    text_hi = text_address + text_size;
    if (text_hi < text_lo) text_hi = text_lo = 0;
    memset(owner_page, 0, sizeof owner_page);
    free(entry_state);
    entry_state = calloc(ROWS ? ROWS : 1, 1);
    if (!entry_state) refuse("modules: out of memory");
    for (p = 0; p < NPAGES; p++) exec_page[p] = 1;
    if (sys_install() != 0) refuse("modules: no access fault handler for this system");
    game_thread = sys_thread();
    installed = 1;
    port_cd_written_hook = written;
    port_module_known = known;
    port_page_blocked = blocked;
    for (p = 0; p < NPAGES; p++)
        if (page_has_source(p)) written(PORT_RAM_BASE + p * PAGE, 1);
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
