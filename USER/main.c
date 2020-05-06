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
#include "w25qxx.h"
#include "sdmmc_sdcard.h"
#include "nand.h"    
#include "ftl.h"  
#include "ff.h"
#include "touch.h"
#include "exfuns.h"
/************************************************
 ALIENTEK 阿波罗STM32H7开发板 实验42
 FATFS实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#include "tkc/thread.h"
#include "platforms/common/rtos.h"

extern void sleep_ms(int ms);
extern void sys_tick_init(int SYSCLK);
extern ret_t platform_prepare(void);
extern void systick_enable_int(void);
extern int gui_app_start(int lcd_w, int lcd_h);

void* awtk_thread(void* args) {
  gui_app_start(lcdltdc.width, lcdltdc.height);

  return NULL;
}

static ret_t awtk_start_ui_thread(void) {
  tk_thread_t* ui_thread = tk_thread_create(awtk_thread, NULL);
  return_value_if_fail(ui_thread != NULL, RET_BAD_PARAMS);

  tk_thread_set_priority(ui_thread, 3);
  tk_thread_set_name(ui_thread, "awtk");
  tk_thread_set_stack_size(ui_thread, 0x8000);

  return tk_thread_start(ui_thread);
}

int main(void)
{
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	
	
	Cache_Enable();                	
	MPU_Memory_Protection();        
	HAL_Init();				        		
	Stm32_Clock_Init(160,5,2,4); 
	delay_init(400);						
	uart_init(115200);						
	usmart_dev.init(200); 		
	LED_Init();								
	KEY_Init();								
	SDRAM_Init();      
	LCD_Init();								
  W25QXX_Init();				
	LTDC_Display_Dir(1);	
	sys_tick_init(400);
	
	tp_dev.init();
	
  platform_prepare();
		
  rtos_init();
  awtk_start_ui_thread();
  rtos_start();
	
	LCD_ShowString(30,50,200,16,16,"Apollo STM32H7"); 
	LCD_ShowString(30,70,200,16,16,"FATFS TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2017/8/16");	 	 
	LCD_ShowString(30,130,200,16,16,"Use USMART for test");	      
 	while(SD_Init())//检测不到SD卡
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(30,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0_Toggle;//DS0闪烁
	}
    FTL_Init();
 	exfuns_init();							//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 					//挂载SD卡 
 	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		LCD_ShowString(30,150,200,16,16,"Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",FM_ANY,0,fatbuf,FF_MAX_SS);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(30,150,200,16,16,"Flash Disk Format Finish");	//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"Flash Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}	
	res=f_mount(fs[2],"2:",1); 				//挂载NAND FLASH.	
	if(res==0X0D)//NAND FLASH磁盘,FAT文件系统错误,重新格式化NAND FLASH
	{
		LCD_ShowString(30,150,200,16,16,"NAND Disk Formatting...");//格式化NAND
		res=f_mkfs("2:",FM_ANY,0,fatbuf,FF_MAX_SS);	//格式化FLASH,2,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"2:NANDDISK");	//设置Flash磁盘的名字为：NANDDISK
			LCD_ShowString(30,150,200,16,16,"NAND Disk Format Finish");		//格式化完成
		}else LCD_ShowString(30,150,200,16,16,"NAND Disk Format Error ");	//格式化失败
		delay_ms(1000);
	}    
	LCD_Fill(30,150,240,150+16,WHITE);		//清除显示	
	while(exf_getfree("0:",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(30,150,240,150+16,WHITE);	//清除显示			  
		delay_ms(200);
		LED0_Toggle;//DS0闪烁
	}													  			    
 	POINT_COLOR=BLUE;//设置字体为蓝色	   
	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
 	LCD_ShowNum(30+8*14,170,total>>10,5,16);	//显示SD卡总容量 MB
 	LCD_ShowNum(30+8*14,190,free>>10,5,16);     //显示SD卡剩余容量 MB			    
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED0_Toggle;
	} 		 
}

