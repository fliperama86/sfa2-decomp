/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013caf0(Object *object, u8 index, u8 arg);
int func_801b0f8c_slot04_00(Object *obj);
int func_801b0c84_slot04_00(Object *obj);
int func_801b0b90_slot04_00(Object *obj);
int func_801b1084_slot04_00(Object *obj);
int func_801b0ff8_slot04_00(Object *obj);
int func_801b0e84_slot04_00(Object *obj);
int func_801b0ef8_slot04_00(Object *obj);
int func_801b1110_slot04_00(Object *obj);
int func_801b0d10_slot04_00(Object *obj);
int func_801b0dd0_slot04_00(Object *obj);

void func_801b098c_slot04_00(Object *obj) {
    if (func_8013d180(obj, 5, 0x1f) && func_801b0f8c_slot04_00(obj)) return;
    if (func_8013d210(obj) && func_801b0c84_slot04_00(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b0b90_slot04_00(obj)) return;
    if (func_8013caf0(obj, 0, 0x17) && func_801b1084_slot04_00(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x18) && func_801b0ff8_slot04_00(obj)) return;
    if (obj->field_7e == 0) {
        if ((u8)func_8013de2c(obj, 2, 4) && func_801b0e84_slot04_00(obj)) return;
        if ((u8)func_8013de2c(obj, 3, 0) && func_801b0ef8_slot04_00(obj)) return;
    } else {
        if ((u8)func_8013de2c(obj, 3, 0) && func_801b0ef8_slot04_00(obj)) return;
        if ((u8)func_8013de2c(obj, 2, 4) && func_801b0e84_slot04_00(obj)) return;
    }
    if ((u8)func_8013de2c(obj, 4, 3) && func_801b1110_slot04_00(obj)) return;
    if ((u8)func_8013de2c(obj, 6, 0xd) && func_801b0d10_slot04_00(obj)) return;
    if ((u8)func_8013de2c(obj, 7, 0xe)) func_801b0dd0_slot04_00(obj);
}
