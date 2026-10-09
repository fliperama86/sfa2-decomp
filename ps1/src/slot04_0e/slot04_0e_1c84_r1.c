/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cac8(Object *object, u8 a, u8 b);
u8 func_8013caf0(Object *object, u8 a, u8 b);
int func_8013cb70(Object *object, u8 a, u8 b);
int func_8013d0c8(Object *object);
int func_8013d0fc(Object *object);
u8 func_8013d130(Object *object, u8 a, u8 b);
u8 func_801417cc(Object *object);
void func_801b2658_slot04_0e(Object *obj);
int func_801b24a0_slot04_0e(Object *obj);
int func_801b253c_slot04_0e(Object *obj);
int func_801b225c_slot04_0e(Object *obj);
int func_801b21d4_slot04_0e(Object *obj);
int func_801b1ec4_slot04_0e(Object *obj);
int func_801b1f3c_slot04_0e(Object *obj);
int func_801b22e4_slot04_0e(Object *obj);
int func_801b2348_slot04_0e(Object *obj);
int func_801b1fa0_slot04_0e(Object *obj);
int func_801b2008_slot04_0e(Object *obj);
int func_801b20bc_slot04_0e(Object *obj);
int func_801b23b0_slot04_0e(Object *obj);
int func_801b2428_slot04_0e(Object *obj);

void func_801b1c84_slot04_0e(Object *obj) {
    if (func_8013d210(obj) && (u8)func_801b24a0_slot04_0e(obj)) return;
    if ((u8)func_8013d1a8(obj) && (u8)func_801b253c_slot04_0e(obj)) return;
    if ((u8)func_8013cb70(obj, 1, 0x15) && (u8)func_801b225c_slot04_0e(obj)) return;
    if (func_8013caf0(obj, 2, 0x19) && (u8)func_801b21d4_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 3, 9) && (u8)func_801b1ec4_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 4, 3) && (u8)func_801b1f3c_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 5, 0) && (u8)func_801b22e4_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 6, 2) && (u8)func_801b2348_slot04_0e(obj)) return;
    if ((u8)func_8013d0c8(obj) && (u8)func_801b1fa0_slot04_0e(obj)) return;
    if ((u8)func_8013d0fc(obj) && (u8)func_801b2008_slot04_0e(obj)) return;
    if (func_8013d130(obj, 7, 0x1d) && (u8)func_801b20bc_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 8, 0xd) && (u8)func_801b23b0_slot04_0e(obj)) return;
    if ((u8)func_8013cac8(obj, 9, 0xe)) func_801b2428_slot04_0e(obj);
}

int func_801b1ec4_slot04_0e(Object *obj) {
    if (obj->field_240 != 0) return 0;
    if (!(u8)func_801417cc(obj)) return 0;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_15a = 0;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b1f3c_slot04_0e(Object *obj) {
    if (!(u8)func_801417cc(obj)) return 0;
    obj->field_15a = 1;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b1fa0_slot04_0e(Object *obj) {
    if (!(u8)func_801417cc(obj)) return 0;
    obj->field_15a = 0xd;
    obj->field_159 = 1;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}

int func_801b2008_slot04_0e(Object *obj) {
    if (!func_801418bc(obj)) return 0;
    if (obj->field_45 == 0) return 0;
    if (obj->field_50 >= 0) return 0;
    if (obj->pos_y < obj->field_70 - 0x20) return 0;
    obj->field_48 = 1;
    if (!(obj->field_130 & 0x8000)) {
        obj->field_48 = 0xff;
    }
    obj->field_15a = 2;
    obj->field_159 = 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    func_801b2658_slot04_0e(obj);
    return 1;
}
