/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"

void func_8013b0c4();
void func_8013afa4();
void func_8013b004();
void func_8013b064();

void func_8013af1c(a, b, c)
int a, b, c;
{
    if (data_80188ed0.in_00 < data_80188ed0.in_04) {
        if (data_80188ed0.in_00 < data_80188ed0.in_08) {
            func_8013afa4(a, b, c);
        } else {
            func_8013b064(a, b, c);
        }
    } else if (data_80188ed0.in_08 < data_80188ed0.in_04) {
        func_8013b064(a, b, c);
    } else {
        func_8013b004(a, b, c);
    }
}

void func_8013afa4(a, b, c)
int a, b, c;
{
    data_80188ed0.out_3c = data_80188ed0.f_0c;
    data_80188ed0.out_40 = data_80188ed0.f_24;
    data_80188ed0.out_44 = data_80188ed0.f_10;
    data_80188ed0.out_48 = data_80188ed0.f_28;
    func_8013b0c4(a, b, c);
}

void func_8013b004(a, b, c)
int a, b, c;
{
    data_80188ed0.out_3c = data_80188ed0.f_14;
    data_80188ed0.out_40 = data_80188ed0.f_2c;
    data_80188ed0.out_44 = data_80188ed0.f_18;
    data_80188ed0.out_48 = data_80188ed0.f_30;
    func_8013b0c4(a, b, c);
}

void func_8013b064(a, b, c)
int a, b, c;
{
    data_80188ed0.out_3c = data_80188ed0.f_1c;
    data_80188ed0.out_40 = data_80188ed0.f_34;
    data_80188ed0.out_44 = data_80188ed0.f_20;
    data_80188ed0.out_48 = data_80188ed0.f_38;
    func_8013b0c4(a, b, c);
}
