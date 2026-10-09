/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013caf0(Object *object, u8 index, u8 arg);
int func_801b3984_slot04_05(Object *obj, int a1, int a2);
void func_801b142c_slot04_05(Object *obj);
int func_801b1330_slot04_05(Object *obj);
int func_801b1124_slot04_05(Object *obj);
int func_801b1098_slot04_05(Object *obj);
int func_801b0e1c_slot04_05(Object *obj);
int func_801b0e90_slot04_05(Object *obj);
int func_801b0f04_slot04_05(Object *obj);
int func_801b1270_slot04_05(Object *obj);
int func_801b11b0_slot04_05(Object *obj);
int func_801b0f7c_slot04_05(Object *obj);
int func_801b0ff0_slot04_05(Object *obj);
int func_801b0d94_slot04_05(Object *obj);

void func_801b0bd4_slot04_05(Object *obj) {
    func_801b142c_slot04_05(obj);
    if ((u8)func_8013d1a8(obj) && func_801b1330_slot04_05(obj)) return;
    if (func_8013caf0(obj, 0, 0x14) && func_801b1124_slot04_05(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x15) && func_801b1098_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 2, 5) && func_801b0e1c_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 1) && func_801b0e90_slot04_05(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0x13) && func_801b0f04_slot04_05(obj)) return;
    if (func_8013de2c(obj, 5, 0xd) && func_801b1270_slot04_05(obj)) return;
    if (func_8013de2c(obj, 6, 0xe) && func_801b11b0_slot04_05(obj)) return;
    if (func_801b3984_slot04_05(obj, 7, 0) && func_801b0f7c_slot04_05(obj)) return;
    if (func_801b3984_slot04_05(obj, 8, 1) && func_801b0ff0_slot04_05(obj)) return;
    if (func_8013d210(obj)) func_801b0d94_slot04_05(obj);
}
