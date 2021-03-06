#include <stdlib.h>
#include <stdint.h>

/* AHB1 Base Addresses ******************************************************/
#define STM32_RCC_BASE 0x40023800 /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* AHB2 Base Addresses ******************************************************/
#define STM32_GPIOA_BASE 0x40020000U /* 0x40020000U-0x400203FFU: GPIO Port A */
#define STM32_GPIOC_BASE 0x40020800U /* 0x40020800-0x40020BFF: GPIO Port C */

/* Register Offsets *********************************************************/
#define STM32_RCC_AHB1ENR_OFFSET 0x0030 // AHB1 Peripheral Clock enableregister
#define STM32_GPIO_MODER_OFFSET 0x0000  // GPIO port mode register
#define STM32_GPIO_OTYPER_OFFSET 0x0004 // GPIO port output type register
#define STM32_GPIO_PUPDR_OFFSET 0x000c  // GPIO port pull-up/pull-downregister
#define STM32_GPIO_IDR_OFFSET 0x0010    // GPIO port input data register
#define STM32_GPIO_ODR_OFFSET 0x0014    // GPIO port output data register
#define STM32_GPIO_BSRR_OFFSET 0x0018   // GPIO port bit set/reset register

//      //REMEMBER//    //
// MODER: Define como o pino vai ser usado (entrada, saída, analógico ou função alternativa).
// OTYPER: Se saída, seleciona a topologia (push pull ou open drain).
// OSPEEDR: Seleciona a máxima velocidade possível para o pino, quando configurado como saída.
// PUPDR: Seleciona resistores de pull up ou pull down para o pino (tanto para saída quanto entrada).
// IDR: Permite a leitura do valor do pino, quando o GPIO é configurado como entrada.
// ODR: Seta o valor do pino, quando o GPIO está configurado como saída. Também pode ser lido, para se saber o valor atual da saída.
// BSRR: Permite também a escrita de valores nos pinos de saída mas de forma atômica e individualizada.
// LCKR: Permite travar a configuração de um determinado pino, evitando mudanças indevidas por bugs, por exemplo.
// AFR[2]: Realiza a seleção da função alternativa para o pino.

/* Register Addresses *******************************************************/
#define STM32_RCC_AHB1ENR (STM32_RCC_BASE + STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOA_MODER   (STM32_GPIOA_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_OTYPER  (STM32_GPIOA_BASE + STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOA_PUPDR   (STM32_GPIOA_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_IDR     (STM32_GPIOA_BASE + STM32_GPIO_IDR_OFFSET)
#define STM32_GPIOA_ODR     (STM32_GPIOA_BASE + STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOA_BSRR    (STM32_GPIOA_BASE + STM32_GPIO_BSRR_OFFSET)

#define STM32_GPIOC_MODER (STM32_GPIOC_BASE + STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER (STM32_GPIOC_BASE + STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR (STM32_GPIOC_BASE + STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR (STM32_GPIOC_BASE + STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */
#define RCC_AHB1ENR_GPIOAEN (1 << 0) /* Bit 0: IO port A clock enable */
#define RCC_AHB1ENR_GPIOCEN (1 << 2) /* Bit 2: IO port C clock enable */

/* GPIO port mode register */
#define GPIO_MODER_INPUT (0)  // Input
#define GPIO_MODER_OUTPUT (1) // General purpose output mode
#define GPIO_MODER_ALT (2)    // Alternate mode
#define GPIO_MODER_ANALOG (3) // Analog mode

/* GPIO port registers configs */
#define GPIO_MODERA0_SHIFT (0) // MODDER 0
#define GPIO_MODERA0_MASK (3 << GPIO_MODERA0_SHIFT)

#define GPIO_MODERC13_SHIFT (26)
#define GPIO_MODERC13_MASK (3 << GPIO_MODERC13_SHIFT)

/* GPIO port output type register */
#define GPIO_OTYPER_PP (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD (1) /* 1=Output open-drain */

#define GPIO_OTC13_SHIFT (13)
#define GPIO_OTC13_MASK (1 << GPIO_OTC13_SHIFT)

/* GPIO port pull-up/pull-down register */
#define GPIO_PUPDR_NONE (0)     /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP (1)   /* Pull-up */
#define GPIO_PUPDR_PULLDOWN (2) /* Pull-down */

#define GPIO_PUPDRA0_SHIFT (0)
#define GPIO_PUPDRA0_MASK (3 << GPIO_PUPDRA0_SHIFT)

#define GPIO_PUPDRC13_SHIFT (26)
#define GPIO_PUPDRC13_MASK (3 << GPIO_PUPDRC13_SHIFT) //Why reserved? see later this ************//

/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n) (1 << (n))
#define GPIO_BSRR_RST(n) (1 << (n + 16))

/* GPIO por IDR register */
#define GPIO_IDR0_SHIFT (0)
#define GPIO_IDRA0_MASK (1 << GPIO_IDR0_SHIFT)


// LED DELAY
#define LED_DELAY 90000

// .bss & .data tests
static uint32_t led_status, KEY_status;

int main(int argc, char *argv[])
{
    uint32_t led_time = LED_DELAY;
    uint32_t reg;

    /* Ponteiros para registradores */
    uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    uint32_t *pGPIOC_MODER = (uint32_t *)STM32_GPIOC_MODER;
    uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
    uint32_t *pGPIOC_PUPDR = (uint32_t *)STM32_GPIOC_PUPDR;
    uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;

    uint32_t *pGPIOA_MODER = (uint32_t *)STM32_GPIOA_MODER;
    uint32_t *pGPIOA_PUPDR = (uint32_t *)STM32_GPIOA_PUPDR;
    uint32_t *pGPIOA_IDR = (uint32_t *) STM32_GPIOA_IDR;

    // Habilitar o clock GPIOC
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN; // reg = reg | (1 << 2)
    *pRCC_AHB1ENR = reg;

    // Configura PC13 como saida pull-up off e pull-down off
    reg = *pGPIOC_MODER;
    reg &= ~(GPIO_MODERC13_MASK);
    reg |= (GPIO_MODER_OUTPUT << GPIO_MODERC13_SHIFT);
    *pGPIOC_MODER = reg;

    reg = *pGPIOC_OTYPER;
    reg &= ~(GPIO_OTC13_MASK);
    reg |= (GPIO_OTYPER_PP << GPIO_OTC13_SHIFT);
    *pGPIOC_OTYPER = reg;

    reg = *pGPIOC_PUPDR;
    reg &= ~(GPIO_PUPDRC13_MASK);
    reg |= (GPIO_PUPDR_NONE << GPIO_PUPDRC13_SHIFT);
    *pGPIOC_PUPDR = reg;

    // Habilitar o clock GPIOA
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOAEN;
    *pRCC_AHB1ENR = reg;

    // Configura PA0 como entrada Pull-up
    reg = *pGPIOA_MODER;
    reg &= ~(GPIO_MODERA0_MASK);
    reg |= (GPIO_MODER_INPUT << GPIO_MODERA0_SHIFT);
    *pGPIOA_MODER = reg;

    reg = *pGPIOA_PUPDR;
    reg &= ~(GPIO_PUPDRA0_MASK);
    reg |= (GPIO_PUPDR_PULLUP << GPIO_PUPDRA0_SHIFT);
    *pGPIOA_PUPDR = reg;


    while (1)
    {
        /* GPIOA0 register read */
        KEY_status = (GPIO_IDRA0_MASK & *pGPIOA_IDR); //Every AHB Cycle KEY set to 0 or not
        if(KEY_status == 0) //KEY button pressed // Frequency (-) port input 0 
        {
            led_time = LED_DELAY;
        }
        else // KEY button released // Frequency (+) port input 1
        {
            led_time = LED_DELAY/2;
        }

        *pGPIOC_BSRR = GPIO_BSRR_SET(13);
        led_status = 0;
        for (uint32_t i = 0; i < led_time; i++)
            if(KEY_status != (GPIO_IDRA0_MASK & *pGPIOA_IDR)) break;

        *pGPIOC_BSRR = GPIO_BSRR_RST(13);
        led_status = 1;
        for (uint32_t i = 0; i < led_time; i++)
            if(KEY_status != (GPIO_IDRA0_MASK & *pGPIOA_IDR)) break;
        
    }

    return EXIT_SUCCESS;
}
