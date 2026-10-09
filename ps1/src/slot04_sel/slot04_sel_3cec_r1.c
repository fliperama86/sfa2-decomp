/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8016e68c;
extern u8 data_8016e69c;
extern u16 data_801b7de4_slot04_sel[];
extern u8 data_801b8324_slot04_sel[];
extern u8 data_801b8328_slot04_sel[];
extern TextBuf *data_801b8474_slot04_sel[];
extern TextBuf *data_801b8540_slot04_sel[];
extern TextBuf *data_801b85e8_slot04_sel[];
extern TextBuf data_801b8618_slot04_sel;
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
void func_801b45f4_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, u16 kind);

void func_801b3cec_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *s3 = data_801b8540_slot04_sel[obj->side];
    TextBuf *s4 = data_801b85e8_slot04_sel[obj->side];
    TextBuf *s1 = data_801b8474_slot04_sel[obj->side];
    if (rec->field_0b == 0) {
        if (((game_state.field_07 >> obj->side) & 1) == 0) {
            return;
        }
        data_801b9d30_slot04_sel = rec->field_0a;
        data_801b9d34_slot04_sel = rec->field_0b;
        func_801b45f4_slot04_sel(obj, rec, 1);
        rec->field_0a = data_801b9d30_slot04_sel;
        rec->field_0b = data_801b9d34_slot04_sel;
        s1->field_04 = data_801b7de4_slot04_sel[obj->side + 2];
        s1->field_06 = data_801b8618_slot04_sel.field_09 * (data_801b9d30_slot04_sel + 1) + 0x90;
        func_801519b4((Object *)s3);
        func_801519b4((Object *)s4);
        func_801519b4((Object *)s1);
        if (rec->field_0b == 0) {
            return;
        }
        data_8016e69c = data_801b9d30_slot04_sel;
        func_801205c4(obj->side, 1);
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[0]] = 0x1b;
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[1]] = 0x1b;
        data_801b8618_slot04_sel.buf[data_801b8324_slot04_sel[data_801b9d30_slot04_sel]] = data_801b8328_slot04_sel[obj->side];
        if (data_801b9d30_slot04_sel != 0) {
            game_state.field_56 = (s8)data_8016e68c;
        } else {
            game_state.field_56 = 0;
        }
    } else {
        func_801519b4((Object *)s3);
        func_801519b4((Object *)s4);
    }
}
