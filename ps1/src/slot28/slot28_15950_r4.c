/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8002616c_slot28(Object *obj) {
    Object *b;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + 0x60000;
    if (obj->pos_x >= 0x88) {
        obj->state_1 = obj->state_1 + 1;
        b = obj->parent;
        b->variant_index = 0xff;
        func_80130768(obj, 2, (SequenceStep **)obj->box_tables);
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0x2d;
            b->table_index = 2;
            b->field_01 = 1;
            b->field_90 = (void *)0x80060000;
            b->data_ptr_a = obj->data_ptr_a;
            b->data_ptr_b = obj->data_ptr_b;
            b->pos_x = 0xb0;
            b->pos_y = 0x70;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 2;
            b->box_tables = (BoxTables *)((ObjectWords *)obj)->word_58;
        }
    }
}
