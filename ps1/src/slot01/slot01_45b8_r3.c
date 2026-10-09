/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80033ba4_slot01;
extern int data_80033b9c_slot01;
extern int data_80033ba0_slot01;
void func_80014b28_slot01(Object *obj, int arg);

void func_800148a4_slot01(Object *obj) {
    u8 *m = &game_state.mode;
    Object *p = obj->field_3c;
    data_80033ba4_slot01.p = p;
    data_80033b9c_slot01 = *m | game_state.field_07;
    if ((data_80033b9c_slot01 >> p->side) & 1) {
        obj->field_05 = 0;
        obj->field_01 = 1;
        obj->field_04++;
        data_80033ba0_slot01 = 0;
        if ((*m >> data_80033ba4_slot01.p->side) & 1) {
            data_80033ba0_slot01 = 1;
            obj->field_05++;
        }
        func_80014b28_slot01(obj, (u8)data_80033ba0_slot01);
    }
}

extern ObjectFn data_80015dd4_slot01[];
void func_8001496c_slot01(Object *obj) {
    data_80015dd4_slot01[obj->field_05](obj);
    func_80131094(obj);
    func_80120028(obj);
}
