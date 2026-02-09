#include "mik32_hal_pcc.h"    // Подключение заголовка для управления PCC (Clock Control)
#include "mik32_hal_gpio.h"    // Подключение заголовка для управления GPIO (General Purpose Input/Output)

// Определение для состояния пинов
#define GPIO_PIN_SET   1      // Значение для установки сигнала (включено)
#define GPIO_PIN_RESET 0      // Значение для сброса сигнала (выключено)

#define BOARD_LITE           // Определяем тип платы для условной компиляции
#define LAZER_PIN GPIO_PIN_2 // Определяем пин, к которому подключен лазерный светодиод

// Прототипы функций
void SystemClock_Config(); // Функция для настройки системного тактирования
void GPIO_Init();         // Функция для инициализации GPIO

int main()
{
    SystemClock_Config(); // Настройка системного тактирования

    GPIO_Init(); // Инициализация GPIO

    while (1) // Бесконечный цикл
    {
#ifdef BOARD_LITE // Если определен BOARD_LITE
        // Цикл мигания лазерным светодиодом 5 раз
        for (int i = 0; i < 5; i++) 
        {
            HAL_GPIO_WritePin(GPIO_0, LAZER_PIN, GPIO_PIN_SET); // Включаем светодиод
            HAL_DelayMs(500); // Задержка 500 мс
            HAL_GPIO_WritePin(GPIO_0, LAZER_PIN, GPIO_PIN_RESET); // Выключаем светодиод
            HAL_DelayMs(500); // Задержка 500 мс
        }
        HAL_DelayMs(1000); // Задержка перед следующей итерацией
#endif
    }
}

void SystemClock_Config(void)
{
    PCC_InitTypeDef PCC_OscInit = {0}; // Инициализация структуры для настройки PCC

    // Конфигурация параметров осциллятора
    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL; // Включаем все типы осцилляторов
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M; // Указываем тип осциллятора 32MHz
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED; // Настройка на фиксированное значение
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K; // Указываем 32K осциллятор
    PCC_OscInit.AHBDivider = 0; // Делитель AHB
    PCC_OscInit.APBMDivider = 0; // Делитель APBM
    PCC_OscInit.APBPDivider = 0; // Делитель APBP
    PCC_OscInit.HSI32MCalibrationValue = 128; // Значение калибровки HSI 32M
    PCC_OscInit.LSI32KCalibrationValue = 8; // Значение калибровки LSI 32K
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO; // Автоматический выбор источника RTC
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K; // Выбор источника CPU для RTC
    HAL_PCC_Config(&PCC_OscInit); // Применение конфигурации PCC
}

void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // Инициализация структуры для настройки GPIO

    // Включение тактирования для GPIO
    __HAL_PCC_GPIO_0_CLK_ENABLE(); 
    __HAL_PCC_GPIO_1_CLK_ENABLE(); 
    __HAL_PCC_GPIO_2_CLK_ENABLE(); 
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE(); // Включение тактирования для прерываний

#ifdef BOARD_LITE // Если определен BOARD_LITE
    GPIO_InitStruct.Pin = LAZER_PIN; // Устанавливаем пин для лазера
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT; // Устанавливаем режим как выход
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE; // Устанавливаем отсутствие подтягивающего резистора
    HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct); // Инициализация пина для лазера
#endif
}
