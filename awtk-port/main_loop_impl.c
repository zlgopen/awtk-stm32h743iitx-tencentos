/**
 * file:   main_loop_stm32_raw.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  main loop for stm32
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-05-11 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "ltdc.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "string.h"
#include "ltdc.h"
#include "main_loop/main_loop_simple.h"

lcd_t* lcd_impl_create(wh_t w, wh_t h);

uint8_t platform_disaptch_input(main_loop_t* loop) {
	/*TODO*/

  return 0;
}

lcd_t* platform_create_lcd(wh_t w, wh_t h) {
	return lcd_impl_create(w, h);
}

#include "main_loop/main_loop_raw.inc"
