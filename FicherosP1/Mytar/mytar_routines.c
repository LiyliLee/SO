#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes)
{
	// Complete the function
	int nBytesRead=0;
	int c ,ret;
	if (origin==NULL ||destination==NULL) return -1;

	while(((c=getc(origin))!=EOF) && nBytesRead!=nBytes){
		ret = putc(c,destination);
		nBytesRead++;
		if(ret ==EOF)	return -1;

	}

	return nBytesRead;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file)
{
	// Complete the function
	char* buffer =NULL;
	int sizeofFile=0;
	char c ;

	if (file == NULL)	return NULL;

	while ((c=getc(file)!=EOF)) sizeofFile++;
	if(sizeofFile>0){
		sizeofFile++; //para '\0'

		buffer = malloc(sizeof(char)*(sizeofFile ));

		fseek(file, -sizeofFile, SEEK_CUR);

		int leido = fread(buffer,sizeof(char),sizeofFile,file);
		if (leido ==0)  return NULL;
	}

	return buffer;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles)
{

	char* buffer;
	// Complete the function
	stHeaderEntry* array= NULL;
	int nr_files=0;
	if(tarFile==NULL) return NULL;

	if (fread(&nr_files, sizeof(int),1,tarFile)==0) return NULL;

	array= malloc(sizeof(stHeaderEntry)*nr_files);

	for (int i =0; i< nr_files;i++)
	{
		buffer= loadstr(tarFile);
		if (buffer==NULL) return NULL;

		array[i].name=malloc(strlen(buffer));
		strcpy(	array[i].name, buffer);

		int leido = fread(&array[i].size,sizeof(int),1,tarFile);
		if (leido ==0) return NULL;

	}

	(*nFiles) =nr_files;
	return (array);
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[])
{
	// Complete the function
	FILE * tarFile = NULL;
	FILE * inputFile =NULL;
	stHeaderEntry* array = NULL;
	int i=0;
	int nBytes=0;
	int fileByte =0;
	if ( (tarFile=fopen(tarName,"w"))  == NULL)	return EXIT_FAILURE;

	array= malloc(sizeof(stHeaderEntry)*nFiles);

	//int del numero de fichero + nFiles * int con el numero de bytes de cada fichero
	nBytes= sizeof(int) +nFiles*sizeof(unsigned int);
	//+nombre de cada fichero + '\0'
	for (i=0; i<nFiles;i++)
	{
		nBytes+= strlen(fileNames[i])+1;
	}
	//copiar datos primero
	fseek(tarFile,nBytes,SEEK_SET);
	//copiar archivos
	for (i =0;i<nFiles;i++)
	{
		if((inputFile = fopen(fileNames[i],"r") )== NULL) return EXIT_FAILURE;
		if ((fileByte= copynFile(inputFile,tarFile,INT_MAX)) == -1) return EXIT_FAILURE;
		array[i].size=fileByte;
		array[i].name= malloc(strlen(fileNames[i])+1);
		strcpy(array[i].name,fileNames[i]);
		fclose(inputFile);
	}

	//una vez teniendo todos los datos, completamos informaciones 
	fseek(tarFile,0L,SEEK_SET); //ir al principio para rellenar lo que hemos saltado
	fwrite(&nFiles,sizeof(int),1,tarFile);
	for (i=0; i<nFiles;i++)
	{
		if (fwrite(array[i].name,strlen(fileNames[i])+1, 1, tarFile) ==0) return EXIT_FAILURE; 
		if (fwrite(&array[i].size,sizeof(unsigned int), 1, tarFile) ==0) return EXIT_FAILURE; 

	}

	

	//eliminamos la memoria reservado en stHeaderEntry* array 
	for (i=0; i<nFiles;i++){
		free(array[i].name);	}
	free (array);

	fclose(tarFile);
	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[])
{
	// Complete the function
	FILE * tarFile = NULL;
	FILE* outFile= NULL;
	if((tarFile = fopen(tarName,"r") )== NULL) return EXIT_FAILURE;

	int nFiles=0;
	int i;
	int filebytes=0;
	
	//obtener informaciones
	stHeaderEntry* array = readHeader(tarFile,&nFiles);

	if (array==NULL) 
	{
		printf("1");
		return EXIT_FAILURE;
	}

	for (i=0; i<nFiles;i++)
	{
		outFile= fopen( array[i].name,"w");
		if(outFile==NULL) return EXIT_FAILURE;

		filebytes= copynFile(tarFile,outFile,array[i].size);
		printf("%d",filebytes);
		if (filebytes != array[i].size) return EXIT_FAILURE;

		fclose(outFile);

	}

	fclose(tarFile);

	return EXIT_SUCCESS;

}
