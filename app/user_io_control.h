#ifndef USER_IO_CONTROL_H
#define USER_IO_CONTROL_H
extern xTaskHandle IO_TASK_PCB;//IO线程句柄
extern SemaphoreHandle_t serial_sem;//串口资源互斥量
void io_test();

#endif