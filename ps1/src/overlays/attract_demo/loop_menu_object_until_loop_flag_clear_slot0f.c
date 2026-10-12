/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void call_handler_by_handler_index_b_slot0f(Object *o);
extern Slot0fObj data_800f857c_slot0f;
extern void (*data_800e8778_slot0f[])(Object *);

void loop_menu_object_until_loop_flag_clear_slot0f(void) {
    Slot0fObj *o = &data_800f857c_slot0f;
    data_800f857c_slot0f.field_03 = 0;
    data_800f857c_slot0f.field_02 = 0;
    data_800f857c_slot0f.field_01 = 0;
    o->state_index = 0;
    data_800f857c_slot0f.loop_flag = 1;
    do {
        o->input_bits = data_801a696a | data_801a6976;
        data_800e8778_slot0f[(s8)o->state_index]((Object *)o);
        call_handler_by_handler_index_b_slot0f((Object *)o);
        request_sound_command_and_switch_thread(1);
    } while (o->loop_flag != 0);
}

void clear_states_and_increment_active_count_slot0f(Object *o) {
    u8 n = o->active_count;
    o->state_3 = 0;
    o->state_2 = 0;
    o->state_1 = 0;
    o->state_0 = 0;
    o->ot_depth = 0;
    o->field_0a = 0;
    o->active_count = n + 1;
}
