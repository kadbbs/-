/*PE7 MISO
 * PE8 LED
 * PE9 SCK       ʱ��
 * PE10 MOSI
 * PE11 DC         0����              1data
 *
 * PE12 RESET  0��λʹ��
 *
 *
 *
 * */
#include <stdarg.h>
#include "../system.h"
#include "debug.h"
#include "st7789.h"
#include "font.h"
/* Ĭ��LCD�Ļ�����ɫ�ͱ���ɫ */
u16 POINT_COLOR = 0x0000;   /* ������ɫ */
u16 BACK_COLOR = 0xFFFF;    /* ����ɫ */
//st7789���ų�ʼ��
void ST7789_GPIO_init(){
        GPIO_InitTypeDef GPIO_init_structure={0};

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
        GPIO_init_structure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
        GPIO_init_structure.GPIO_Mode=GPIO_Mode_Out_PP;
        GPIO_init_structure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOE,&GPIO_init_structure);
//        LCD_CS(0);
//        LCD_PWR(1);
//        LCD_RST(1);
//        LCD_WR(1);
//        LCD_SDA(1);
//        LCD_SCK(1);
//        LCD_LED(1);
}


//#define soft
#define hard

#ifdef soft
/*
 *spi_soft
 * lcd_write_reg
 *
 *���ܣ���lcd����оƬдһ������
 *����uint_8 reg
 *
 * */
void lcd_write_reg(u8 reg){


        u8 i;

        LCD_CS(0);
        LCD_WR(0);

        for(i = 0; i < 8; i++)
        {
            LCD_SCK(0);
            if(reg & 0x80)
                LCD_SDA(1);
            else
                LCD_SDA(0);
            LCD_SCK(1);
            reg <<= 1;
        }
        LCD_CS(1);
//        LCD_WR(1);

}




/*
 *spi_soft
 * lcd_write_data
 *
 * ���ܣ���lcd����оƬдһ������
 *����uint_8 data
 *
 * */

void lcd_write_data(u8 data){
    u8 i;

    LCD_CS(0);
    LCD_WR(1);

    for(i = 0; i < 8; i++)
    {
        LCD_SCK(0);
        if(data & 0x80)
            LCD_SDA(1);
        else
            LCD_SDA(0);
        LCD_SCK(1);
        data <<= 1;
    }
    LCD_CS(1);
//    LCD_WR(1);


}


#endif

#ifdef hard

/*
 *spi_hard
 * lcd_write_reg
 *
 *���ܣ���lcd����оƬдһ������
 *����uint_8 reg
 *
 * */
void lcd_write_reg(u8 reg){
    LCD_CS(0);
    LCD_WR(0);


    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET);
    SPI_I2S_SendData(SPI1, (uint16_t)reg);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==SET);

    LCD_CS(1);
}






/*
 *spi_hard
 * lcd_write_data
 *
 * ���ܣ���lcd����оƬдһ������
 *����uint_8 data
 *
 * */

void lcd_write_data(u8 data){
    LCD_CS(0);
    LCD_WR(1);


    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET);
    SPI_I2S_SendData(SPI1,(uint16_t) data);

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==SET);
    LCD_CS(1);
}



#endif

/**************************************************************
�������� : lcd_write_ram
�������� : ʹ��д�����ݵ�RAM
������� : ��
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_write_ram(void)
{
    lcd_write_reg(0x2C);
}

/**************************************************************
�������� : lcd_set_address
�������� : ��������
������� : x1,y1����ʼ��ַ��x2,y2���յ��ַ
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_set_address(u16 x1, u16 y1, u16 x2, u16 y2)
{
    lcd_write_reg(0x2a);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1&0xff);
    lcd_write_data(x2 >> 8);
    lcd_write_data(x2&0xff);

    lcd_write_reg(0x2b);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1&0xff);
    lcd_write_data(y2 >> 8);
    lcd_write_data(y2&0xff);
    lcd_write_reg(0x2c);
}

/**************************************************************
�������� : lcd_display_on
�������� : ����ʾ
������� : ��
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_display_on(void)
{
//    LCD_PWR(1);
    lcd_write_reg(0x29);
}

/**************************************************************
�������� : lcd_display_off
�������� : �ر���ʾ
������� : ��
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_display_off(void)
{
//    LCD_PWR(0);
    lcd_write_reg(0x28);
}

/**************************************************************
�������� : lcd_clear
�������� : lcd��������
������� : color��Ҫ��������ɫ
����ֵ      : ��
��ע       : �ȹر���ʾ���ȵ�������ɫ����д�뵽RAM���ٴ���ʾ
**************************************************************/
void lcd_clear(u16 color)
{
    u16 i, j;

    lcd_display_off();      /* �ر���ʾ */
    lcd_set_address(0, 0, LCD_WIDTH - 1, LCD_HEIHGT - 1);
    lcd_write_ram();

    for(i = 0; i < LCD_HEIHGT; i++)
    {
        for(j = 0; j < LCD_WIDTH; j++)
        {
            lcd_write_data(color >> 8);
            lcd_write_data(color & 0x00ff);
        }
    }
    lcd_display_on();       /* ����ʾ */
}

/**************************************************************
�������� : lcd_draw_point
�������� : lcd��һ����
������� : x,y  --> �������꣬color --> �����ɫ
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_draw_point(u16 x, u16 y, u16 color)
{
    lcd_set_address(x, y, x, y);
    lcd_write_ram();
    lcd_write_data(color >> 8);
    lcd_write_data(color & 0x00ff);
}


/**************************************************************
�������� : lcd_draw_point
�������� : lcd��һ����
������� : x,y  --> �������꣬color --> �����ɫ
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_draw_point_across(u16 x, u16 y, u16 color)
{



    lcd_set_address(y, 320-x, y, 320-x);
    lcd_write_ram();
    lcd_write_data(color >> 8);
    lcd_write_data(color & 0x00ff);
}

/**************************************************************
�������� : lcd_draw_point
�������� : lcd��һ��area
������� : x,y  --> �������꣬color --> �����ɫ k -->����������
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_draw_area(u16 x, u16 y, u16 color,u8 k)
{
    u16 x1;
    for(int i=0;i<k;i++){
        x1=x;
        for(int p=0;p<k;p++){
            lcd_draw_point_across(x1++,y,color);
        }
        y++;
    }
}



/**************************************************************
�������� : lcd_show_char
�������� : lcd��ʾһ���ַ�
������� : x,y:��ʼ����
           num:Ҫ��ʾ���ַ�:" "--->"~"
           size:�����С
           mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_show_char(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u16 temp, t1, t;
    u16 y0 = y;
    u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size/2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ���   */

    num = num - ' ';    /* �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩ */

    for(t = 0; t < csize; t++)  /*������ӡ�������ص㵽LCD */
    {
        if(16 == size)
        {
            temp = asc2_1608[num][t];   /* ����1608���� */
        }
        else if(24 == size)
        {
            temp = asc2_2412[num][t];   /* ����2412���� */
        }
        else if(32 == size)
        {
            temp = asc2_3216[num][t];   /* ����3216��������� */
        }
        else
        {
            return;     /* û���ҵ���Ӧ���ֿ� */
        }
        for(t1 = 0; t1 < 8; t1++)   /* ��ӡһ�����ص㵽Һ�� */
        {
            if(temp & 0x80)
            {
                lcd_draw_point(x, y, POINT_COLOR);
            }
            else if(0 == mode)
            {
                lcd_draw_point(x, y, BACK_COLOR);
            }
            temp <<= 1;
            y++;

            if(y >= LCD_HEIHGT)
            {
                return;     /* �������� */
            }
            if((y - y0) == size)
            {
                y = y0;
                x++;
                if(x >= LCD_WIDTH)
                {
                    return; /* �������� */
                }
                break;
            }
        }
    }
}

void show_lcd_data(char *server_data,u32 len){


    lcd_clear(RED);
    int y=0;
    int x=0;
    for(int t=0;t<len;t++){
        if(y==13*24)
            break;
        if(server_data[t]=='\n'){
            y=y+24;
            x=0;
            continue;
        }
        if(x==240){
            x=0;
            y=y+24;
        }

        lcd_show_char(x,y,server_data[t],24,1);
        x=x+24;

    }


}


void lcd_show_str_crosswise(char *server_data,u32 len){


    lcd_clear(RED);
    int y=0;
    int x=0;
    for(int t=0;t<len;t++){
        if(y==10*24)
            break;
        if(server_data[t]=='\n'){
            y=y+24;
            x=0;
            continue;
        }
        if(x==13*24){
            x=0;
            y=y+24;
        }

        lcd_show_char_crosswise(x,y,server_data[t],24,1);
        x=x+24;

    }


}


void lcd_show_char_crosswise(u16 x, u16 y, u8 num, u8 size, u8 mode){
    u16 temp, t1, t;
//        u16 y0 = 320-x;
        u16 x0=y;
        u16 swap_x=x;
        x=y;

        y=320-swap_x;
        u16 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size/2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ���   */

        num = num - ' ';    /* �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩ */

        for(t = 0; t < csize; t++)  /*������ӡ�������ص㵽LCD */
        {
            if(16 == size)
            {
                temp = asc2_1608[num][t];   /* ����1608���� */
            }
            else if(24 == size)
            {
                temp = asc2_2412[num][t];   /* ����2412���� */
            }
            else if(32 == size)
            {
                temp = asc2_3216[num][t];   /* ����3216��������� */
            }
            else
            {
                return;     /* û���ҵ���Ӧ���ֿ� */
            }
            for(t1 = 0; t1 < 8; t1++)   /* ��ӡһ�����ص㵽Һ�� */
            {
                if(temp & 0x80)
                {
                    lcd_draw_point(x, y, POINT_COLOR);
                }
                else if(0 == mode)
                {
                    lcd_draw_point(x, y, BACK_COLOR);
                }
                temp <<= 1;
                x++;


                if(x >= LCD_WIDTH)
                {
                    return;     /* �������� */
                }
                if((x - x0) == size)
                {
                    x = x0;
                    y--;
//                    LCD_HEIHGT
                    if(y <0)
                    {
                        return; /* �������� */
                    }
                    break;
                }
            }
        }

}


/**************************************************************
�������� : lcd_show_string
�������� : lcd��ʾ�ַ���
������� : x,y:��ʼ����
           width,height�������С
           *p:�ַ�����ʼ��ַ
           size:�����С
           mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
����ֵ      : ��
��ע       : ��
**************************************************************/
void lcd_show_string(u16 x, u16 y, u16 width, u16 height, u8 *p, u8 size, u8 mode)
{
    u16 x0 = x;
    width += x;
    height += y;

    while((*p<='~') && (*p>=' '))       /* �ж��ǲ��ǷǷ��ַ�! */
    {
        if(x >= width)
        {
            x = x0;
            y += size;
        }

        if(y >= height)
        {
            break;
        }
        lcd_show_char(x, y, *p, size, mode);
        x += size/2;
        p++;
    }
}

/**************************************************************
�������� : lcd_show_chinese
�������� : lcd��ʾ�ַ���
������� : x,y:��ʼ����
           *ch:�����ַ�����ʼ��ַ
           size:�����С
           n:���ָ���
           mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
����ֵ      : ��
��ע       : ��
**************************************************************/
//void lcd_show_chinese(u16 x, u16 y, const u8 * ch, u16 size, u8 n, u8 mode)
//{
//    u32 temp, t, t1;
//    u16 y0 = y;
//    u32 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size) * n;   /* �õ������ַ���Ӧ������ռ���ֽ��� */
//
//    for(t = 0; t < csize; t++)
//    {
//        temp = ch[t];   /* �õ��������� */
//
//        for(t1 = 0; t1 < 8; t1++)
//        {
//            if(temp & 0x80)
//            {
//                lcd_draw_point(x, y, POINT_COLOR);
//            }
//            else if(mode==0)
//            {
//                lcd_draw_point(x, y, BACK_COLOR);
//            }
//            temp <<= 1;
//            y++;
//            if((y - y0) == size)
//            {
//                y = y0;
//                x++;
//                break;
//            }
//        }
//    }
//}
void lcd_show_chinese(u16 x, u16 y, const u8 * ch, u16 size, u8 n, u8 mode)
{
    lcd_display_off();
    u16 x_tmp=x;
    y=320-(y*size);
    u32 temp, t, t1;
    u16 y0 = y;
    u32 csize = ((size / 8) + ((size % 8) ? 1 : 0)) * (size) * n;   /* �õ������ַ���Ӧ������ռ���ֽ��� */

    for(t = 0; t < csize; t++)
    {
        temp = ch[t];   /* �õ��������� */

        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)
            {
                lcd_draw_point(x, y, POINT_COLOR);
            }
            else if(mode==0)
            {
                lcd_draw_point(x, y, BACK_COLOR);
            }
            temp <<= 1;
            y--;
            if((y0-y) == size)
            {
                y = y0;
                x++;
                if(x>(x_tmp+size/n))
                    return;
                break;
            }
        }
    }
    lcd_display_on();
}
//_lcd_dev lcddev;
//���ù��λ��
//Xpos:������
//Ypos:������
//��ʼдGRAM
//void LCD_WriteRAM_Prepare(void)
//{
//    LCD_WR_REG(lcddev.wramcmd);
//}
//void LCD_SetCursor(u16 Xpos, u16 Ypos)
//{
//    LCD_WR_REG(lcddev.setxcmd);
//    LCD_WR_DATA(Xpos>>8);
//    LCD_WR_DATA(Xpos&0XFF);
//
//    LCD_WR_REG(lcddev.setycmd);
//    LCD_WR_DATA(Ypos>>8);
//    LCD_WR_DATA(Ypos&0XFF);
//}
//void LCD_DrawPoint(u16 x,u16 y)
//{
//    LCD_SetCursor(x,y);     //���ù��λ��
//    LCD_WriteRAM_Prepare(); //��ʼд��GRAM
//    LCD_WriteRAM(POINT_COLOR);
//}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
    int a,b;
    int di;
    a=0;b=r;
    di=3-(r<<1);             //�ж��¸���λ�õı�־
    while(a<=b)
    {
        lcd_draw_point(x0+a,y0-b,BLUE);             //5
        lcd_draw_point(x0+b,y0-a,BLUE);             //0
        lcd_draw_point(x0+b,y0+a,BLUE);             //4
        lcd_draw_point(x0+a,y0+b,BLUE);             //6
        lcd_draw_point(x0-a,y0+b,BLUE);             //1
        lcd_draw_point(x0-b,y0+a,BLUE);
        lcd_draw_point(x0-a,y0-b,BLUE);             //2
        lcd_draw_point(x0-b,y0-a,BLUE);             //7
        a++;
        //ʹ��Bresenham�㷨��Բ
        if(di<0)di +=4*a+6;
        else
        {
            di+=10+4*(a-b);
            b--;
        }
    }
}

/**************************************************************
 * hard
�������� : lcd_init
�������� : lcd��ʼ��
������� : ��
����ֵ      : ��
��ע       : ��
Ĭ��LCD�Ļ�����ɫ�ͱ���ɫ
u16 POINT_COLOR = 0x0000;    ������ɫ
u16 BACK_COLOR = 0xFFFF;    ����ɫ
**************************************************************/
void lcd_init(void)
{

    ST7789_GPIO_init();
    spi1_init();
    //SPI1_SetSpeed(SPI_BaudRatePrescaler_4);
    LCD_RST(1);
    delay_ms(1);
    LCD_RST(0);
    delay_ms(10);
    LCD_RST(1);
    delay_ms(120);

    /* Sleep Out */

    lcd_write_reg(0x11);


    //lcd_write_reg(0x11);

    /* wait for power stability */
    delay_ms(120);


    /* Memory Data Access Control */
    lcd_write_reg(0x36);
    lcd_write_data(0x00);

    /* RGB 5-6-5-bit  */
    lcd_write_reg(0x3A);
//    lcd_write_data(0x65);
    lcd_write_data(0x05);


    /* Porch Setting */
    lcd_write_reg(0xB2);
    lcd_write_data(0x0C);
    lcd_write_data(0x0C);
    lcd_write_data(0x00);
    lcd_write_data(0x33);
    lcd_write_data(0x33);

    /*  Gate Control */
    lcd_write_reg(0xB7);
    lcd_write_data(0x35);
//    lcd_write_data(0x72);

    /* VCOM Setting */
    lcd_write_reg(0xBB);
    lcd_write_data(0x28);   //Vcom=1.625V
//    lcd_write_data(0x3D);   //Vcom=1.625V

    /* LCM Control */
    lcd_write_reg(0xC0);
    lcd_write_data(0x2C);

    /* VDV and VRH Command Enable */
    lcd_write_reg(0xC2);
    lcd_write_data(0x01);

    /* VRH Set */
    lcd_write_reg(0xC3);
//    lcd_write_data(0x19);
    lcd_write_data(0x0b);

    /* VDV Set */
    lcd_write_reg(0xC4);
    lcd_write_data(0x20);

    /* Frame Rate Control in Normal Mode */
    lcd_write_reg(0xC6);
    lcd_write_data(0x0F);   //60MHZ

    /* Power Control 1 */
    lcd_write_reg(0xD0);
    lcd_write_data(0xA4);
    lcd_write_data(0xA1);

    /* Positive Voltage Gamma Control */
    lcd_write_reg(0xE0);
    lcd_write_data(0xD0);
    lcd_write_data(0x01);
//    lcd_write_data(0x04);

    lcd_write_data(0x08);
//    lcd_write_data(0x0D);

//    lcd_write_data(0x11);
//    lcd_write_data(0x13);
//    lcd_write_data(0x2B);
//    lcd_write_data(0x3F);
//    lcd_write_data(0x54);
//    lcd_write_data(0x4C);
//    lcd_write_data(0x18);
//    lcd_write_data(0x0D);
//    lcd_write_data(0x0B);
//    lcd_write_data(0x1F);
//    lcd_write_data(0x23);
    lcd_write_data (0x0f);
    lcd_write_data (0x11);
    lcd_write_data (0x2a);
    lcd_write_data (0x36);
    lcd_write_data (0x55);
    lcd_write_data (0x44);
    lcd_write_data (0x3a);
    lcd_write_data (0x0b);
    lcd_write_data (0x06);
    lcd_write_data (0x11);
    lcd_write_data (0x20);
    /* Negative Voltage Gamma Control */
    lcd_write_reg(0xE1);
//    lcd_write_data(0xD0);
//    lcd_write_data(0x04);
//    lcd_write_data(0x0C);
//    lcd_write_data(0x11);
//    lcd_write_data(0x13);
//    lcd_write_data(0x2C);
//    lcd_write_data(0x3F);
//    lcd_write_data(0x44);
//    lcd_write_data(0x51);
//    lcd_write_data(0x2F);
//    lcd_write_data(0x1F);
//    lcd_write_data(0x1F);
//    lcd_write_data(0x20);
//    lcd_write_data(0x23);
    lcd_write_data (0xd0);
    lcd_write_data (0x02);
    lcd_write_data (0x07);
    lcd_write_data (0x0a);
    lcd_write_data (0x0b);
    lcd_write_data (0x18);
    lcd_write_data (0x34);
    lcd_write_data (0x43);
    lcd_write_data (0x4a);
    lcd_write_data (0x2b);
    lcd_write_data (0x1b);
    lcd_write_data (0x1c);
    lcd_write_data (0x22);
    lcd_write_data (0x1f);

    /* Display Inversion On */
//    lcd_write_reg(0x21);
    lcd_write_reg(0x29);

//    lcd_clear(BACK_COLOR);
//    /*����ʾ*/
//    LCD_PWR(1);
}

//void ST7789VCMI24panelinitialcode(void)
//{
////-----------------------------------ST7789V reset sequence------------------------------------//
//LCD_RESET(1);
//delayms(1); //Delay 1ms
//LCD_RESET=(0);
//delayms(10); //Delay 10ms
//LCD_RESET=(1);
//delayms(120); //Delay 120ms
////---------------------------------------------------------------------------------------------------//
//LLCD_WRITE_CMD (0x11);
//delayms(120); //Delay 120ms
////------------------------------display and color format setting--------------------------------//
//LLCD_WRITE_CMD (0x36);
//LLCD_WRITE_DATA (0x00);
//LLCD_WRITE_CMD (0x3a);
//LLCD_WRITE_DATA (0x05);
////--------------------------------ST7789V Frame rate setting----------------------------------//
//LLCD_WRITE_CMD (0xb2);
//LLCD_WRITE_DATA (0x0c);
//LLCD_WRITE_DATA (0x0c);
//LLCD_WRITE_DATA (0x00);
//LLCD_WRITE_DATA (0x33);
//LLCD_WRITE_DATA (0x33);
//LLCD_WRITE_CMD (0xb7);
//LLCD_WRITE_DATA (0x35);
////---------------------------------ST7789V Power setting--------------------------------------//
//LLCD_WRITE_CMD (0xbb);
//LLCD_WRITE_DATA (0x28);
//LLCD_WRITE_CMD (0xc0);
//LLCD_WRITE_DATA (0x2c);
//LLCD_WRITE_CMD (0xc2);
//LLCD_WRITE_DATA (0x01);
//LLCD_WRITE_CMD (0xc3);
//LLCD_WRITE_DATA (0x0b);
//LLCD_WRITE_CMD (0xc4);
//LLCD_WRITE_DATA (0x20);
//LLCD_WRITE_CMD (0xc6);
//LLCD_WRITE_DATA (0x0f);
//LLCD_WRITE_CMD (0xd0);
//LLCD_WRITE_DATA (0xa4);
//LLCD_WRITE_DATA (0xa1);
////--------------------------------ST7789V gamma setting---------------------------------------//
//LLCD_WRITE_CMD (0xe0);
//LLCD_WRITE_DATA (0xd0);
//LLCD_WRITE_DATA (0x01);
//LLCD_WRITE_DATA (0x08);
//LLCD_WRITE_DATA (0x0f);
//LLCD_WRITE_DATA (0x11);
//LLCD_WRITE_DATA (0x2a);
//LLCD_WRITE_DATA (0x36);
//LLCD_WRITE_DATA (0x55);
//LLCD_WRITE_DATA (0x44);
//LLCD_WRITE_DATA (0x3a);
//LLCD_WRITE_DATA (0x0b);
//LLCD_WRITE_DATA (0x06);
//LLCD_WRITE_DATA (0x11);
//LLCD_WRITE_DATA (0x20);
//LLCD_WRITE_CMD (0xe1);
//LLCD_WRITE_DATA (0xd0);
//LLCD_WRITE_DATA (0x02);
//LLCD_WRITE_DATA (0x07);
//LLCD_WRITE_DATA (0x0a);
//LLCD_WRITE_DATA (0x0b);
//LLCD_WRITE_DATA (0x18);
//LLCD_WRITE_DATA (0x34);
//LLCD_WRITE_DATA (0x43);
//LLCD_WRITE_DATA (0x4a);
//LLCD_WRITE_DATA (0x2b);
//LLCD_WRITE_DATA (0x1b);
//LLCD_WRITE_DATA (0x1c);
//LLCD_WRITE_DATA (0x22);
//LLCD_WRITE_DATA (0x1f);
//LLCD_WRITE_CMD (0x29);
//}


//void dome(){
//    lcd_write_reg()
//
//
//}



/*
lprintf();
������lcd�ϴ�ӡ�ַ�

*/
static int my_pow(int x,int y){


    for(int i=1;i<y+1;i++){
        x=x*i;
    }
    return x;
}

static int num_ch(char *buff,char *format,int num){

    //numΪ����


    int i=1;
    int re;
    while(1){
        re=my_pow(10,i);
        if(re>num){
            break;
        }else if(my_pow(10,i)==num){
            i++;
            break;
        }

        i++;
    }

    //��ʱֻʹ��%d��ʽ
    if(format[1]=='d'){

        for(int t=0;t<i;t++){

            buff[i-1-t]=num%my_pow(10,t+1)+48;
        }

    }
    return i;
}

void lprintf(const char *format,...) {
    //lcd_clear(RED);
    lcd_clear(WHITE);


    int x=0,y=0;
    va_list ap;
    va_start(ap, format);
    char c=0;
    int i=0;
    int flog=0;
    int val=0;
    char ch;
    char *chs;
    char buf[100];

    while(1){


        c=format[i++];

        if(c=='%'){
            flog=1;
            //printf("%d\n",flog);
            continue;
        }else if (c=='\0')
        {
            break;
        }else if(c=='\n'){
            x=0;
            y+=24;
            continue;
        }

        if(flog==1){
            flog=0;
            switch (c)
            {
            case 'd':{
                val=va_arg(ap,int);
                //  int len=sprintf(buf,"%d",val);
                 int len=num_ch(buf,"%d",val);

                 if(y>240){
                    //���
                     break;
                 }
                 if(x>24*13){
                     x=0;
                     y+=24;
                 }
                 lcd_show_char_crosswise(x,y,buf[0],24,0);

                 x+=24;

                //write(1,buf,len);
                break;

            }
            case 'c':{

                    ch=va_arg(ap,int);
                    // int len_c=sprintf(buf,"%d",ch);
                    int len_c=num_ch(buf,"%d",ch);

                    if(y>240){
                                        //���
                                         break;
                                     }
                                     if(x>24*13){
                                         x=0;
                                         y+=24;
                                     }

                                     lcd_show_char_crosswise(x,y,ch,24,0);

                                     x+=24;

//                    write(1,&ch,len_c);
                    break;

            }
            case 's':{
                chs=va_arg(ap,char*);
                while(1){
                    if(*chs=='\0'){
                        break;
                    }

                    if(*chs=='\n'){

                        x=0;
                        y+=24;
                        chs++;
                        continue;

                    }
                    if(y>240){
                                        //���
                                         break;
                                     }
                                     if(x>24*13){
                                         x=0;
                                         y+=24;
                                     }

                                     lcd_show_char_crosswise(x,y,*chs,24,0);

                                     x+=24;
//                    write(1,chs,1);
                    chs++;

                }
                break;

        }

        }
        }else{

//            write(1,&c,1);
            if(y>240){
                                //���
                                 break;
                             }
                             if(x>24*13){
                                 x=0;
                                 y+=24;
                             }
                             lcd_show_char_crosswise(x,y,c,24,0);

                             x+=24;
        }
    }
    va_end(ap);
}


