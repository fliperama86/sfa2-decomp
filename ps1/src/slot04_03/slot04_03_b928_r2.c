/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cdf0(Object *object, u8 a, u8 b);
int func_8013cec8(Object *object, u8 a, u8 b);
int func_8013ce5c(Object *object, u8 a, u8 b);
int func_8013cdc8(Object *object, u8 a, u8 b);
int func_801b103c_slot04_03(Object *obj);
int func_801b0b9c_slot04_03(Object *obj);
int func_801b0eac_slot04_03(Object *obj);
int func_801b0e24_slot04_03(Object *obj);
int func_801b0da4_slot04_03(Object *obj);
int func_801b0f84_slot04_03(Object *obj);
int func_801b10c8_slot04_03(Object *obj);
int func_801b0d10_slot04_03(Object *obj);
int func_801b0c90_slot04_03(Object *obj);

void func_801b0a38_slot04_03(Object *obj) {
    if (func_8013d210(obj) && func_801b103c_slot04_03(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b0b9c_slot04_03(obj)) return;
    if (func_8013cdf0(obj, 0, 4) && func_801b0eac_slot04_03(obj)) return;
    if (func_8013cec8(obj, 1, 6) && func_801b0e24_slot04_03(obj)) return;
    if (func_8013ce5c(obj, 2, 5) && func_801b0da4_slot04_03(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0xd) && func_801b0f84_slot04_03(obj)) return;
    if ((u8)func_8013de2c(obj, 4, 0xe) && func_801b10c8_slot04_03(obj)) return;
    if (func_8013cdc8(obj, 5, 2) && func_801b0d10_slot04_03(obj)) return;
    if (func_8013cdc8(obj, 6, 1)) func_801b0c90_slot04_03(obj);
}

