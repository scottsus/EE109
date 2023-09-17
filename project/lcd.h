void lcd_init(void);
void lcd_moveto(unsigned char, unsigned char);
void lcd_stringout(char *);
void lcd_intout(int);
void lcd_intout_coord(unsigned char, unsigned char, int);
void lcd_numout(int);
void lcd_writecommand(unsigned char);
void lcd_writedata(unsigned char);
