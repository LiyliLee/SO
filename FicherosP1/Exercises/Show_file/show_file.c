#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	//int c,ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	// while ((c = getc(file)) != EOF) {
	// 	/* Print byte to stdout */
	// 	ret=putc((unsigned char) c, stdout);

	// 	if (ret==EOF){
	// 		fclose(file);
	// 		err(3,"putc() failed!!");
	// 	}
	// }

	int count ,numread=0, numwritten=0;
	char buffer[100];
	while (!feof(file))
	{
		count=fread(buffer,sizeof(char),100,file);
		if (ferror(file))
		{
			perror("Read error");
			break;
		}
		numread+=count;
		
		if (!count)
		{
			perror("Write error");
			break;
		}
		else if (count < 100)
		{
			count = fwrite(buffer,sizeof(char),count, stdout);
		}
		else{
			count = fwrite(buffer,sizeof(char),100, stdout);
		}
		
		numwritten+=count;
	}
	printf("\n");
	printf("Read %d item\n",numread);
	printf("Write %d item\n",numwritten);

	fclose(file);
	return 0;
}
