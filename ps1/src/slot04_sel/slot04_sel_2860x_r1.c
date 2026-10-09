/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b7c6c_slot04_sel[])(Object *, Slot04SelRec9d38 *);
extern Slot04SelRec800c data_801b800c_slot04_sel;
extern Slot04SelRec800c data_801b8030_slot04_sel;
extern Slot04SelRec800c data_801b8054_slot04_sel;
extern Slot04SelRec800c data_801b8078_slot04_sel;
extern Slot04SelRec800c data_801b809c_slot04_sel;
extern Slot04SelRec800c data_801b80c0_slot04_sel;
extern Slot04SelRec800c data_801b80e4_slot04_sel;
extern Slot04SelRec800c data_801b8108_slot04_sel;
extern Slot04SelRec800c data_801b812c_slot04_sel;
extern Slot04SelRec800c data_801b8150_slot04_sel;
extern Slot04SelRec800c data_801b8174_slot04_sel;
extern Slot04SelRec800c data_801b8198_slot04_sel;
extern Slot04SelRec800c data_801b81bc_slot04_sel;
extern Slot04SelRec800c data_801b81e0_slot04_sel;
extern Slot04SelRec800c data_801b8204_slot04_sel;
extern Slot04SelRec800c data_801b8228_slot04_sel;
extern Slot04SelRec800c data_801b824c_slot04_sel;
extern Slot04SelRec800c data_801b8270_slot04_sel;
extern Slot04SelRec800c data_801b833c_slot04_sel;
extern Slot04SelRec8978 data_801b89d4_slot04_sel[];
extern s16 data_801b89d8_slot04_sel;
extern s16 data_801b89da_slot04_sel;
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern u8 data_801b9d68_slot04_sel;
extern u8 data_801b9d6c_slot04_sel;

/* The local w holds the first player and then the second record. Written with one local for each, this function differs from the original in 2 instruction slots. */
void func_801b2860_slot04_sel(void) {
    Slot04SelRec9d38 *r;
    Object *second;
    void *w;
    func_801519b4(&data_801b833c_slot04_sel);
    data_801b89d8_slot04_sel = 0xa8;
    data_801b89da_slot04_sel = 0x30;
    func_801519b4(data_801b89d4_slot04_sel);
    func_801519b4(&data_801b800c_slot04_sel);
    func_801519b4(&data_801b8030_slot04_sel);
    func_801519b4(&data_801b8054_slot04_sel);
    func_801519b4(&data_801b8078_slot04_sel);
    func_801519b4(&data_801b809c_slot04_sel);
    func_801519b4(&data_801b80c0_slot04_sel);
    func_801519b4(&data_801b80e4_slot04_sel);
    func_801519b4(&data_801b8108_slot04_sel);
    func_801519b4(&data_801b812c_slot04_sel);
    func_801519b4(&data_801b8150_slot04_sel);
    func_801519b4(&data_801b8174_slot04_sel);
    func_801519b4(&data_801b8198_slot04_sel);
    func_801519b4(&data_801b81bc_slot04_sel);
    func_801519b4(&data_801b81e0_slot04_sel);
    func_801519b4(&data_801b8204_slot04_sel);
    func_801519b4(&data_801b8228_slot04_sel);
    func_801519b4(&data_801b824c_slot04_sel);
    func_801519b4(&data_801b8270_slot04_sel);
    w = &player_left;
    r = data_801b9d38_slot04_sel;
    data_801b7c6c_slot04_sel[r->field_00](w, r);
    second = (Object *)w + 1;
    w = r + 1;
    data_801b7c6c_slot04_sel[r[1].field_00](second, w);
    if (game_state.field_07 == (data_801b9d38_slot04_sel[0].field_04 | data_801b9d38_slot04_sel[1].field_04)) {
        r[0].field_00 = 0;
        r[1].field_00 = 0;
        data_8018f5a0->field_50++;
    }
    data_801b9d68_slot04_sel = game_state.field_07;
    data_801b9d6c_slot04_sel = game_state.mode;
}
