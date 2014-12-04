#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boot.h"
#include "Fat12Boot.h"
#include "Fat12Entry.h"

//global variables
FILE *floppyimg = NULL;
int mounted = 0;
struct Fat12Boot fb;
char *fn = '\0';
struct Fat12Entry *info;

/* FAT12 entries into info point */
void load() {

fseek(floppyimg, 11, SEEK_SET); //skip 11 bytes

    //BYTES_PER_SECTOR (2 bytes)
    fread(&fb.BYTES_PER_SECTOR, 2, 1, floppyimg);

    //SECTORS_PER_CLUSTER (2 bytes)
    fread(&fb.SECTORS_PER_CLUSTER, 1, 1, floppyimg);

    //RESERVED_SECTORS (2 bytes)
    fread(&fb.RESERVED_SECTORS, 2, 1, floppyimg);

    //NUM_OF_FATS (1 byte)
    fread(&fb.NUM_OF_FATS, 1, 1, floppyimg);

    //MAX_ROOT_DIRS (2 bytes)
    fread(&fb.MAX_ROOT_DIRS, 2, 1, floppyimg);

    //Initialize 'entry'
    info = (struct Fat12Entry *)malloc(sizeof(struct Fat12Entry) * fb.MAX_ROOT_DIRS);

    //TOTAL_SECTORS (2 bytes)
    fread(&fb.TOTAL_SECTORS, 2, 1, floppyimg);

    fseek(floppyimg, 1, SEEK_CUR); //skip 1 byte

    //SECTORS_PER_FAT (2 bytes)
    fread(&fb.SECTORS_PER_FAT, 2, 1, floppyimg);

    //SECTORS_PER_TRACK (2 bytes)
    fread(&fb.SECTORS_PER_TRACK, 2, 1, floppyimg);

    //NUM_OF_HEADS (2 bytes)
    fread(&fb.NUM_OF_HEADS, 2, 1, floppyimg);

    fseek(floppyimg, 11, SEEK_CUR); //skip 11 bytes

    //VOUME_ID (4 bytes)
    fread(&fb.VOLUME_ID, 4, 1, floppyimg);

    //VOLUME_LABEL
    fread(&fb.VOLUME_LABEL, 11, 1, floppyimg);

    //Move to beginning of ROOT DIRECTORY
    fseek(floppyimg, ((fb.NUM_OF_FATS * fb.SECTORS_PER_FAT) + 1) * fb.BYTES_PER_SECTOR, SEEK_SET);

    for (int i = 0; i < fb.MAX_ROOT_DIRS; i++) {
        //FILENAME (8 bytes)
        fread(&info[i].FILENAME, 8, 1, floppyimg);

        //EXT (3 bytes)
        fread(&info[i].EXT, 3, 1, floppyimg);

        //ATTRIBUTES (1 byte)
        fread(&info[i].ATTRIBUTES, 1, 1, floppyimg);

        //RESERVED (10 bytes)
        fread(&info[i].RESERVED, 2, 1, floppyimg);

        //CREATION_TIME (2 bytes)
        fread(&info[i].CREATION_TIME, 2, 1, floppyimg);

        //CREATION_DATE (2 bytes)
        fread(&info[i].CREATION_DATE, 2, 1, floppyimg);

        //LAST_ACCESS_DATE (2 bytes)
        fread(&info[i].LAST_ACCESS_DATE, 2, 1, floppyimg);

        fseek(floppyimg, 2, SEEK_CUR); //skip 2 bytes

        //MODIFY_TIME (2 bytes)
        fread(&info[i].MODIFY_TIME, 2, 1, floppyimg);

        //MODIFY_DATE (2 bytes)
        fread(&info[i].MODIFY_DATE, 2, 1, floppyimg);

        //START_CLUSTER (2 bytes)
        fread(&info[i].START_CLUSTER, 2, 1, floppyimg);

        //FILE_SIZE (4 bytes)
        fread(&info[i].FILE_SIZE, 4, 1, floppyimg);
    }
}

/*
*   Clears file information and closes the file */
void unmount() {
printf("    Unmounting the floppy img for you `%s`... ", fn);
fn = '\0';
fclose(floppyimg);
mounted = 0;
unload();
printf("Done!\n");
}

/*
*   Clears 'fb'*/
void unload() {
    fb = (struct Fat12Boot){0};
    info = (struct Fat12Entry *){0};
}

/*
*   Saves file information and opens the file
*/
void mount(char *file) {
printf("    The floppy image was properly mounted good job! `%s`... ", file);
fn = (char *)malloc(strlen(file) + 1);
strcpy(fn, file);
floppyimg = fopen(file, "r");
mounted = 1;
load();
printf("Done!\n");
}

/*
*   Prints FAT12 image structure
*/
void structure(int l) {
printf("        number of FAT:                      %d\n", fb.NUM_OF_FATS);
printf("        number of sectors used by FAT:      %d\n", fb.SECTORS_PER_FAT);
printf("        number of sectors per cluster:      %d\n", fb.SECTORS_PER_CLUSTER);
printf("        number of ROOT Entries:             %d\n", fb.MAX_ROOT_DIRS);
printf("        number of bytes per sector          %d\n", fb.BYTES_PER_SECTOR);
if (l) {
printf("        ---Sector #---      ---Sector Types---\n");
printf("             0                    BOOT\n");

for(int i = 0; i < fb.NUM_OF_FATS; i++)
	printf("          %02d -- %02d                FAT%d\n", (fb.SECTORS_PER_FAT * i) + 1, fb.SECTORS_PER_FAT * (i + 1), i);

printf("          %02d -- %02d                ROOT DIRECTORY\n", fb.SECTORS_PER_FAT * fb.NUM_OF_FATS, (fb.MAX_ROOT_DIRS / 16) + (fb.SECTORS_PER_FAT * fb.NUM_OF_FATS)); 
}
}

/* files in the FAT12 image */
void traverse(int l) {
    
if (l) {
printf("    *****************************\n");
printf("    ** FILE ATTRIBUTE NOTATION **\n");
printf("    **                         **\n");
printf("    ** R ------ READ ONLY FILE **\n");
printf("    ** S ------ SYSTEM FILE    **\n");
printf("    ** H ------ HIDDEN FILE    **\n");
printf("    ** A ------ ARCHIVE FILE   **\n");
printf("    *****************************\n");
printf("\n");

for (int i = 0; i < fb.MAX_ROOT_DIRS; i++) {
if (info[i].FILENAME[0] != 0x00 && info[i].START_CLUSTER != 0) {
char att[6] = {'-', '-', '-', '-', '-'};
unsigned char a = info[i].ATTRIBUTES[0];
if (a == 0x01) {
att[0] = 'R';
}
if (a == 0x02) {
att[1] = 'H';
}
if (a == 0x04){
att[2] = 'S';
}
if (a == 0x20) {
att[5] = 'A';
}
if (a == 0x10) {
for (int j = 0; j < 6; j++)
att[j] = '-';
}

if (info[i].ATTRIBUTES[0] == 0x10) {
printf("%.6s    %d %d       < DIR >      /%.8s                 %d\n", att, info[i].CREATION_DATE, info[i].CREATION_TIME, info[i].FILENAME, info[i].START_CLUSTER);
printf("%.6s    %d %d       < DIR >      /%.8s/.                 %d\n", att, info[i].CREATION_DATE, info[i].CREATION_TIME, info[i].FILENAME, info[i].START_CLUSTER);
printf("%.6s    %d %d       < DIR >      /%.8s/..                 %d\n", att, info[i].CREATION_DATE, info[i].CREATION_TIME, info[i].FILENAME, 0);
} else {
printf("%.6s    %d %d       %lu      /%.8s.%.3s                 %d\n", att, info[i].CREATION_DATE, info[i].CREATION_TIME, info[i].FILE_SIZE, info[i].FILENAME, info[i].EXT, info[i].START_CLUSTER);
}
}
}

} else {
for (int i = 0; i < fb.MAX_ROOT_DIRS; i++) {
if (info[i].FILENAME[0] != 0x00 && info[i].START_CLUSTER != 0) {
if (info[i].ATTRIBUTES[0] == 0x10) {
printf("/%.8s                       < DIR >\n", info[i].FILENAME);
printf("/%.8s/.                     < DIR >\n", info[i].FILENAME);
printf("/%.8s/..                    < DIR >\n", info[i].FILENAME);
} else {
printf("/%.8s.%.3s\n", info[i].FILENAME, info[i].EXT);
}
}
}
}
}

/*hex dump of FAT tables */
void showfat() {
unsigned char in;

fseek(floppyimg, fb.BYTES_PER_SECTOR, SEEK_SET);

printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
for (int i = 0; i < (fb.NUM_OF_FATS * fb.SECTORS_PER_FAT * fb.BYTES_PER_SECTOR); i++) {
if (i % 16 == 0 || i == 0) {
    printf("\n");
    printf("%4x", i);
}
fread(&in, 1, 1, floppyimg);
printf("%5x", in);
}

printf("\n");
}

/*
*   Outputs a hex dump of a specific sector (512 bytes)
*/
void showsector(int sector) {
    unsigned char in;

    fseek(floppyimg, fb.BYTES_PER_SECTOR * sector, SEEK_SET);

    printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
    for (int i = 0; i < fb.BYTES_PER_SECTOR; i++) {
        if (i % 16 == 0 || i == 0) {
            printf("\n");
            printf("%4x", i);
        }
        fread(&in, 1, 1, floppyimg);
        printf("%5x", in);
    }

    printf("\n");
}

/*
*   get the name of the file */

char *getFile() {
    return fn;
}

/*
*   Outputs a hex dump of a specific file
*/
void showfile(char *file) {
    char *fn;
	char *ext;
	char *full_fn;
	char *p;
	
	//Parse fn
	p = strtok(file, ".");
	fn = p;
	
	//Parse extension
	p = strtok(NULL, ".");
	ext = p;
	
	full_fn = (char *)malloc(strlen(fn) + strlen(ext) + 1);
	strcpy(full_fn, fn);
	strcat(full_fn, ext);
	
	printf("%s\n", full_fn);
	
for (int k = 0; k < fb.MAX_ROOT_DIRS; k++) {
if (info[k].FILENAME != 0x00 && info[k].START_CLUSTER != 0) {
if(equals(full_fn, (char *)info[k].FILENAME)) {
				
unsigned char in;
				
fseek(floppyimg, ((fb.MAX_ROOT_DIRS / 16) + (fb.SECTORS_PER_FAT * fb.NUM_OF_FATS) - 1) + (fb.BYTES_PER_SECTOR * info[k].START_CLUSTER), SEEK_SET);

printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
				for (int j = 0; j < info[k].FILE_SIZE; j++) {
					if (j % 16 == 0 || j == 0) {
						printf("\n");
						printf("%4x", j);
					}
					fread(&in, 1, 1, floppyimg);
					printf("%5x", in);
				}

				printf("\n");
			}
		}
	}
}

/* Comparing characters for filenames*/
int equals(char * str1, char *str2) {
	for (int i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i])
			return 0;
	}
return 1;
}

// return 1 for mounted

int isMounted() {
return mounted;
}

