/* Project-authored typed settings API, shared by startup and a future UI. */
#ifndef PORT_SETTINGS_H
#define PORT_SETTINGS_H
#include <stddef.h>
#include <stdio.h>
struct port_settings { int pixel_perfect_ui, arcade_video, center_ui, fast_loads; };
extern struct port_settings port_settings;
void port_settings_defaults(struct port_settings *out);
int port_settings_read(FILE *file,struct port_settings *out,char *err,size_t size);
int port_settings_load(const char *path,struct port_settings *out,int missing_ok,char *err,size_t size);
int port_settings_save(const char *path,const struct port_settings *value,char *err,size_t size);
#endif
