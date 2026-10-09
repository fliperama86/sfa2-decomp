/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
u8 func_8013caf0(Object *object, u8 index, u8 arg);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_801b1470_slot04_07(Object *obj);
int func_801b137c_slot04_07(Object *obj);
int func_801b1870_slot04_07(Object *obj);
int func_801b16f4_slot04_07(Object *obj);
int func_801b1668_slot04_07(Object *obj);
int func_801b17fc_slot04_07(Object *obj);
int func_801b1788_slot04_07(Object *obj);
int func_801b18e8_slot04_07(Object *obj);
int func_801b15b0_slot04_07(Object *obj);
void func_801b14f8_slot04_07(Object *obj);

void func_801b11dc_slot04_07(Object *obj) {
    if (func_8013d210(obj) && func_801b1470_slot04_07(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b137c_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 0, 3) && func_801b1870_slot04_07(obj)) return;
    if (func_8013caf0(obj, 1, 0x14) && func_801b16f4_slot04_07(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x15) && func_801b1668_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 0) && func_801b17fc_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 4, 1) && func_801b1788_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 5, 2) && func_801b18e8_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 6, 0xe) && func_801b15b0_slot04_07(obj)) return;
    if ((u8)func_8013de2c(obj, 7, 0xd)) func_801b14f8_slot04_07(obj);
}
