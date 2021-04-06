unzFile zip_open(const char *path);
int zip_close(unzFile unzfile);
int zopen(unzFile unzfile, const char *filename);
int zread(unzFile unzfile, int fp, void *buf, unsigned size);
int zclose(unzFile unzfile, int fp);
int zlength(unzFile unzfile, const char *filename);
