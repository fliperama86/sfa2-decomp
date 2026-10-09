/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b8dd8_slot04_sel[])(Object *, Slot04SelRec9d78 *);
extern Slot04SelRec8eac data_801b8eac_slot04_sel;
extern Slot04SelRec8eac data_801b8edc_slot04_sel;
extern Slot04SelRec8eac data_801b8f00_slot04_sel;
extern Slot04SelRec8eac data_801b8f30_slot04_sel;
extern u8 data_801b8c91_slot04_sel;
extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];
extern HudState *data_8018f5a0;

/* The local w holds the first player and then the second record. Written with one local for each, this function differs from the original in 7 instruction slots. */
void func_801b5954_slot04_sel(void) {
    Slot04SelRec9d78 *r;
    Object *second;
    void *w;

    func_801519b4((Object *)&data_801b8eac_slot04_sel);
    func_801519b4((Object *)&data_801b8edc_slot04_sel);
    func_801519b4((Object *)&data_801b8f00_slot04_sel);
    func_801519b4((Object *)&data_801b8f30_slot04_sel);
    w = &player_left;
    r = data_801b9d78_slot04_sel;
    data_801b8dd8_slot04_sel[r->field_00](w, r);
    second = (Object *)w + 1;
    w = r + 1;
    data_801b8dd8_slot04_sel[r[1].field_00](second, w);
    if (game_state.field_07 != 0) {
        if (r[0].field_0e != 0 || r[1].field_0e != 0) {
            if ((u8)(data_801b8c91_slot04_sel - 4) < 2 || data_801b8c91_slot04_sel == 6) {
                r[0].field_0e = 0;
                r[1].field_0e = 0;
            } else if (data_801b8c91_slot04_sel == 3) {
                r[0].field_00 = 0;
                r[1].field_00 = 0;
                r[0].field_04 = 0;
                r[1].field_04 = 0;
                r[0].field_0e = 0;
                r[1].field_0e = 0;
                data_8018f5a0->field_50--;
            } else {
                r[0].field_00 = 0;
                r[1].field_00 = 0;
                data_8018f5a0->field_4e++;
                data_8018f5a0->field_50 = 0;
            }
        }
    }
}
