/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2528_slot04_0b[];

int is_link_flags_zero_slot04_0b(Object *obj) {
    return obj->link_flags == 0;
}

int return_true_always_slot04_0b(Object *obj) {
    return 1;
}

void func_801b1550_slot04_0b(Object *obj) {
    data_801c2528_slot04_0b[obj->field_15a](obj);
}
