/* Tables that the port's build tool writes (port_tables.c) and the runtime
 * reads. Written by hand; the build tool only produces the definitions. */
#ifndef PORT_TABLES_H
#define PORT_TABLES_H

/* sha256: the 32 bytes that the build pins for the content of the image (the chunk of the archive that is loaded at
 * `address`, exactly its bytes); null where the build gave none. archives: the names (upper case, as the inventory writes them) of all the archives that carry the image's content,
 * ending with a null pointer; null where the build gave none. */
struct port_image    { const char *name; unsigned address; const char *like; unsigned slot; const char *const *archives; const unsigned char *sha256; };
struct port_function { unsigned address; void *impl; const char *name; int image; int overridden; };  /* image: index, -1 = resident; overridden: 1 when the C behind impl is an override of the port's own (port/overrides), else 0 */
struct port_absent   { unsigned address; const char *name; int image; int library; };

extern const struct port_image    port_images[];    extern const unsigned port_image_count;
extern const struct port_function port_functions[]; extern const unsigned port_function_count;
extern const struct port_absent   port_absents[];   extern const unsigned port_absent_count;
/* A stand-alone program of the disc whose C is built into the port (port/programs.toml). name: the program's name in the declaration; file: the base name
 * of its file on the disc, lower case; sha256: the 32 bytes of the file's SHA-256; main: the C of the program's main function (a program's functions are not
 * at their PS1 addresses; no two rows have one file name: the build refuses that); setup, teardown: generated routines that make the calls the list names before main and after it, null where it names none. */
struct port_builtin_program { const char *name; const char *file; const unsigned char *sha256; void *main; void (*setup)(void); void (*teardown)(void); };

extern const struct port_builtin_program port_programs[];  extern const unsigned port_program_count;
/* The macOS build (port/mac): the routines of the runtime that the guest image calls by the stubs it has for them (`name` is the stub's symbol). */
struct port_bound { const char *name; void *host; };
extern const struct port_bound port_bounds[];  extern const unsigned port_bound_count;
extern const unsigned char port_program_sha256[32];  /* the executable's SHA-256 pinned by the build configuration */

#endif
