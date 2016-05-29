#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#define	BLKSIZE		4096


int main(int amount, char * arguments[]) {

	char * ptr;
        char * cpy = "copy";
	int bytes;
	struct stat status;
	mode_t mode;
        FILE *dest_file;
        FILE *src_file;
        
        
        
		
	if (amount != 4){ // Anzahl der Argumente überprüfen 3+1 wegen Netbeans Pfadangabe
            
        	fprintf(stderr, "Es müssen 3 Argumente übergeben werden: copy Quelle Ziel \n"); //Ausgabe erfolgt mit stderr = Standard Fehlerausgabe
        	return EXIT_FAILURE;
	}
        
        
        /*   Auf copy Argumenbt überprüfen
        if(strcmp(arguments[1], cpy) != 0 ){
            
            fprintf(stderr, "Parameter 2 muss 'copy' heißen!");
            
        }*/
        
        src_file = fopen(arguments[2], "r"); //versuch die  Quelle als Read Only zu Öffnen *Check open()*
        
	if(src_file < 0){ //wenn Datei nicht zu öffnen (=0) wird Fehler ausgegeben
            
		fprintf(stderr, "Datei existiert nicht oder hat keine Leserechte auf Datei: %s \n", arguments[2]);
		return EXIT_FAILURE;
	}

		// read access modes ??????????????????????????
        /*
	if(fstat(file, &status) < 0) {
		perror("fstat: ");
	        exit(EXIT_FAILURE);
	}*/
	
	mode = status.st_mode & ~S_IFMT; // Auslesen der Quelldateirechte

	
	dest_file = fopen(arguments[3], "w"); //Datei wird erstellt, falls vorhanden, dann überschrieben
        
        //chmod mit mode
        chmod(arguments[3], mode); //Rechts Setzen wie bei Quelldatei
        
	if(dest_file < 0 && errno == EEXIST){ //überprüfung ob Datei bereits existiert
            
		fprintf(stderr, "Datei %s exestiert bereits, soll sie überschrieben werden(y/n)?", arguments[3]);
                
		char answer;
                
		if((answer = getchar()) == 'y') { //wurde mit y betsätigt?
                    
			//Copy File /home/user/Downloads/Test1.txt
                        
		} 
                else {
			exit(EXIT_FAILURE);
		}
                
	} else {
		fprintf(stderr, "Fehler, falsche Datei: %s Grund: %s \n", arguments[3], strerror(errno));
		exit(EXIT_FAILURE);
	}
        
        

/*

	ptr = (char *)malloc(BLKSIZE);
	if(ptr == NULL) {
	        perror("malloc: ");
	        exit(EXIT_FAILURE);
   	}
        
	  
	do {
	    bytes = read( src_file, ptr, BLKSIZE);
	    if(bytes < 0) {
	      perror("read: ");
	      exit(EXIT_FAILURE);
        }
	    bytes = write( dest_file, ptr, bytes);
	    
        if(bytes < 0) {
	      perror("write: ");
	      exit(EXIT_FAILURE);
        }

	} while (bytes == BLKSIZE);
	
	if(fchmod(dest_file, mode) < 0) 
		perror("Fehler bei: fchmod Ziel:"); //keine Rechte
	
	free(ptr);*/
//	assert(close(src_file)>=0);
//	assert(close(dest_file)>=0);

        fclose(dest_file);
        fclose(src_file);
        
    return EXIT_SUCCESS;
}

