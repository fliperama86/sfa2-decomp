/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80028e8c_slot27[];
extern ObjectFn data_80028e94_slot27[];
extern SequenceStep *data_8002904c_slot27[];
void func_80016d90_slot27(Object *obj);
void func_80016de4_slot27(Object *obj);

void func_80016c38_slot27(Object *obj) {
    *(u16 *)&obj->pos_x = *(u16 *)&ref_other.p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&ref_other.p->pos_y;
    obj->field_09 = ref_other.p->field_09 - 1;
    data_80028e8c_slot27[obj->field_03](obj);
}

void func_80016cb4_slot27(Object *obj) {
    func_80131094(obj);
}

void func_80016cd4_slot27(Object *obj) {
    data_80028e94_slot27[obj->field_05](obj);
    func_80016de4_slot27(obj);
}

void func_80016d28_slot27(Object *obj) {
    if (ref_other.p->field_6b == 0) {
        func_80016d90_slot27(obj);
    } else {
        obj->field_05++;
        *(int *)data_8019045c = 1;
        func_80130768(obj, 1, data_8002904c_slot27);
    }
}
