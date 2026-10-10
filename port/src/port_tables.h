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
extern const unsigned char port_program_sha256[32];  /* the executable's SHA-256 pinned by the build configuration */

#endif
