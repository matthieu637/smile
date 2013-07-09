#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	//printf("%c", argv[0]);
	//printf("%s\n", argv[1]);
	//printf("%f\n", atof(argv[1]));
	usleep( atof(argv[1]) );
}


