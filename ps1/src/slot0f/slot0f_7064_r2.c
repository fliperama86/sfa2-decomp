/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800f7d34_slot0f;
extern u8 data_800f7d4c_slot0f[];
extern Slot12Cell data_800f7e90_slot0f[];
void func_800e5d18_slot0f(Object *obj, Slot12Prim *p, int w, int h);
void func_800e72bc_slot0f(Object *obj);
void func_800e73bc_slot0f(Object *obj);

void func_800e71ec_slot0f(Object *obj) {
    u8 *p;
    int i;
    obj->pos_y = 0;
    obj->field_01 = 0;
    obj->field_09 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->pos_x = 0;
    obj->field_0c = 0;
    obj->pos_x = 0;
    obj->pos_y = 0x100;   /* after the zero stores: this order gives the original's register use */
    obj->field_04++;
    if (obj->field_03 == 5) {
        data_800f7d34_slot0f = 0;
        i = 4;
        p = data_800f7d4c_slot0f;
        do {
            *p = 0;
            p--;
            i--;
        } while (i >= 0);
        if (obj->field_03 == 5) goto tail;
    }
    func_800e5d18_slot0f(obj, (Slot12Prim *)((u8 *)data_800f7e90_slot0f + obj->field_03 * 64), 0x20, 0x20);
tail:
    func_800e72bc_slot0f(obj);
    func_800e73bc_slot0f(obj);
}
