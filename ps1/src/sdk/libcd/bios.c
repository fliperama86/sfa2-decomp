// SPDX-License-Identifier: MIT
#include "libcd_internal.h"
#include "../libspu/libspu_internal.h"
#include <common.h>

// internal type representing the CD buffer size
typedef char Result_t[8];

typedef struct {
    unsigned char ack;   // deferred acknowledge of a data ready interrupt
    unsigned char sync;  // sync state
    unsigned char ready; // ready state
    unsigned char c;
} CD_intr;

typedef struct {
    CD_intr* intr;
    Result_t* result;
    unsigned char* cd_com;
    int* cd_status;
    unsigned char** cd_pos;
} CD_init_struct; // this SDK version has no version string here

typedef struct Alarm_t {
    int unk0;
    int unk4;
    char* unk8;
} Alarm_t;

#ifdef SDK_PART
extern Result_t D_80039260;
extern Result_t D_80039268;
extern Result_t D_80039270;
extern volatile Alarm_t Alarm;
extern CdlCB CD_cbsync;
extern CdlCB CD_cbready;
extern CdlCB CD_cbread;
extern int D_80032AB0;
extern int CD_status;
extern int CD_status1;
extern int CD_nopen;
extern unsigned char CD_pos[];
extern unsigned char CD_mode;
extern unsigned char CD_com;
extern char* D_80032AC8[];
extern char* D_80032B48[];
extern int D_80032B68[];
extern int D_80032BE8[];
extern int D_80032C68[];
extern int D_80032CE8[];
#else
// The reference defines these four without an initialiser: common symbols,
// which the original linker placed in the common area, far from this file's
// sections and inside the zero-filled end of the executable. A unit cannot
// own a range there, so they are named here and placed by address.
extern Result_t D_80039260;
extern Result_t D_80039268;
extern Result_t D_80039270;
extern volatile Alarm_t Alarm;

CdlCB CD_cbsync = NULL;
CdlCB CD_cbready = NULL;
// Not in the reference, which pads here. The name is not original: called
// when a read ends, with CdlComplete or CdlDiskError.
CdlCB CD_cbread = NULL;
int D_80032AB0 = 0;
int CD_status = 0;
int CD_status1 = 0;
int CD_nopen = 0;
unsigned char CD_pos[] = {2, 0, 0, 0};
unsigned char CD_com = 0;
unsigned char CD_mode = 0;
char* D_80032AC8[] = {
    "CdlSync",    "CdlNop",
    "CdlSetloc",  "CdlPlay",
    "CdlForward", "CdlBackword",
    "CdlReadN",   "CdlStandby",
    "CdlStop",    "CdlPause",
    "CdlReset",   "CdlMute",
    "CdlDemute",  "CdlSetfilter",
    "CdlSetmode", "?",
    "CdlGetlocL", "CdlGetlocP",
    "?",          "CdlGetTN",
    "CdlGetTD",   "CdlSeekL",
    "CdlSeekP",   "?",
    "?",          "?",
    "?",          "CdlReadS",
    "?",          "?",
    "?",          "?",
};
char* D_80032B48[] = {
    "NoIntr",  "DataReady", "Complete", "Acknowledge",
    "DataEnd", "DiskError", "?",        "?",
};
static int D_80032B68[] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
                           0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int D_80032BE8[] = {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
static int D_80032C68[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static int D_80032CE8[] = {0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0,
                           0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

#include "registers.h"

#ifdef SDK_PART
extern volatile int* D_80032D78;
extern volatile SPU_RXX* D_80032D7C;
extern volatile CD_intr Intr;
extern volatile int CD_read_state[];
#else
static volatile int* D_80032D78 = (int*)0x1F801020;
static volatile SPU_RXX* D_80032D7C = (SPU_RXX*)0x1F801C00;
static volatile CD_intr Intr = {0};
// Initialised: the image has it in the data section, after Intr.
static volatile int CD_read_state[10] = {0};
#endif

// The two inline functions below carry a message string each. Every part of
// this file would emit its own copy, away from where the image has it, so a
// part names the string instead and the whole file owns it.
#ifdef SDK_PART
extern char CD_alarm_msg[];
extern char CD_open_msg[];
#else
#define CD_alarm_msg "%s:(%s) timeout Sync=%s, Ready=%s\n"
#define CD_open_msg "CD open(%02x)...\n"
#endif

static inline void _memcpy(void* _dst, void* _src, size_t _size) {
    char* pDst = (char*)_dst;
    char* pSrc = (char*)_src;

    if (pDst == NULL) {
        return;
    }

    while (_size--) {
        *pDst++ = *pSrc++;
    }
}

static inline void set_alarm(char* name) {
    // schedule timeout for 480 vblanks from now
    ((Alarm_t*)&Alarm)->unk0 = VSync(-1) + 480;
    ((Alarm_t*)&Alarm)->unk4 = 0;
    ((Alarm_t*)&Alarm)->unk8 = name;
}

static inline int get_alarm(void) {
    if (((Alarm_t*)&Alarm)->unk0 < VSync(-1) ||
        ((Alarm_t*)&Alarm)->unk4++ > 0x1E0000) {
        printf(CD_alarm_msg, ((Alarm_t*)&Alarm)->unk8,
               D_80032AC8[CD_com], D_80032B48[Intr.sync],
               D_80032B48[Intr.ready]);
        CD_flush();
        return -1;
    }
    return 0;
}

int getintr(void) {
    unsigned char nReg;
    Result_t buf;
    int i, j;
    int bHasError;

    bHasError = 0;

    if (Intr.ack) {
        Intr.ack = 0;
        *libcd_CDRegister0 = 1;
        *libcd_CDRegister3 = 7;
        *libcd_CDRegister2 = 7;
    }

    *libcd_CDRegister0 = 1;

    if ((*libcd_CDRegister3 & 0x7) == 0) {
        return 0;
    }

    nReg = *libcd_CDRegister3 & 0x7;

    if (nReg == 1) {
        Intr.ack = 1;
    } else {
        *libcd_CDRegister0 = 1;
        *libcd_CDRegister3 = 7;
        *libcd_CDRegister2 = 7;
    }

    i = 0;
    for (i = 0; i < 8; i++) {
        if ((*libcd_CDRegister0 & 0x20) == 0) {
            break;
        }
        buf[i] = *libcd_CDRegister1;
    }
    for (; i < 8; i++) {
        buf[i] = 0;
    }

    if (nReg != 3 || D_80032C68[CD_com]) {
        if (!(CD_status & CdlStatShellOpen) && (buf[0] & CdlStatShellOpen)) {
            CD_nopen++;
        }
        CD_status = buf[0];
        CD_status1 = buf[1];
        bHasError = CD_status;
        bHasError &= (CdlStatError | CdlStatSeekError | CdlStatIdError | CdlStatShellOpen);
    }
    if (nReg == 5) {
        printf("%s: DiskError(%02x:%02x)\n", D_80032AC8[CD_com], CD_status,
               CD_status1);
    }
    switch (nReg) {
    case 3:
        if (bHasError) {
            Intr.sync = CdlDiskError;
            _memcpy(&D_80039260, &buf, sizeof(Result_t));
            return 2;
        }
        if (D_80032B68[CD_com]) {
            Intr.sync = CdlAcknowledge;
            _memcpy(&D_80039260, &buf, sizeof(Result_t));
            return 1;
        }
        Intr.sync = CdlComplete;
        _memcpy(&D_80039260, &buf, sizeof(Result_t));
        return 2;
    case 2:
        Intr.sync = bHasError ? CdlDiskError : CdlComplete;
        _memcpy(&D_80039260, &buf, sizeof(Result_t));
        return 2;
    case 1:
        Intr.ready = bHasError ? CdlDiskError : CdlDataReady;
        _memcpy(&D_80039268, &buf, sizeof(Result_t));
        return 4;
    case 4:
        Intr.ready = Intr.c = CdlDataEnd;
        _memcpy(&D_80039270, &buf, sizeof(Result_t));
        _memcpy(&D_80039268, &buf, sizeof(Result_t));
        return 4;
    case 5:
        Intr.sync = Intr.ready = CdlDiskError;
        _memcpy(&D_80039260, &buf, sizeof(Result_t));
        _memcpy(&D_80039268, &buf, sizeof(Result_t));
        return 6;
    default:
        printf("CDROM: unknown intr(%d)\n", nReg);
        return -1;
    }
}

static inline void callback(void) {
    int interrupt;
    unsigned char temp_s1;

    temp_s1 = *libcd_CDRegister0 & 3;

    while (true) {
        interrupt = getintr();
        if (interrupt == 0) {
            break;
        }

        if ((interrupt & 4) && CD_cbready != NULL) {
            CD_cbready(Intr.ready, &D_80039268);
        }
        if ((interrupt & 2) && CD_cbsync != NULL) {
            CD_cbsync(Intr.sync, &D_80039260);
        }
    }
    *libcd_CDRegister0 = temp_s1;
}

int CD_sync(int mode, u_char* result) {
    int i;
    int sync;

    set_alarm("CD_sync");

    while (true) {
        if (get_alarm()) {
            return -1;
        }

        if (CheckCallback()) {
            callback();
        }

        sync = Intr.sync;
        if (sync == CdlComplete || sync == CdlDiskError) {
            Intr.sync = CdlComplete;
            _memcpy(result, &D_80039260, sizeof(Result_t));
            return sync;
        }

        if (mode != 0) {
            return 0;
        }
    }
}

int CD_ready(int mode, u_char* result) {
    int i;
    int c;
    int ready;

    set_alarm("CD_ready");
    while (true) {
        if (get_alarm()) {
            return -1;
        }
        if (CheckCallback()) {
            callback();
        }
        c = Intr.c;
        if (c != 0) {
            Intr.c = 0;
            _memcpy(result, &D_80039270, sizeof(Result_t));
            return c;
        }
        ready = Intr.ready;
        if (ready != 0) {
            Intr.ready = 0;
            _memcpy(result, &D_80039268, sizeof(Result_t));
            return ready;
        }
        if (mode != 0) {
            return 0;
        }
    }
}

int CD_cw(u8 com, u8* param, u_char* result, s32 arg3) {
    int i;

    if (D_80032AB0 > 1) {
        printf("%s...\n", D_80032AC8[com]);
    }
    if ((D_80032CE8[com] != 0) && (param == NULL)) {
        if (D_80032AB0 > 0) {
            printf("%s: no param\n", D_80032AC8[com]);
        }
        return -2;
    }
    CD_sync(CdlSync, NULL);
    if (Intr.ack) {
        Intr.ack = 0;
        *libcd_CDRegister0 = 1;
        *libcd_CDRegister3 = 7;
        *libcd_CDRegister2 = 7;
    }
    if (com == CdlSetloc) {
        for (i = 0; i < 4; i++) {
            CD_pos[i] = param[i];
        }
    }
    Intr.sync = CdlNoIntr;
    if (D_80032BE8[com]) {
        Intr.ready = CdlNoIntr;
    }
    *libcd_CDRegister0 = 0;
    for (i = 0; i < D_80032BE8[com + 0x40]; i++) {
        *libcd_CDRegister2 = *param++;
    }
    CD_com = com;
    *libcd_CDRegister1 = com;
    if (arg3 != 0) {
        return 0;
    }

    set_alarm("CD_cw");

    while (Intr.sync == CdlNoIntr) {
        if (get_alarm()) {
            return -1;
        }
        if (CheckCallback()) {
            callback();
        }
    }
    _memcpy(result, &D_80039260, sizeof(Result_t));

    return -(Intr.sync == CdlDiskError);
}

int CD_vol(CdlATV* vol) {
    *libcd_CDRegister0 = 2;
    *libcd_CDRegister2 = vol->val0;
    *libcd_CDRegister3 = vol->val1;
    *libcd_CDRegister0 = 3;
    *libcd_CDRegister1 = vol->val2;
    *libcd_CDRegister2 = vol->val3;
    *libcd_CDRegister3 = 0x20;
    return 0;
}

// The name is not original (the original name of this function is unknown).
// Waits while the lid is open and returns the number of waits.
inline int CD_wait_shell(void) {
    CdlCB old;
    int count;

    old = CD_cbsync;
    count = 0;
    while (CD_status & CdlStatShellOpen) {
        CD_cbsync = NULL;
        printf(CD_open_msg, CD_status);
        count++;
        CD_cw(CdlNop, NULL, NULL, 0);
        VSync(60);
        CD_cbsync = old;
    }
    return count;
}

int CD_flush(void) {
    *libcd_CDRegister0 = 1;
    while (*libcd_CDRegister3 & 7) {
        *libcd_CDRegister0 = 1;
        *libcd_CDRegister3 = 7;
        *libcd_CDRegister2 = 7;
    }

    Intr.ready = Intr.c = CdlNoIntr;
    Intr.sync = CdlComplete;
    *libcd_CDRegister0 = 0;
    *libcd_CDRegister3 = 0;
    *D_80032D78 = 0x1325;
}

int CD_initvol(void) {
    CdlATV vol;

    if (D_80032D7C->main_volx.left == 0 && D_80032D7C->main_volx.right == 0) {
        D_80032D7C->main_vol.left = 0x3FFF;
        D_80032D7C->main_vol.right = 0x3FFF;
    }

    D_80032D7C->cd_vol.left = 0x3FFF;
    D_80032D7C->cd_vol.right = 0x3FFF;
    D_80032D7C->spucnt = 0xC001;
    vol.val0 = vol.val2 = 0x80;
    vol.val1 = vol.val3 = 0;
    *libcd_CDRegister0 = 2;
    *libcd_CDRegister2 = vol.val0;
    *libcd_CDRegister3 = vol.val1;
    *libcd_CDRegister0 = 3;
    *libcd_CDRegister1 = vol.val2;
    *libcd_CDRegister2 = vol.val3;
    *libcd_CDRegister3 = 0x20;

    return 0;
}

void CD_initintr(void) {
    int i;
    volatile int* q;
    CD_cbready = NULL;
    CD_cbsync = NULL;
    CD_status = 0;
    for (q = CD_read_state, i = 9; i != -1; i--) {
        *q++ = 0;
    }
    ResetCallback();
    InterruptCallback(2, callback);
}

#ifdef SDK_PART
extern CD_init_struct D_80032D84;
extern volatile int* D_80032D9C;
extern volatile int* D_80032DA0;
extern volatile int* D_80032DA4;
extern volatile int* D_80032DA8;
extern volatile int* D_80032DAC;
#else
static CD_init_struct D_80032D84 = {
    .intr = &Intr,
    .result = &D_80039260,
    .cd_com = &CD_com,
    .cd_status = &CD_status,
    .cd_pos = &CD_pos};
static volatile int* D_80032D9C = (int*)0x1F801018;
static volatile int* D_80032DA0 = (int*)0x1F8010F0;
static volatile int* D_80032DA4 = (int*)0x1F8010B0;
static volatile int* D_80032DA8 = (int*)0x1F8010B4;
static volatile int* D_80032DAC = (int*)0x1F8010B8;
#endif

int CD_init(void) {
    int i;
    volatile int* q;

    printf("CD_init:addr=%08x\n", &D_80032D84);
    CD_cbready = NULL;
    CD_cbsync = NULL;
    CD_status = 0;
    for (q = CD_read_state, i = 9; i != -1; i--) {
        *q++ = 0;
    }
    ResetCallback();
    InterruptCallback(2, callback);

    *libcd_CDRegister0 = 1;
    while (*libcd_CDRegister3 & 7) {
        *libcd_CDRegister0 = 1;
        *libcd_CDRegister3 = 7;
        *libcd_CDRegister2 = 7;
    }

    Intr.ready = Intr.c = CdlNoIntr;
    Intr.sync = CdlComplete;

    *libcd_CDRegister0 = 0;
    *libcd_CDRegister3 = 0;
    *D_80032D78 = 0x1325;

    CD_cw(CdlNop, NULL, NULL, 0);
    if (CD_status & CdlStatShellOpen) {
        CD_cw(CdlNop, NULL, NULL, 0);
    }
    CD_wait_shell();

    if (CD_cw(CdlReset, NULL, NULL, 0)) {
        return -1;
    }

    if (CD_cw(CdlDemute, NULL, NULL, 0)) {
        return -1;
    }

    return -(CD_sync(CdlSync, NULL) != CdlComplete);
}

// The name is not original. Restarts a read from the saved state, retrying.
int CD_read_restart(void);

// Not in the reference; the name is not original. Data ready callback of a
// running read: takes one sector per interrupt, restarts the read after an
// error and ends it when no sector is left or no retry remains. The image has
// it after `callback`, at the end of the file, like an inline function whose
// address is taken.
static inline void cb_read(int intr, u_char* result) {
    if (intr == CdlDataReady) {
        if (CD_read_state[6] > 0) {
            CD_getsector((void*)CD_read_state[3], CD_read_state[5]);
            CD_read_state[3] += CD_read_state[5] * 4;
            CD_read_state[6]--;
        }
    } else {
        CD_read_state[6] = -1;
    }
    CD_read_state[7] = VSync(-1);
    if (CD_read_state[6] < 0 && CD_read_state[0] > 0) {
        CD_read_restart();
    }
    if (CD_read_state[6] <= 0) {
        CD_cbsync = (CdlCB)CD_read_state[8];
        CD_cbready = (CdlCB)CD_read_state[9];
        CD_cw(CdlPause, NULL, NULL, 0);
        if (CD_cbread != NULL) {
            CD_cbread(CD_read_state[6] == 0 ? CdlComplete : CdlDiskError, result);
        }
    }
}

int CD_read_restart(void) {
    u_char mode;
    int i;

    CD_cbready = NULL;
    CD_cbsync = NULL;
    while (CD_read_state[0]-- > 0) {
        if (CD_read_state[0] < 7) {
            printf("CD read retry %2d(%02x:%02x:%02x)\n", CD_read_state[0],
                   CD_pos[0], CD_pos[1], CD_pos[2]);
            if (CD_wait_shell()) {
                for (i = 7; i != -1; i--) {
                    VSync(60);
                }
            }
        }
        *libcd_CDRegister0 = 1;
        while (*libcd_CDRegister3 & 7) {
            *libcd_CDRegister0 = 1;
            *libcd_CDRegister3 = 7;
            *libcd_CDRegister2 = 7;
        }
        Intr.ready = Intr.c = CdlNoIntr;
        Intr.sync = CdlComplete;
        *libcd_CDRegister0 = 0;
        *libcd_CDRegister3 = 0;
        *D_80032D78 = 0x1325;

        mode = CD_read_state[4];
        if (CD_cw(CdlSetmode, &mode, NULL, 0)) {
            continue;
        }
        if (CD_cw(CdlSetloc, CD_pos, NULL, 0)) {
            continue;
        }
        CD_cbready = (CdlCB)cb_read;
        CD_read_state[3] = CD_read_state[2];
        CD_cw(CdlReadN, NULL, NULL, 1);
        CD_read_state[6] = CD_read_state[1];
        CD_read_state[7] = VSync(-1) + 480;
        return CD_read_state[6];
    }
    return CD_read_state[6] = -1;
}

// The name is not original. Saves the read request and starts it.
int CD_read_begin(int arg0, int arg1, int mode) {
    CdlCB sync;
    CdlCB ready;

    CD_read_state[4] = mode;
    switch (CD_read_state[4] & 0x30) {
    case 0:
        CD_read_state[5] = 0x200;
        break;
    case 0x20:
        CD_read_state[5] = 0x249;
        break;
    default:
        CD_read_state[5] = 0x246;
        break;
    }
    CD_read_state[2] = arg0;
    sync = CD_cbsync;
    CD_read_state[1] = arg1;
    ready = CD_cbready;
    CD_read_state[0] = 8;
    CD_read_state[8] = (int)sync;
    CD_read_state[9] = (int)ready;
    if (CD_status & (CdlStatRead | CdlStatSeek | CdlStatPlay)) {
        CD_cw(CdlPause, NULL, NULL, 0);
    }
    CD_sync(CdlSync, NULL);
    return -(CD_read_restart() < 1);
}

// The name is not original. Waits for the running read; result is the
// number of sectors left, or -1 on timeout.
int CD_read_wait(int mode, u_char* result) {
    int err;

    set_alarm("CD_read");
    while (true) {
        if ((err = get_alarm())) {
            return err;
        }
        if (CheckCallback()) {
            callback();
        }
        _memcpy(result, &D_80039268, sizeof(Result_t));
        if (VSync(-1) > CD_read_state[7] + 0x3C) {
            CD_read_restart();
        }
        if (CD_read_state[6] == 0) {
            CD_datasync(0);
        }
        if (mode != 0 || CD_read_state[6] <= 0) {
            return CD_read_state[6];
        }
    }
}

int CD_datasync(int mode) {
    int ret;

    set_alarm("CD_datasync");
    while (true) {
        if (get_alarm()) {
            ret = -1;
            break;
        }
        if (!(*D_80032DAC & 0x01000000)) {
            ret = 0;
            break;
        }
        if (mode != 0) {
            ret = 1;
            break;
        }
    }
    return ret;
}

int CD_getsector(void* buffer, size_t size) {
    *libcd_CDRegister0 = 0;
    *libcd_CDRegister3 = 0x80;
    *D_80032D9C = 0x20943;
    *D_80032D78 = 0x1323;
    *D_80032DA0 |= 0x8000;
    *D_80032DA4 = buffer;
    *D_80032DA8 = size | 0x10000;
    *D_80032DAC = 0x11000000;
    while (*D_80032DAC & 0x01000000) {
    };
    *D_80032D78 = 0x1325;
    return 0;
}

void CD_set_test_parmnum(int parmNum) { D_80032CE8[25] = parmNum; }
