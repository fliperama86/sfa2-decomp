/*
 * Declarations for the C of the opening movie's program (PAC/Z2O.EXE): its
 * data by address and the library routines its functions call that the game's
 * headers do not declare. Not part of the matching build.
 *
 * A datum is named data_ADDRESS_z2o with the address in eight hex digits. The
 * types are inferred from the use in the functions, not original declarations.
 * The addresses are in [program.symbols] of programs.toml. The program's
 * functions reach their data through an address built with lui, not through the
 * global pointer register (programs.toml says why none is declared).
 *
 * The routine func_80158470 is the game's name (protos.h) for the library
 * routine that the program holds its own copy of, at 0x80013654;
 * programs.toml gives that address.
 */
#ifndef Z2O_H
#define Z2O_H

#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* 0x8002f8a9: one byte, stored by func_800116c8_z2o (inferred: a channel number; its readers have no C here yet). */
extern u8 data_8002f8a9_z2o;
/* 0x8002f870: a word stored by func_800119b8_z2o (inferred: a parameter of the stream). */
extern int data_8002f870_z2o;
/* 0x8002f86c: a counter that func_80011810_z2o adds 1 to (inferred: completed frames, from that routine's use as the stream's frame routine). */
extern int data_8002f86c_z2o;
/* 0x8002f890: a flag word func_80011428_z2o waits on until it is 0; volatile because an interrupt changes it (inferred). */
extern volatile int data_8002f890_z2o;
/* 0x8002f81c and 0x8002f818: two words; the wait copies the first to the second (inferred: a count of strips; their other users have no C here yet). */
extern int data_8002f81c_z2o;
extern int data_8002f818_z2o;
/* 0x8001b354: a table of 12 byte entries (inferred: the disc's files; the first word of an entry is returned, the second stored through a pointer). */
extern u32 data_8001b354_z2o[][3];
/* 0x80031e58: the index (0 or 1) of the display environment that is current (inferred). */
extern int data_80031e58_z2o;
/* 0x80049eac: two display environments of 20 bytes each (inferred from the stride). */
extern u8 data_80049eac_z2o[2][20];
/* 0x8001b34c: the volume word (inferred: the disc's volume, 0 to 255; set by func_800115a8_z2o; func_80011630_z2o reads its low byte). */
extern u32 data_8001b34c_z2o;
/* 0x8001b350: the stereo flag word (inferred: not 0 sets the disc volume to left and right apart; set by func_80011630_z2o). */
extern int data_8001b350_z2o;
/* 0x8002f8a4: the four volume bytes handed to CdMix (inferred from the call). */
extern u8 data_8002f8a4_z2o[4];
extern int CdMix(u8 *vol);
extern int CdInit(void);
extern int CdControlB(u8 com, u8 *param, u8 *result);
extern u8 *CdIntToPos(int i, u8 *p);
/* 0x8002f800: the disc mode byte handed to CdControlB(0x0e, ...), the library's CdlSetmode (inferred); set to 0x80 by func_8001139c_z2o. */
extern u8 data_8002f800_z2o;
/* 0x8002f804: a pointer word (inferred: the buffer that CdGetSector is handed). */
extern u32 *data_8002f804_z2o;
extern void StUnSetRing(void);
extern int CdReady(int mode, u8 *result);
extern void CdGetSector(void *madr, int size);
extern void *DecDCToutCallback(void (*func)(void));
extern void func_800118f0_z2o(void);
extern int ResetGraph(int mode);
extern void SetDispMask(int mask);
extern void *CdReadCallback(void (*func)(void));
extern DispEnv *SetDefDispEnv(DispEnv *env, int x, int y, int w, int h);
extern int ClearImage(Rect *rect, u8 r, u8 g, u8 b);
extern int DrawSync(int mode);
extern int VSync(int mode);

#endif
