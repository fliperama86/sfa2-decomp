/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_801e46b4_slot0b[];
void func_801e06b0_slot0b(ModObj *obj);
void func_801e0a78_slot0b(ModObj *obj, void *a, int b, int c);

void func_801e0840_slot0b(ModObj *obj) {
}

void func_801e0848_slot0b(Object *o) {
    Object *p = ((ModObj *)o)->field_3c;
    ModObj *obj = (ModObj *)o;
    if (data_801ae02c != 0) {
        func_801e06b0_slot0b(obj);
    } else {
        func_801e0a78_slot0b(obj, data_801e46b4_slot0b[ref_other.p->side], p->side, data_801a27d0);
    }
}

void func_801e08b8_slot0b(Object *obj) {
    func_8011f240();
}
