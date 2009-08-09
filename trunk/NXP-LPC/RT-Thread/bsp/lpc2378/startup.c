/*
 * File      : startup.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     xuxinming    first version
 */

#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_FINSH
#include <finsh.h>
extern void finsh_system_init(void);
#endif

#include <LPC23xx.h>
#include <board.h>

/**
 * @addtogroup LPC2478
 */
/*@{*/

extern int  rt_application_init(void);
#ifdef RT_USING_DEVICE
extern rt_err_t rt_hw_serial_init(void);
#endif

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#else
extern int __bss_end;
#endif
 
 
/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
	/* init hardware interrupt */
	rt_hw_interrupt_init();
	
	/* init board */
	rt_hw_board_init();
	
	/* init tick */
	rt_system_tick_init();
	
	/* init kernel object */
	rt_system_object_init();
	
	rt_show_version();

	/* init timer system */
	rt_system_timer_init();

	/* init memory system */
#ifdef RT_USING_HEAP
#ifdef __CC_ARM
 	rt_system_heap_init((void*)(&Image$$RW_IRAM1$$ZI$$Limit ), (void*)0x40008000) ;
//	rt_system_heap_init((void*)0x40005200, (void*)0x40007000) ;
#else
	rt_system_heap_init((void*)&__bss_end, (void*)0x40008000);
#endif
#endif

	/* init scheduler system */
	rt_system_scheduler_init();

 #ifdef RT_USING_LWIP
	/* register ethernetif device */ 
	eth_system_device_init();

 	rt_hw_eth_init();
#endif	

#ifdef RT_USING_DEVICE
	/* init hardware serial device */
//	rt_hw_serial_init();

	/*init all registed devices*/
	rt_device_init_all();
#endif

	/* init application */
	rt_application_init();

#ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
	finsh_set_device("uart1");
#endif
	
	/* init idle thread */
	rt_thread_idle_init();

	/* start scheduler */
	rt_system_scheduler_start();	

	/* never reach here */
	return ;
}

#ifdef __CC_ARM
int main(void)
{
	rtthread_startup();
	
	return 0;
}
#endif

/*@}*/
