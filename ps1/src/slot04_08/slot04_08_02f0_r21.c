/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4618_slot04_08(Object *obj);
void func_801b48d8_slot04_08(Object *obj);
void func_801b4658_slot04_08(Object *obj);
void func_801b4790_slot04_08(Object *obj);
extern ObjectFn data_801c3de4_slot04_08[];

void func_801b45d8_slot04_08(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b4618_slot04_08(obj);
    } else {
        func_801b48d8_slot04_08(obj);
    }
}

void func_801b4618_slot04_08(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b4658_slot04_08(obj);
    } else {
        func_801b4790_slot04_08(obj);
    }
}

void func_801b4658_slot04_08(Object *obj) {
    data_801c3de4_slot04_08[obj->field_07](obj);
}
