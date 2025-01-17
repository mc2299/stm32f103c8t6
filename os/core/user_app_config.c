//#include "user_app_config.h"
//freertos kernel head file
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "xprintf.h"
#include "user_usart.h"
//freertos app head file
#include "user_app_common.h"
#include "tcpip.h"
#include "init.h"
int freertos_app_add();

const sys_base_event_t sys_init_ok = SYS_INIT_OK;//枚举
const sys_base_event_t flash_init_ok = FLASH_INIT_OK;//枚举
const sys_base_event_t fs_mount_ok = FS_MOUNT_OK;//枚举
const sys_base_event_t fs_file_operate_ok = FS_FILE_OPERATE_OK;//枚举
const sys_base_event_t io_operate_ok = IO_OPERATE_OK;//枚举


void net_init_done_callback(void *args){
	xprintf_s("lwip init success\r\n");
}

//初始化参数和内核参数，并且添加app
int software_init(){
	//设置io流载体
	xdev_out(usart1_obj.u_putc);
	xdev_in(usart1_obj.u_getc);
	//创建事件组
	sys_base_event_group = xEventGroupCreate();
	//将事件置位
	xEventGroupSetBits(sys_base_event_group,sys_init_ok);
	//串口资源互斥量初始化
	serial_sem = xSemaphoreCreateBinary();
	//post
	xSemaphoreGive(serial_sem);
	//初始化文件系统
	
	//初始化lwip
	//lwip_init();
	//tcpip_init(net_init_done_callback,0);
	freertos_app_add();
}
//添加app,创建任务						
int freertos_app_add(){
	xTaskCreate(	show_sys_clock,
							"show_sys_clock_task",	
							128,
							NULL,
							3,
							&RCC_SYS_CLOCK );
	xTaskCreate(	open_led,
							"open_led_task",	
							128,
							NULL,
							3,
							&OPEN_LED_Task_TCB );
	xTaskCreate(	usart_heart_beat,
							"usart_heart_beat_task",	
							128,
							NULL,
							3,
							&USART_HEART_BEAT );					
	xTaskCreate(	pwr_mode_switch,
							"pwr_mode_switch_task",	
							128,
							NULL,
							3,
							&PWR_MODE_SWITCH );
	xTaskCreate(	pwr_mode_send_msg,
							"pwr_mode_send_msg_task",	
							128,
							NULL,
							3,
							&PWR_MODE_SEND_MSG );
	xTaskCreate(	crc_cal,
							"crc_cal_task",	
							128,
							NULL,
							3,
							&CRC_CAL );
  xTaskCreate(	crc_post_binary_sem,
							"crc_post_binary_sem_task",	
							128,
							NULL,
							3,
							&CRC_POST_BINARY_SEM );
	xTaskCreate(	flash_test,
							"flash_size_task",	
							128,
							NULL,
							3,
							&FLASH_SIZE_TASK_PCB );		
	xTaskCreate(fs_test,
							"fs_test_task",	
							128,
							NULL,
							3,
							&FS_TEST_TASK_PCB );
	xTaskCreate(io_test,
							"io_test_task",	
							128,
							NULL,
							3,
							&IO_TASK_PCB );
	xTaskCreate(shell_test,
							"shell_test_task",	
							512,
							NULL,
							3,
							&Shell_Test_Task_TCB );
	xTaskCreate(exti_test,
							"exti_test_task",	
							128,
							NULL,
							3,
							&Exti_Test_Task_TCB );
	xTaskCreate(basic_timer_test,
							"basic_timer_test_task",	
							128,
							NULL,
							3,
							&Basic_Timer_Test_Task_TCB );
	xTaskCreate(device_test,
							"device_test_task",	
							512,
							NULL,
							3,
							&Device_Test_Task_TCB );
	xTaskCreate(tcp_server,
							"tcp_server_task",	
							256,
							NULL,
							3,
							&Tcp_Server_Task_TCB );
	return 0;
}
//任务堆栈溢出检查，溢出后执行本函数
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ){
		xprintf_s("task %s stackoverflow!\r\n",pcTaskName);
}
