/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c1630_slot04_05[];
extern int data_801c1700_slot04_05;

void start_input_slot_on_match_slot04_05(Object *obj, int a1, int a2);
void update_input_slot_repeat_or_timeout_slot04_05(Object *obj, int a1, int a2);

int update_input_slot_sequence_result_slot04_05(Object *obj, int a1, int a2) {
    u8 i = a1;

    data_801c1700_slot04_05 = 0;
    if (obj->slots[i].stage == 0) {
        start_input_slot_on_match_slot04_05(obj, i, (u8)a2);
    } else {
        update_input_slot_repeat_or_timeout_slot04_05(obj, i, (u8)a2);
    }
    return data_801c1700_slot04_05;
}

void start_input_slot_on_match_slot04_05(Object *obj, int a1, int a2) {
    u8 k = a2;

    if (obj->pressed_bits & data_801c1630_slot04_05[k * 3]) {
        obj->slots[(u8)a1].stage++;
        ((u8 *)&obj->slots[(u8)a1])[2] = ((u8 *)&data_801c1630_slot04_05[k * 3])[2];
        obj->slots[(u8)a1].timer = ((u8 *)&data_801c1630_slot04_05[k * 3])[4];
    } else {
        obj->slots[(u8)a1].stage = 0;
    }
}
