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
/* 0x8002f888 and 0x8002f88c: the count of strips handed out so far and the count at which the stream is done (inferred from func_80011708_z2o:
   the first is added 1 to at each call, and the strips go on while it is below the second, an unsigned comparison). */
extern u32 data_8002f888_z2o;
extern u32 data_8002f88c_z2o;
/* 0x80031e68: a flag word (inferred: not 0 asks for StCdInterrupt, then it is cleared). */
extern int data_80031e68_z2o;
/* 0x8002f874: the rectangle of the picture that the next strip goes to (inferred; x is the halfword that is stepped). */
extern Rect data_8002f874_z2o;
/* 0x8002f87c: the pointer word of the strip buffer, 0x8002f880 and 0x8002f884: the strip's length in words and the size handed to DecDCTout (inferred). */
extern u32 *data_8002f87c_z2o;
extern int data_8002f880_z2o;
extern int data_8002f884_z2o;
extern void StCdInterrupt(void);
extern void DecDCTout(u32 *buf, int size);
/* Program functions that other functions call (prototypes from their listings' use of the argument registers; the open's are inferred). */
extern u32 func_800119e8_z2o(int index, u32 *size);
extern int func_80010c38_z2o(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9);
/* 0x80031e6c, 0x80031e70, 0x80031e74: the pad words: the word the buttons newly pressed are taken against (set to 0 by the main and not written again by
   it), the last state read from PadRead and the buttons newly pressed (inferred from func_800108e8_z2o). */
extern u32 data_80031e6c_z2o;
extern u32 data_80031e70_z2o;
extern u32 data_80031e74_z2o;
/* 0x8003de88 and 0x80049e8c: the low and the high half of the buttons newly pressed (inferred). */
extern u32 data_8003de88_z2o;
extern u32 data_80049e8c_z2o;
/* 0x80049ea4: a rectangle (8 bytes, just before the display environments) that the main clears the picture's area with (inferred). */
extern Rect data_80049ea4_z2o;
/* The four areas whose addresses the main hands to the open of the movie (inferred from the use as arguments; their types are not known). */
extern u8 data_80049ed4_z2o[];
extern u8 data_80031e84_z2o[];
extern u8 data_8003de8c_z2o[];
extern u8 data_800876f8_z2o[];
extern void func_80011afc_z2o(void);
extern long ResetCallback(void);
extern void StopCallback(void);
extern void PadInit(int mode);
extern u32 PadRead(int id);
extern void PadStop(void);
extern void InitGeom(void);
extern int func_800110d0_z2o(int a0, int a1);
extern void func_80010be4_z2o(void);
extern void func_80010ae8_z2o(void);
extern int func_80011018_z2o(int index, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7, u32 a8, u32 a9);
extern int func_80011a24_z2o(void);
extern void func_8001139c_z2o(void);
extern void func_80011428_z2o(void);
extern void func_800115a8_z2o(u32 volume);
extern void func_80011630_z2o(int stereo);
extern void func_800116c8_z2o(int channel);
extern void func_800119b8_z2o(int value);

#endif
