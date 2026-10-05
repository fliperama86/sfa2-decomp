/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015c938(int a, u8 *p);

void func_80150638(void) {
    int code;
    int h;
    if ((*(u32 *)&data_80190948 & 0xff00ff) == 0x20002) {
        data_80190948.field_07 = 1;
        code = func_8015c814();
        if (code == 0x1b || code == 0x11) {
            if (func_8015c938(1, &data_80190948.field_0c) == 2 && !(data_80190948.field_0c & 0x10)) {
                if (code == 0x11) {
                    if (data_80190948.field_2c < func_8015cfc8(&data_80190948.field_11)) {
                        h = data_80190948.field_18;
                        code = (h & 0xff) + ((h & 0xff00) >> 5);
                        if (data_8017eb44[code] >= 0) {
                            data_80190948.field_02 = 4;
                        } else {
                            data_80190948.field_02 = 5;
                        }
                    }
                }
                func_8015cb08(0x11, 0);
            }
        }
    }
}
