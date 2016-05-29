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


int main(int size, char * arguments[]) {

	char * ptr;
	int bytes;
	struct stat status;
	mode_t mode;
		
	if (size != 3){
            
        	fprintf(stderr, "Es müssen 3 Argumente übergeben werden: copy Quelle Ziel \n");
        	return EXIT_FAILURE;
	}
    
        
        int fdin = open(arguments[1], O_RDONLY); //versuch die  Quelle zu Öffnen
        
	if(fdin < 0){
            
		fprintf(stderr, "Kann datei nich öffnen: %s \n", arguments[1]);
		return EXIT_FAILURE;
	}

		// read access modes
        
	if(fstat(fdin, &status) < 0) {
		perror("fstat: ");
	        exit(EXIT_FAILURE);
	}
	
	mode = status.st_mode & ~S_IFMT;

	
	int fdout = open(arguments[2], O_WRONLY| O_CREAT| O_EXCL, 0);
	if(fdout < 0 && errno == EEXIST){
		fprintf(stderr, "Datei %s exestiert bereits, überschreiben(y/n)?", arguments[2]);
		char ch;
		if((ch = getchar()) == 'y') {
			if ((fdout = open(arguments[2], O_WRONLY | O_TRUNC)) == -1) {
				fprintf(stderr, "Fehler, falsche Datei: %s Grund: %s \n", arguments[2], strerror(errno));
				exit(EXIT_FAILURE);
			}
		} else {
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Fehler, falsche Datei: %s Grund: %s \n", arguments[2], strerror(errno));
		exit(EXIT_FAILURE);
	}



	ptr = (char *)malloc(BLKSIZE);
	if(ptr == NULL) {
	        perror("malloc: ");
	        exit(EXIT_FAILURE);
   	}
        
	  
	do {
	    bytes = read( fdin, ptr, BLKSIZE);
	    if(bytes < 0) {
	      perror("read: ");
	      exit(EXIT_FAILURE);
        }
	    bytes = write( fdout, ptr, bytes);
	    
        if(bytes < 0) {
	      perror("write: ");
	      exit(EXIT_FAILURE);
        }

	} while (bytes == BLKSIZE);
	
	if(fchmod(fdout, mode) < 0) 
		perror("cannot fchmod destination:");
	
	free(ptr);
	assert(close(fdin)>=0);
	assert(close(fdout)>=0);

    return (EXIT_SUCCESS);
}

