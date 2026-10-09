/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot0fRecf32c data_800df32c_slot0f;
extern s8 data_800f8554_slot0f[];
extern s8 data_800f8555_slot0f;
extern s8 data_800f8556_slot0f;
extern s8 data_800f8557_slot0f;
extern s8 data_800f8558_slot0f;
extern u8 data_800f8559_slot0f;
extern u8 data_800f855a_slot0f;
extern u8 data_800f855b_slot0f;
extern u8 data_800f8564_slot0f[2][8];

void func_800e32dc_slot0f(void) {
    Slot0fRecf32c buf = data_800df32c_slot0f;
    int i;
    int j;

    data_8016e685 = *(u8 *)data_800f8554_slot0f;
    game_state.field_11 = *(u8 *)data_800f8554_slot0f;
    if (data_800f8555_slot0f == 4) {
        data_8016e688 = 1;
        game_state.field_13 = 0xff;
        data_8016e687 = buf.b[4];
    } else {
        data_8016e688 = 0;
        data_8016e687 = buf.b[data_800f8555_slot0f];
        game_state.field_13 = data_8016e687;
    }
    ((u8 *)table_8016e664)[0x25] = *(u8 *)&data_800f8556_slot0f;
    game_state.field_15 = *(u8 *)&data_800f8556_slot0f + 1;
    data_8016e68a = *(u8 *)&data_800f8557_slot0f;
    game_state.field_12 = *(u8 *)&data_800f8557_slot0f;
    data_8016e68c = *(u8 *)&data_800f8558_slot0f;
    game_state.field_14 = *(u8 *)&data_800f8558_slot0f;
    data_8016e68b = data_800f8559_slot0f;
    data_8016e68f[0] = data_800f855a_slot0f;
    data_8016e690[0] = data_800f855b_slot0f;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            table_8016e664[i][j] = data_800f8564_slot0f[i][j];
        }
    }
    func_80120554((Object *)0, 0, 0x205);
    func_8014f4d4(6, 2);
}
