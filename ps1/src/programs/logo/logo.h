/*
 * Declarations for the C of the logo's program (PAC/LOGO.EXE): its data by
 * address and the library routines its functions call that the game's headers
 * do not declare. Not part of the matching build.
 *
 * A datum is named data_ADDRESS_logo with the address in eight hex digits.
 * A word that the code reaches as the global pointer plus an offset is named
 * by its absolute address (the global pointer is 0x8002eafc, see
 * programs.toml). The types are inferred from the use in the functions, not
 * original declarations. The addresses are in [program.symbols] of
 * programs.toml.
 *
 * The routines func_80157fc4 and func_80158470 are the game's names (protos.h)
 * for the library routines that the program holds its own copies of, at
 * 0x80012f04 and 0x800133b0; programs.toml gives those addresses.
 */
#ifndef LOGO_H
#define LOGO_H

#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* Library routines (documented prototypes of Sony's library). */
extern void DecDCTout(u32 *buf, int size);
extern void StCdInterrupt(void);

/* gp + 0x04: the buffer the decoder writes a strip to (inferred). */
extern u32 *data_8002eb00_logo;
/* gp + 0x14: where the expansion of run-length data writes (inferred). */
extern u8 *data_8002eb10_logo;
/* gp + 0x28: the rectangle of the picture that the next strip goes to (inferred). */
extern Rect data_8002eb24_logo;
/* gp + 0x40: a flag word, 1 while a strip is being decoded (inferred). */
extern int data_8002eb3c_logo;
/* gp + 0x54: the index (0 or 1) of the display environment shown (inferred). */
extern int data_8002eb50_logo;
/* Two display environments of 20 bytes each (inferred from the stride). */
extern u8 data_8002eb8c_logo[2][20];
/* A flag word: not 0 asks for StCdInterrupt (inferred). */
extern int data_8002edb8_logo;

#endif
