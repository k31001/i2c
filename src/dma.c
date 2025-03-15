#include "dma.h"
#include "rcc.h"

/**
 * @brief  스트림 레지스터를 가져옵니다.
 * @param  DMAx: DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: DMA 스트림
 * @return 스트림 레지스터 포인터
 */
static DMA_Stream_TypeDef* DMA_GetStreamRegister(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    if (DMAx == DMA1) {
        return (DMA_Stream_TypeDef *)((uint32_t)DMA1 + 0x010 + (0x018 * stream));
    } else { // DMA2
        return (DMA_Stream_TypeDef *)((uint32_t)DMA2 + 0x010 + (0x018 * stream));
    }
}

/**
 * @brief  LIFCR 또는 HIFCR 플래그 인덱스를 가져옵니다.
 * @param  stream: DMA 스트림
 * @return 플래그 인덱스
 */
static uint32_t DMA_GetClearFlagPos(DMA_Stream stream)
{
    static const uint32_t flag_pos[] = {
        0,  // Stream 0: LIFCR와 비트 0부터 시작
        6,  // Stream 1: LIFCR와 비트 6부터 시작
        16, // Stream 2: LIFCR와 비트 16부터 시작
        22, // Stream 3: LIFCR와 비트 22부터 시작
        0,  // Stream 4: HIFCR와 비트 0부터 시작
        6,  // Stream 5: HIFCR와 비트 6부터 시작
        16, // Stream 6: HIFCR와 비트 16부터 시작
        22  // Stream 7: HIFCR와 비트 22부터 시작
    };
    
    return flag_pos[stream];
}

/**
 * @brief  DMA 스트림을 초기화합니다.
 * @param  DMAx: 초기화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 초기화할 DMA 스트림
 * @param  config: DMA 초기화 구조체 포인터
 * @return None
 */
void DMA_Init(DMA_TypeDef *DMAx, DMA_Stream stream, DMA_Config *config)
{
    // DMA 클럭 활성화
    if (DMAx == DMA1) {
        RCC->AHB1ENR |= (1 << 21); // DMA1 클럭 활성화
    } else if (DMAx == DMA2) {
        RCC->AHB1ENR |= (1 << 22); // DMA2 클럭 활성화
    }
    
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 스트림 비활성화
    DMA_Stream->CR &= ~(1 << 0); // EN 비트 클리어
    
    // 스트림이 비활성화될 때까지 대기
    while (DMA_Stream->CR & (1 << 0));
    
    // 모든 인터럽트 플래그 클리어
    if (stream < 4) {
        DMAx->LIFCR = (0x3F << DMA_GetClearFlagPos(stream));
    } else {
        DMAx->HIFCR = (0x3F << DMA_GetClearFlagPos(stream % 4));
    }
    
    // CR 레지스터 설정
    uint32_t tmpreg = 0;
    
    // 채널 선택
    tmpreg |= (config->Channel << 25);
    
    // 메모리 및 주변장치 증가 모드
    tmpreg |= (config->MemInc << 10);
    tmpreg |= (config->PeriphInc << 9);
    
    // 메모리 및 주변장치 데이터 크기
    tmpreg |= (config->MemDataSize << 13);
    tmpreg |= (config->PeriphDataSize << 11);
    
    // 동작 모드 (일반, 순환, 주변장치 흐름 제어)
    if (config->Mode == DMA_MODE_CIRCULAR) {
        tmpreg |= (1 << 8); // CIRC 비트 설정
    } else if (config->Mode == DMA_MODE_PFCTRL) {
        tmpreg |= (1 << 5); // PFCTRL 비트 설정
    }
    
    // 우선순위
    tmpreg |= (config->Priority << 16);
    
    // 데이터 전송 방향
    tmpreg |= (config->Direction << 6);
    
    // 메모리-메모리 모드
    if (config->Direction == DMA_DIR_MEMORY_TO_MEMORY) {
        tmpreg |= (1 << 4); // MINC 비트 설정
    }
    
    // 설정 적용
    DMA_Stream->CR = tmpreg;
    
    // FCR 레지스터 설정 (FIFO 설정)
    uint32_t ftcr = 0;
    
    if (config->FIFOMode) {
        ftcr |= (1 << 2); // DMDIS 비트 설정 (FIFO 모드 활성화)
        ftcr |= (config->FIFOThreshold << 0); // FTH 비트 설정
    }
    
    // 메모리 및 주변장치 버스트 설정
    DMA_Stream->CR &= ~(0x3 << 23 | 0x3 << 21); // MBURST 및 PBURST 비트 클리어
    DMA_Stream->CR |= (config->MemBurst << 23); // MBURST 비트 설정
    DMA_Stream->CR |= (config->PeriphBurst << 21); // PBURST 비트 설정
    
    // FIFO 설정 적용
    DMA_Stream->FCR = ftcr;
}

/**
 * @brief  DMA 스트림을 비활성화합니다.
 * @param  DMAx: 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 비활성화할 DMA 스트림
 * @return None
 */
void DMA_DeInit(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 스트림 비활성화
    DMA_Stream->CR &= ~(1 << 0); // EN 비트 클리어
    
    // 스트림이 비활성화될 때까지 대기
    while (DMA_Stream->CR & (1 << 0));
    
    // 모든 레지스터 리셋
    DMA_Stream->CR = 0;
    DMA_Stream->NDTR = 0;
    DMA_Stream->PAR = 0;
    DMA_Stream->M0AR = 0;
    DMA_Stream->M1AR = 0;
    DMA_Stream->FCR = 0x00000021;
    
    // 모든 인터럽트 플래그 클리어
    if (stream < 4) {
        DMAx->LIFCR = (0x3F << DMA_GetClearFlagPos(stream));
    } else {
        DMAx->HIFCR = (0x3F << DMA_GetClearFlagPos(stream % 4));
    }
}

/**
 * @brief  DMA 전송을 구성합니다.
 * @param  DMAx: 구성할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 구성할 DMA 스트림
 * @param  SrcAddress: 소스 주소
 * @param  DstAddress: 대상 주소
 * @param  DataLength: 전송할 데이터 항목 수
 * @return None
 */
void DMA_ConfigTransfer(DMA_TypeDef *DMAx, DMA_Stream stream, uint32_t SrcAddress, uint32_t DstAddress, uint16_t DataLength)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 스트림이 활성화되어 있는지 확인하고 비활성화
    if (DMA_Stream->CR & (1 << 0)) {
        DMA_Stream->CR &= ~(1 << 0); // EN 비트 클리어
        while (DMA_Stream->CR & (1 << 0)); // 비활성화될 때까지 대기
    }
    
    // 전송 방향 확인
    uint32_t direction = (DMA_Stream->CR & (3 << 6)) >> 6;
    
    if (direction == DMA_DIR_PERIPH_TO_MEMORY) {
        // 주변장치에서 메모리로
        DMA_Stream->PAR = SrcAddress;
        DMA_Stream->M0AR = DstAddress;
    } else if (direction == DMA_DIR_MEMORY_TO_PERIPH) {
        // 메모리에서 주변장치로
        DMA_Stream->PAR = DstAddress;
        DMA_Stream->M0AR = SrcAddress;
    } else if (direction == DMA_DIR_MEMORY_TO_MEMORY) {
        // 메모리에서 메모리로
        DMA_Stream->PAR = SrcAddress;
        DMA_Stream->M0AR = DstAddress;
    }
    
    // 데이터 항목 수 설정
    DMA_Stream->NDTR = DataLength;
}

/**
 * @brief  DMA 스트림을 활성화합니다.
 * @param  DMAx: 활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 활성화할 DMA 스트림
 * @return None
 */
void DMA_Enable(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 스트림 활성화
    DMA_Stream->CR |= (1 << 0); // EN 비트 설정
}

/**
 * @brief  DMA 스트림을 비활성화합니다.
 * @param  DMAx: 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 비활성화할 DMA 스트림
 * @return None
 */
void DMA_Disable(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 스트림 비활성화
    DMA_Stream->CR &= ~(1 << 0); // EN 비트 클리어
    
    // 스트림이 비활성화될 때까지 대기
    while (DMA_Stream->CR & (1 << 0));
}

/**
 * @brief  DMA 전송 완료 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 전송 중, 1: 전송 완료
 */
uint8_t DMA_IsTransferComplete(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    uint32_t flag_pos = DMA_GetClearFlagPos(stream);
    
    if (stream < 4) {
        return (DMAx->LISR & (1 << (flag_pos + 5))) ? 1 : 0; // TCIF 비트 확인
    } else {
        return (DMAx->HISR & (1 << (flag_pos + 5))) ? 1 : 0; // TCIF 비트 확인
    }
}

/**
 * @brief  DMA 전송 오류 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 오류 없음, 1: 전송 오류
 */
uint8_t DMA_IsTransferError(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    uint32_t flag_pos = DMA_GetClearFlagPos(stream);
    
    if (stream < 4) {
        return (DMAx->LISR & (1 << (flag_pos + 3))) ? 1 : 0; // TEIF 비트 확인
    } else {
        return (DMAx->HISR & (1 << (flag_pos + 3))) ? 1 : 0; // TEIF 비트 확인
    }
}

/**
 * @brief  DMA 전송 중단 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 중단 없음, 1: 전송 중단
 */
uint8_t DMA_IsTransferAborted(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    // 중단 상태는 별도의 플래그가 없으므로, EN 비트 확인으로 대체
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    return (DMA_Stream->CR & (1 << 0)) ? 0 : 1; // EN 비트가 클리어되면 중단된 것으로 간주
}

/**
 * @brief  DMA 절반 전송 완료 플래그를 확인합니다.
 * @param  DMAx: 확인할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 확인할 DMA 스트림
 * @return 0: 절반 전송 완료 아님, 1: 절반 전송 완료
 */
uint8_t DMA_IsHalfTransferComplete(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    uint32_t flag_pos = DMA_GetClearFlagPos(stream);
    
    if (stream < 4) {
        return (DMAx->LISR & (1 << (flag_pos + 4))) ? 1 : 0; // HTIF 비트 확인
    } else {
        return (DMAx->HISR & (1 << (flag_pos + 4))) ? 1 : 0; // HTIF 비트 확인
    }
}

/**
 * @brief  DMA 플래그를 지웁니다.
 * @param  DMAx: 플래그를 지울 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 플래그를 지울 DMA 스트림
 * @return None
 */
void DMA_ClearFlags(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    uint32_t flag_pos = DMA_GetClearFlagPos(stream);
    
    if (stream < 4) {
        DMAx->LIFCR = (0x3F << flag_pos); // 모든 플래그 클리어
    } else {
        DMAx->HIFCR = (0x3F << flag_pos); // 모든 플래그 클리어
    }
}

/**
 * @brief  DMA 인터럽트를 활성화합니다.
 * @param  DMAx: 인터럽트를 활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 인터럽트를 활성화할 DMA 스트림
 * @param  transferComplete: 전송 완료 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  halfTransfer: 절반 전송 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  transferError: 전송 오류 인터럽트 (0: 비활성화, 1: 활성화)
 * @param  directModeError: 직접 모드 오류 인터럽트 (0: 비활성화, 1: 활성화)
 * @return None
 */
void DMA_EnableInterrupts(DMA_TypeDef *DMAx, DMA_Stream stream, uint8_t transferComplete, uint8_t halfTransfer, uint8_t transferError, uint8_t directModeError)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 인터럽트 비활성화
    DMA_Stream->CR &= ~(0x1E << 0); // 인터럽트 비트 클리어
    
    // 선택된 인터럽트 활성화
    if (transferComplete) {
        DMA_Stream->CR |= (1 << 4); // TCIE 비트 설정
    }
    
    if (halfTransfer) {
        DMA_Stream->CR |= (1 << 3); // HTIE 비트 설정
    }
    
    if (transferError) {
        DMA_Stream->CR |= (1 << 2); // TEIE 비트 설정
    }
    
    if (directModeError) {
        DMA_Stream->FCR |= (1 << 1); // DMDIS 비트 설정 (FIFO 모드 활성화)
        DMA_Stream->FCR |= (1 << 7); // FEIE 비트 설정
    }
}

/**
 * @brief  DMA 인터럽트를 비활성화합니다.
 * @param  DMAx: 인터럽트를 비활성화할 DMA 컨트롤러 (DMA1 또는 DMA2)
 * @param  stream: 인터럽트를 비활성화할 DMA 스트림
 * @return None
 */
void DMA_DisableInterrupts(DMA_TypeDef *DMAx, DMA_Stream stream)
{
    // 스트림 레지스터 가져오기
    DMA_Stream_TypeDef *DMA_Stream = DMA_GetStreamRegister(DMAx, stream);
    
    // 모든 인터럽트 비활성화
    DMA_Stream->CR &= ~(0x1E << 0); // 모든 인터럽트 비트 클리어
    DMA_Stream->FCR &= ~(1 << 7);   // FEIE 비트 클리어
}
