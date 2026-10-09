/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800199dc_slot28(Object *obj);
extern void (*data_80035400_slot28[])(Object *);

void func_80019788_slot28(Object *obj) {
    func_800199dc_slot28(obj);
    data_80035400_slot28[obj->field_05](obj);
}
