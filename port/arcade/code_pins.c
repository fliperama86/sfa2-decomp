#include "code_pins.h"
#include <string.h>

const struct arcade_code_pin *arcade_code_pin_find(const char *image)
{
    unsigned i;
    if (!image) return NULL;
    for (i = 0; i < arcade_code_pin_count; i++)
        if (!strcmp(arcade_code_pins[i].image, image)) return &arcade_code_pins[i];
    return NULL;
}

int arcade_code_zero(const struct arcade_code_pin *p, unsigned char *bytes, size_t size)
{
    unsigned i;
    if (!p || !bytes || size != p->size) return -1;
    for (i = 0; i < p->count; i++) {
        const struct arcade_code_range *r = &p->ranges[i];
        if (r->offset > size || r->size > size - r->offset) return -1;
        memset(bytes + r->offset, 0, r->size);
    }
    return 0;
}
