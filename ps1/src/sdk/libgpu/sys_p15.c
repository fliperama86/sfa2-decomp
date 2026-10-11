/* Part of libgpu/sys.c of the SDK reference, cut from this project's adapted copy of that file (the folder's README says what the copy changed): functions _clr, _dws, _drs. Made by hand in the form of the parts that sdkimport.py generates; where a function differs from the adapted copy's text its comments say so. */
#define SDK_PART 1
// SPDX-License-Identifier: MIT
//! PSYQ=3.3
#include "common.h"
#include <psxsdk/libgpu.h>

/* A declaration of the copy routine whose parameter types are not the
   compiler's built-in ones. With it the compiler calls the routine instead
   of copying in place, as the image does. Whether the library's own header
   read like this is not known. */
void *memcpy(unsigned char *dst, unsigned char *src, int n);

typedef struct {
    /* 0x00 */ const char* rcsid; // aIdSysCV1831995
    /* 0x04 */ void (*addque)(void (*)(int*, int), int*, int);      // _addque
    /* 0x08 */ int (*addque2)(void (*)(int*, int), int*, int, int); // _addque2
    /* 0x0C */ int (*clr)(RECT*, int);                              // _clr
    /* 0x10 */ void (*ctl)(u_long);                                 // _ctl
    /* 0x14 */ int (*cwb)(int*, int);                               // _cwb
    /* 0x18 */ void (*cwc)(int);                                    // _cwc
    /* 0x1C */ int (*drs)(RECT*, int*);                             // _drs
    /* 0x20 */ int (*dws)(RECT*, int*);                             // _dws
    /* 0x24 */ int (*exeque)(void);                                 // _exeque
    /* 0x28 */ int (*getctl)(int);                                  // _getctl
    /* 0x2C */ int (*otc)(int, int);                                // _otc
    /* 0x30 */ int (*param)(int);                                   // _param
    /* 0x34 */ int (*reset)(int);                                   // _reset
    /* 0x38 */ u_long (*status)(void);                              // _status
    /* 0x3C */ int (*sync)(int);                                    // _sync
} gpu; // size = 0x40

struct QueueItem {
    void (*unk0)(int*, int);
    int* unk4;
    int unk8;
    int unk[0x15];
};

void _addque(void (*)(int*, int), int*, int);
int _addque2(void (*)(int*, int), int*, int, int);
int _clr(RECT*, int);
void _ctl(u_long);
int _cwb(int*, int);
void _cwc(int);
int _drs(RECT*, int*);
int _dws(RECT*, int*);
int _exeque(void);
int _getctl(int);
int _otc(int, int);
int _param(int);
int _reset(int);
u_long _status(void);
int _sync(int);
int gpu_probe(int);

int printf(char*, ...);
u_long get_ce(short, short);
u_long get_cs(short, short);
u_long get_tw(RECT* tw);
u_long get_ofs(short, short);

struct GpuState {
    u8 type;
    u8 queue;
    u8 debug;
    u8 reverse;
    short w;
    short h;
    int pad;
    void (*cb)();
    DRAWENV draw;
    DISPENV disp;
}; // size = 0x80

#ifdef SDK_PART
extern struct GpuState gs;
extern int D_80037E20[0x10];
#else
static struct GpuState gs;
static int D_80037E20[0x10];
#endif
#ifdef SDK_PART
extern u8 ctlbuf[0x100];
extern int D_last[3];
extern int D_intr_addque2;
extern int D_intr_exeque;
extern int D_intr_reset;
extern volatile struct QueueItem D_80037F54[0x40];
extern int D_80039254;
extern int D_80039258;
#else
static u8 ctlbuf[0x100];
/* func, arg, param of the last command run. Name not original. With `volatile` here
   get_alarm is exact; without it, it differs in 4 slots. Left plain until the
   project decides whether volatile is allowed on state the reference does not have. */
static int D_last[3];
static int D_intr_addque2;
static int D_intr_exeque;
static int D_intr_reset;
static volatile struct QueueItem D_80037F54[0x40];
static int D_80039254;
static int D_80039258;
#endif

#ifdef SDK_PART
extern u16 D_8002C214[];
extern u16 D_8002C228[];
#else
static u16 D_8002C214[] = {0x400, 0, 0x400, 0, 0x400, 0, 0x400, 0, 0x400, 0};
static u16 D_8002C228[] = {0x200, 0, 0x400, 0, 0x400, 0, 0x200, 0, 0x400, 0};
#endif

#ifdef SDK_PART
extern u_long D_8002C23C[5];
extern u_long D_8002C250[5];
#else
static u_long D_8002C23C[5] = {0x04FFFFFF, 0x80000000, 0, 0, 0};
static u_long D_8002C250[5] = {0x04FFFFFF, 0, 0, 0, 0};
#endif

#ifdef SDK_PART
extern gpu D_8002C220;
extern gpu* D_8002C260;
extern void (*GPU_printf)(const char* fmt, ...);
extern volatile int* GPU_DATA;
extern volatile u_long* GPU_STATUS;
extern volatile int* DMA2_MADR;
extern volatile int* DMA2_BCR;
extern volatile int* DMA2_CHCR;
extern volatile int* DMA6_MADR;
extern volatile int* DMA6_BCR;
extern volatile int* DMA6_CHCR;
extern volatile int* DPCR;
extern volatile int _qin;
extern volatile int _qout;
#else
static gpu D_8002C220 = {
    .rcsid = "$Id: sys.c,v 1.83 1995/05/25 13:43:27 suzu Exp $",
    .addque = _addque,
    .addque2 = _addque2,
    .clr = _clr,
    .ctl = _ctl,
    .cwb = _cwb,
    .cwc = _cwc,
    .drs = _drs,
    .dws = _dws,
    .exeque = _exeque,
    .getctl = _getctl,
    .otc = _otc,
    .param = _param,
    .reset = _reset,
    .status = _status,
    .sync = _sync,
};
static gpu* D_8002C260 = &D_8002C220;
void (*GPU_printf)(const char* fmt, ...) = printf;
static volatile int* GPU_DATA = (int*)0x1F801810;
static volatile u_long* GPU_STATUS = (u_long*)0x1F801814;
static volatile int* DMA2_MADR = (int*)0x1F8010A0;
static volatile int* DMA2_BCR = (int*)0x1F8010A4;
static volatile int* DMA2_CHCR = (int*)0x1F8010A8;
static volatile int* DMA6_MADR = (int*)0x1F8010E0;
static volatile int* DMA6_BCR = (int*)0x1F8010E4;
static volatile int* DMA6_CHCR = (int*)0x1F8010E8;
static volatile int* DPCR = (int*)0x1F8010F0;
static volatile int _qin = 0;
static volatile int _qout = 0;
#endif

#ifndef CLAMP
#define CLAMP(value, low, high)                                                \
    value < low ? low : (value > high ? high : value)
#endif

// gpu commands
#define CMD_CLEAR_CACHE 0x01000000
#define CMD_FILL_RECTANGLE_IN_VRAM(color) ((color & 0xFFFFFF) | 0x02000000)
#define CMD_MONOCHROME_RECTANGLE(color) ((color & 0xFFFFFF) | 0x60000000)
#define CMD_COPY_VRAM_TO_CPU 0xC0000000
#define CMD_COPY_CPU_TO_VRAM 0xA0000000

// gpu display control commands (see
// https://psx-spx.consoledev.net/graphicsprocessingunitgpu/#gpu-display-control-commands-gp1)
#define CMD_RESET_GPU 0x00000000
#define CMD_RESET_CMD_BUF 0x01000000
#define CMD_ACK_GPU_INTR 0x02000000
#define CMD_DISP_ENABLE 0x03000000
#define CMD_DATA_REQUEST 0x04000000
#define CMD_SET_DISP_AREA_START 0x05000000
#define CMD_SET_H_DISP_RANGE 0x06000000
#define CMD_SET_V_DISP_RANGE 0x07000000
#define CMD_SET_DISP_MODE 0x08000000
#define CMD_GPU_READ_REG 0x10000000
#define CMD_SET_VRAM_SIZE 0x20000000

#define DATA_REQ_OFF 0
#define DATA_REQ_FIFO 1
#define DATA_REQ_CPU_TO_GPU 2
#define DATA_REQ_GPU_TO_CPU 3

// status reg bits
#define STATUS_READY_TO_RECEIVE_CMD (1 << 26)
#define STATUS_READY_TO_SEND_VRAM_TO_CPU (1 << 27)

int ResetGraph(int level);

int SetGraphReverse(int mode);

int SetGraphDebug(int level);

int SetGraphQueue(int mode);

int GetGraphType(void);

int GetGraphDebug(void);

u_long DrawSyncCallback(void (*func)());

void SetDispMask(int mask);

int DrawSync(int mode);

void checkRECT(const char* log, RECT* r);

int ClearImage(RECT* rect, u8 r, u8 g, u8 b);

int LoadImage(RECT* rect, u_long* p);

int StoreImage(RECT* rect, u_long* p);

int MoveImage(RECT* rect, int x, int y);

OT_TYPE* ClearOTag(OT_TYPE* ot, int n);

OT_TYPE* ClearOTagR(OT_TYPE* ot, int n);

void DrawPrim(void* p);

void DrawOTag(OT_TYPE* p);

DRAWENV* PutDrawEnv(DRAWENV* env);

DRAWENV* GetDrawEnv(DRAWENV* env);

DISPENV* PutDispEnv(DISPENV* env);

DISPENV* GetDispEnv(DISPENV* env);

u_long GetODE(void);

void SetTexWindow(DR_TWIN* p, RECT* tw);

void SetDrawArea(DR_AREA* p, RECT* r);

void SetDrawOffset(DR_OFFSET* p, u_short* ofs);

void SetPriority(DR_PRIO* p, int pbc, int pbw);

void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT* tw);

void SetDrawEnv(DR_ENV* dr_env_in, DRAWENV* env);

int get_mode(int dfe, int dtd, int tpage);

u_long get_cs(short x, short y);

u_long get_ce(short x, short y);

u_long get_ofs(short x, short y);

u_long get_tw(RECT* tw);

u_long get_dx(DISPENV* env);

u_long _status(void);

int _otc(int arg0, int arg1);

// Clears Frame Buffer
int _clr(RECT* arg0, int color) {
    int* ptr;
    int lim_w;
    int lim_h;

    /* The rectangle is limited in place, the lower limit is 1, and each upper limit is read once, into a local, inside the conditional. The adapted copy's text limited a copy of the rectangle from 0 with the reference's CLAMP macro, which names the upper limit twice. */
    /* Stand-in, not recovered source: the cast that reads gs.w as volatile, in this statement and in the next on gs.h (one stand-in at two places), reproduces the original's order of instructions. Without it this function differs from the original in 13 instruction slots. */
    /* The figure is for the two casts taken out together, not for each alone. */
    arg0->w = arg0->w < 1 ? 1 : (lim_w = *(volatile short*)&gs.w - 1, arg0->w > lim_w ? lim_w : arg0->w);
    /* Stand-in, not recovered source: the second place of the stand-in above, the same cast on gs.h, reproduces the original's order of instructions. Without it this function differs from the original in 13 instruction slots. */
    /* The figure is the one above: the two places together. */
    arg0->h = arg0->h < 1 ? 1 : (lim_h = *(volatile short*)&gs.h - 1, arg0->h > lim_h ? lim_h : arg0->h);

    if ((arg0->x & 0x3F) || (arg0->w & 0x3F)) {
        ptr = &D_80037E20[8];
        D_80037E20[0] = 0x07000000 | ((int)ptr & 0xFFFFFF); // set up otag
        D_80037E20[1] = 0xE3000000; // set drawing area top left
        D_80037E20[2] = 0xE4FFFFFF; // set drawing area bottom right
        D_80037E20[3] = 0xE5000000; // set drawing offset
        D_80037E20[4] = 0xE6000000;

        D_80037E20[5] = CMD_MONOCHROME_RECTANGLE(color);
        D_80037E20[6] = LOW(arg0->x);
        D_80037E20[7] = LOW(arg0->w);
        D_80037E20[8] = 0x03FFFFFF;
        D_80037E20[9] = 0xE3000000 | _param(3); // set drawing area top left
        D_80037E20[10] =
            0xE4000000 | _param(4); // set drawing area bottom right
        D_80037E20[11] = 0xE5000000 | _param(5); // set drawing offset
    } else {
        D_80037E20[0] = 0x04FFFFFF;
        D_80037E20[1] = 0xE6000000; // mask bit setting

        D_80037E20[2] = CMD_FILL_RECTANGLE_IN_VRAM(color);
        D_80037E20[3] = LOW(arg0->x);
        D_80037E20[4] = LOW(arg0->w);
    }
    _cwc(&D_80037E20[0]);
    return 0;
}

// LoadImage
int _dws(RECT* arg0, int* arg1) {
    int temp_a0;
    int size;
    int var_s0;
    int var_s4;
    int lim_w;
    int lim_h;

    set_alarm();
    var_s4 = 0;

    /* The rectangle is limited in place, the lower limit is 1, and each upper limit is read once, into a local, inside the conditional. The adapted copy's text limited a copy of the rectangle from 0 with the reference's CLAMP macro, which names the upper limit twice. */
    /* Stand-in, not recovered source: the cast that reads gs.w as volatile, in this statement and in the next on gs.h (one stand-in at two places), reproduces the original's order of instructions. Without it this function differs from the original in 13 instruction slots. */
    /* The figure is for the two casts taken out together, not for each alone. */
    arg0->w = arg0->w < 1 ? 1 : (lim_w = *(volatile short*)&gs.w - 1, arg0->w > lim_w ? lim_w : arg0->w);
    /* Stand-in, not recovered source: the second place of the stand-in above, the same cast on gs.h, reproduces the original's order of instructions. Without it this function differs from the original in 13 instruction slots. */
    /* The figure is the one above: the two places together. */
    arg0->h = arg0->h < 1 ? 1 : (lim_h = *(volatile short*)&gs.h - 1, arg0->h > lim_h ? lim_h : arg0->h);

    temp_a0 = ((arg0->w * arg0->h) + 1) / 2;
    if (temp_a0 <= 0) {
        return -1;
    }
    var_s0 = temp_a0 % 16;
    size = temp_a0 / 16;
    while (!(*GPU_STATUS & STATUS_READY_TO_RECEIVE_CMD)) {
        if (get_alarm()) {
            return -1;
        }
    }

    *GPU_STATUS = CMD_DATA_REQUEST | DATA_REQ_OFF;

    *GPU_DATA = CMD_CLEAR_CACHE;
    *GPU_DATA = var_s4 ? 0xB0000000 : CMD_COPY_CPU_TO_VRAM;
    *GPU_DATA = LOW(arg0->x);
    *GPU_DATA = LOW(arg0->w);

    while (var_s0--) {
        *GPU_DATA = *arg1++;
    }

    if (size != 0) {
        *GPU_STATUS = CMD_DATA_REQUEST | DATA_REQ_CPU_TO_GPU;
        *DMA2_MADR = arg1;
        *DMA2_BCR = (size << 0x10) | 0x10;
        *DMA2_CHCR = 0x01000201;
    }

    return 0;
}

// StoreImage
// Transfers image data from the frame buffer to main memory.
int _drs(RECT* arg0, int* arg1) {
    int temp_a0;
    int size;
    int var_s0;
    int lim_w;
    int lim_h;

    set_alarm();

    /* The rectangle is limited in place, the lower limit is 1, and each upper limit is read once, into a local, inside the conditional. The adapted copy's text limited a copy of the rectangle from 0 with the reference's CLAMP macro, which names the upper limit twice. */
    /* Stand-in, not recovered source: the cast that reads gs.w as volatile, in this statement and in the next on gs.h (one stand-in at two places), reproduces the original's order of instructions. Without it this function differs from the original in 12 instruction slots. */
    /* The figure is for the two casts taken out together, not for each alone. */
    arg0->w = arg0->w < 1 ? 1 : (lim_w = *(volatile short*)&gs.w - 1, arg0->w > lim_w ? lim_w : arg0->w);
    /* Stand-in, not recovered source: the second place of the stand-in above, the same cast on gs.h, reproduces the original's order of instructions. Without it this function differs from the original in 12 instruction slots. */
    /* The figure is the one above: the two places together. */
    arg0->h = arg0->h < 1 ? 1 : (lim_h = *(volatile short*)&gs.h - 1, arg0->h > lim_h ? lim_h : arg0->h);

    temp_a0 = ((arg0->w * arg0->h) + 1) / 2;
    if (temp_a0 <= 0) {
        return -1;
    }
    var_s0 = temp_a0 % 16;
    size = temp_a0 / 16;
    while (!(*GPU_STATUS & STATUS_READY_TO_RECEIVE_CMD)) {
        if (get_alarm()) {
            return -1;
        }
    }

    *GPU_STATUS = CMD_DATA_REQUEST | DATA_REQ_OFF;

    *GPU_DATA = CMD_CLEAR_CACHE;
    *GPU_DATA = CMD_COPY_VRAM_TO_CPU;
    *GPU_DATA = LOW(arg0->x);
    *GPU_DATA = LOW(arg0->w);

    while (!(*GPU_STATUS & STATUS_READY_TO_SEND_VRAM_TO_CPU)) {
        if (get_alarm()) {
            return -1;
        }
    }

    while (var_s0--) {
        *arg1++ = *GPU_DATA;
    }

    if (size != 0) {
        *GPU_STATUS = CMD_DATA_REQUEST | DATA_REQ_GPU_TO_CPU;
        *DMA2_MADR = arg1;
        *DMA2_BCR = (size << 0x10) | 0x10;
        *DMA2_CHCR = 0x01000200;
    }

    return 0;
}

void _ctl(u_long arg0);

int _getctl(int arg0);

int _cwb(int* arg0, int arg1);

void _cwc(int arg0);

int _param(int arg0);

void _addque(void (*arg0)(int*, int), int* arg1, int arg2);

int _addque2(void (*arg0)(int*, int), int* arg1, int arg2, int arg3);

int _exeque(void);

extern inline int _reset(int level) {
    D_intr_reset = SetIntrMask(0);
    _qout = 0;
    _qin = _qout;
    switch (level & 7) {
    case 0:
        *DMA2_CHCR = 0x401;
        *DPCR |= 0x800;
        *GPU_STATUS = CMD_RESET_GPU;
        memset2(ctlbuf, 0, sizeof(ctlbuf));
        memset2(D_80037F54, 0, sizeof(D_80037F54));
        break;
    case 1:
        *DMA2_CHCR = 0x401;
        *DPCR |= 0x800;
        *GPU_STATUS = CMD_ACK_GPU_INTR;
        *GPU_STATUS = CMD_RESET_CMD_BUF;
        break;
    default:
        break;
    }
    SetIntrMask(D_intr_reset);
    if ((level & 7) == 0) {
        return gpu_probe(level);
    }
    return 0;
}

int _sync(int arg0);

void set_alarm(void);

int get_alarm(void);

// NOTE: the name gpu_probe is not original. Reads the GPU type register and
// returns the graph type (0 to 4), used by _reset.
int gpu_probe(int level);

void memset2(s8* ptr, int value, int num);
