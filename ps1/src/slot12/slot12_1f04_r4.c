/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
void func_800123e0_slot12(void);
void func_8001231c_slot12(int a);

void func_8001224c_slot12(Object *obj) {
    data_8002d56c_slot12->field_02 = 0;
    data_8002d56c_slot12->sequence = obj->sequence;
    data_8002d56c_slot12->pos_x = obj->pos_x;
    data_8002d56c_slot12->pos_y = obj->pos_y;
    data_8002d56c_slot12->field_0b = obj->field_0b;
    data_8002d56c_slot12->field_7a = obj->field_7a;
    data_8002d56c_slot12->field_7c = obj->field_7c;
    data_8002d56c_slot12->field_0d = 0;
    func_800123e0_slot12();
}

void func_800122cc_slot12(Object *obj) {
    u8 v;

    if (!(game_state.field_44 & 0x80)) {
        v = obj->field_3a;
        if (v != 0) {
            obj->field_3a = obj->field_3a & 0xff00;
            func_8001231c_slot12(v);
        }
    }
}
