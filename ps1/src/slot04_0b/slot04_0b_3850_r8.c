/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011ffdc(Object *o);
void func_801b4db8_slot04_0b(Object *o, int index);

void func_801b46b8_slot04_0b(Object *o) {
    o->field_05++;
    o->field_09 = 0;
    o->pos_x = ref_other.p->pos_x;
    o->pos_y = ref_other.p->pos_y;
    func_801b4db8_slot04_0b(o, 0x19);
    func_8011ffdc(o);
}

void func_801b4728_slot04_0b(Object *o) {
    if (ref_other.p->field_3a & 1) {
        o->field_05 = 0;
        o->field_04++;
    } else {
        func_8011ffdc(o);
    }
}
