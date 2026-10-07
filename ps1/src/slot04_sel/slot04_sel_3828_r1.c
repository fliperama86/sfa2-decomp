/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e698[];
extern u16 data_801b7d98_slot04_sel[];
extern u8 data_801b7d9c_slot04_sel[][8];
extern u8 data_801b7ddc_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b8570_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void func_801205c4(int a, unsigned b);
void func_801b4774_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);

void func_801b3828_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *a = data_801b8540_slot04_sel[obj->side];
    TextBuf *b = data_801b8570_slot04_sel[obj->side];
    TextBuf *c = data_801b85e8_slot04_sel[obj->side];
    int i;
    if (rec->field_07 == 0 && ((game_state.field_07 >> obj->side) & 1)) {
        data_801b9d30_slot04_sel = rec->field_06;
        data_801b9d34_slot04_sel = rec->field_07;
        func_801b4774_slot04_sel(obj, rec, 7);
        rec->field_06 = data_801b9d30_slot04_sel;
        rec->field_07 = data_801b9d34_slot04_sel;
        b->field_04 = data_801b7d98_slot04_sel[obj->side] + a->field_08 * data_801b9d30_slot04_sel;
        b->field_06 = 0x48;
        func_801519b4((Object *)b);
        for (i = 0; i < 8; i++) {
            a->buf[i + 5] = data_801b7d9c_slot04_sel[data_801b9d30_slot04_sel][i];
        }
        func_801519b4((Object *)a);
        func_801519b4((Object *)c);
        if (rec->field_07 != 0) {
            obj->field_cf = data_801b7ddc_slot04_sel[data_801b9d30_slot04_sel];
            data_8016e698[obj->side] = data_801b9d30_slot04_sel;
            func_801205c4(obj->side, 1);
            for (i = 0; i < 8; i++) {
                a->buf[i + 5] = data_801b7d9c_slot04_sel[data_801b9d30_slot04_sel][i];
            }
            a->buf[4] = data_801b8328_slot04_sel[obj->side];
            rec->field_00++;
        }
    }
}
