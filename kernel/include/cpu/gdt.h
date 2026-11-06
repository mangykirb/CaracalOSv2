#ifndef CARACAL_GDT_H
#define CARACAL_GDT_H

#include <stdint.h>

void gdt_init(void);
uint16_t tss_get_tr(void);

#endif
