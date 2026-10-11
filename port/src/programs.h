/* The stand-alone programs of the disc whose C is built into the PC program (the generated table port_programs, port_tables.h).
 *
 * port_program_exec(disc_name, sector, size, header) is what Exec (overrides.c) calls once it has found the file the game asked for:
 * disc_name is the file's name as the disc's listing gives it, sector and size its place on the disc, header what the game gave Exec,
 * the game's address of the program header's field pc0 (file offset 0x10; gp0 follows at +4, t_addr at +8, t_size at +12).
 *
 * The row is found by the file's base name: what follows the last `/` or `\`, without a version suffix `;N`, compared with the
 * row's `file` without regard to case. No row: the result is 0 and nothing was done. With a row, three checks come before any C
 * of the program runs, and a check that fails ends the program (port_halt, status PORT_EXIT_PROGRAM) with a line:
 *   - the file: all `size` bytes are read from the disc and their SHA-256 must be the row's (the line names the file, the program
 *     and both hashes: the C was written from another file);
 *   - the header: `header` and its 16 bytes must lie in the mapped RAM and equal the file's bytes 0x10 to 0x1f (the line names the
 *     first field that differs, with both values);
 *   - the body: t_size bytes from file offset 0x800 must exist in the file (none are needed when t_size is 0), the span t_addr
 *     to t_addr + t_size must lie in the mapped RAM, and the RAM there must equal the file's bytes (the line names what failed;
 *     for a difference the first address and both bytes). The program's C reads its data at these addresses.
 * Then, in this order: the row's setup when it is not null, main (called as void (*)(void)), the row's teardown when it is not
 * null; one trace line before and one after, each naming the program; and last port_press_report with the program's name (the
 * movie decoder's line of counts, press.h). The result is 1. Nothing else is cleaned up after main.
 * The checks are in port/tools/test_hostprograms.py. Nothing here is compared with a console. */
#ifndef PORT_PROGRAMS_H
#define PORT_PROGRAMS_H

int port_program_exec(const char *disc_name, unsigned sector, unsigned size, void *header);

#endif
