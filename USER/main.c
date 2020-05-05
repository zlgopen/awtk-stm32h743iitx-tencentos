#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "lcd.h"
#include "sdram.h"
#include "usmart.h"
#include "pcf8574.h"
#include "mpu.h"
#include "malloc.h"
#include "sdmmc_sdcard.h"
/************************************************
 ALIENTEK 阿波罗STM32H7开发板 实验40
 SD卡实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}
//测试SD卡的读取
//从secaddr地址开始,读取seccnt个扇区的数据
//secaddr:扇区地址
//seccnt:扇区数
void sd_test_read(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf;
	u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//申请内存,从SDRAM申请内存
	sta=SD_ReadDisk(buf,secaddr,seccnt);//读取secaddr扇区开始的内容
	if(sta==0)			
	{	 
		printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)printf("%x ",buf[i]);//打印secaddr开始的扇区数据    	   
		printf("\r\nDATA ENDED\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);	//释放内存	   
}

//测试SD卡的写入(慎用,最好写全是0XFF的扇区,否则可能损坏SD卡.)
//从secaddr地址开始,写入seccnt个扇区的数据
//secaddr:扇区地址
//seccnt:扇区数
void sd_test_write(u32 secaddr,u32 seccnt)
{
	u32 i;
	u8 *buf;
	u8 sta=0;
	buf=mymalloc(SRAMEX,seccnt*512);	//从SDRAM申请内存
	for(i=0;i<seccnt*512;i++) 			//初始化写入的数据,是3的倍数.
	{
		buf[i]=i*3;
	}
	sta=SD_WriteDisk(buf,secaddr,seccnt);//从secaddr扇区开始写入seccnt个扇区内容
	if(sta==0)							
	{	 
		printf("Write over!\r\n"); 
	}else printf("err:%d\r\n",sta);
	myfree(SRAMEX,buf);					//释放内存	   
}
int main(void)
{
    u8 key;		 
	u32 sd_size;
	u8 t=0;	
	u8 *buf;
	
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	usmart_dev.init(200); 		    		//初始化USMART	
	LED_Init();								//初始化LED
	KEY_Init();								//初始化按键
	SDRAM_Init();                   		//初始化SDRAM
	LCD_Init();								//初始化LCD
    my_mem_init(SRAMIN);            		//初始化内部内存池
    my_mem_init(SRAMEX);            		//初始化外部SDRAM内存池
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"Aopllo STM32H7");	
	LCD_ShowString(30,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/8/15");   
	LCD_ShowString(30,130,200,16,16,"KEY0:Read Sector 0");	   
 	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0_Toggle;//DS0闪烁
	}
	show_sdcard_info();	//打印SD卡相关信息
 	POINT_COLOR=BLUE;	//设置字体为蓝色 
	//检测SD卡成功 											    
	//检测SD卡成功 											    
	LCD_ShowString(30,150,200,16,16,"SD Card OK    ");
	LCD_ShowString(30,170,200,16,16,"SD Card Size:     MB");
	LCD_ShowNum(30+13*8,170,SDCardInfo.CardCapacity>>20,5,16);//显示SD卡容量	
    while(1)
    {
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下了
		{
			buf=mymalloc(0,512);		//申请内存
			if(SD_ReadDisk(buf,0,1)==0)	//读取0扇区的内容
			{	
				LCD_ShowString(30,190,200,16,16,"USART1 Sending Data...");
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//打印0扇区数据    	   
				printf("\r\nDATA ENDED\r\n");
				LCD_ShowString(30,190,200,16,16,"USART1 Send Data Over!");
			}
			myfree(0,buf);//释放内存	   
		}   
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED0_Toggle;
			t=0;
		}
	}    
}

