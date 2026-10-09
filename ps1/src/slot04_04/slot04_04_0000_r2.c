/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013cfdc(Object *object, u8 a, u8 b);
int func_801b1414_slot04_04(Object *obj);
u8 func_801b0de0_slot04_04(Object *obj);
u8 func_801b0e54_slot04_04(Object *obj);
u8 func_801b0ecc_slot04_04(Object *obj);
u8 func_801b0f64_slot04_04(Object *obj);
u8 func_801b0fe8_slot04_04(Object *obj);
u8 func_801b107c_slot04_04(Object *obj);
u8 func_801b111c_slot04_04(Object *obj);
u8 func_801b11b0_slot04_04(Object *obj);
int func_801b123c_slot04_04(Object *obj);
int func_801b1328_slot04_04(Object *obj);

void func_801b0bf0_slot04_04(Object *obj) {
    if ((u8)func_8013d1a8(obj) && (u8)func_801b1414_slot04_04(obj)) return;
    if ((u8)func_8013cc48(obj, 0, 0x18) && func_801b111c_slot04_04(obj)) return;
    if ((u8)func_8013ce5c(obj, 1, 6) && func_801b107c_slot04_04(obj)) return;
    if ((u8)func_8013cdf0(obj, 2, 5) && func_801b0fe8_slot04_04(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 0x25) && func_801b0ecc_slot04_04(obj)) return;
    if ((u8)func_8013cdc8(obj, 4, 1) && func_801b0f64_slot04_04(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0x13) && func_801b0e54_slot04_04(obj)) return;
    if (func_8013cfdc(obj, 6, 1) && func_801b0de0_slot04_04(obj)) return;
    if (func_8013d210(obj) && func_801b11b0_slot04_04(obj)) return;
    if ((u8)func_8013cac8(obj, 7, 0xd) && (u8)func_801b123c_slot04_04(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0xe)) func_801b1328_slot04_04(obj);
}
