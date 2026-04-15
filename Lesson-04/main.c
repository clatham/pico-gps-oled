#include <pico/stdlib.h>

#ifdef CYW43_WL_GPIO_LED_PIN
    #include "pico/cyw43_arch.h"
#endif


//
//  main entry point
//

int main(int argc, char *argv[])
{
    // setup anything our program needs


    stdio_init_all();
    
    
    // standard pico will define this
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // pico w will define this
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_init();

#endif


    // infinite loop that doesn't need to return
    
    while(true)
    {
#if defined(PICO_DEFAULT_LED_PIN)
        gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));

#elif defined(CYW43_WL_GPIO_LED_PIN)
         cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, ! cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));

#endif

        printf("Hello, world!\n");
    }

    
    return 0;
}
