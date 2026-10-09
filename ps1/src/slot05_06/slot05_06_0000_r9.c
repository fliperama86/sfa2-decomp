/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013ccdc(Object *object, u8 a, u8 b);
u8 func_8013ccb4(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cac8(Object *object, u8 index, u8 arg);
int func_801c90b4_slot05_06(Object *obj);
int func_801c913c_slot05_06(Object *obj);
int func_801c8d60_slot05_06(Object *obj);
int func_801c8c64_slot05_06(Object *obj);
int func_801c8bfc_slot05_06(Object *obj);
int func_801c8fe4_slot05_06(Object *obj);
int func_801c8cf0_slot05_06(Object *obj);
int func_801c926c_slot05_06(Object *obj);
int func_801c904c_slot05_06(Object *obj);
int func_801c8de4_slot05_06(Object *obj);
int func_801c8ee4_slot05_06(Object *obj);

void func_801c8a0c_slot05_06(Object *obj) {
    if (func_8013d210(obj) && (u8)func_801c90b4_slot05_06(obj)) return;
    if ((u8)func_8013d1a8(obj) && (u8)func_801c913c_slot05_06(obj)) return;
    if (func_8013ccdc(obj, 0, 0x22) && (u8)func_801c8d60_slot05_06(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x14) && (u8)func_801c8c64_slot05_06(obj)) return;
    if (func_8013ccb4(obj, 2, 0x21) && (u8)func_801c8bfc_slot05_06(obj)) return;
    if (func_8013ccb4(obj, 3, 0x23) && (u8)func_801c8fe4_slot05_06(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 0) && (u8)func_801c8cf0_slot05_06(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 5) && (u8)func_801c926c_slot05_06(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 0xe) && (u8)func_801c904c_slot05_06(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xd) && (u8)func_801c8de4_slot05_06(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0xe)) func_801c8ee4_slot05_06(obj);
}
