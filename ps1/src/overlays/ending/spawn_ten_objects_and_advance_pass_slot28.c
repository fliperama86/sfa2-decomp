/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ba4_slot28[];
extern Object *data_80051bd0_slot28[];
extern u8 data_800469ec_slot28[];
extern u8 data_80046d34_slot28[];
extern SequenceStep *data_80047bdc_slot28[];
extern SequenceStep *data_80047be4_slot28[];
void copy_words_to_two_buffer_copies_slot28(Object *obj);
void set_sequence_of_fourth_hud_object_14_slot28(Object *obj, int arg);
void init_object_data_pointers_a_14_slot28(Object *obj);

/* Sets up a scene of eleven objects (inferred). When the given object's
   field_f0 is 0 it sets the HUD state's countdown_timer to 600 and adds 1 to its
   pass_index, calls copy_words_to_two_buffer_copies_slot28, and prepares the object held in
   data_80051bd0_slot28 (position y -0x60, field_01 1, sequence set with
   set_sequence_from_table_5). Then it asks pop_block_from_pool for nine objects; each one it
   gets is filled in and entered in the table data_80051ba4_slot28 at index 1
   to 9. It asks for one more, sets it up with init_object_data_pointers_a_14_slot28 and
   set_sequence_from_table_5 and enters it at index 0. It calls start_fade_mode_1a01_and_wait,
   set_menu_command_by_code (1, 0x504) and set_sequence_of_fourth_hud_object_14_slot28 (the given object, 4). */
void spawn_ten_objects_and_advance_pass_slot28(Object *obj)
{
    Object *o;
    int i;
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 1 written at its uses (the two stores of active_count and field_01) this function differs from the original in 4 instruction slots. */
    int active;
    /* Stand-in, not recovered source: the local mode, a second name for the constant 2 passed to the last set_sequence_from_table_5 call (the original loads it into a1 at the call, no register of its own), reproduces the original's order of instructions. Without it this function differs from the original in 2 instruction slots. */
    int mode;
    Object **slot;
    if (obj->field_f0 == 0) {
        /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        /* Without it this function differs from the original in 3 instruction slots. */
        do {
            i = 1;
            active = 1;
        } while (0);
        mode = 2;
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index++;
        copy_words_to_two_buffer_copies_slot28(obj);
        o = data_80051bd0_slot28[0];
        slot = &data_80051ba4_slot28[1];
        o->pos_y = -0x60;
        o->field_01 = 1;
        set_sequence_from_table_5(o, 0, data_80047bdc_slot28);
        for (; i < 10; i++) {
            o = (Object *)pop_block_from_pool();
            if (o != 0) {
                o->field_02 = 0x65;
                o->ot_depth = 2;
                o->spawn_x = 0x60;
                o->spawn_y = 0x1e0;
                o->field_90 = (void *)0x80060000;
                o->data_ptr_a = data_800469ec_slot28;
                o->data_ptr_b = data_80046d34_slot28;
                o->active_count = active;
                o->table_index = i;
                o->field_01 = active;
                o->image_row = 0;
                o->box_tables = (BoxTables *)data_80047be4_slot28;
                *slot = o;
            }
            slot++;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            init_object_data_pointers_a_14_slot28(o);
            o->pos_x = 0xc8;
            o->pos_y = 0xa0;
            o->spawn_x = 0x60;
            o->spawn_y = 0x1e0;
            o->image_row = 0;
            o->ot_depth = 2;
            set_sequence_from_table_5(o, mode, data_80047be4_slot28);
            data_80051ba4_slot28[0] = o;
            o->ot_depth = 3;
        }
        start_fade_mode_1a01_and_wait();
        set_menu_command_by_code(1, 0x504);
        set_sequence_of_fourth_hud_object_14_slot28(obj, 4);
    }
}
