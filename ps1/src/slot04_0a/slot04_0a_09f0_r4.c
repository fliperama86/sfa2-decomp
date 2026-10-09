/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013cf34(Object *object, u8 a, u8 b);
int func_8013cdf0(Object *object, u8 a, u8 b);
int func_8013cdc8(Object *object, u8 a, u8 b);
int func_801b1030_slot04_0a(Object *obj);
int func_801b0f34_slot04_0a(Object *obj);
int func_801b156c_slot04_0a(Object *obj);
int func_801b14d0_slot04_0a(Object *obj);
int func_801b13c8_slot04_0a(Object *obj);
int func_801b1428_slot04_0a(Object *obj);
int func_801b1608_slot04_0a(Object *obj);
int func_801b1218_slot04_0a(Object *obj);
int func_801b129c_slot04_0a(Object *obj);
int func_801b1320_slot04_0a(Object *obj);
int func_801b10c4_slot04_0a(Object *obj);
int func_801b116c_slot04_0a(Object *obj);

void func_801b0d44_slot04_0a(Object *obj) {
    if (func_8013d210(obj) && func_801b1030_slot04_0a(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b0f34_slot04_0a(obj)) return;
    if (func_8013cf34(obj, 0, 5) && func_801b156c_slot04_0a(obj)) return;
    if ((u8)func_8013cdf0(obj, 1, 4) && func_801b14d0_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 2, 0x1b) && func_801b13c8_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0x1c) && func_801b1428_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 4, 0) && func_801b1608_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 5, 3) && func_801b1218_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 6, 1) && func_801b129c_slot04_0a(obj)) return;
    if ((u8)func_8013cdc8(obj, 7, 2) && func_801b1320_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 8, 0xd) && func_801b10c4_slot04_0a(obj)) return;
    if ((u8)func_8013de2c(obj, 9, 0xe)) func_801b116c_slot04_0a(obj);
}
