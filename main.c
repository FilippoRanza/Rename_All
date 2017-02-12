#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define RESET 0

#define TRUE 1
#define FALSE 0

#define INT_SEQ  10
#define CHAR_SEQ 26
#define HEX_SEQ  16

#define RECURSIVE 1
#define VERBOSE 2


#define MAX_TAIL_LEN 10
#define MAX_NAME_LEN 256
#define MAX_EXT_LEN  64
#define MAX_BASE_LEN 182
#define MAX_PATH_LEN 2048

#define NO_OPT -1

#define SET_BNAME 'b'
#define SET_DIR   'd'
#define SET_EXT   'e'

#define SET_HEX   'h'
#define SET_CHAR  'c'
#define SET_REC   'r'
#define SET_VERB  'v'

typedef struct {
    char *dirname;
    char *bname;
    char *ext;
    int seq;
    int wmode;

} in_opt;

void reverse(char *str);

char *maketail(int mode);

char *makenewname();

char opttype(char *str);

void setopt(int argc,char **argv);

void unknownopterr(char o,char *str);

int filter(char *fname,char *ext);

void iterate();

void renameall(char *dir);


char *prgname;
in_opt opt;

int main(int argc, char **argv){

    if(argc == 1){
        fprintf(stderr,"%s : Automatic  File Rename Program\n",*argv);
        fprintf(stderr,"User must provide at last ONE argument,just to confirm\n");
        fprintf(stderr,"New file name structure : BASE_SEQ<.EXT>\n");
        fprintf(stderr,"BASE : common headr for all renamed files\n");
        fprintf(stderr,"SEQ : unique serial number for each renamed file\n");
        fprintf(stderr,"EXT : optional files extension used as filter\n");
        fprintf(stderr,"By default only file in selected directory are renamed\n");
        fprintf(stderr,"Directories are always ignored\n");
        fprintf(stderr,"Recognized options:\n");
        fprintf(stderr,"\t-b name : set base name,default : \"Rename_\"\n");
        fprintf(stderr,"\t-d dir  : set working directory,default : \".\"\n");
        fprintf(stderr,"\t-e ext  : set file extension for filtering,default : \"\" \n");
        fprintf(stderr,"\t-h : generate hexadecimal sequence\n");
        fprintf(stderr,"\t-c : generate character sequence\n");
        fprintf(stderr,"\t-r : enable recusive mode: search in subdirectory\n");
        fprintf(stderr,"\t-v : enable verbose mode: describe every action\n\n");
        fprintf(stderr,"Examples:\n");
        fprintf(stderr,"%s -b Wien_ -e .jpeg\n",*argv);
        fprintf(stderr,"\twill rename every jpeg file in corrent directory into Wien_###.jpeg\n");
        fprintf(stderr,"\twhere ### is a number from 0 to number of jpeg files\n");

        exit(8);
    }

    prgname = *argv;

    setopt(argc,argv);

    renameall(opt.dirname);

    return 0;
}


void reverse(char *str){

    char *end = str;
    char c;

    while(*end)
        end++;
    end--;

    while(str < end){
        c = *str;
        *str = *end;
        *end = c;
        end--;
        str++;
    }

}

char *maketail(int mode){

    static char out[MAX_TAIL_LEN];
    static int next = 0;

    int count = next;
    char *tmp = out;

    char base = (mode == CHAR_SEQ) ? 'A' : '0';

    do{

        *tmp = (count % mode);
        count /= mode;

        if(mode == HEX_SEQ && (*tmp >= 10))
               *tmp += 'A' - 10;
        else
            *tmp += base;
        tmp++;
    }while(count);

    *tmp = '\0';

    next++;

    reverse(out);

    return out;
}

char *makenewname(){

    static char newname[MAX_NAME_LEN];

    char *tail = maketail(opt.seq);

    strcpy(newname,opt.bname);
    strcat(newname,tail);
    strcat(newname,opt.ext);

    return newname;

}

char opttype(char *str){
    if(*str != '-')
        return NO_OPT;
    str++;
    return *str;
}

void unknownopterr(char o,char *str){
    fprintf(stderr,"%s: unknown option : %c\n",prgname,o);
    fprintf(stderr,"%s: %s will be ignored\n",prgname,str);
}

void setopt(int argc,char **argv){

    opt.bname = "Rename_";
    opt.dirname = malloc(MAX_PATH_LEN);
    if(opt.dirname == NULL)
        exit(4);
    opt.ext = "";
    opt.seq = INT_SEQ;
    opt.wmode = RESET;

    realpath(".",opt.dirname);

    argc--;
    argv++;

    char prev = NO_OPT;

    while(argc--){

        char *arg = *argv++;

        char val = opttype(arg);

        if(val == NO_OPT){
            switch(prev){
            case SET_BNAME:
                opt.bname = arg;
                break;
            case SET_DIR:
                realpath(arg,opt.dirname);
                break;
            case SET_EXT:
                if(*arg == '.'){
                    opt.ext = arg;
                }
                else{
                    static char texp[MAX_EXT_LEN];
                    char *tmp = texp;
                    *tmp++ = '.';
                    strcpy(tmp,arg);
                    opt.ext = texp;
                }
                break;
            default:
                unknownopterr(prev,arg);
            }
            prev = NO_OPT;
        }
        else{
            switch(val){
            case SET_HEX:
                opt.seq = HEX_SEQ;
                break;
            case SET_CHAR:
                opt.seq = CHAR_SEQ;
                break;
            case SET_VERB:
                opt.wmode |= VERBOSE;
                break;
            case SET_REC:
                opt.wmode |= RECURSIVE;
                break;
            default:
                prev = val;
            }

        }

    }

}

int filter(char *fname,char *ext){

    char *endf = fname;
    char *ende = ext;

    while(*endf)
        endf++;

    while(*ende)
        ende++;

    while(*endf == *ende){

        if(ende == ext)
            return TRUE;
        if(endf == fname)
            return FALSE;

        endf--;
        ende--;

    }

    return FALSE;
}


void renameall(char *dir){

    chdir(dir);

    struct dirent *entry;
    DIR *dirptr = opendir(".");
    struct stat buff;

    if(dirptr == NULL)
        exit(1);


    while((entry = readdir(dirptr)) != NULL){
        if(*entry->d_name == '.')
            continue;

        if((stat(entry->d_name,&buff)) == -1){

            exit(2);
        }

        if(!S_ISDIR(buff.st_mode)){

            if(filter(entry->d_name,opt.ext)){

                    char *name = makenewname();
                    if(opt.wmode & VERBOSE)
                        printf("%s : rename %s to %s\n",prgname,entry->d_name,name);

                    rename(entry->d_name,name);

            }

        }
        else if(opt.wmode & RECURSIVE){

            if(opt.wmode & VERBOSE)
                printf("%s : recursion on %s\n",prgname,entry->d_name);

            char *recname = malloc(MAX_PATH_LEN);

            if(recname == NULL)
                exit(4);

            realpath(entry->d_name,recname);
            renameall(recname);
            free(recname);
            chdir(dir);

        }
    }

}







