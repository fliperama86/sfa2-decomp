/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cbdc(Object *object, u8 index, u8 arg);
int func_8013cc48(Object *object, u8 index, u8 arg);
int func_8013cac8(Object *object, u8 index, u8 arg);
int func_801b0da4_slot04_0b(Object *obj);
int func_801b1380_slot04_0b(Object *obj);
int func_801b12f4_slot04_0b(Object *obj);
int func_801b10dc_slot04_0b(Object *obj);
int func_801b127c_slot04_0b(Object *obj);
int func_801b1150_slot04_0b(Object *obj);
int func_801b1008_slot04_0b(Object *obj);
int func_801b0f30_slot04_0b(Object *obj);
int func_801b0ea0_slot04_0b(Object *obj);
int func_801b11e4_slot04_0b(Object *obj);
int func_801b140c_slot04_0b(Object *obj);

void func_801b0bfc_slot04_0b(Object *obj) {
    if ((u8)func_8013d1a8(obj) && func_801b0da4_slot04_0b(obj)) return;
    if (func_8013d210(obj) && func_801b0ea0_slot04_0b(obj)) return;
    if (func_8013cbdc(obj, 0, 0x15) && func_801b1380_slot04_0b(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x18) && func_801b140c_slot04_0b(obj)) return;
    if (func_8013cc48(obj, 2, 0x17) && func_801b12f4_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 3, 4) && func_801b10dc_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 4, 5) && func_801b127c_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 5, 0) && func_801b1150_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 6, 1) && func_801b11e4_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 7, 0xe) && func_801b1008_slot04_0b(obj)) return;
    if (func_8013cac8(obj, 8, 0xd)) func_801b0f30_slot04_0b(obj);
}
