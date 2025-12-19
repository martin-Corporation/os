#pragma once

#include <stdint.h>

void pic_configure(uint8_t offset_pic_1, uint8_t offset_pic_2);
void pic_send_end_of_interrupt(int irq);
void pic_disable();
void pic_mask(int irq);
void pic_unmask(int irq);
uint16_t pic_read_irq_request_register();
uint16_t pic_read_in_service_register();
