/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078458_slot00(Object *obj, int mode);

void func_80078258_slot00(Object *obj) {
    func_80078458_slot00(obj, 1);
}

void func_80078278_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->facing_side = p->facing_side;
    if (obj->facing_side != 0) {
        obj->pos_x = (u16)p->pos_x - 7;
    } else {
        obj->pos_x = (u16)p->pos_x + 7;
    }
    obj->pos_y = (u16)p->pos_y + 5;
    obj->variant_index = (get_random_byte() & 0x1f) + 0xa;
    func_80078458_slot00(obj, 3);
}

void func_80078308_slot00(Object *obj) {
    func_80078458_slot00(obj, (get_random_byte() & 3) | 4);
}
