/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_8002c3a8_slot12[4];
void func_80016864_slot12(Object *obj);
void func_80016a7c_slot12(Object *obj);
void func_80016c50_slot12(Object *obj);
void func_80016de4_slot12(Object *obj);

void func_800166d0_slot12(Object *obj) {
    s16 t;

    obj->field_09 = 4;
    obj->field_01 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->pos_x = 0;
    obj->pos_y = 0;
    obj->field_0c = 0;
    obj->field_04 = obj->field_04 + 1;
    data_8002c3a8_slot12[0].value = 0;
    data_8002c3a8_slot12[1].value = 0;
    data_8002c3a8_slot12[2].value = 0;
    data_8002c3a8_slot12[3].value = 0;
    obj->pos_x = 0xe8;
    obj->pos_y = 0x58;
    obj->field_5e = 0xa;
    obj->field_76 = 0;
    obj->field_48 = table_8016e5c4[game_state.field_2b8].field_08;
    if (game_state.field_2bf == 0) {
        obj->field_50 = (int)game_state.config + 0x19e;
        obj->field_54 = *(s16 *)((u8 *)game_state.config + 0x220);
    } else {
        obj->field_50 = (int)game_state.config + 0x1de;
        obj->field_54 = *(s16 *)((u8 *)game_state.config + 0x222);
    }
    t = obj->field_54;
    obj->field_5c = t;
    if (t >= 0) {
        obj->field_5c = t & 0x3f;
        func_80016de4_slot12(obj);
        game_state.field_2bf = 0;
        obj->field_70 = 0xf;
        func_80016a7c_slot12(obj);
        func_80016c50_slot12(obj);
        func_80016864_slot12(obj);
    } else {
        game_state.field_2bf = 0;
        obj->field_4c = 0xb0;
        func_80016a7c_slot12(obj);
        func_80016c50_slot12(obj);
    }
}
