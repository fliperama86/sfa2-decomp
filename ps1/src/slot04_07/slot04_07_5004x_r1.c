/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5118_slot04_07(Object *obj);
void func_8011ffdc(Object *o);

void func_801b5004_slot04_07(Object *obj) {
    int t;
    int c;

    func_801b5118_slot04_07(obj);
    c = 0xfc0000;
    if (*(s32 *)&obj->field_60 >= c) {
        *(s32 *)&obj->field_60 = c;
        obj->field_45 = obj->field_45 | 1;
    }
    c = 0xd80000;
    if (*(s32 *)&obj->field_70 >= c) {
        *(s32 *)&obj->field_70 = c;
        obj->field_45 = obj->field_45 | 2;
    }
    if (obj->field_45 == 3) {
        obj->field_04 = obj->field_04 + 1;
    }
    c = game_state.field_1d + game_state.field_24;
    t = c;
    if ((t & 1) != 0) {
        if ((obj->field_45 & 1) == 0) {
            obj->field_80 = 1;
            *(s32 *)&obj->sequence = *(s32 *)&obj->field_20;
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_5c;
            *(s32 *)&obj->field_14 = *(s32 *)&obj->field_60;
            func_8011ffdc(obj);
        }
    } else if ((obj->field_45 & 2) == 0) {
        obj->field_80 = 1;
        *(s32 *)&obj->sequence = *(s32 *)&obj->field_68;
        *(s32 *)&obj->field_10 = *(s32 *)&obj->box_tables;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_70;
        func_8011ffdc(obj);
    }
}
