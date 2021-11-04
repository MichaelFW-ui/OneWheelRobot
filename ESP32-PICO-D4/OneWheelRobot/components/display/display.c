#include <stdio.h>
#include "display.h"
#include "sdkconfig.h"

void func(void)
{
    printf("%d\n", CONFIG_DISPLAY_I2C_PORT);
}
