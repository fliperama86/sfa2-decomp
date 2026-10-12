/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ebc1c_slot06_0c[];

void load_palette_record_by_index_slot06_0c(Slot06Pal *pal);

void init_object_with_three_sequence_cursors_slot06_0c(Object *obj) {
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x5c);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x70);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x4c);

    obj->field_01 = 0;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    p0->seq_rec_index = 0xe;
    p1->seq_rec_index = 0xf;
    p2->seq_rec_index = 0x13;
    p0->rect_x = 0x1a0;
    p1->rect_x = 0x260;
    p2->rect_x = 0x200;
    p0->rect_y = 0x60;
    p1->rect_y = 0x60;
    p2->rect_y = 0x60;
    /* The listing stores state_0 twice: the incremented value above, then 1. */
    obj->state_0 = 1;
    load_palette_record_by_index_slot06_0c(p0);
    load_palette_record_by_index_slot06_0c(p1);
    load_palette_record_by_index_slot06_0c(p2);
    set_sequence_entry(obj, data_801ebc1c_slot06_0c[1]);
}
