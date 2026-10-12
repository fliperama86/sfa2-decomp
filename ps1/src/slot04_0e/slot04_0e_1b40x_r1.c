/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Functions of the same module, declared here because the published headers
   lack them. Inferred from the code; not original declarations. */
u8 func_801b6a7c_slot04_0e(Object *obj);
void func_801b6afc_slot04_0e(Object *obj);

/* What it does (inferred, not an original name): enters a character
   action. It sets the state byte state_3 to 3 and field_159 to 1, then
   func_80130504 derives the direction pair field_12a, field_129 from the
   input word field_134. When func_801b6a7c_slot04_0e reports a condition
   (field_129 not 0 and field_12a equal to 2, and then either field_219 or
   bit 14 of field_130 depending on field_cd), func_801b6afc_slot04_0e
   runs. Otherwise the counter function func_80141f28 gets half = field_12a / 2
   and sequence 12 + half (field_48 equal to 0) or 18 + half (otherwise) is
   selected, 3 more when field_129 is not 0. One local, a, carries three
   values in turn (half, then 12 or 18, then the sequence number); that is
   inferred from the build. */
void func_801b1b40_slot04_0e(Object *obj) {
    /* Both locals are 16 bits wide: as int, a differs in 14 instruction slots (172 bytes) and half in 7 (184 bytes). */
    u16 a;
    u16 half;

    obj->state_3 = 3;
    obj->field_159 = 1;
    func_80130504(obj);
    if (func_801b6a7c_slot04_0e(obj) != 0) {
        func_801b6afc_slot04_0e(obj);
    } else {
        a = obj->field_12a >> 1;
        half = a;
        func_80141f28(obj, a);
        a = 0x12;
        if (obj->field_48 == 0) {
            a = 0xc;
        }
        if (obj->field_129 == 0) {
            a = a + half;
        } else {
            /* Written in two steps; as `a = a + 3 + half;` this function differs from the original in 2 instruction slots. */
            a += 3;
            a += half;
        }
        func_801307e0(obj, a);
    }
}
