#include <pico/stdlib.h>
#ifdef CYW43_WL_GPIO_LED_PIN
    #include "pico/cyw43_arch.h"
#endif


//
//  main entry point
//

int main(int argc, char *argv[])
{
    stdio_init_all();
    
    
    // standard pico will define this
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // pico w will define this
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_init();

#endif


    uint64_t now;
    uint64_t ledTime = 0;  // force led change on first check

    while(true)
    {
        now = time_us_64();

        
        if(now >= ledTime)
        {
            ledTime += 500000;  // wait a half second

#if defined(PICO_DEFAULT_LED_PIN)
            gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));

#elif defined(CYW43_WL_GPIO_LED_PIN)
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));

#endif
        }
    }

    
    return 0;
}
