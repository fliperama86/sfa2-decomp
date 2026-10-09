/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1984_slot04_sel(void);
void func_801b1a8c_slot04_sel(Object *obj, Slot04SelRec *rec, int a);

extern TextBuf *data_801b7618_slot04_sel[];
extern TextItem *data_801b749c_slot04_sel[];
extern Object *data_801b7504_slot04_sel[];
extern Object *data_801b6f64_slot04_sel[];
extern u16 data_801b6e7c_slot04_sel[];
extern u8 data_801b7348_slot04_sel[];
extern u8 data_801b7350_slot04_sel[];
extern u8 data_801b9ce8_slot04_sel;
extern u8 data_801b9cec_slot04_sel;

void func_801b10d0_slot04_sel(Object *obj, Slot04SelRec *rec) {
    TextBuf *s3 = data_801b7618_slot04_sel[obj->side];
    TextItem *s2 = data_801b749c_slot04_sel[obj->side];

    if (rec->field_09 == 0) {
        if ((game_state.field_07 >> obj->side) & 1) {
            data_801b9ce8_slot04_sel = rec->field_08;
            data_801b9cec_slot04_sel = rec->field_09;
            func_801b1a8c_slot04_sel(obj, rec, 1);
            rec->field_08 = data_801b9ce8_slot04_sel;
            rec->field_09 = data_801b9cec_slot04_sel;
            s2->field_04 = data_801b6e7c_slot04_sel[obj->side];
            s2->field_06 = s3->field_09 * (data_801b9ce8_slot04_sel + 1) + 0x60;
            func_801519b4(s3);
            func_801519b4(s2);
            func_801519b4(data_801b7504_slot04_sel[obj->side]);
            if (rec->field_09 != 0) {
                data_8016e694[obj->side] = data_801b9ce8_slot04_sel;
                func_801205c4(obj->side, 1);
                s3->buf[data_801b7348_slot04_sel[0]] = 0x1b;
                s3->buf[data_801b7348_slot04_sel[1]] = 0x1b;
                s3->buf[data_801b7348_slot04_sel[data_801b9ce8_slot04_sel]] = data_801b7350_slot04_sel[obj->side];
                obj->field_d8 = data_801b9ce8_slot04_sel;
                if (obj->field_d8 != 0) {
                    obj->field_d4 = (obj->field_d4 & 1) + 4;
                    ref_other.p = obj;
                    func_801b1984_slot04_sel();
                    if (data_8019045c[0] != 0) {
                        obj->field_d4 = obj->field_d4 ^ data_8019045c[0];
                    }
                }
                rec->field_00 = rec->field_00 + 1;
            }
        } else if (((game_state.mode >> obj->side) & 1) == 0) {
            func_801519b4(data_801b6f64_slot04_sel[obj->side]);
        } else {
            func_801519b4(s3);
            func_801519b4(data_801b7504_slot04_sel[obj->side]);
        }
    } else {
        rec->field_00 = 2;
    }
}
