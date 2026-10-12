/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Functions of the same module, declared here because the published headers
   lack them. Inferred from the code; not original declarations. */
u8 is_second_input_direction_active_slot04_0e(Object *obj);
void func_801b6afc_slot04_0e(Object *obj);

/* What it does (inferred, not an original name): enters a character
   action. It sets the state byte state_3 to 3 and active_flag_3 to 1, then
   set_direction_from_pressed_bits derives the direction pair direction_index, input_dir_second from the
   input word field_134. When is_second_input_direction_active_slot04_0e reports a condition
   (input_dir_second not 0 and direction_index equal to 2, and then either field_219 or
   bit 14 of field_130 depending on field_cd), func_801b6afc_slot04_0e
   runs. Otherwise the counter function add_to_meter_clamped gets half = direction_index / 2
   and sequence 12 + half (variant_index equal to 0) or 18 + half (otherwise) is
   selected, 3 more when input_dir_second is not 0. One local, a, carries three
   values in turn (half, then 12 or 18, then the sequence number); that is
   inferred from the build. */
void select_sequence_from_direction_inputs_slot04_0e(Object *obj) {
    /* Both locals are 16 bits wide: as int, a differs in 14 instruction slots (172 bytes) and half in 7 (184 bytes). */
    u16 a;
    u16 half;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    if (is_second_input_direction_active_slot04_0e(obj) != 0) {
        func_801b6afc_slot04_0e(obj);
    } else {
        a = obj->direction_index >> 1;
        half = a;
        add_to_meter_clamped(obj, a);
        a = 0x12;
        if (obj->variant_index == 0) {
            a = 0xc;
        }
        if (obj->input_dir_second == 0) {
            a = a + half;
        } else {
            /* Written in two steps; as `a = a + 3 + half;` this function differs from the original in 2 instruction slots. */
            a += 3;
            a += half;
        }
        set_sequence_by_side_and_mode(obj, a);
    }
}
