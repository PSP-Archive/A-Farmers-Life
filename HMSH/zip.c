#include "inc/main.h"

static unzFile zipFile = NULL;

int OpenZip(char *zippath)
{
	if(zipFile) CloseZip();
	zipFile = zip_open(zippath);
 	if(zipFile==NULL) {error_callback("Could not open %s", zippath); return 0;}
 		
 	return 1;
}

void CloseZip()
{
	if(zipFile) zip_close(zipFile);
}



Image * imageOpenZipFile(char *zippath, char *fname, ...)
{
	va_list ap;
	char filename[MAX_PATH];

	va_start(ap, fname);
	vsprintf(filename, fname, ap);
	va_end(ap);
	
	debug_printf(1, "Opening %s/%s", zippath, filename);
	
	Image * image = malloc(sizeof(Image));
	if(!image) return NULL;
	
	int zipDirFile;
 	
  long size = zlength(zipFile, filename);
  if(size==0) return NULL;
 	char *buf = malloc(size);
 	if(!buf) return NULL;
 	zipDirFile = zopen(zipFile, filename);
 	if(!zipDirFile) return NULL;
 	if(zread(zipFile, zipDirFile, buf, size)<0) return NULL;
 	zclose(zipFile, zipDirFile);
 	
 	image = loadImageMemory(buf, size);
 	
 	free(buf);
 	
 	return image;
}

Map * mapOpenZipFile(char *zippath, char *filename)
{
	debug_printf(1, "Opening %s/%s", zippath, filename);
	
	Map * map = malloc(sizeof(Map));
	if(!map) return NULL;

	int zipDirFile;

  long size = zlength(zipFile, filename);
  if(size==0) return NULL;
 	char *buf = malloc(size);
 	if(!buf) return NULL;
 	zipDirFile = zopen(zipFile, filename);
 	if(!zipDirFile) return NULL;
 	if(zread(zipFile, zipDirFile, buf, size)<0) return NULL;
 	
 	map = mapProcessFile(buf, size);

 	free(buf);
 	
 	return map;
}

Titlescreen * titlescreenOpenZipFile(char *zippath, char *filename)
{
	debug_printf(1, "Opening %s/%s", zippath, filename);
	
	Titlescreen * title = malloc(sizeof(Titlescreen));
	if(!title) return NULL;

	int zipDirFile;

  long size = zlength(zipFile, filename);
  if(size==0) return NULL;
 	char *buf = malloc(size);
 	if(!buf) return NULL;
 	zipDirFile = zopen(zipFile, filename);
 	if(!zipDirFile) return NULL;
 	if(zread(zipFile, zipDirFile, buf, size)<0) return NULL;

 	title = titlescreenProcessFile(buf, size);

 	free(buf);

 	return title;
}

