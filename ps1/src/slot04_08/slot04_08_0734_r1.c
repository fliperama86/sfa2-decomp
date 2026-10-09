/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cdf0(Object *object, u8 a, u8 b);
u8 func_8013caf0(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
int func_8013cdc8(Object *object, u8 a, u8 b);
u8 func_8013cfb4(Object *object, u8 a, u8 b);
int func_801b1000_slot04_08(Object *obj);
int func_801b11f8_slot04_08(Object *obj);
int func_801b0f80_slot04_08(Object *obj);
int func_801b0ea0_slot04_08(Object *obj);
int func_801b0e10_slot04_08(Object *obj);
int func_801b09c4_slot04_08(Object *obj);
int func_801b0a38_slot04_08(Object *obj);
int func_801b0ab4_slot04_08(Object *obj);
int func_801b0b2c_slot04_08(Object *obj);
int func_801b0ba8_slot04_08(Object *obj);
int func_801b0c24_slot04_08(Object *obj);
int func_801b0ca0_slot04_08(Object *obj);
int func_801b0d18_slot04_08(Object *obj);
int func_801b0d94_slot04_08(Object *obj);
int func_801b108c_slot04_08(Object *obj);
int func_801b1144_slot04_08(Object *obj);

void func_801b0734_slot04_08(Object *obj) {
    if (func_8013d210(obj) && func_801b1000_slot04_08(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b11f8_slot04_08(obj)) return;
    if ((u8)func_8013cdf0(obj, 0xb, 4) && func_801b0f80_slot04_08(obj)) return;
    if (func_8013caf0(obj, 0xa, 0x1a) && func_801b0ea0_slot04_08(obj)) return;
    if (func_8013ccb4(obj, 9, 0x24) && func_801b0e10_slot04_08(obj)) return;
    if ((u8)func_8013cdc8(obj, 0, 2) && func_801b09c4_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 4, 3) && func_801b0a38_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 1, 0) && func_801b0ab4_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 2, 1) && func_801b0b2c_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 3, 2) && func_801b0ba8_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 8, 7) && func_801b0c24_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 5, 4) && func_801b0ca0_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 6, 5) && func_801b0d18_slot04_08(obj)) return;
    if (func_8013cfb4(obj, 7, 6) && func_801b0d94_slot04_08(obj)) return;
    if ((u8)func_8013de2c(obj, 0xc, 0xd) && func_801b108c_slot04_08(obj)) return;
    if ((u8)func_8013de2c(obj, 0xd, 0xe)) func_801b1144_slot04_08(obj);
}
