#ifndef SCR_RGB_INFO_H
#define SCR_RGB_INFO_H

#define MAX_LIGHT_LEVELS 255
#define NUM_LIGHT_LEVELS (MAX_LIGHT_LEVELS + 1)
#define BITS_PER_BYTE 8
#define MAX_BYTE 255

class XScreenRgbInfo {
public: 
  XScreenRgbInfo(unsigned long red_mask, unsigned long green_mask, unsigned long blue_mask,
		 char bytes_per_pixel, char bytes_per_rgb, unsigned char *p_screenbuf);
  ~XScreenRgbInfo();
  
  unsigned long ext_to_native(int red, int green, int blue);
  void light_native(unsigned char *pColor, int intensity);
  void fog_native(unsigned char *pColor, int intensity);
 
  unsigned char *p_screenbuf;
  char bytes_per_pixel;

  
  int ext_max_red;
  int ext_max_green;
  int ext_max_blue;
  int *red_light_table, *green_light_table, *blue_light_table,
      *red_fog_table, *green_fog_table, *blue_fog_table;
  char bytes_per_rgb;
  unsigned long red_mask, green_mask, blue_mask;
  int red_shift, green_shift, blue_shift, red_max, green_max, blue_max;
  
  void compute_light_table();
  void compute_fog_table();
  void compute_color_resolution();  
};

#endif