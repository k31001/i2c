/**
 * @file gpio.c
 * @brief STM32F411 GPIO 드라이버 구현
 * @author 
 * @date 2025-03-15
 */

#include "gpio.h"
#include "rcc.h"

/**
 * @brief  GPIO 핀을 초기화합니다.
 * @param  GPIOx: 초기화할 GPIO 포트 (GPIOA-GPIOH)
 * @param  config: GPIO 초기화 설정 구조체 포인터
 * @return None
 * @note   이 함수는 GPIO 포트의 클럭을 활성화하고 지정된 설정으로 초기화합니다.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config)
{
    uint32_t position = 0;
    uint32_t temp = 0;
    
    // GPIO 포트의 클럭 활성화
    if (GPIOx == GPIOA) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    } else if (GPIOx == GPIOB) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    } else if (GPIOx == GPIOC) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    } else if (GPIOx == GPIOD) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    } else if (GPIOx == GPIOE) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    } else if (GPIOx == GPIOH) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    }
    
    // 핀 모드 설정
    if (config->Pin <= 15) {
        // 단일 핀 설정
        position = config->Pin;
        
        // 모드 설정 (MODER)
        temp = GPIOx->MODER.w;
        temp &= ~(3U << (position * 2));  // 해당 핀의 모드 비트 클리어
        temp |= (config->Mode << (position * 2));  // 새 모드 설정
        GPIOx->MODER.w = temp;
        
        // 출력 타입 설정 (OTYPER)
        temp = GPIOx->OTYPER.w;
        temp &= ~(1U << position);  // 해당 핀의 출력 타입 비트 클리어
        temp |= (config->Otype << position);  // 새 출력 타입 설정
        GPIOx->OTYPER.w = temp;
        
        // 출력 속도 설정 (OSPEEDR)
        temp = GPIOx->OSPEEDR;
        temp &= ~(3U << (position * 2));  // 해당 핀의 속도 비트 클리어
        temp |= (config->Speed << (position * 2));  // 새 속도 설정
        GPIOx->OSPEEDR = temp;
        
        // 풀업/풀다운 설정 (PUPDR)
        temp = GPIOx->PUPDR;
        temp &= ~(3U << (position * 2));  // 해당 핀의 풀업/풀다운 비트 클리어
        temp |= (config->PuPd << (position * 2));  // 새 풀업/풀다운 설정
        GPIOx->PUPDR = temp;
        
        // 대체 기능 설정 (AFRL/AFRH)
        if (config->Mode == GPIO_MODE_ALT) {
            if (position < 8) {
                // AFRL 레지스터 (핀 0-7)
                temp = GPIOx->AFR[0];
                temp &= ~(0xF << (position * 4));  // 해당 핀의 AF 비트 클리어
                temp |= (config->AF << (position * 4));  // 새 AF 설정
                GPIOx->AFR[0] = temp;
            } else {
                // AFRH 레지스터 (핀 8-15)
                temp = GPIOx->AFR[1];
                temp &= ~(0xF << ((position - 8) * 4));  // 해당 핀의 AF 비트 클리어
                temp |= (config->AF << ((position - 8) * 4));  // 새 AF 설정
                GPIOx->AFR[1] = temp;
            }
        }
    } else {
        // 여러 핀 설정
        for (position = 0; position < 16; position++) {
            if (config->Pin & (1 << position)) {
                // 모드 설정 (MODER)
                temp = GPIOx->MODER.w;
                temp &= ~(3U << (position * 2));  // 해당 핀의 모드 비트 클리어
                temp |= (config->Mode << (position * 2));  // 새 모드 설정
                GPIOx->MODER.w = temp;
                
                // 출력 타입 설정 (OTYPER)
                temp = GPIOx->OTYPER.w;
                temp &= ~(1U << position);  // 해당 핀의 출력 타입 비트 클리어
                temp |= (config->Otype << position);  // 새 출력 타입 설정
                GPIOx->OTYPER.w = temp;
                
                // 출력 속도 설정 (OSPEEDR)
                temp = GPIOx->OSPEEDR;
                temp &= ~(3U << (position * 2));  // 해당 핀의 속도 비트 클리어
                temp |= (config->Speed << (position * 2));  // 새 속도 설정
                GPIOx->OSPEEDR = temp;
                
                // 풀업/풀다운 설정 (PUPDR)
                temp = GPIOx->PUPDR;
                temp &= ~(3U << (position * 2));  // 해당 핀의 풀업/풀다운 비트 클리어
                temp |= (config->PuPd << (position * 2));  // 새 풀업/풀다운 설정
                GPIOx->PUPDR = temp;
                
                // 대체 기능 설정 (AFRL/AFRH)
                if (config->Mode == GPIO_MODE_ALT) {
                    if (position < 8) {
                        // AFRL 레지스터 (핀 0-7)
                        temp = GPIOx->AFR[0];
                        temp &= ~(0xF << (position * 4));  // 해당 핀의 AF 비트 클리어
                        temp |= (config->AF << (position * 4));  // 새 AF 설정
                        GPIOx->AFR[0] = temp;
                    } else {
                        // AFRH 레지스터 (핀 8-15)
                        temp = GPIOx->AFR[1];
                        temp &= ~(0xF << ((position - 8) * 4));  // 해당 핀의 AF 비트 클리어
                        temp |= (config->AF << ((position - 8) * 4));  // 새 AF 설정
                        GPIOx->AFR[1] = temp;
                    }
                }
            }
        }
    }
}

/**
 * @brief  GPIO 핀 설정을 초기 상태로 재설정합니다.
 * @param  GPIOx: 초기화할 GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 초기화할 핀 번호 (0-15)
 * @return None
 */
void GPIO_DeInit(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    uint32_t position = 0;
    
    if (pin <= 15) {
        // 단일 핀 설정
        position = pin;
        
        // 모드 설정 (MODER) - 입력 모드로 설정
        GPIOx->MODER.w &= ~(3U << (position * 2));
        
        // 출력 타입 설정 (OTYPER) - 푸시풀로 설정
        GPIOx->OTYPER.w &= ~(1U << position);
        
        // 출력 속도 설정 (OSPEEDR) - 저속으로 설정
        GPIOx->OSPEEDR &= ~(3U << (position * 2));
        
        // 풀업/풀다운 설정 (PUPDR) - 풀업/풀다운 없음으로 설정
        GPIOx->PUPDR &= ~(3U << (position * 2));
        
        // 대체 기능 설정 (AFRL/AFRH) - AF0으로 설정
        if (position < 8) {
            GPIOx->AFR[0] &= ~(0xF << (position * 4));
        } else {
            GPIOx->AFR[1] &= ~(0xF << ((position - 8) * 4));
        }
    } else {
        // 여러 핀 설정
        for (position = 0; position < 16; position++) {
            if (pin & (1 << position)) {
                // 모드 설정 (MODER) - 입력 모드로 설정
                GPIOx->MODER.w &= ~(3U << (position * 2));
                
                // 출력 타입 설정 (OTYPER) - 푸시풀로 설정
                GPIOx->OTYPER.w &= ~(1U << position);
                
                // 출력 속도 설정 (OSPEEDR) - 저속으로 설정
                GPIOx->OSPEEDR &= ~(3U << (position * 2));
                
                // 풀업/풀다운 설정 (PUPDR) - 풀업/풀다운 없음으로 설정
                GPIOx->PUPDR &= ~(3U << (position * 2));
                
                // 대체 기능 설정 (AFRL/AFRH) - AF0으로 설정
                if (position < 8) {
                    GPIOx->AFR[0] &= ~(0xF << (position * 4));
                } else {
                    GPIOx->AFR[1] &= ~(0xF << ((position - 8) * 4));
                }
            }
        }
    }
}

/**
 * @brief  GPIO 핀의 출력 값을 설정합니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @param  state: 설정할 상태 (0: Low, 1: High)
 * @return None
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state)
{
    if (state) {
        // 핀을 High로 설정
        GPIOx->BSRR = (uint32_t)pin;
    } else {
        // 핀을 Low로 설정 (상위 16비트에 1을 쓰면 해당 핀이 리셋됨)
        GPIOx->BSRR = (uint32_t)pin << 16;
    }
}

/**
 * @brief  GPIO 핀의 입력 값을 읽습니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @return 핀의 입력 상태 (0: Low, 1: High)
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    // 핀 번호에 해당하는 비트만 읽어서 0이 아닌지 확인
    if ((GPIOx->IDR & pin) != 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief  GPIO 핀의 상태를 반전시킵니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  pin: 핀 번호 (0-15)
 * @return None
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    // 현재 ODR 값을 읽고 해당 핀의 비트를 XOR 연산으로 반전
    GPIOx->ODR ^= pin;
}

/**
 * @brief  GPIO 포트의 출력 값을 설정합니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @param  value: 설정할 16비트 값
 * @return None
 */
void GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value)
{
    // 포트 전체 16비트 출력 값 설정
    GPIOx->ODR = value;
}

/**
 * @brief  GPIO 포트의 입력 값을 읽습니다.
 * @param  GPIOx: GPIO 포트 (GPIOA-GPIOH)
 * @return 포트의 16비트 입력 값
 */
uint16_t GPIO_ReadPort(GPIO_TypeDef *GPIOx)
{
    // 포트 전체 16비트 입력 값 반환
    return (uint16_t)GPIOx->IDR;
}