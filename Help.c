/* Kopierprogramm */
/* ist Zieldatei bereits vorhanden, wird abgefragt, ob sie überschrieben 
   werden soll
*/

// gcc -Wall -g -o name name.c

#define	BLKSIZE		4096
#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

int main (int argc, char ** argv)
{
	char * ptr;
	int fdin, fdout, bytes;
	long long counter = 0;
	char ch;	
	struct stat status;
	mode_t mode;
	 
	// Check # args	
	if (argc != 3) {
		fprintf(stderr, "   usage: copy source dest \n");
		exit(EXIT_FAILURE);
	}
	
	// open source	 
	if ((fdin = open (argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "   Error! Wrong file: %s Reason: %s \n", 
                    argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	// read access modes
	if(fstat(fdin, &status) < 0) {
		perror("fstat: ");
	        exit(EXIT_FAILURE);
	}
	
	mode = status.st_mode & ~S_IFMT;                                        
	
	// create destination  
    if ((fdout = open (argv[2], O_CREAT | O_WRONLY | O_EXCL, 0)) < 0) {
       	if (errno == EEXIST) {
    		fprintf(stderr, "Destination %s already exists, overwrite (y/n)?", 
                    argv[2]);
    		if ((ch = getchar()) == 'y') {
    			if ((fdout = open (argv[2], O_WRONLY | O_TRUNC)) == -1) {
    				fprintf(stderr, "   Error! Wrong file: %s Reason: %s \n", 
                        argv[2], strerror(errno));
					exit(EXIT_FAILURE);
				}  
			}  else { 
				exit(EXIT_SUCCESS);
			}	
		} else {
			fprintf(stderr, "   Error! Wrong file: %s Reason: %s \n", 
                argv[2], strerror(errno));
			exit(EXIT_FAILURE);
		}
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
	counter += bytes;
	    bytes = write( fdout, ptr, bytes);
	    
        if(bytes < 0) {
	      perror("write: ");
	      exit(EXIT_FAILURE);
        }

	} while (bytes == BLKSIZE);
	
	printf("%lld", counter);

	if(fchmod(fdout, mode) < 0) 
		perror("cannot fchmod destination:");
	
	free(ptr);
	assert(close(fdin)>=0);
	assert(close(fdout)>=0);
	return EXIT_SUCCESS;   			
}
