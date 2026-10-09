/* Tables that the port's build tool writes (port_tables.c) and the runtime
 * reads. Written by hand; the build tool only produces the definitions. */
#ifndef PORT_TABLES_H
#define PORT_TABLES_H

struct port_image    { const char *name; unsigned address; const char *like; unsigned slot; };
struct port_function { unsigned address; void *impl; const char *name; int image; };  /* image: index, -1 = resident */
struct port_absent   { unsigned address; const char *name; int image; int library; };

extern const struct port_image    port_images[];    extern const unsigned port_image_count;
extern const struct port_function port_functions[]; extern const unsigned port_function_count;
extern const struct port_absent   port_absents[];   extern const unsigned port_absent_count;

#endif
