/* Project-authored INI subset and atomic save. No assets, GPU or game dependency. */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "settings.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define OPEN_NEW(p) _open(p,_O_WRONLY|_O_CREAT|_O_EXCL|_O_BINARY,_S_IREAD|_S_IWRITE)
#define FDOPEN _fdopen
#define CLOSE _close
#define UNLINK _unlink
#else
#include <unistd.h>
#define OPEN_NEW(p) open(p,O_WRONLY|O_CREAT|O_EXCL,0600)
#define FDOPEN fdopen
#define CLOSE close
#define UNLINK unlink
#endif
struct port_settings port_settings;
/* The defaults, by the owner's decision of 2026-10-23: every setting on but the arcade intro, which needs a pack that the player makes,
 * and the arcade assets (arcade_assets, later that day), which need the player's arcade set and are work in progress.
 * The graphics settings need PsyZ's patched backend: a build without PsyZ (the controls, which open no window) keeps them off, so that
 * its defaults never ask for what it cannot do. The build tools compile this file with PORT_HAVE_PSYZ when they link PsyZ. */
#ifdef PORT_HAVE_PSYZ
#define GRAPHICS_DEFAULT 1
#else
#define GRAPHICS_DEFAULT 0
#endif
void port_settings_defaults(struct port_settings *out) { out->pixel_perfect_ui=GRAPHICS_DEFAULT;out->arcade_video=GRAPHICS_DEFAULT;out->center_ui=GRAPHICS_DEFAULT;out->fast_loads=1;out->memory_card=1;out->sharp_bilinear=GRAPHICS_DEFAULT;out->arcade_intro=0;out->arcade_assets=0; }
static int failure(char *err,size_t size,unsigned line,const char *why)
{
    if(err && size) {
        if(line) snprintf(err,size,"settings line %u: %s",line,why);
        else snprintf(err,size,"settings: %s",why);
    }
    return -1;
}
static char *trim(char *s)
{
    char *end;
    while(isspace((unsigned char)*s))s++;
    end=s+strlen(s);while(end>s && isspace((unsigned char)end[-1]))*--end=0;
    return s;
}
static int line_read(char *text,unsigned row,int *section,unsigned *seen,
                     struct port_settings *next,char *err,size_t size)
{
    char *s=text,*comment,*equal,*key,*value;int bit,b;
    if(row==1 && strlen(s)>=3 && (unsigned char)s[0]==0xef &&
       (unsigned char)s[1]==0xbb && (unsigned char)s[2]==0xbf)s+=3;
    comment=strpbrk(s,"#;");if(comment)*comment=0;
    s=trim(s);if(!*s)return 0;
    if(*s=='[') {
        if(strcmp(s,"[experimental]"))return failure(err,size,row,"unknown section");
        *section=1;return 0;
    }
    if(!*section)return failure(err,size,row,"key outside [experimental]");
    equal=strchr(s,'=');if(!equal)return failure(err,size,row,"expected key = true/false");
    *equal=0;key=trim(s);value=trim(equal+1);
    if(!strcmp(key,"pixel_perfect_ui"))bit=1;
    else if(!strcmp(key,"arcade_video"))bit=2;
    else if(!strcmp(key,"center_ui"))bit=4;
    else if(!strcmp(key,"fast_loads"))bit=8;
    else if(!strcmp(key,"memory_card"))bit=16;
    else if(!strcmp(key,"sharp_bilinear"))bit=32;
    else if(!strcmp(key,"arcade_intro"))bit=64;
    else if(!strcmp(key,"arcade_assets"))bit=128;
    else return failure(err,size,row,"unknown key");
    if(*seen&(unsigned)bit)return failure(err,size,row,"duplicate key");
    if(!strcmp(value,"true"))b=1;
    else if(!strcmp(value,"false"))b=0;
    else return failure(err,size,row,"expected true or false");
    *seen|=(unsigned)bit;
    if(bit==1)next->pixel_perfect_ui=b;else if(bit==2)next->arcade_video=b;else if(bit==4)next->center_ui=b;else if(bit==8)next->fast_loads=b;else if(bit==16)next->memory_card=b;else if(bit==32)next->sharp_bilinear=b;else if(bit==64)next->arcade_intro=b;else next->arcade_assets=b;
    return 0;
}
int port_settings_read(FILE *file,struct port_settings *out,char *err,size_t size)
{
    char line[512];size_t n=0;unsigned row=1,seen=0;int ch,section=0;
    struct port_settings next;port_settings_defaults(&next);
    while((ch=fgetc(file))!=EOF) {
        if(ch==0)return failure(err,size,row,"NUL byte in text");
        if(ch=='\n') {
            line[n]=0;if(line_read(line,row,&section,&seen,&next,err,size))return -1;
            n=0;row++;
        } else {
            if(n+1>=sizeof line)return failure(err,size,row,"line too long");
            line[n++]=(char)ch;
        }
    }
    if(ferror(file))return failure(err,size,row,"read failed");
    if(n) { line[n]=0;if(line_read(line,row,&section,&seen,&next,err,size))return -1; }
    *out=next;return 0;
}
int port_settings_load(const char *path,struct port_settings *out,int missing_ok,char *err,size_t size)
{
    struct port_settings next;FILE *file=fopen(path,"rb");int rc;
    if(!file) {
        if(missing_ok && errno==ENOENT) { port_settings_defaults(out);return 0; }
        return failure(err,size,0,"cannot open file");
    }
    rc=port_settings_read(file,&next,err,size);
    if(fclose(file) && !rc)rc=failure(err,size,0,"close failed");
    if(!rc)*out=next;
    return rc;
}
int port_settings_save(const char *path,const struct port_settings *value,char *err,size_t size)
{
    char *tmp;size_t n=strlen(path);int fd,bad=0;FILE *file;
    if((value->pixel_perfect_ui!=0 && value->pixel_perfect_ui!=1) ||
       (value->arcade_video!=0 && value->arcade_video!=1) ||
       (value->center_ui!=0 && value->center_ui!=1) ||
       (value->fast_loads!=0 && value->fast_loads!=1) ||
       (value->memory_card!=0 && value->memory_card!=1) ||
       (value->sharp_bilinear!=0 && value->sharp_bilinear!=1) ||
       (value->arcade_intro!=0 && value->arcade_intro!=1) ||
       (value->arcade_assets!=0 && value->arcade_assets!=1))return failure(err,size,0,"invalid value for save");
    tmp=malloc(n+5);if(!tmp)return failure(err,size,0,"out of memory");
    memcpy(tmp,path,n);memcpy(tmp+n,".tmp",5);
    fd=OPEN_NEW(tmp);
    if(fd<0) { free(tmp);return failure(err,size,0,"cannot create exclusive temporary file"); }
    file=FDOPEN(fd,"wb");
    if(!file) { CLOSE(fd);UNLINK(tmp);free(tmp);return failure(err,size,0,"cannot open temporary stream"); }
    if(fprintf(file,"[experimental]\npixel_perfect_ui = %s\narcade_video = %s\ncenter_ui = %s\nfast_loads = %s\nmemory_card = %s\nsharp_bilinear = %s\narcade_intro = %s\narcade_assets = %s\n",
       value->pixel_perfect_ui?"true":"false",value->arcade_video?"true":"false",value->center_ui?"true":"false",
       value->fast_loads?"true":"false",value->memory_card?"true":"false",value->sharp_bilinear?"true":"false",value->arcade_intro?"true":"false",value->arcade_assets?"true":"false")<0)bad=1;
    if(fclose(file))bad=1;
    if(!bad) {
#ifdef _WIN32
        if(!MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))bad=1;
#else
        if(rename(tmp,path))bad=1;
#endif
    }
    if(bad)UNLINK(tmp);
    free(tmp);return bad?failure(err,size,0,"save failed; prior file kept") : 0;
}
