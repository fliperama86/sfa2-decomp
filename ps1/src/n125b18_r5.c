/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"

/* Own externs instead of externs.h: the original loads this byte signed (lb),
   externs.h declares it u8. */
extern s8 data_801a6989;
extern void (*table_8017193c[])(Object *);

u8 func_8014e0b4(Object *object);
void func_8014e3d0(Object *object);
void func_8013060c(Object *object);
void func_8012dd5c(Object *object);
void func_80129678(Object *object);
void func_801299d0(Object *object);

void func_80129468(Object *object) {
    if (game_state.field_30 != 0 && game_state.mode == object->side + 1) {
        if (data_801a6989 == 0 && object->field_165 == 0 && object->field_06 != 8 && object->field_7e == 0 && (s16)object->field_c6 < 0x30) {
            object->field_c6 = (s16)object->field_c6 + 1;
        }
        if (data_801a6989 == 1 && object->field_165 == 0 && object->field_06 != 8 && object->field_7e == 0 && (s16)object->field_c6 < 0x60) {
            object->field_c6 = (s16)object->field_c6 + 1;
        }
        if (data_801a6989 == 2 && object->field_165 == 0 && object->field_06 != 8 && object->field_7e == 0 && (s16)object->field_c6 < 0x90) {
            object->field_c6 = (s16)object->field_c6 + 1;
        }
    }
    if (object->field_cd != 0) {
        if (func_8014e0b4(object)) {
            func_8014e3d0(object);
        }
    } else if (game_state.field_30 == 0 || game_state.mode == object->side + 1) {
        func_8013060c(object);
        func_8012dd5c(object);
    }
    func_80129678(object);
    func_801299d0(object);
    table_8017193c[object->field_05](object);
}
