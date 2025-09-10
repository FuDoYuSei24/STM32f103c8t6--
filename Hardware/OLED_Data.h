#ifndef __OLED_DATA_H
#define __OLED_DATA_H

#include <stdint.h>

/*字符集定义*/
/*以下两个宏定义只可解除其中一个的注释*/
//#define OLED_CHARSET_UTF8			//字符集是UTF8
#define OLED_CHARSET_GB2312		//字符集是GB2312

/*字模基本单元*/
typedef struct 
{
	
#ifdef OLED_CHARSET_UTF8			//定义字符集为UTF8
	char Index[5];					//UTF8汉字索引，空间为5字节
#endif
	
#ifdef OLED_CHARSET_GB2312			//定义字符集为GB2312
	char Index[3];					//汉字索引，空间为3字节
#endif
	
	uint8_t Data[32];				//字模数据
} ChineseCell_t;

/*ASCII码字模数据声明*/
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];
extern const uint8_t OLED_F12x24[][36];
/*汉字字模数据声明*/
extern const ChineseCell_t OLED_CF16x16[];

/*图像数据声明*/
extern const uint8_t Diode[];
extern const uint8_t Return[];
/*按照上面的格式，在这个位置加入新的图像数据的声明*/
//...

#endif


