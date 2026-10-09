/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e69a[];
extern u16 data_801b7de4_slot04_sel[];
extern u8 data_801b8320_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void func_801205c4(int a, unsigned b);
void func_801b45f4_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);
void func_801b44ec_slot04_sel(void);

void func_801b3a7c_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *a = data_801b8540_slot04_sel[obj->side];
    TextBuf *b = data_801b85e8_slot04_sel[obj->side];
    TextBuf *c = data_801b8474_slot04_sel[obj->side];
    if (rec->field_09 == 0 && ((game_state.field_07 >> obj->side) & 1)) {
        data_801b9d30_slot04_sel = rec->field_08;
        data_801b9d34_slot04_sel = rec->field_09;
        func_801b45f4_slot04_sel(obj, rec, 1);
        rec->field_08 = data_801b9d30_slot04_sel;
        rec->field_09 = data_801b9d34_slot04_sel;
        c->field_04 = data_801b7de4_slot04_sel[obj->side];
        c->field_06 = b->field_09 * (data_801b9d30_slot04_sel + 1) + 0x60;
        func_801519b4((Object *)a);
        func_801519b4((Object *)b);
        func_801519b4((Object *)c);
        if (rec->field_09 != 0) {
            data_8016e69a[obj->side] = data_801b9d30_slot04_sel;
            func_801205c4(obj->side, 1);
            b->buf[data_801b8320_slot04_sel[0]] = 0x1b;
            b->buf[data_801b8320_slot04_sel[1]] = 0x1b;
            b->buf[data_801b8320_slot04_sel[data_801b9d30_slot04_sel]] = data_801b8328_slot04_sel[obj->side];
            obj->field_d8 = data_801b9d30_slot04_sel;
            if (obj->field_d8 != 0) {
                obj->field_d4 = (obj->field_d4 & 1) + 4;
                ref_other.p = obj;
                func_801b44ec_slot04_sel();
                if (data_8019045c[0] != 0) {
                    obj->field_d4 = obj->field_d4 ^ data_8019045c[0];
                }
            }
            rec->field_00++;
        }
    }
}
