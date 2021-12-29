#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char magic[8];
	char dummy;
} pokeball_header;

typedef struct {
	char name[128];
	long size;
} __attribute__((packed)) pokeball_entry;

int main(int argc, char **argv) {
	FILE *fptr;
	char *fname = argv[argc-1];
	char *data = (char*)malloc(sizeof(pokeball_header));
	int sz;

	printf("PokeBall Archive Utility v1.0 by NDRAEY (Andrey Pavlenko)\n\n");

	if(argc<2) {printf("Usage: pokeball-unpack archive\n\n");return 1;}
	
	printf("Chosen file: %s\n",fname);
	fptr = fopen(fname,"rb");
	fseek(fptr, 0L, SEEK_END);
	sz = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	
	printf("Opening...\n");
	if(!fptr) {
		printf("Error (fptr is null)\n");
		return 1;
	}
	fread(data,sizeof(pokeball_header),1,fptr);
	pokeball_header* hdr = (pokeball_header*)data;
	
	if(strncmp(hdr->magic,"POKEBALL",8)!=0) {
		printf("Invalid Pokéball archive!\n");
		fclose(fptr);
		return 1;
	}

	FILE *newfile;
	while(1){
		char *newdata = (char*)malloc(sizeof(pokeball_entry));
		if(!newdata){
			printf("malloc() for newdata failed!!!\n");
			fclose(fptr);
			return 1;
		}
		fread(newdata,sizeof(pokeball_entry),1,fptr);
		pokeball_entry *real = (pokeball_entry*)newdata;
		char *cont = (char*)malloc(real->size);
		fread(cont,real->size+4,1,fptr); // Why 4?

		newfile = fopen(real->name, "w");
		printf("Name: %s\n",real->name);
		printf("Size: %ld\n",(real->size+4));
		for(int k=0; k<real->size+4; k++){
			fputc(*cont++,newfile);
		}
		fclose(newfile);
		free(newdata);
		free(cont);
		if(ftell(fptr)>=sz) {break;}
	}
	
	fclose(fptr);
	free(hdr);
	return 0;
}
