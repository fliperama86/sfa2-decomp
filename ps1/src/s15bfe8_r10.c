/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015f020(int a, int b);

int func_8015ce04(void) {
    return func_8015eb28();
}

int func_8015ce24(void) {
    return func_8015e880();
}

int func_8015ce44(int a, int b, int c) {
    int i;

    for (i = 3; i != -1; i--) {
        if (!func_8015e788(b, a, c)) {
            return 1;
        }
    }
    return 0;
}
