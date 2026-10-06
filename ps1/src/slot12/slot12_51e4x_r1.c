/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002bc78_slot12;
extern u8 data_8002bc90_slot12[];
extern Slot12Cell data_8002bdd4_slot12[];
void func_80014434_slot12(Object *obj, Slot12Prim *p, int w, int h);
void func_800152b4_slot12(Object *obj);
void func_80015334_slot12(Object *obj);

void func_800151e4_slot12(Object *obj) {
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
        data_8002bc78_slot12 = 0;
        i = 4;
        p = data_8002bc90_slot12;
        do {
            *p = 0;
            p--;
            i--;
        } while (i >= 0);
        if (obj->field_03 == 5) goto tail;
    }
    func_80014434_slot12(obj, (Slot12Prim *)((u8 *)data_8002bdd4_slot12 + obj->field_03 * 64), 0x20, 0x20);
tail:
    func_800152b4_slot12(obj);
    func_80015334_slot12(obj);
}
