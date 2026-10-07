/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078458_slot00(Object *obj, int index);
extern void func_80078b50_slot00(Object *obj);
extern void func_80078b94_slot00(Object *obj);
extern void func_80078d78_slot00(Object *obj);

void func_800785f4_slot00(Object *obj) {
    obj->field_60 = 0x1f;
    obj->field_61 = 0x1f;
    obj->field_06 = obj->field_06 + 1;
    func_80131094(obj);
}
