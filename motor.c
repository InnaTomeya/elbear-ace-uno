#include "mik32_hal.h"
#include "mik32_hal_gpio.h"
#include "mik32_hal_pcc.h"

// Определяем пины через битовую маску
#define MOTOR_PIN   (1 << 1)  // PORT0.1
#define BUTTON_PIN  (1 << 8)  // PORT0.8

// Прототип функции
static void GPIO_Init(void);

int main(void) {
    HAL_Init();        // Инициализация HAL
    GPIO_Init();       // Инициализация GPIO

    // Мотор изначально выключен
    HAL_GPIO_WritePin(GPIO_0, MOTOR_PIN, 0);

    while (1) {
        // Читаем состояние кнопки
        uint8_t buttonState = HAL_GPIO_ReadPin(GPIO_0, BUTTON_PIN);

        if (buttonState == 0) { 
            // Кнопка нажата → включаем мотор
            HAL_GPIO_WritePin(GPIO_0, MOTOR_PIN, 1);
        } else { 
            // Кнопка не нажата → выключаем мотор
            HAL_GPIO_WritePin(GPIO_0, MOTOR_PIN, 0);
        }
    }

    return 0;
}

static void GPIO_Init(void) {
    // Включаем тактирование GPIO
    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE(); // на случай прерываний

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // --- Настройка кнопки ---
    GPIO_InitStruct.Pin = BUTTON_PIN;             // маска пина
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_INPUT; // вход
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_UP;      // подтяжка вверх
    HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);

    // --- Настройка мотора ---
    GPIO_InitStruct.Pin = MOTOR_PIN;              // маска пина
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT; // выход
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;    // без подтяжки
    HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);
}