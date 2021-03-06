#include "XScreenRgbInfo.h"

XScreenRgbInfo::XScreenRgbInfo(unsigned long red_mask, unsigned long green_mask, unsigned long blue_mask,
		 char bytes_per_pixel, char bytes_per_rgb, unsigned char *p_screenbuf) { 

  this->p_screenbuf = p_screenbuf;
  
  this->red_mask = red_mask;
  this->green_mask = green_mask;
  this->blue_mask = blue_mask;
  
  this->bytes_per_pixel = bytes_per_pixel;
  this->bytes_per_rgb = bytes_per_rgb;
  
  ext_max_red = 255;
  ext_max_green = 255;
  ext_max_blue = 255;
  
  compute_color_resolution();
  
  red_light_table = new int[(red_max + 1) * NUM_LIGHT_LEVELS];
  green_light_table = new int[(green_max + 1) * NUM_LIGHT_LEVELS];
  blue_light_table = new int[(blue_max + 1) * NUM_LIGHT_LEVELS];
  
  red_fog_table = new int[(red_max + 1) * NUM_LIGHT_LEVELS];
  green_fog_table = new int[(green_max + 1) * NUM_LIGHT_LEVELS];
  blue_fog_table = new int[(blue_max + 1) * NUM_LIGHT_LEVELS];
  
  compute_light_table();
  compute_fog_table();
}

XScreenRgbInfo::~XScreenRgbInfo() {
  delete [] red_light_table;
  delete [] green_light_table;
  delete [] blue_light_table;
  delete [] red_fog_table;
  delete [] green_fog_table;
  delete [] blue_fog_table;
}

void XScreenRgbInfo::compute_color_resolution() {
  int red_mask_tmp=red_mask,
                   green_mask_tmp=green_mask,
                                  blue_mask_tmp=blue_mask;

  // determine number of bits of resolution for r, g, and b

  // note that the max values go from 0 to xxx_max, meaning the total
  // count (as needed in for loops for instance) is xxx_max+1

  for(red_shift=0;
      (red_mask_tmp & 0x01) == 0 ;
      red_shift++, red_mask_tmp>>=1);
  for(red_max=1;
      (red_mask_tmp & 0x01) == 1 ;
      red_max*=2, red_mask_tmp>>=1);
  red_max--;

  for(green_shift=0;
      (green_mask_tmp & 0x01) == 0 ;
      green_shift++, green_mask_tmp>>=1);
  for(green_max=1;
      (green_mask_tmp & 0x01) == 1 ;
      green_max*=2, green_mask_tmp>>=1);
  green_max--;

  for(blue_shift=0;
      (blue_mask_tmp & 0x01) == 0 ;
      blue_shift++, blue_mask_tmp>>=1);
  for(blue_max=1;
      (blue_mask_tmp & 0x01) == 1 ;
      blue_max*=2, blue_mask_tmp>>=1);
  blue_max--;
    
} 

unsigned long XScreenRgbInfo::ext_to_native(int red, int green, int blue) { 
  unsigned long red_rescaled, green_rescaled, blue_rescaled;

  red_rescaled = red * red_max / ext_max_red;
  green_rescaled = green * green_max / ext_max_green;
  blue_rescaled = blue * blue_max / ext_max_blue;

  return (red_rescaled<<red_shift)
         | (green_rescaled<<green_shift)
         | (blue_rescaled<<blue_shift);    
}


void XScreenRgbInfo::compute_light_table() {
  int i,c;
  int *tableptr;

  // between 0 and given intensity

  tableptr = red_light_table;
  for(c=0; c<=red_max; c++) { // color of red
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of red
      *tableptr++ = c * i / MAX_LIGHT_LEVELS;
    }
  }
  tableptr = green_light_table;
  for(c=0; c<=green_max; c++) { // color of green
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of green
      *tableptr++ = c * i / MAX_LIGHT_LEVELS;
    }
  }
  tableptr = blue_light_table;
  for(c=0; c<=blue_max; c++) { // color of blue
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of blue
      *tableptr++ = c * i / MAX_LIGHT_LEVELS;
    }
  }
}

void XScreenRgbInfo::compute_fog_table() {
  int i,c;
  int *tableptr;

  tableptr = red_fog_table;
  for(c=0; c<=red_max; c++) { // color of red
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of red
      // between normal intensity and white
      *tableptr++ = c + (int) (((float)i/MAX_LIGHT_LEVELS)*(red_max - c));
    }
  }
  tableptr = green_fog_table;
  for(c=0; c<=green_max; c++) { // color of green
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of green
      // between normal intensity and white
      *tableptr++ = c + (int) (((float)i/MAX_LIGHT_LEVELS)*(green_max - c));
    }
  }
  tableptr = blue_fog_table;
  for(c=0; c<=blue_max; c++) { // color of blue
    for(i=0; i<NUM_LIGHT_LEVELS; i++) { // intensity of blue
      // between normal intensity and white
      *tableptr++ = c + (int) (((float)i/MAX_LIGHT_LEVELS)*(blue_max - c));
    }
  }  
}

void XScreenRgbInfo::light_native(unsigned char *pColor, int intensity) {
  // decode pixel bytes into r,g,b, assuming LSB-first byte order

  unsigned char *c2 = pColor;

  register int p;
  unsigned long color=0;
  unsigned long color_shift = 0;
  for(p=0; p<bytes_per_rgb; p++) {
    color += (*c2++) << color_shift;
    color_shift += BITS_PER_BYTE;
  }
  // skip the rest of the bytes between the bytes_per_rgb and bytes_per_pixel
  for(p=bytes_per_rgb; p<bytes_per_pixel;p++) { c2++; }

  unsigned long r = (color & red_mask) >> red_shift;
  unsigned long g = (color & green_mask) >> green_shift;
  unsigned long b = (color & blue_mask) >> blue_shift;

  color =
    (red_light_table[r*NUM_LIGHT_LEVELS+intensity] <<red_shift) |
    (green_light_table[g*NUM_LIGHT_LEVELS+intensity] <<green_shift) |
    (blue_light_table[b*NUM_LIGHT_LEVELS+intensity] <<blue_shift);

  {
    register int i;
    unsigned long mask = 255;
    char shift = 0;

    // stuff bytes in the unsigned long col into the screen buffer, in
    // little-endian order
    for(c2-=bytes_per_pixel, i=0; i<bytes_per_pixel; i++) {
      *c2 = (color & mask) >> shift;
      c2++;
      mask <<= BITS_PER_BYTE;
      shift += BITS_PER_BYTE;
    }
  }
  
}

void XScreenRgbInfo::fog_native(unsigned char *pColor, int intensity) { 
  
    // decode pixel bytes into r,g,b, assuming LSB-first byte order

  unsigned char *c2 = pColor;

  register int p;
  unsigned long color=0;
  unsigned long color_shift = 0;
  for(p=0; p<bytes_per_rgb; p++) {
    color += (*c2++) << color_shift;
    color_shift += BITS_PER_BYTE;
  }
  // skip the rest of the bytes between the bytes_per_rgb and bytes_per_pixel
  for(p=bytes_per_rgb; p<bytes_per_pixel;p++) { c2++; }

  unsigned long r = (color & red_mask) >> red_shift;
  unsigned long g = (color & green_mask) >> green_shift;
  unsigned long b = (color & blue_mask) >> blue_shift;

  color =
    (red_fog_table[r*NUM_LIGHT_LEVELS+intensity] <<red_shift) |
    (green_fog_table[g*NUM_LIGHT_LEVELS+intensity] <<green_shift) |
    (blue_fog_table[b*NUM_LIGHT_LEVELS+intensity] <<blue_shift);

  {
    register int i;
    unsigned long mask = 255;
    char shift = 0;

    // stuff bytes in the unsigned long col into the screen buffer, in
    // little-endian order
    for(c2-=bytes_per_pixel, i=0; i<bytes_per_pixel; i++) {
      *c2 = (color & mask) >> shift;
      c2++;
      mask <<= BITS_PER_BYTE;
      shift += BITS_PER_BYTE;
    }
  }
  
}