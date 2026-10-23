/* The arcade loader's seam with the runtime: the setting arcade_assets. The
 * loader builds what the game reads from the user's arcade set where it can,
 * and the disc gives the rest until the loader builds it too. The design is
 * in port/arcade/README.md. */
#ifndef PORT_ARCADE_H
#define PORT_ARCADE_H

#include <stddef.h>

struct port_disc;

/* With enabled, open the arcade set at path and check it against the examined
 * one, with a line. Without, nothing. -1 with err when the set is refused. */
int port_arcade_configure(int enabled, const char *path, char *err, size_t size);
int port_arcade_enabled(void);
/* The stop line of a start without a disc: the first thing the game needs
 * that the loader cannot build yet. */
void port_arcade_missing(char *line, size_t size);
/* The PS1 program is in RAM, from the disc: build each piece the loader has
 * over it, with a line. With check, change nothing: compare each piece with
 * what the disc gave and print each one that differs. Returns the number of
 * pieces that differ (0 when not checking). */
unsigned port_arcade_program(unsigned char *ram, int check);
/* Follow the game's reads of the disc: the first read of each file that the
 * loader does not build prints a line. -1 with err if the disc's files cannot
 * be listed. */
int port_arcade_attach(struct port_disc *d, char *err, size_t size);

#endif
