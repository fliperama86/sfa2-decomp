/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028a88_slot12[])(Object *);
extern Slot12Prim data_8002d528_slot12[];
extern SequenceStep *data_80028a84_slot12;
void func_80014434_slot12(Object *obj, Slot12Prim *cells, int a, int b);
void func_80014300_slot12(Object *obj, void *cell);

void func_80017408_slot12(Object *obj) {
    data_80028a88_slot12[obj->field_04](obj);
}

void func_80017448_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    obj->field_12 = 0xc0;
    obj->field_16 = 0x80;
    obj->field_09 = 5;
    obj->field_01 = 0;
    obj->field_04++;
    func_80130768(o, 0, &data_80028a84_slot12);
    func_80014434_slot12(o, data_8002d528_slot12, 0x100, 0xc0);
}

void func_800174bc_slot12(Object *obj) {
    func_80014300_slot12(obj, &data_8002d528_slot12[data_801a27d0]);
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
}

void func_80017520_slot12(Object *obj) {
    func_8011f240();
}
