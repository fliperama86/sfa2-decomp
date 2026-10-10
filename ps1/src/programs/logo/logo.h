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
extern int CdInit(void);
extern int CdControlB(u8 com, u8 *param, u8 *result);
extern u8 *CdIntToPos(int i, u8 *p);
extern int CdMix(u8 *vol);
extern int ResetGraph(int mode);
extern void SetDispMask(int mask);
extern int DrawSync(int mode);
extern int VSync(int mode);
extern int ClearImage(Rect *rect, u8 r, u8 g, u8 b);

/* The game's DispEnv (types.fields) is the 20 byte display environment of the library; the program's two
   environments are data_8002eb8c_logo, reached through a cast to it. */
extern DispEnv *SetDefDispEnv(DispEnv *env, int x, int y, int w, int h);

/* A callback of the disc library: (status, result bytes). */
typedef void (*CdCallback)(u8 status, u8 *result);
extern CdCallback CdReadCallback(CdCallback func);

/* gp + 0x04: the buffer the decoder writes a strip to (inferred). */
extern u32 *data_8002eb00_logo;
/* gp + 0x14: where the expansion of run-length data writes (inferred); the
   still picture's three-word pattern is read from it by func_800111d0_logo. */
extern u8 *data_8002eb10_logo;
/* gp + 0x18: a block of memory that a picture is built in (inferred). */
extern u32 *data_8002eb14_logo;
/* gp + 0x28: the rectangle of the picture that the next strip goes to (inferred). */
extern Rect data_8002eb24_logo;
/* gp + 0x38: not 0 sets the disc volume to left and right apart (inferred). */
extern int data_8002eb34_logo;
/* gp + 0x3c: a callback for the disc library (inferred; its use is the call of CdReadCallback). */
extern CdCallback data_8002eb38_logo;
/* gp + 0x40: a flag word, 1 while a strip is being decoded (inferred); volatile because an interrupt changes it (inferred). */
extern volatile int data_8002eb3c_logo;
/* gp + 0x44: the four volume bytes handed to CdMix (inferred from the call). */
extern u8 data_8002eb40_logo[4];
/* gp + 0x54: the index (0 or 1) of the display environment shown (inferred). */
extern int data_8002eb50_logo;
/* Two display environments of 20 bytes each (inferred from the stride). */
extern u8 data_8002eb8c_logo[2][20];
/* A flag word: not 0 asks for StCdInterrupt (inferred). */
extern int data_8002edb8_logo;

#endif
