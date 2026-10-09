/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b0f78_slot04_09(Object *obj);
int func_801b1070_slot04_09(Object *obj);
int func_801b110c_slot04_09(Object *obj);
int func_801b11a4_slot04_09(Object *obj);
int func_801b12c4_slot04_09(Object *obj);
int func_801b1368_slot04_09(Object *obj);
int func_801b13ec_slot04_09(Object *obj);
int func_801b1474_slot04_09(Object *obj);
int func_801b14fc_slot04_09(Object *obj);
int func_801b1598_slot04_09(Object *obj);
int func_801b1644_slot04_09(Object *obj);

void func_801b0dcc_slot04_09(Object *obj) {
    if ((u8)func_8013d1a8(obj) && func_801b0f78_slot04_09(obj)) return;
    if (func_8013cc48(obj, 0, 0x15) && func_801b1644_slot04_09(obj)) return;
    if ((u8)func_8013cbdc(obj, 1, 0x14) && func_801b14fc_slot04_09(obj)) return;
    if ((u8)func_8013cb70(obj, 2, 0x19) && func_801b1598_slot04_09(obj)) return;
    if (func_8013cac8(obj, 3, 0x10) && func_801b12c4_slot04_09(obj)) return;
    if (func_8013cac8(obj, 4, 2) && func_801b1368_slot04_09(obj)) return;
    if (func_8013cac8(obj, 5, 4) && func_801b13ec_slot04_09(obj)) return;
    if (func_8013cac8(obj, 6, 0xd) && func_801b110c_slot04_09(obj)) return;
    if (func_8013cac8(obj, 7, 1) && func_801b1474_slot04_09(obj)) return;
    if (func_8013cac8(obj, 8, 0xe) && func_801b11a4_slot04_09(obj)) return;
    if (func_8013d210(obj)) func_801b1070_slot04_09(obj);
}
