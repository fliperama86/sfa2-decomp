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
extern void StUnSetRing(void);
extern int CdReady(int mode, u8 *result);
extern void CdGetSector(void *madr, int size);
extern void *DecDCToutCallback(void (*func)(void));
extern void DecDCTReset(int mode);
extern int DecDCTvlc(u32 *bs, u32 *buf);
extern u32 *StSetRing(u32 *ring, int size);
extern int StSetStream(int mode, int start, int end, void (*func1)(void), void (*func2)(void));
extern int StGetNext(u32 **addr, u32 **header);
extern int StFreeRing(u32 *base);
extern void DecDCTin(u32 *buf, int mode);
extern long ResetCallback(void);
extern void StopCallback(void);
extern void PadInit(int mode);
extern u32 PadRead(int id);
extern void PadStop(void);
extern int CdControl(u8 com, u8 *param, u8 *result);
extern int CdRead2(int mode);
extern int CdPosToInt(u8 *p);
extern int CdRead(int sectors, u32 *buf, int mode);
extern int CdReadSync(int mode, u8 *result);

/* The program's own functions that other functions call. */
extern void func_80010928_logo(void);
extern int func_80010a80_logo(int sector);
extern void func_80010a38_logo(void);
extern u32 func_80011758_logo(int index, u32 *size);
extern void func_80011794_logo(void);
extern void func_80011838_logo(void);
extern void func_800107e8_logo(u8 *base, int str_sector, int bin_sector, int vmode, int stereo);
extern int func_80010c20_logo(void);
extern void func_80010e28_logo(void);
extern void func_80010f20_logo(void);
extern void func_80010fc4_logo(void);
extern void func_800110f4_logo(u8 *table, u16 *codes);
extern void func_800111d0_logo(int x, int y);
extern void func_80010f38_logo(void);
extern void func_80011060_logo(void);

/* The game's DispEnv (types.fields) is the 20 byte display environment of the library; the program's two
   environments are data_8002eb8c_logo, reached through a cast to it. */
extern DispEnv *SetDefDispEnv(DispEnv *env, int x, int y, int w, int h);

extern long SetVideoMode(long mode);

/* gp + 0x00: the disc mode byte handed to CdControlB(0x0e, ...), the library's CdlSetmode (inferred); set to 0x80 by func_80010e28_logo. */
extern u8 data_8002eafc_logo;
/* gp + 0x0c: two buffers (inferred: pointers; the first is the one CdGetSector is handed, the table is indexed by data_8002eb20_logo in func_80010a80_logo). */
extern u32 *data_8002eb08_logo[2];
/* gp + 0x04: the buffer the decoder writes a strip to (inferred). */
extern u32 *data_8002eb00_logo;
/* gp + 0x08: the ring the stream is set up on (inferred from the call to StSetRing). */
extern u32 *data_8002eb04_logo;
/* gp + 0x14: where the expansion of run-length data writes (inferred); the
   still picture's three-word pattern is read from it by func_800111d0_logo. */
extern u8 *data_8002eb10_logo;
/* gp + 0x18: a block of memory that a picture is built in (inferred). */
extern u32 *data_8002eb14_logo;
/* gp + 0x1c: the buffer the stills' file is read into (inferred: base + 0x21100). */
extern u32 *data_8002eb18_logo;
/* gp + 0x20: set to 0 by func_800107e8_logo (inferred: no use found in these functions). */
extern int data_8002eb1c_logo;
/* gp + 0x24: the index (0 or 1) of the buffer in data_8002eb08_logo that the next frame is decoded into (inferred). */
extern int data_8002eb20_logo;
/* gp + 0x30 and gp + 0x34: the frame number of the stream's previous and current frame (inferred: 0x8002eb30 is read from word 2 of a frame's header). */
extern int data_8002eb2c_logo;
extern int data_8002eb30_logo;
/* gp + 0x28: the rectangle of the picture that the next strip goes to (inferred). */
extern Rect data_8002eb24_logo;
/* gp + 0x38: not 0 sets the disc volume to left and right apart (inferred). */
extern int data_8002eb34_logo;
/* gp + 0x3c: the video mode handed to SetVideoMode (inferred: 0 or 1). */
extern int data_8002eb38_logo;
/* gp + 0x40: a flag word, 1 while a strip is being decoded (inferred); volatile because an interrupt changes it (inferred). */
extern volatile int data_8002eb3c_logo;
/* gp + 0x44: the four volume bytes handed to CdMix (inferred from the call). */
extern u8 data_8002eb40_logo[4];
/* gp + 0x48: the filter of the disc, file and channel, both 1 (inferred from its use in CdControl(0x0d, ...)). */
extern u8 data_8002eb44_logo[2];
/* gp + 0x4c and gp + 0x50: two tables of pointers in the stills' file, the first the same buffer as data_8002eb18_logo and the second 0x40 bytes
   after it (inferred: base + 0x21100 and base + 0x21140). The first table's pointers go to the colors of a still, the second's to its codes, as
   func_800110f4_logo takes them; the pointers are relocated after the file is read. */
extern u8 **data_8002eb48_logo;
extern u16 **data_8002eb4c_logo;
/* gp + 0x54: the index (0 or 1) of the display environment shown (inferred). */
extern int data_8002eb50_logo;
/* Two display environments of 20 bytes each (inferred from the stride). */
extern u8 data_8002eb8c_logo[2][20];
/* A disc position, four bytes (inferred: the place CdIntToPos writes and CdControl(0x15, ...) reads). */
extern u8 data_8002ebb4_logo[4];
/* gp + 0x84, gp + 0x88, gp + 0x8c: the pad words: the last state read from PadRead, a copy of it and the buttons newly pressed (inferred from
   func_800106fc_logo). */
extern u32 data_8002eb80_logo;
extern u32 data_8002eb84_logo;
extern u32 data_8002eb88_logo;
/* The start of the program's big buffer (inferred: base is passed to func_800107e8_logo, which places its areas from it). */
extern u8 data_8002ede0_logo[];
/* A flag word: not 0 asks for StCdInterrupt (inferred). */
extern int data_8002edb8_logo;

#endif
