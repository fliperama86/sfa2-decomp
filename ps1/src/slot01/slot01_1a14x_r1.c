/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern u8 data_8002cbfc_slot01[];
extern u8 **data_8002ce3c_slot01[];
extern int *data_800192dc_slot01[];
extern int data_8002121c_slot01[];
extern void *data_80021170_slot01[];
extern SequenceStep *data_80021224_slot01[];
extern Slot01Rec152ec data_800152ec_slot01;
void set_sequence_from_slot01_table_slot01(Object *obj, u8 a);
void set_record_cursor_and_spawn_y_slot01(Object *obj, Slot01Rec152ec *a1);

/*
 * Sets up an object as a panel piece (inferred, not an original name). It
 * sets the common fields and the default handlers, then takes one of three
 * shapes by the object's table_index: with bit 7 set it shows a picked entry
 * of data_8002ce3c_slot01 (chosen by the kind and kind_copy of the object
 * game_state.focus_object points at) and starts an animation; otherwise it is
 * placed at the left margin (table_index not 0) or at the right margin and
 * mirrored when variant_index is not 0 (table_index 0), and records a cursor.
 */
void init_intro_object_by_table_index_slot01(Object *obj) {
    Object *other = game_state.focus_object;
    int sel;
    int s;
    obj->field_01 = 1;
    obj->ot_depth = 1;
    obj->data_ptr_a = data_80019330_slot01;
    obj->data_ptr_b = data_800195b8_slot01;
    obj->field_90 = (void *)0x80059000;
    obj->state_1 = 0;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->table_index_2 = 0;
    obj->facing_side = 0;
    obj->state_0++;
    if (obj->table_index & 0x80) {
        /* The store to game_state.random_index is written in each arm; one store after the join through a local differs in 19 instruction lines. */
        if (other->kind_copy < 0 && game_state.field_0c != 0) {
            game_state.random_index = get_random_byte() & 3;
        } else {
            if (other->kind_copy < 0) {
                s = 0;
                /* health is read as a signed halfword; the plain u16 spelling differs in 3 instruction lines. */
                if ((s16)other->health < 0x80) {
                    if ((s16)other->health < 0xf) {
                        s = 0x20;
                    } else {
                        s = 0x10;
                    }
                }
                s += get_random_byte() & 0xf;
                sel = ~other->kind_copy;
            } else {
                sel = other->kind_copy;
                s = data_8002cbfc_slot01[other->kind * 24 + game_state.input_match_offset];
            }
            game_state.random_index = data_8002ce3c_slot01[other->kind][sel][s];
        }
        obj->ot_depth = 0;
        /* pos_x is set before pos_y; the other order differs in 3 instruction lines. */
        obj->pos_x = box_margin[0] + 0x18;
        obj->pos_y = 0xa0;
        /* The original reads game_state.random_index back for the table index; using a local instead differs in 20 instruction lines. */
        data_8002121c_slot01[0] = data_800192dc_slot01[other->kind][game_state.random_index];
        obj->field_90 = (void *)0x80071668;
        obj->data_ptr_a = data_80021170_slot01[other->kind * 2];
        obj->data_ptr_b = data_80021170_slot01[other->kind * 2 + 1];
        set_sequence_from_table_5(obj, 0, data_80021224_slot01);
    } else {
        if (obj->table_index != 0) {
            obj->pos_x = box_margin[0] + 5;
            obj->pos_y = 0x60;
            set_record_cursor_and_spawn_y_slot01(obj, &data_800152ec_slot01);
        } else {
            obj->pos_x = box_margin[0] + 0x17f;
            obj->pos_y = 0x90;
            obj->vel_x = -0xc;
            obj->accel_x = 2;
            obj->countdown = 0x1f;
            if (obj->variant_index != 0) {
                obj->pos_x = box_margin[0] - 0x175;
                /* The mirrored values are negated from the fields; the constants 0xc and -2 differ in 10 and 12 instruction lines. */
                obj->vel_x = -obj->vel_x;
                obj->accel_x = -obj->accel_x;
            }
            set_record_cursor_and_spawn_y_slot01(obj, &data_800152ec_slot01);
        }
        set_sequence_from_slot01_table_slot01(obj, 0);
    }
}
