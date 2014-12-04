#include <stdio.h>
#include "shell.h"


void printHelp() {
   
 printf("Here are some executable command for the floppy console\n");
    printf("  fmount: mounts the floppy image.\n");
    printf("    Example:    `fmount <image>`\n");
    printf("  fumount: Unmounts a the floppy image.\n");
    printf("    Example:    `fumount`\n");
    printf("  structure: Lists the structure of the floppy disk.\n");
    printf("    Example:    `structure [-l]`\n");
    printf("  traverse: contents within root directory\n");
    printf("    Example:     `traverse [-l]`\n");
    printf("  showfat: FAT tables.\n");
    printf("    Example:    `showfat`\n");
    printf("  showsector: Shows the contents of a sector.\n");
    printf("    Example:    `showsector <number>`\n");
    printf("  showfile: Show the contents of a file.\n");
    printf("    Example:    `showfile <file>`\n");
    printf("  help: shows commands.\n");
    printf("    Example:    `help`\n");
    printf("  exit: Exits the flop program.\n");
    printf("    Example:    `exit`\n");
}

void printInvalid() {
    printf("    Not a Valid command please. Use `help` for a list of all commands.\n");
}


void printIncorrect() {
    printf("    Wrong use. Try `help` for assistance.\n");
}
