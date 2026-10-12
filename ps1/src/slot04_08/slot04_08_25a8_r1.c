/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_08Rec3cb8 *data_801c3cb8_slot04_08[];
extern u16 data_801c3cc4_slot04_08[];

void start_next_sequence_from_step_table_slot04_08(Object *obj) {
    Slot04_08Rec3cb8 *t = data_801c3cb8_slot04_08[obj->direction_index >> 1];
    int a = t[((Slot04aObj *)obj)->step_index].a;

    if (a < 0) {
        obj->state_3++;
    } else {
        obj->state_3 = 3;
        if (t[((Slot04aObj *)obj)->step_index].b != 0) {
            obj->facing_side ^= 1;
        }
        ((Slot04aObj *)obj)->step_index++;
        set_sequence_by_side_and_mode(obj, a);
    }
}

void func_801b2650_slot04_08(Object *obj) {

    u16 t = obj->step_flags;
    int m = -0x100;

    if ((t & 0xff) != 0) {
        obj->step_flags = t & m;
        game_state.state_param = 0x18;
        func_80146960(obj);
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x319);
        apply_damage_to_other_return_defeated(obj, (s16)(data_801c3cc4_slot04_08[(s16)(obj->direction_index * 2 + (((Slot04aObj *)obj)->step_index + 0xffff))] | m), 0);
        obj->state_3++;
        obj->countdown = 0x10;
    }
    update_sequence(obj);
}
