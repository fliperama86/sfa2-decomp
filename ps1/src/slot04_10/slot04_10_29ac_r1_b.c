/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6a54_slot04_10[];
extern ObjectRef data_80190468;

void func_80138ae8(GameState *state, Object *object);
void func_80142adc(Object *object);
void func_801b1078_slot04_10(Object *obj);
void func_801b2f98_slot04_10(Object *obj);
void func_801b3220_slot04_10(Object *obj);

void func_801b2f98_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        func_801b1078_slot04_10(obj);
    }
}

void func_801b2fec_slot04_10(Object *obj) {
    data_801c6a54_slot04_10[obj->state_3](obj);
}

void func_801b302c_slot04_10(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    ((Slot04bObj *)obj)->field_298 = 0;
    obj->field_225 = 1;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8((GameState *)data_80190468.p, obj);
    a = 0x49;
    if (obj->field_49 != 0) {
        a = 0x53;
    }
    func_801307e0(obj, a + (obj->direction_index >> 1));
    func_801b3220_slot04_10(obj);
}
