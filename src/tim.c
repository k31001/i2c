#include "tim.h"
#include "rcc.h"

/**
 * @brief 타이머 인터럽트 정의
 */
#define TIM_IT_UPDATE                      (0x0001)
#define TIM_IT_CC1                         (0x0002)
#define TIM_IT_CC2                         (0x0004)
#define TIM_IT_CC3                         (0x0008)
#define TIM_IT_CC4                         (0x0010)
#define TIM_IT_COM                         (0x0020)
#define TIM_IT_TRIGGER                     (0x0040)
#define TIM_IT_BREAK                       (0x0080)

/**
 * @brief 타이머 기본 기능을 초기화합니다.
 */
void TIM_Base_Init(TIM_TypeDef *TIMx, TIM_Base_Config *config)
{
    // 타이머 클럭 활성화
    if (TIMx == TIM1) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    } else if (TIMx == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (TIMx == TIM3) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (TIMx == TIM4) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    } else if (TIMx == TIM5) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    } else if (TIMx == TIM9) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    } else if (TIMx == TIM10) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    } else if (TIMx == TIM11) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    }
    
    // 타이머 리셋
    TIMx->CR1 = 0;
    TIMx->CR2 = 0;
    TIMx->SMCR = 0;
    TIMx->DIER = 0;
    TIMx->SR = 0;
    
    // 프리스케일러 설정
    TIMx->PSC = config->Prescaler;
    
    // 자동 리로드 값 설정
    TIMx->ARR = config->Period;
    
    // 카운터 모드 설정
    uint32_t tmpcr1 = TIMx->CR1;
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    
    switch (config->CounterMode) {
        case TIM_COUNTER_UP:
            // 이미 0으로 설정되어 있으므로 필요 없음
            break;
        case TIM_COUNTER_DOWN:
            tmpcr1 |= TIM_CR1_DIR;
            break;
        case TIM_COUNTER_CENTER_ALIGNED1:
            tmpcr1 |= TIM_CR1_CMS_0;
            break;
        case TIM_COUNTER_CENTER_ALIGNED2:
            tmpcr1 |= TIM_CR1_CMS_1;
            break;
        case TIM_COUNTER_CENTER_ALIGNED3:
            tmpcr1 |= TIM_CR1_CMS;
            break;
        default:
            break;
    }
    
    // 클럭 분주기 설정
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)config->ClockDivision;
    
    // 자동 리로드 프리로드 설정
    if (config->AutoReloadPreload == TIM_AUTORELOAD_PRELOAD_ENABLE) {
        tmpcr1 |= TIM_CR1_ARPE;
    } else {
        tmpcr1 &= ~TIM_CR1_ARPE;
    }
    
    TIMx->CR1 = tmpcr1;
    
    // 고급 타이머인 경우 반복 카운터 설정 (TIM1)
    if (TIMx == TIM1) {
        TIMx->RCR = config->RepetitionCounter;
    }
    
    // 카운터를 재설정합니다.
    TIMx->CNT = 0;
    
    // 생성 이벤트를 생성하여 설정을 업데이트합니다.
    TIMx->EGR = TIM_EGR_UG;
}

/**
 * @brief 타이머 초기화를 해제합니다.
 */
void TIM_DeInit(TIM_TypeDef *TIMx)
{
    // 타이머 멈춤
    TIMx->CR1 &= ~TIM_CR1_CEN;
    
    // 모든 레지스터 초기화
    TIMx->CR1 = 0;
    TIMx->CR2 = 0;
    TIMx->SMCR = 0;
    TIMx->DIER = 0;
    TIMx->SR = 0;
    TIMx->EGR = 0;
    TIMx->CCMR1 = 0;
    TIMx->CCMR2 = 0;
    TIMx->CCER = 0;
    TIMx->CNT = 0;
    TIMx->PSC = 0;
    TIMx->ARR = 0xFFFF;
    TIMx->CCR1 = 0;
    TIMx->CCR2 = 0;
    TIMx->CCR3 = 0;
    TIMx->CCR4 = 0;
    
    // 고급 타이머인 경우 추가 레지스터 초기화 (TIM1)
    if (TIMx == TIM1) {
        TIMx->RCR = 0;
        TIMx->BDTR = 0;
    }
    
    // 타이머 클럭 비활성화
    if (TIMx == TIM1) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
    } else if (TIMx == TIM2) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
    } else if (TIMx == TIM3) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
    } else if (TIMx == TIM4) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;
    } else if (TIMx == TIM5) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;
    } else if (TIMx == TIM9) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;
    } else if (TIMx == TIM10) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN;
    } else if (TIMx == TIM11) {
        RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;
    }
}

/**
 * @brief 타이머를 시작합니다.
 */
void TIM_Start(TIM_TypeDef *TIMx)
{
    // 타이머 카운터 활성화
    TIMx->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 타이머를 중지합니다.
 */
void TIM_Stop(TIM_TypeDef *TIMx)
{
    // 타이머 카운터 비활성화
    TIMx->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 타이머 카운터 값을 설정합니다.
 */
void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t counter)
{
    TIMx->CNT = counter;
}

/**
 * @brief 타이머 카운터 값을 얻습니다.
 */
uint32_t TIM_GetCounter(TIM_TypeDef *TIMx)
{
    return TIMx->CNT;
}

/**
 * @brief 타이머 주기를 설정합니다.
 */
void TIM_SetPeriod(TIM_TypeDef *TIMx, uint32_t period)
{
    TIMx->ARR = period;
}

/**
 * @brief 타이머 주기를 얻습니다.
 */
uint32_t TIM_GetPeriod(TIM_TypeDef *TIMx)
{
    return TIMx->ARR;
}

/**
 * @brief 타이머 프리스케일러를 설정합니다.
 */
void TIM_SetPrescaler(TIM_TypeDef *TIMx, uint16_t prescaler)
{
    TIMx->PSC = prescaler;
}

/**
 * @brief 타이머 프리스케일러를 얻습니다.
 */
uint16_t TIM_GetPrescaler(TIM_TypeDef *TIMx)
{
    return TIMx->PSC;
}

/**
 * @brief 타이머 출력 비교 채널을 초기화합니다.
 */
void TIM_OC_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_OC_Config *config)
{
    uint32_t tmpccmrx = 0;
    uint32_t tmpccer = 0;
    
    // 채널을 비활성화
    TIMx->CCER &= ~(0xF << (channel * 4));
    
    // 채널에 따라 레지스터 구성
    switch (channel) {
        case TIM_CHANNEL_1:
            // 출력 비교 1 모드 설정
            tmpccmrx = TIMx->CCMR1;
            tmpccmrx &= ~TIM_CCMR1_OC1M;
            tmpccmrx |= (config->OCMode << 4);
            
            // 출력 비교 1 프리로드 활성화
            tmpccmrx |= TIM_CCMR1_OC1PE;
            
            TIMx->CCMR1 = tmpccmrx;
            
            // 출력 비교 1 펄스 값 설정
            TIMx->CCR1 = config->Pulse;
            
            // 캡처/비교 1 출력 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~TIM_CCER_CC1P;
            tmpccer |= (config->OCPolarity << 1);
            
            // 출력 상태 설정
            if (config->OutputState == TIM_OUTPUT_STATE_ENABLE) {
                tmpccer |= TIM_CCER_CC1E;
            }
            
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_2:
            // 출력 비교 2 모드 설정
            tmpccmrx = TIMx->CCMR1;
            tmpccmrx &= ~TIM_CCMR1_OC2M;
            tmpccmrx |= (config->OCMode << 12);
            
            // 출력 비교 2 프리로드 활성화
            tmpccmrx |= TIM_CCMR1_OC2PE;
            
            TIMx->CCMR1 = tmpccmrx;
            
            // 출력 비교 2 펄스 값 설정
            TIMx->CCR2 = config->Pulse;
            
            // 캡처/비교 2 출력 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~TIM_CCER_CC2P;
            tmpccer |= (config->OCPolarity << 5);
            
            // 출력 상태 설정
            if (config->OutputState == TIM_OUTPUT_STATE_ENABLE) {
                tmpccer |= TIM_CCER_CC2E;
            }
            
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_3:
            // 출력 비교 3 모드 설정
            tmpccmrx = TIMx->CCMR2;
            tmpccmrx &= ~TIM_CCMR2_OC3M;
            tmpccmrx |= (config->OCMode << 4);
            
            // 출력 비교 3 프리로드 활성화
            tmpccmrx |= TIM_CCMR2_OC3PE;
            
            TIMx->CCMR2 = tmpccmrx;
            
            // 출력 비교 3 펄스 값 설정
            TIMx->CCR3 = config->Pulse;
            
            // 캡처/비교 3 출력 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~TIM_CCER_CC3P;
            tmpccer |= (config->OCPolarity << 9);
            
            // 출력 상태 설정
            if (config->OutputState == TIM_OUTPUT_STATE_ENABLE) {
                tmpccer |= TIM_CCER_CC3E;
            }
            
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_4:
            // 출력 비교 4 모드 설정
            tmpccmrx = TIMx->CCMR2;
            tmpccmrx &= ~TIM_CCMR2_OC4M;
            tmpccmrx |= (config->OCMode << 12);
            
            // 출력 비교 4 프리로드 활성화
            tmpccmrx |= TIM_CCMR2_OC4PE;
            
            TIMx->CCMR2 = tmpccmrx;
            
            // 출력 비교 4 펄스 값 설정
            TIMx->CCR4 = config->Pulse;
            
            // 캡처/비교 4 출력 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~TIM_CCER_CC4P;
            tmpccer |= (config->OCPolarity << 13);
            
            // 출력 상태 설정
            if (config->OutputState == TIM_OUTPUT_STATE_ENABLE) {
                tmpccer |= TIM_CCER_CC4E;
            }
            
            TIMx->CCER = tmpccer;
            break;
            
        default:
            break;
    }
    
    // 고급 타이머인 경우 (TIM1) 메인 출력 활성화
    if (TIMx == TIM1) {
        TIMx->BDTR |= TIM_BDTR_MOE;
    }
}

/**
 * @brief 타이머 출력 비교 채널을 활성화합니다.
 */
void TIM_OC_EnableChannel(TIM_TypeDef *TIMx, TIM_Channel channel)
{
    switch (channel) {
        case TIM_CHANNEL_1:
            TIMx->CCER |= TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIMx->CCER |= TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIMx->CCER |= TIM_CCER_CC3E;
            break;
        case TIM_CHANNEL_4:
            TIMx->CCER |= TIM_CCER_CC4E;
            break;
        default:
            break;
    }
}

/**
 * @brief 타이머 출력 비교 채널을 비활성화합니다.
 */
void TIM_OC_DisableChannel(TIM_TypeDef *TIMx, TIM_Channel channel)
{
    switch (channel) {
        case TIM_CHANNEL_1:
            TIMx->CCER &= ~TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIMx->CCER &= ~TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIMx->CCER &= ~TIM_CCER_CC3E;
            break;
        case TIM_CHANNEL_4:
            TIMx->CCER &= ~TIM_CCER_CC4E;
            break;
        default:
            break;
    }
}

/**
 * @brief 타이머 출력 비교 펄스 값을 설정합니다.
 */
void TIM_OC_SetPulse(TIM_TypeDef *TIMx, TIM_Channel channel, uint32_t pulse)
{
    switch (channel) {
        case TIM_CHANNEL_1:
            TIMx->CCR1 = pulse;
            break;
        case TIM_CHANNEL_2:
            TIMx->CCR2 = pulse;
            break;
        case TIM_CHANNEL_3:
            TIMx->CCR3 = pulse;
            break;
        case TIM_CHANNEL_4:
            TIMx->CCR4 = pulse;
            break;
        default:
            break;
    }
}

/**
 * @brief 타이머 입력 캡처 채널을 초기화합니다.
 */
void TIM_IC_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_IC_Config *config)
{
    uint32_t tmpccmrx = 0;
    uint32_t tmpccer = 0;
    
    // 채널을 비활성화
    TIMx->CCER &= ~(0xF << (channel * 4));
    
    // 채널에 따라 레지스터 구성
    switch (channel) {
        case TIM_CHANNEL_1:
            // 입력 캡처 1 필터 및 프리스케일러 설정
            tmpccmrx = TIMx->CCMR1;
            tmpccmrx &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC);
            tmpccmrx |= (config->ICSelection) | (config->ICFilter << 4) | (config->ICPrescaler << 2);
            TIMx->CCMR1 = tmpccmrx;
            
            // 입력 캡처 1 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
            
            // 극성에 따라 설정
            switch (config->ICPolarity) {
                case TIM_ICPOLARITY_RISING:
                    // 이미 0으로 설정되어 있음
                    break;
                case TIM_ICPOLARITY_FALLING:
                    tmpccer |= TIM_CCER_CC1P;
                    break;
                case TIM_ICPOLARITY_BOTH_EDGE:
                    tmpccer |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);
                    break;
                default:
                    break;
            }
            
            // 채널 활성화
            tmpccer |= TIM_CCER_CC1E;
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_2:
            // 입력 캡처 2 필터 및 프리스케일러 설정
            tmpccmrx = TIMx->CCMR1;
            tmpccmrx &= ~(TIM_CCMR1_CC2S | TIM_CCMR1_IC2F | TIM_CCMR1_IC2PSC);
            tmpccmrx |= ((config->ICSelection) << 8) | ((config->ICFilter) << 12) | ((config->ICPrescaler) << 10);
            TIMx->CCMR1 = tmpccmrx;
            
            // 입력 캡처 2 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
            
            // 극성에 따라 설정
            switch (config->ICPolarity) {
                case TIM_ICPOLARITY_RISING:
                    // 이미 0으로 설정되어 있음
                    break;
                case TIM_ICPOLARITY_FALLING:
                    tmpccer |= TIM_CCER_CC2P;
                    break;
                case TIM_ICPOLARITY_BOTH_EDGE:
                    tmpccer |= (TIM_CCER_CC2P | TIM_CCER_CC2NP);
                    break;
                default:
                    break;
            }
            
            // 채널 활성화
            tmpccer |= TIM_CCER_CC2E;
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_3:
            // 입력 캡처 3 필터 및 프리스케일러 설정
            tmpccmrx = TIMx->CCMR2;
            tmpccmrx &= ~(TIM_CCMR2_CC3S | TIM_CCMR2_IC3F | TIM_CCMR2_IC3PSC);
            tmpccmrx |= (config->ICSelection) | (config->ICFilter << 4) | (config->ICPrescaler << 2);
            TIMx->CCMR2 = tmpccmrx;
            
            // 입력 캡처 3 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);
            
            // 극성에 따라 설정
            switch (config->ICPolarity) {
                case TIM_ICPOLARITY_RISING:
                    // 이미 0으로 설정되어 있음
                    break;
                case TIM_ICPOLARITY_FALLING:
                    tmpccer |= TIM_CCER_CC3P;
                    break;
                case TIM_ICPOLARITY_BOTH_EDGE:
                    tmpccer |= (TIM_CCER_CC3P | TIM_CCER_CC3NP);
                    break;
                default:
                    break;
            }
            
            // 채널 활성화
            tmpccer |= TIM_CCER_CC3E;
            TIMx->CCER = tmpccer;
            break;
            
        case TIM_CHANNEL_4:
            // 입력 캡처 4 필터 및 프리스케일러 설정
            tmpccmrx = TIMx->CCMR2;
            tmpccmrx &= ~(TIM_CCMR2_CC4S | TIM_CCMR2_IC4F | TIM_CCMR2_IC4PSC);
            tmpccmrx |= ((config->ICSelection) << 8) | ((config->ICFilter) << 12) | ((config->ICPrescaler) << 10);
            TIMx->CCMR2 = tmpccmrx;
            
            // 입력 캡처 4 극성 설정
            tmpccer = TIMx->CCER;
            tmpccer &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
            
            // 극성에 따라 설정
            switch (config->ICPolarity) {
                case TIM_ICPOLARITY_RISING:
                    // 이미 0으로 설정되어 있음
                    break;
                case TIM_ICPOLARITY_FALLING:
                    tmpccer |= TIM_CCER_CC4P;
                    break;
                case TIM_ICPOLARITY_BOTH_EDGE:
                    tmpccer |= (TIM_CCER_CC4P | TIM_CCER_CC4NP);
                    break;
                default:
                    break;
            }
            
            // 채널 활성화
            tmpccer |= TIM_CCER_CC4E;
            TIMx->CCER = tmpccer;
            break;
            
        default:
            break;
    }
}

/**
 * @brief 타이머 입력 캡처 채널을 활성화합니다.
 */
void TIM_IC_EnableChannel(TIM_TypeDef *TIMx, TIM_Channel channel)
{
    switch (channel) {
        case TIM_CHANNEL_1:
            TIMx->CCER |= TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIMx->CCER |= TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIMx->CCER |= TIM_CCER_CC3E;
            break;
        case TIM_CHANNEL_4:
            TIMx->CCER |= TIM_CCER_CC4E;
            break;
        default:
            break;
    }
}

/**
 * @brief 타이머 입력 캡처 채널을 비활성화합니다.
 */
void TIM_IC_DisableChannel(TIM_TypeDef *TIMx, TIM_Channel channel)
{
    switch (channel) {
        case TIM_CHANNEL_1:
            TIMx->CCER &= ~TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            TIMx->CCER &= ~TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            TIMx->CCER &= ~TIM_CCER_CC3E;
            break;
        case TIM_CHANNEL_4:
            TIMx->CCER &= ~TIM_CCER_CC4E;
            break;
        default:
            break;
    }
}

/**
 * @brief 타이머 입력 캡처 값을 얻습니다.
 */
uint32_t TIM_IC_GetCapture(TIM_TypeDef *TIMx, TIM_Channel channel)
{
    uint32_t capture_value = 0;
    
    switch (channel) {
        case TIM_CHANNEL_1:
            capture_value = TIMx->CCR1;
            break;
        case TIM_CHANNEL_2:
            capture_value = TIMx->CCR2;
            break;
        case TIM_CHANNEL_3:
            capture_value = TIMx->CCR3;
            break;
        case TIM_CHANNEL_4:
            capture_value = TIMx->CCR4;
            break;
        default:
            break;
    }
    
    return capture_value;
}

/**
 * @brief 타이머 원-펄스 모드를 초기화합니다.
 */
void TIM_OnePulse_Init(TIM_TypeDef *TIMx, TIM_Channel channel, TIM_OnePulse_Config *config)
{
    // 채널에 대한 출력 비교 설정
    TIM_OC_Config oc_config;
    oc_config.OCMode = TIM_OCMODE_PWM2;
    oc_config.OutputState = config->OutputState;
    oc_config.Pulse = config->Pulse;
    oc_config.OCPolarity = config->OCPolarity;
    
    TIM_OC_Init(TIMx, channel, &oc_config);
    
    // 원-펄스 모드 설정
    if (config->OPMode == TIM_OPMODE_SINGLE) {
        TIMx->CR1 |= TIM_CR1_OPM;
    } else {
        TIMx->CR1 &= ~TIM_CR1_OPM;
    }
}

/**
 * @brief 타이머 원-펄스 모드를 활성화합니다.
 */
void TIM_OnePulse_Enable(TIM_TypeDef *TIMx)
{
    // 원-펄스 모드 활성화
    TIMx->CR1 |= TIM_CR1_OPM;
    
    // 타이머 시작
    TIMx->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 타이머 원-펄스 모드를 비활성화합니다.
 */
void TIM_OnePulse_Disable(TIM_TypeDef *TIMx)
{
    // 원-펄스 모드 비활성화
    TIMx->CR1 &= ~TIM_CR1_OPM;
    
    // 타이머 중지
    TIMx->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 타이머 인터럽트를 활성화합니다.
 */
void TIM_EnableInterrupt(TIM_TypeDef *TIMx, uint32_t interrupt)
{
    TIMx->DIER |= interrupt;
}

/**
 * @brief 타이머 인터럽트를 비활성화합니다.
 */
void TIM_DisableInterrupt(TIM_TypeDef *TIMx, uint32_t interrupt)
{
    TIMx->DIER &= ~interrupt;
}

/**
 * @brief 타이머 인터럽트 플래그 상태를 얻습니다.
 */
uint8_t TIM_GetFlagStatus(TIM_TypeDef *TIMx, uint32_t flag)
{
    return (TIMx->SR & flag) ? 1 : 0;
}

/**
 * @brief 타이머 인터럽트 플래그를 클리어합니다.
 */
void TIM_ClearFlag(TIM_TypeDef *TIMx, uint32_t flag)
{
    TIMx->SR &= ~flag;
}

/**
 * @brief 타이머 딜레이를 시작합니다(폴링 방식).
 */
void TIM_Delay_ms(TIM_TypeDef *TIMx, uint32_t milliseconds)
{
    // 타이머 리셋
    TIMx->CR1 = 0;
    TIMx->CNT = 0;
    
    // 1ms 간격으로 카운트하도록 프리스케일러 설정
    // 시스템 클럭이 84MHz일 경우 84000으로 나누면 1ms 마다 타이머 증가
    // 실제로는 84000-1 값을 사용 (0부터 시작하므로)
    uint32_t system_clock = SystemCoreClock;
    TIMx->PSC = (system_clock / 1000) - 1;
    
    // 타이머 주기를 원하는 밀리초만큼 설정
    TIMx->ARR = milliseconds;
    
    // 타이머 업데이트 이벤트를 생성하여 설정을 적용
    TIMx->EGR = TIM_EGR_UG;
    
    // 타이머 시작
    TIMx->CR1 |= TIM_CR1_CEN;
    
    // 타이머 종료를 기다림
    while (!(TIMx->SR & TIM_SR_UIF));
    
    // 플래그 클리어
    TIMx->SR &= ~TIM_SR_UIF;
    
    // 타이머 중지
    TIMx->CR1 &= ~TIM_CR1_CEN;
}
