/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800267bc_slot27[])(Object *);
extern SequenceStep *data_800268b0_slot27[];
void func_8001188c_slot27(Object *obj);

void func_80011660_slot27(Object *object) {
    data_800267bc_slot27[object->field_05](object);
}

void func_800116a0_slot27(Object *obj) {
    Object *p = obj->other;
    int k = obj->field_48;

    if (k != obj->field_03) {
        obj->field_03 = k;
    }
    k = p->kind;
    if (k != obj->field_48) {
        obj->field_48 = k;
        func_8001188c_slot27(obj);
        func_80130768(obj, obj->field_48, data_800268b0_slot27);
    }
    k = p->side;
    if ((game_state.mode >> k) & 1) {
        obj->field_05 = obj->field_05 + 1;
        obj->field_45 = p->field_d4;
        func_8001188c_slot27(obj);
    }
    func_80131094(obj);
}
