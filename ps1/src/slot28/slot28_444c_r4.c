/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8002acb8_slot28[])(Object *);
extern SequenceStep *data_80029ef4_slot28[];
extern Object *data_8005187c_slot28[];

void func_80014854_slot28(Object *obj, int arg) {
    func_80130768(data_8005187c_slot28[3], arg, data_80029ef4_slot28);
}

void func_80014884_slot28(Object *object) {
    data_8002acb8_slot28[object->field_04](object);
}

void func_800148c4_slot28(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}

void func_800148d8_slot28(Object *obj) {
    if (obj->field_03 == 0) {
        func_80131094(obj);
    } else {
        func_80131094(obj);
        if ((obj->field_3a & 0x7fff) == 1) {
            obj->pos_y = *(u16 *)&obj->field_50 + 8;
        } else {
            obj->pos_y = *(u16 *)&obj->field_50;
        }
    }
}
