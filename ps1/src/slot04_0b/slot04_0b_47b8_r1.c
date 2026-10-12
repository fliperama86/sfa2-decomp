/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4db8_slot04_0b(Object *obj, int index);
void func_801b4e0c_slot04_0b(Object *obj, int dx, int dy);

void func_801b47b8_slot04_0b(Object *obj) {
    obj->image_row = 0x10;
    obj->state_1++;
    func_801b4e0c_slot04_0b(obj, 6, 0);
    func_801b4db8_slot04_0b(obj, 0x16);
    func_8011ffdc(obj);
}

void func_801b4810_slot04_0b(Object *obj) {
    Object *n;
    if (ref_other.p->step_flags & 1) {
        obj->state_1++;
        func_801b4e0c_slot04_0b(obj, -0x19, -3);
        func_801b4db8_slot04_0b(obj, 0x17);
        n = (Object *)func_8011f1e0();
        if (n != 0) {
            n->active_count = 1;
            n->field_02 = 6;
            n->table_index = 2;
            n->parent = obj->parent;
            n->sound_side = ((Slot04aObj *)ref_other.p)->side;
            n->mode_id = 0x20;
            n->field_90 = ref_other.p->field_90;
            n->data_ptr_a = ref_other.p->data_ptr_a;
            n->data_ptr_b = ref_other.p->data_ptr_b;
            if (n->sound_side == 0) {
                n->spawn_x = 0x100;
            } else {
                n->spawn_x = 0x120;
            }
            n->spawn_y = 0x1e0;
        }
    }
    func_8011ffdc(obj);
}
