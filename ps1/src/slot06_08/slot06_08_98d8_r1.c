/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_random_countdown_slot06_08(Object *obj);
void set_sequence_from_table_by_index_slot06_08(Object *obj, int arg);
void advance_sequence_rec_4(Object *object);

extern ObjectFn data_801ea6c0_slot06_08[];
extern ObjectFn data_801ea6d0_slot06_08[];
extern SeqRec *data_801ea6b8_slot06_08[];

void call_handler_by_state_0_slot06_08(Object *obj) {
    data_801ea6c0_slot06_08[obj->state_0](obj);
}

void init_state_0_random_countdown_slot06_08(Object *obj) {
    obj->field_01 = 0;
    obj->state_0++;
    set_random_countdown_slot06_08(obj);
    set_sequence_from_table_by_index_slot06_08(obj, 0);
}

void call_handler_by_state_1_unless_paused_slot06_08(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            data_801ea6d0_slot06_08[obj->state_1](obj);
        }
    }
}

void count_down_then_advance_state_1_slot06_08(Object *obj) {
    int v;
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->state_1++;
        v = get_random_byte() & 0xf;
        obj->countdown = (v & 0xf) ? v : v + 8;
        set_sequence_from_table_by_index_slot06_08(obj, 1);
    } else {
        advance_sequence_rec_4(obj);
    }
}

void count_down_then_reset_state_1_slot06_08(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown == 0) {
            obj->state_1 = 0;
            set_random_countdown_slot06_08(obj);
            set_sequence_from_table_by_index_slot06_08(obj, 0);
            return;
        }
    }
    advance_sequence_rec_4(obj);
}

void release_object_record_slot06_08(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_random_countdown_slot06_08(Object *obj) {
    int a = (get_random_byte() & 3) << 8;
    int b = get_random_byte() & 0xff;
    s16 c = (b & 0xff) ? b : b + 8;
    obj->countdown = c + a;
}

void set_sequence_from_table_by_index_slot06_08(Object *obj, int arg) {
    set_sequence_from_table_4(obj, data_801ea6b8_slot06_08, (u8)arg);
}
