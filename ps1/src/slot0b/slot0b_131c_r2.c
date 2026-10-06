/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;

extern Slot0bFrame data_801e4c30_slot0b[];
/* Declared without a prototype on purpose. The definition takes (Slot0bObj *,
   u8). With that prototype the call below masks its second argument, one
   instruction more than the original has; with an int parameter in the
   definition the definition grows by one instruction instead. Both were
   tried. Compatible with a caller that saw no prototype; not shown. This
   is about the code that this compiler emits for the two units. It does
   not say that the two declarations are compatible in portable C: a port
   has to give the function one prototype. */
void func_801e1598_slot0b();
void func_801e1624_slot0b(Slot0bObj *obj);

void func_801e13bc_slot0b(Slot0bObj *obj) {
    if (data_80190468.p->field_ab == 1 || obj->anim.loop < 0) {
        obj->field_46 = 0xf;
        obj->field_4c = 0x180000;
        obj->field_50 = 0xfff40000;
        obj->field_58 = 0xc000;
        obj->anim.cur = data_801e4c30_slot0b;
        obj->anim.timer = 1;
        obj->field_54 = 0xfffe8000;
        obj->field_05++;
        obj->anim.loop = obj->anim.cur->loop;
        if (obj->field_03 != 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
            obj->field_50 = -obj->field_50;
            obj->field_58 = -obj->field_58;
        }
        func_801e1598_slot0b(obj, obj->field_03 + 2);
    }
    func_801e1624_slot0b(obj);
}
