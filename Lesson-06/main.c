#include <pico/stdlib.h>
#ifdef CYW43_WL_GPIO_LED_PIN
    #include "pico/cyw43_arch.h"
#endif
#include "ssd1306.h"
#include "pico-ssd1306-spi/example/BMSPA_font.h"

/* SPI pin definitions */
#define SPI_PORT spi0
#define PIN_CS   17   /* Chip Select (directly connect to GND if only one device) */
#define PIN_SCK  18   /* SPI clock */
#define PIN_MOSI 19   /* SPI MOSI (data) */
#define PIN_DC   20   /* Data/Command */
#define PIN_RST  21   /* Reset */


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


    /* Initialize SPI at 1MHz */
    spi_init(SPI_PORT, 1000000);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    /* Chip select - optional, can be tied to GND */
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 0);  /* Select the device */
    
    
    ssd1306_t disp;
    disp.external_vcc = false;
    
    ssd1306_init(&disp, 128, 64, PIN_DC, PIN_RST, SPI_PORT);


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
            
            ssd1306_clear(&disp);
            
            ssd1306_draw_string(&disp, 24, 24, 1, "Hello, world!");
            
            char buf[10];
            snprintf(buf, 10, "%llu", now);
            ssd1306_draw_string(&disp, 40, 32, 1, buf);
            
            ssd1306_show(&disp);
        }
    }

    
    return 0;
}
