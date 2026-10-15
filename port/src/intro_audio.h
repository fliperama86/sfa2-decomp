/* Normal-port sound integration for the complete intro. */
#ifndef PORT_INTRO_AUDIO_HOST_H
#define PORT_INTRO_AUDIO_HOST_H
#include <stddef.h>
int port_intro_audio_prepare(char *,size_t);
void port_intro_audio_start(void);
void port_intro_audio_gain(unsigned);
void port_intro_audio_stop(void);
#endif
