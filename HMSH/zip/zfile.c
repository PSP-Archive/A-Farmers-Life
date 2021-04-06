#include "../inc/main.h"


static char basedir[MAX_PATH];
char *basedirend;

unzFile zip_open(const char *path)
{
	unzFile unzfile = NULL;
	
	if ((unzfile = unzOpen(path)) != NULL)
		return unzfile;

	strcpy(basedir, path);

	basedirend = basedir + strlen(basedir);
	*basedirend++ = '/';

	return NULL;
}


int zip_close(unzFile unzfile)
{
	if (unzfile)
	{
		unzClose(unzfile);
		unzfile = NULL;
		return 1;
	}
	return 0;
}


int zopen(unzFile unzfile, const char *filename)
{
	if (unzfile == NULL)
	{
		FILE *fp;

		strcpy(basedirend, filename);
		return (fp = fopen(basedir, "rb")) ? (int)fp : -1;
	}

	if (unzLocateFile(unzfile, filename, 0) == UNZ_OK)
		if (unzOpenCurrentFile(unzfile) == UNZ_OK)
			return (int)unzfile;

	return 0;
}


int zread(unzFile unzfile, int fp, void *buf, unsigned size)
{
	if (unzfile == NULL)
		return fread(buf, 1, size, (FILE *)fp);

	return unzReadCurrentFile(unzfile, buf, size);
}

int zclose(unzFile unzfile, int fp)
{
	if (unzfile == NULL)
	{
		if (fp != -1) fclose((FILE *)fp);
		return 0;
	}
	return unzCloseCurrentFile(unzfile);
}


static int zsize(unzFile unzfile, int fd)
{
	unz_file_info info;

	if (unzfile == NULL)
	{
    	FILE *fp = (FILE *)fd;
	    int len, pos = ftell(fp);

	    fseek(fp, 0, SEEK_END);
	    len = ftell(fp);
	    fseek(fp, pos, SEEK_SET);

	    return len;
	}

	unzGetCurrentFileInfo(unzfile, &info, NULL, 0, NULL, 0, NULL, 0);

	return info.uncompressed_size;
}


int zlength(unzFile unzfile, const char *filename)
{
	int f;

	if ((f = zopen(unzfile, filename)) != -1)
	{
		int length = zsize(unzfile, f);
		zclose(unzfile, f);
		return length;
	}
	return 0;
}
