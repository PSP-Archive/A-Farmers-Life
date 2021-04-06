#include "../inc/main.h"

#define MAX_FONT 2
const char *font[] = {
"A", "B", 
};

int DisplayFont(Image * image, int x, int y, char *str)
{
  int fy=0, fx=0;
  for(int s=0;s<strlen(str);s++)
  {
    if(str[s]=='\0')
      return 0;
    if(str[s]=='\n'){
      fx = 0;
      fy++;
      continue;
    }
    for(int i=0;i<MAX_FONT;i++)
    {
      if(font[i][0]==str[s])
      {                   
        float fontwidth = image->imageWidth/MAX_FONT;
        blitAlphaImageToScreen(((float)fontwidth)*i, 0, (float)fontwidth, image->imageHeight, image, x+(((float)fontwidth)*fx), y+(fy*(image->imageHeight)));
      }
    }
    fx++;
  }
}
