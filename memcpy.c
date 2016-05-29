/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: user
 *
 * Created on May 29, 2016, 3:32 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>



int main(int anzahl, char **arguments){

    int src_file, dest_file, strvgl, mode, fsize; //Speicher für die Pointer der Files
    char *src, *dest;
    struct stat status;
    size_t filesize;
    
    strvgl = strcmp(arguments[1], "copy");
    
    if(strvgl != 0){
        
        fprintf(stderr, "argument[1] ist nicht 'copy'");
        return (EXIT_FAILURE);
    }
    
    stat(arguments[1], &status);
    
    mode = status.st_mode;
    printf("Mode:  %d", mode);
    fsize = status.st_size;
    
    /* SOURCE */
    src_file = open(arguments[2], O_RDONLY); //Pfad angabe zum lesen mit Rad only
    
    filesize = lseek(src_file, 0, SEEK_END); // Sucht bis zum /0 und speichert in filesize die größe der Datei

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, src_file, 0); // PROT_READ = Quelle wird nur gelesen, Flag, MAP_MRIVATE = gib an 
    //ob der Bereich von anderen Prozessen gelesen werden darf(hier nicht eine memory copy entsteht) | Pointer in int Zeigt auf Adresse der quelle
    // *addr = NULL Kernel wählt Speicheradresse | off = o also kein offset 
    // in src steht die adresse des files im speicher
    

    
    /* DESTINATION */    
    dest_file = open(arguments[3], O_RDWR | O_CREAT, mode); //int  open(  char  *filename,  int  access,  int  permission  );
    // gibt adresse als int zurück

    ftruncate(dest_file, filesize); // ftruncate() setzt größe der Datei auf vorher ausgelesene filesize

    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dest_file, 0);

    /* COPY */

    memcpy(dest, src, filesize); // memcpy kopiert die in filesize angegebene menge an Bytes von dest nach src

    munmap(src, filesize); //mmap() aufräumen
    munmap(dest, filesize);

    close(src_file);        //open() aufräumen)
    close(dest_file);

    return 0;
}