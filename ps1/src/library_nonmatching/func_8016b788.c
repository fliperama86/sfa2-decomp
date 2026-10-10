/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes (the printed line of the
 * test gives both sizes). The build does not use this file. The differential
 * test next to it (difftest.py, with func_8016b788.py as the contract)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * Provenance: written by this project from the listing of the original. The
 * sound library of the reference that the SDK files come from has an
 * _spu_init of another version of the library, with a different order of
 * stores; it was read only for the names of the chip's registers, and no
 * text of it is used here.
 *
 * What it does (inferred, not an original name): the reset of the sound
 * chip that the library's _SpuInit calls (the library's name for this
 * address is _spu_init). a0 = arg0 is 0 for a cold start and not 0 for a
 * hot one. In order: sets bits 16, 17 and 19 of the word that the DMA
 * pointer D_80033514 points at; clears _spu_transMode, _spu_addrMode and
 * _spu_tsa; clears the control register of the chip (through the pointer
 * _spu_RXX); idles in a short counting loop on two volatile locals; clears
 * the wait counter D_800334FC and polls the status register: while its low
 * 11 bits are not 0 it counts up, and at a count above 5000 it prints a
 * time-out message through printf and stops waiting; sets the four
 * memory-mode words (_spu_mem_mode 2, _spu_mem_mode_plus 3,
 * _spu_mem_mode_unit 8, _spu_mem_mode_unitM 7); writes the transfer mode
 * register 4, clears the main and the reverb volumes, writes 0xffff to the
 * key-off register pair low half and high half, and clears the reverb mode.
 * A cold start then also clears the register pairs for frequency
 * modulation, noise, CD volume and external volume, sets _spu_tsa to 0x200,
 * writes the 16 bytes of the table at 0x80183174 (D_80033540) through
 * _spu_writeByIO, sets all 24 voices through _spu_setVoiceRegs with a
 * local record (all voices, mask 0x1f, volumes 0, pitch 0x3fff, start
 * address 0x200, envelope word 0), clears _spu_addrMode again, writes
 * 0xffff to the key-on low half, ors 0xff into its high half, idles four
 * times, does the same for key-off, and idles four times. Both ends: sets
 * _spu_inTransfer to 1, writes 0xc000 to the control register, clears
 * _spu_transferCallback and _spu_IRQCallback, leaves 0 in v0. The role names are
 * inferred from where the registers sit (offsets from _spu_RXX).
 *
 * Contract (the roles named for fields are inferred):
 *   Arguments: a0 = arg0 (0: cold start, not 0: hot start). The original
 *     leaves 0 in v0 on both paths (read from the listing; the library's
 *     _SpuInit does not use it). This C is declared void, as protos.h
 *     declares func_8016b788, so it returns nothing and the test does not
 *     compare v0.
 *   Reads: the pointers _spu_RXX and D_80033514; the word they point at
 *     (the DMA register); the chip's status register at offset 0x1ae and,
 *     through the callees below, other registers of the block; the table
 *     D_80033540 (16 bytes, only read by _spu_writeByIO).
 *   Writes: the globals above; the chip's register block at the offsets
 *     0x180..0x19a, 0x1aa, 0x1ac, 0x1b0..0x1b6 and, through the two library
 *     callees, the voice registers and the transfer registers; the DMA
 *     register word.
 *   Callees: _spu_writeByIO and _spu_setVoiceRegs run as the original code,
 *     the same in both runs, on a register block that is plain memory in
 *     the test. printf is replaced by a recorder in both runs: it logs its
 *     address, its two arguments and the first 12 characters of the second
 *     (the address of that string is a different one in the two builds, so
 *     it is logged under a mask of 0 and its text is logged instead); what
 *     printf would do is outside the test.
 *   Hardware: the test cannot have the chip. The setup points _spu_RXX at
 *     a block of RAM of its own and D_80033514 at a word of RAM, so that
 *     both runs read and write plain memory. A pass shows that both codes
 *     leave the same final bytes in the block, the word, the globals and
 *     the log, from the same starting bytes. It does not show the order of
 *     the stores (not compared), and a register that answers by itself on
 *     the real chip is not modelled: the status register is a fixed word
 *     per case, so a wait that ends after some polls and not at the first
 *     one, or a wait whose count is cut by a changing status, is not
 *     reached (the loop's exit test after at least one poll is never taken
 *     as true). Cases cover the cold and the hot start, a status with low
 *     bits 0 (no wait) and with them set (the wait runs to its time-out,
 *     the printf call is made), and random content of the block.
 *   Aliasing: the register block, the DMA word and the globals do not
 *     overlap.
 *   Excluded inputs: none other than the above.
 *   Not reached by any input (read from the listing): the exit test of the
 *     wait loop after a poll that finds the low bits 0 (the status does not
 *     change during a run).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A local view of the chip's register block that _spu_RXX points at (only
   the fields this function uses are named; offsets in the comments). */
typedef struct {
    u8 voice[0x180];      /* 0x000: 24 voices of 16 bytes */
    u16 main_vol[2];      /* 0x180 */
    u16 rev_vol[2];       /* 0x184 */
    u16 key_on[2];        /* 0x188 */
    u16 key_off[2];       /* 0x18c */
    u16 chan_fm[2];       /* 0x190 */
    u16 noise_mode[2];    /* 0x194 */
    u16 rev_mode[2];      /* 0x198 */
    u16 gap_19c[7];       /* 0x19c */
    u16 spucnt;           /* 0x1aa */
    u16 data_trans;       /* 0x1ac */
    u16 spustat;          /* 0x1ae */
    u16 cd_vol[2];        /* 0x1b0 */
    u16 ex_vol[2];        /* 0x1b4 */
} SpuRegsView;

/* A local view of the record that _spu_setVoiceRegs takes (the fields this
   function sets; the others are left as they are on the stack). */
typedef struct {
    u32 voice;            /* 0x00 */
    u32 mask;             /* 0x04 */
    s16 vol_l;            /* 0x08 */
    s16 vol_r;            /* 0x0a */
    u32 volmode;          /* 0x0c: not set here */
    u16 pitch;            /* 0x10 */
    u16 pad12;            /* 0x12: not set here */
    u32 addr;             /* 0x14 */
    u32 loop_addr;        /* 0x18: not set here */
    u32 adsr;             /* 0x1c */
} VoiceAttrView;

extern volatile SpuRegsView *_spu_RXX;
extern volatile u32 *D_80033514;
extern s32 _spu_transMode;
extern s32 _spu_addrMode;
extern u16 _spu_tsa;
extern s32 D_800334FC;
extern s32 _spu_mem_mode;
extern s32 _spu_mem_mode_plus;
extern s32 _spu_mem_mode_unit;
extern s32 _spu_mem_mode_unitM;
extern s32 _spu_inTransfer;
extern void (*volatile _spu_transferCallback)(void);
extern void (*volatile _spu_IRQCallback)(void);
extern u16 D_80033540[];
extern char _spu_timeout_msg[];

s32 _spu_writeByIO(u8 *addr, u32 size);
void _spu_setVoiceRegs(VoiceAttrView *attr);

#define IDLE()                          \
    sp4 = 0xD;                          \
    for (sp0 = 0; sp0 < 0xF0; sp0++) {  \
        sp4 *= 3;                       \
    }

void func_8016b788(int arg0) {
    volatile s32 sp0;
    volatile s32 sp4;
    VoiceAttrView attr;
    s32 i;

    *D_80033514 |= 0xB0000;
    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    _spu_RXX->spucnt = 0;
    IDLE();
    D_800334FC = 0;
    while (_spu_RXX->spustat & 0x7FF) {
        if (++D_800334FC > 5000) {
            printf(_spu_timeout_msg, "wait (reset)");
            break;
        }
    }
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    _spu_RXX->data_trans = 4;
    _spu_RXX->main_vol[0] = 0;
    _spu_RXX->main_vol[1] = 0;
    _spu_RXX->rev_vol[0] = 0;
    _spu_RXX->rev_vol[1] = 0;
    _spu_RXX->key_off[0] = 0xFFFF;
    _spu_RXX->key_off[1] = 0xFFFF;
    _spu_RXX->rev_mode[0] = 0;
    _spu_RXX->rev_mode[1] = 0;
    if (arg0 == 0) {
        _spu_RXX->chan_fm[0] = 0;
        _spu_RXX->chan_fm[1] = 0;
        _spu_RXX->noise_mode[0] = 0;
        _spu_RXX->noise_mode[1] = 0;
        _spu_RXX->cd_vol[0] = 0;
        _spu_RXX->cd_vol[1] = 0;
        _spu_RXX->ex_vol[0] = 0;
        _spu_RXX->ex_vol[1] = 0;
        _spu_tsa = 0x200;
        _spu_writeByIO((u8 *)D_80033540, 0x10);
        attr.voice = 0xFFFFFFFF;
        attr.mask = 0x1F;
        attr.vol_l = 0;
        attr.vol_r = 0;
        attr.pitch = 0x3FFF;
        attr.addr = 0x200;
        attr.adsr = 0;
        _spu_addrMode = 0;
        _spu_setVoiceRegs(&attr);
        _spu_RXX->key_on[0] |= 0xFFFF;
        _spu_RXX->key_on[1] |= 0xFF;
        IDLE();
        IDLE();
        IDLE();
        IDLE();
        _spu_RXX->key_off[0] |= 0xFFFF;
        _spu_RXX->key_off[1] |= 0xFF;
        IDLE();
        IDLE();
        IDLE();
        IDLE();
    }
    _spu_inTransfer = 1;
    _spu_RXX->spucnt = 0xC000;
    _spu_transferCallback = 0;
    _spu_IRQCallback = 0;
}
