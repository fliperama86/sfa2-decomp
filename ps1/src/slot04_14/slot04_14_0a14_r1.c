/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013d158(Object *object, u8 index, u8 arg);
int func_8013cc48(Object *object, u8 index, u8 arg);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cbdc(Object *object, u8 index, u8 arg);
int func_8013cac8(Object *object, u8 index, u8 arg);

/* functions of other units of this module */
u8 func_801b0eb4_slot04_14(Object *obj);
u8 func_801b0ce0_slot04_14(Object *obj);
u8 func_801b0f3c_slot04_14(Object *obj);
u8 func_801b16b4_slot04_14(Object *obj);
u8 func_801b1614_slot04_14(Object *obj);
u8 func_801b10f0_slot04_14(Object *obj);
u8 func_801b103c_slot04_14(Object *obj);
u8 func_801b0fc8_slot04_14(Object *obj);
u8 func_801b13e0_slot04_14(Object *obj);
u8 func_801b0ddc_slot04_14(Object *obj);
u8 func_801b1764_slot04_14(Object *obj);
u8 func_801b1450_slot04_14(Object *obj);
u8 func_801b1558_slot04_14(Object *obj);
u8 func_801b0e48_slot04_14(Object *obj);
u8 func_801b1180_slot04_14(Object *obj);
u8 func_801b12b0_slot04_14(Object *obj);

void func_801b0a14_slot04_14(Object *obj) {
    if (func_8013d210(obj) && func_801b0eb4_slot04_14(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b0ce0_slot04_14(obj)) return;
    if (func_8013d158(obj, 0, 0x1e) && func_801b0f3c_slot04_14(obj)) return;
    if ((u8)func_8013cc48(obj, 1, 0x18) && func_801b16b4_slot04_14(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x26) && func_801b1614_slot04_14(obj)) return;
    if ((u8)func_8013cbdc(obj, 3, 0x14) && func_801b10f0_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0x1b) && func_801b103c_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0x1c) && func_801b0fc8_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 4) && func_801b13e0_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xf) && func_801b0ddc_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0x12) && func_801b1764_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 9, 0) && func_801b1450_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 10, 3) && func_801b1558_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 11, 2) && func_801b0e48_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 12, 0xe) && func_801b1180_slot04_14(obj)) return;
    if ((u8)func_8013cac8(obj, 13, 0xd)) func_801b12b0_slot04_14(obj);
}
