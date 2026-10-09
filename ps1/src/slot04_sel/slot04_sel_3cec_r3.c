/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e69d[];
extern u16 data_801b7e60_slot04_sel[];
extern u16 data_801b7e8c_slot04_sel[];
extern u8 data_801b7df4_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void func_801b48f4_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, int kind);

void func_801b408c_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *s1 = data_801b8474_slot04_sel[obj->side];
    u8 *p;
    if (rec->field_0d == 0) {
        if (((game_state.field_07 >> obj->side) & 1) != 0) {
            data_801b9d34_slot04_sel = rec->field_0d;
            func_801b48f4_slot04_sel(obj, rec, 0x14);
            rec->field_0c = data_801b7df4_slot04_sel[rec->field_0c * 5 + data_801b9d30_slot04_sel];
            data_801b9d30_slot04_sel = rec->field_0c;
            rec->field_0d = data_801b9d34_slot04_sel;
            s1->field_04 = data_801b7e60_slot04_sel[data_801b9d30_slot04_sel] + obj->side * 0x48;
            s1->field_06 = data_801b7e8c_slot04_sel[data_801b9d30_slot04_sel];
            func_801519b4(s1);
            if (rec->field_0d != 0) {
                p = data_8016e69d;
                *p = data_801b9d30_slot04_sel;
                func_801205c4(obj->side, 1);
                if (*p == 0) {
                    game_state.field_40 = (u8)(game_state.field_32 % 20);
                } else {
                    game_state.field_40 = *p - 1;
                }
            }
        }
    } else {
        func_801519b4(s1);
    }
}
