/* The registry of the library layer: which tables of host routines and which
 * overrides this program has. A test replaces this file with its own. */
#include "port.h"

const struct port_domain port_domains[] = {
    { "kernel", port_kernel_library },
    { "threads", port_thread_library },
    { "system", port_system_library },
    { "c", port_c_library },
    { "sound", port_sound_library },
    { "card", port_card_library },
    { "cd", port_cd_library },
};
const unsigned port_domain_count = sizeof port_domains / sizeof port_domains[0];

const struct port_override *const port_override_sets[] = { port_game_overrides };
const unsigned port_override_set_count = 1;
