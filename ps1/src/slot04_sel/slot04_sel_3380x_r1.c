/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern TextBuf data_801b7ef4_slot04_sel;
extern Slot04SelRec7ff4 *data_801b7ff4_slot04_sel[];
extern TextBuf data_801b84b4_slot04_sel;
extern TextBuf data_801b84cc_slot04_sel;
extern Slot04SelRec8978 data_801b89d4_slot04_sel[];
extern s16 data_801b89d8_slot04_sel;
extern s16 data_801b89da_slot04_sel;
extern TextBuf data_801b84e8_slot04_sel;
extern TextBuf data_801b8618_slot04_sel;
extern void (*data_801b7d7c_slot04_sel[])(Object *, Slot04SelRec9d38 *);
extern u16 *data_801b7d88_slot04_sel[];
extern u8 data_801b7d90_slot04_sel[];
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern u8 data_801b9d68_slot04_sel;
extern u8 data_801b9d6c_slot04_sel;
extern HudState *data_8018f5a0;
void func_801b44ec_slot04_sel(void);

/* The first read of the second player's side goes through the pointer to the first player. Written player_right.side, as the two later reads are, this function differs from the original in 20 instruction slots. */
void func_801b3380_slot04_sel(void) {
    Object *l = &player_left;
    Object *r;
    Slot04SelRec9d38 *a = data_801b9d38_slot04_sel;
    Slot04SelRec9d38 *b;
    u8 s;
    int t;
    int m;

    func_801519b4(&data_801b7ef4_slot04_sel);
    func_801519b4(data_801b7ff4_slot04_sel[player_left.side]);
    func_801519b4(data_801b7ff4_slot04_sel[l[1].side]);
    func_801519b4(&data_801b84b4_slot04_sel);
    func_801519b4(&data_801b84cc_slot04_sel);
    data_801b89d8_slot04_sel = 0xa8;
    data_801b89da_slot04_sel = 0x30;
    func_801519b4(data_801b89d4_slot04_sel);
    func_801519b4(&data_801b84e8_slot04_sel);
    func_801519b4(&data_801b8618_slot04_sel);
    data_801b7d7c_slot04_sel[a->field_00](l, a);
    r = l + 1;
    b = a + 1;
    data_801b7d7c_slot04_sel[b->field_00](r, b);
    s = player_left.side;
    if ((*data_801b7d88_slot04_sel[s] & 0x800) || (*data_801b7d88_slot04_sel[player_right.side] & 0x800)) {
        t = player_right.side;
        player_left.field_cf = data_801b7d90_slot04_sel[data_8016e698[s]];
        player_right.field_cf = data_801b7d90_slot04_sel[data_8016e698[t]];
        player_left.field_d8 = data_8016e69a[s];
        player_right.field_d8 = data_8016e69a[t];
        if (data_8016e69c != 0) {
            game_state.field_56 = (s8)data_8016e68c;
        } else {
            game_state.field_56 = 0;
        }
        if (data_8016e69d[0] == 0) {
            game_state.field_40 = game_state.field_32 % 20;
        } else {
            game_state.field_40 = data_8016e69d[0] - 1;
        }
        if (a->field_09 == 0 && l->field_d8 != 0) {
            l->field_d4 = (l->field_d4 & 1) + 4;
            ref_other.p = l;
            func_801b44ec_slot04_sel();
            if (data_8019045c[0] != 0) {
                l->field_d4 = l->field_d4 ^ *(u8 *)data_8019045c;
            }
        }
        if (b->field_09 == 0 && r->field_d8 != 0) {
            r->field_d4 = (r->field_d4 & 1) + 4;
            ref_other.p = r;
            func_801b44ec_slot04_sel();
            if (data_8019045c[0] != 0) {
                r->field_d4 = r->field_d4 ^ *(u8 *)data_8019045c;
            }
        }
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_4e++;
    }
    m = game_state.field_07;
    if (m == (a->field_09 | b->field_09) && m != 0) {
        if (a->field_0b != 0 || b->field_0b != 0) {
            a->field_00 = 0;
            b->field_00 = 0;
            data_8018f5a0->field_50++;
        }
    }
    if (data_801b9d68_slot04_sel != game_state.field_07 || data_801b9d6c_slot04_sel != game_state.mode) {
        a->field_00 = 0;
        b->field_00 = 0;
        data_8018f5a0->field_50 = 0;
    }
}
