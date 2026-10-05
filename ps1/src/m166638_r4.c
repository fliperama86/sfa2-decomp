/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80168908(short a, short b, FrameRecordW *out) {
    if (data_801a8994[a] == 1) {
    func_80164db4(a, b);
    out->field_00 = data_80197f04[b].field_00;
    out->field_01 = data_80197f04[b].field_01;
    out->field_02 = data_80197f04[b].field_02;
    out->field_03 = data_80197f04[b].field_03;
    out->field_04 = data_80197f04[b].field_04;
    out->field_06 = data_80197f04[b].field_06;
    return 0;
    }
    return -1;
}
