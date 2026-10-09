/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800eff9c_slot0f[])(Object *);
extern Slot12Prim data_800f7cf4_slot0f[];
extern SequenceStep *data_800eff98_slot0f;
void func_800e5d18_slot0f(Object *obj, Slot12Prim *c, int x, int y);
void func_800e5be4_slot0f(Object *obj, Slot12Sprite *cell);

void func_800e6f2c_slot0f(Object *obj) {
    data_800eff9c_slot0f[obj->field_04](obj);
}

void func_800e6f6c_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    obj->pos_x = 0xc0;
    obj->field_16 = 0x80;
    o->field_09 = 5;
    o->field_01 = 0;
    o->field_04++;
    func_80130768(o, 0, &data_800eff98_slot0f);
    func_800e5d18_slot0f(o, data_800f7cf4_slot0f, 0x100, 0xc0);
}

void func_800e6fe0_slot0f(Object *obj) {
    func_800e5be4_slot0f(obj, (Slot12Sprite *)(&data_800f7cf4_slot0f[data_801a27d0]));
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
}

void func_800e7044_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
