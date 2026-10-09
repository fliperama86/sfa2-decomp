/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern TextBuf *data_801b7618_slot04_sel[];
extern TextItem *data_801b749c_slot04_sel[];
extern Object *data_801b7504_slot04_sel[];
extern Object *data_801b6f64_slot04_sel[];
extern u16 data_801b6e80_slot04_sel[];
extern u8 data_801b734c_slot04_sel[];
extern u8 data_801b7350_slot04_sel[];
extern Slot04SelRec7648 data_801b7648_slot04_sel;
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;
void func_801b1a8c_slot04_sel(Object *obj, Slot04SelRec *r, int c);

void func_801b13b8_slot04_sel(Object *obj, Slot04SelRec *r) {
    TextBuf *s3 = data_801b7618_slot04_sel[obj->side];
    TextItem *s2 = data_801b749c_slot04_sel[obj->side];

    if (r->field_0b == 0) {
        if (((game_state.field_07 >> obj->side) & 1) != 0) {
            data_801b9ce8_slot04_sel = r->field_0a;
            data_801b9cec_slot04_sel = r->field_0b;
            func_801b1a8c_slot04_sel(obj, r, 1);
            r->field_0a = data_801b9ce8_slot04_sel;
            r->field_0b = data_801b9cec_slot04_sel;
            s2->field_04 = data_801b6e80_slot04_sel[obj->side];
            s2->field_06 = data_801b7648_slot04_sel.field_09 * (data_801b9ce8_slot04_sel + 1) + 0x90;
            func_801519b4(s3);
            func_801519b4(s2);
            func_801519b4(data_801b7504_slot04_sel[obj->side]);
            if (r->field_0b != 0) {
                data_8016e696 = data_801b9ce8_slot04_sel;
                func_801205c4(obj->side, 1);
                data_801b7648_slot04_sel.field_0c[data_801b734c_slot04_sel[0]] = 0x1b;
                data_801b7648_slot04_sel.field_0c[data_801b734c_slot04_sel[1]] = 0x1b;
                data_801b7648_slot04_sel.field_0c[data_801b734c_slot04_sel[data_801b9ce8_slot04_sel]] = data_801b7350_slot04_sel[obj->side];
                if (data_801b9ce8_slot04_sel != 0) {
                    game_state.field_56 = (s8)data_8016e68c;
                } else {
                    game_state.field_56 = 0;
                }
            }
        } else if (((game_state.mode >> obj->side) & 1) == 0) {
            func_801519b4(data_801b6f64_slot04_sel[obj->side]);
        } else {
            goto both;
        }
    } else {
both:
        func_801519b4(s3);
        func_801519b4(data_801b7504_slot04_sel[obj->side]);
    }
}
