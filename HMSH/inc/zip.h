
int OpenZip(char *zippath);
void CloseZip();

Image * imageOpenZipFile(char *zippath, char *fname, ...);
Map * mapOpenZipFile(char *zippath, char *filename);
Titlescreen * titlescreenOpenZipFile(char *zippath, char *filename);
