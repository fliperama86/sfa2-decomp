/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eaec0_slot06_07[];

void advance_palette_record_countdown_slot06_07(Slot06Pal *pal);
void load_palette_record_by_index_slot06_07(Slot06Pal *pal);
void set_palette_record_and_copy_data_slot06_07(Slot06Pal *pal, SeqRec *rec);

void call_handler_by_state_0_slot06_07(Object *obj) {
    data_801eaec0_slot06_07[obj->state_0](obj);
}

void init_object_params_and_four_pal_records_slot06_07(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x30);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x40);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x50);
    Slot06Pal *p3 = (Slot06Pal *)((u8 *)obj + 0x6c);
    obj->init_param_b = 7;
    obj->table_base_index = 8;
    obj->init_param_e = 9;
    obj->init_param_j = 10;
    obj->init_param_a = 0x110;
    obj->sequence_index = 0x210;
    obj->size_param = 0x290;
    obj->init_param_h = 0x300;
    obj->field_01 = 0;
    obj->offset_b = 0;
    obj->init_param_c = 0x60;
    obj->init_param_d = 0x70;
    obj->init_param_i = 0x60;
    obj->step_count = obj->step_count + 1;
    load_palette_record_by_index_slot06_07(p0);
    load_palette_record_by_index_slot06_07(p1);
    load_palette_record_by_index_slot06_07(p2);
    load_palette_record_by_index_slot06_07(p3);
}

void update_four_pal_records_slot06_07(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            advance_palette_record_countdown_slot06_07((Slot06Pal *)((u8 *)obj + 0x30));
            advance_palette_record_countdown_slot06_07((Slot06Pal *)((u8 *)obj + 0x40));
            advance_palette_record_countdown_slot06_07((Slot06Pal *)((u8 *)obj + 0x50));
            advance_palette_record_countdown_slot06_07((Slot06Pal *)((u8 *)obj + 0x6c));
        }
    }
}

void release_object_record_slot06_07(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_palette_record_countdown_slot06_07(Slot06Pal *pal) {
    SeqRec *rec;
    int v = pal->countdown - 1;
    pal->countdown = v;
    if ((s16)v == 0) {
        rec = pal->rec;
        rec++;
        if ((s16)pal->step_value < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_palette_record_and_copy_data_slot06_07(pal, rec);
    }
}

void load_palette_record_by_index_slot06_07(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    set_palette_record_and_copy_data_slot06_07(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
