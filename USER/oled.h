#ifndef _OLED_H_
#define _OLED_H_

void oled_send_data(unsigned char c);
void oled_send_cmd(unsigned char c);
void oled_address(unsigned char page,unsigned char column);
void clear_screen(void);
void clear_page(unsigned char page);
void initial_oled(void);
void oled_show_str(unsigned char *c);
void show_voltage(void);

#endif
