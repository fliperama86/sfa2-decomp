/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0e04_slot04_01(Object *obj);
int func_801b0e70_slot04_01(Object *obj);
int func_801b09c4_slot04_01(Object *obj);
int func_801b0ac0_slot04_01(Object *obj);
int func_801b0b4c_slot04_01(Object *obj);
int func_801b0bd8_slot04_01(Object *obj);
int func_801b0c4c_slot04_01(Object *obj);
int func_801b0ce0_slot04_01(Object *obj);
int func_801b0d54_slot04_01(Object *obj);
int func_801b0ef8_slot04_01(Object *obj);
int func_801b0fb8_slot04_01(Object *obj);
u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013d180(Object *object, u8 index, u8 arg);
int func_8013cb70(Object *object, u8 index, u8 arg);
u8 func_8013caf0(Object *object, u8 index, u8 arg);

void func_801b07fc_slot04_01(Object *obj) {
    if (func_8013d180(obj, 6, 0x1f) && func_801b0e04_slot04_01(obj)) return;
    if (func_8013d210(obj) && func_801b0e70_slot04_01(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b09c4_slot04_01(obj)) return;
    if (func_8013cb70(obj, 0, 0x14) && func_801b0ac0_slot04_01(obj)) return;
    if (func_8013caf0(obj, 1, 0x15) && func_801b0b4c_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 2, 4) && func_801b0bd8_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0) && func_801b0c4c_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 4, 2) && func_801b0ce0_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 5, 3) && func_801b0d54_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 8, 0xd) && func_801b0ef8_slot04_01(obj)) return;
    if ((u8)func_8013de2c(obj, 9, 0xe)) func_801b0fb8_slot04_01(obj);
}
