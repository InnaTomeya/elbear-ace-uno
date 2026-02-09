.equ UART_0_RX,          5              # Rx UART_0 управляется выводом PORT_0_5
.equ UART_0_TX,          6              # Tx UART_0 управляется выводом PORT_0_6
.equ PM_BASE,            0x000050000    # Базовый адрес блока питания
.equ CLK_APB_P_SET,      0x1C           # Адрес регистра настройки тактирования
.equ PM_CLK_APB_P_SET,   (PM_BASE + CLK_APB_P_SET)  # Регистрация установки APB тактирования

.equ PM_CLOCK_APB_P_UART_0_S,     1    # Бит настройки тактирования UART_0
.equ PM_CLOCK_APB_P_UART_0_Msk,   (1 << PM_CLOCK_APB_P_UART_0_S)  # Маска для настройки UART_0

.equ PAD_CONFIG_BASE,    0x000050C00    # Базовый адрес конфигурации портов
.equ PORT_0_CFG,         0x00           # Адрес конфигурации PORT_0
.equ PAD_CONFIG_PORT_0_CFG, (PAD_CONFIG_BASE + PORT_0_CFG)  # Полный адрес конфигурации PORT_0

.equ UART_0,             0x00081400     # Базовый адрес для UART_0
.equ UART_1,             0x00           # Смещение для UART_CONTROL1
.equ UART_2,             0x04           # Смещение для UART_CONTROL2
.equ UART_3,             0x08           # Смещение для UART_CONTROL3
.equ UART_DIVIDER,       0x0C           # Смещение для делителя
.equ UART_FLAGS,         0x1C           # Смещение для флагов состояния
.equ UART_TXDATA,        0x28           # Смещение для передачи данных

.equ UART_0_1,          (UART_0 + UART_1)  # Полный адрес для UART_CONTROL1
.equ UART_0_2,          (UART_0 + UART_2)  # Полный адрес для UART_CONTROL2
.equ UART_0_3,          (UART_0 + UART_3)  # Полный адрес для UART_CONTROL3
.equ UART_0_DIVIDER,    (UART_0 + UART_DIVIDER)  # Полный адрес для делителя UART_0
.equ UART_0_FLAGS,      (UART_0 + UART_FLAGS)    # Полный адрес для флагов UART_0
.equ UART_0_TXDATA,     (UART_0 + UART_TXDATA)   # Полный адрес для передачи данных UART_0

.equ UART_1_TE_S,       3              # Бит включения передачи в CONTROL1
.equ UART_1_TE_Msk,     (1 << UART_1_TE_S)  # Маска для включения передачи
.equ UART_1_RE_S,       2              # Бит включения приема в CONTROL1
.equ UART_1_RE_Msk,     (1 << UART_1_RE_S)  # Маска для включения приема
.equ UART_1_UE_S,       0              # Бит включения UART в CONTROL1
.equ UART_1_UE_Msk,     (1 << UART_1_UE_S)  # Маска для включения UART

.equ UART_FLAGS_TEACK_S, 21            # Бит подтверждения готовности к передаче
.equ UART_FLAGS_TEACK_Msk, (1 << UART_FLAGS_TEACK_S)  # Маска готовности к передаче
.equ UART_FLAGS_TC_S,     6              # Бит завершения передачи
.equ UART_FLAGS_TC_Msk,   (1 << UART_FLAGS_TC_S)  # Маска завершения передачи

.section .text
.global _start

_start:
    # Включение тактирования UART
    li t0, PM_CLK_APB_P_SET               # Загружаем адрес регистра тактирования
    lw t1, 0(t0)                          # Читаем текущее значение регистра
    li t2, PM_CLOCK_APB_P_UART_0_Msk     # Загружаем маску для UART_0
    or t1, t1, t2                         # Включаем тактирование UART_0
    sw t1, 0(t0)                          # Записываем новое значение в регистр

    # Настройка портов для UART
    li t0, PAD_CONFIG_PORT_0_CFG          # Загружаем адрес конфигурации PORT_0
    lw t1, 0(t0)                          # Читаем текущее значение конфигурации
    # Сбрасываем режимы для TX и RX
    li t2, ~(0b11 << (2 * UART_0_RX)) & ~(0b11 << (2 * UART_0_TX))
    and t1, t1, t2                        # Применяем изменения к конфигурации
    sw t1, 0(t0)                          # Записываем обновленное значение в конфигурацию

    li t0, PAD_CONFIG_PORT_0_CFG          # Снова загружаем адрес конфигурации PORT_0
    lw t1, 0(t0)                          # Читаем текущее значение конфигурации
    # Устанавливаем режим работы TX и RX
    li t2, (1 << (2 * UART_0_RX)) | (1 << (2 * UART_0_TX))
    or t1, t1, t2                         # Обновляем конфигурацию с включенными режимами
    sw t1, 0(t0)                          # Записываем изменения в конфигурацию

    # Инициализация UART
    li t0, UART_0_1                       # Загружаем адрес регистра контроля UART_0
    sw zero, 0(t0)                        # Сбрасываем регистр контроля UART_0

    li t0, UART_0_2                       # Загружаем адрес второго регистра контроля UART_0
    sw zero, 0(t0)                        # Сбрасываем регистр контроля UART_0

    li t0, UART_0_3                       # Загружаем адрес третьего регистра контроля UART_0
    sw zero, 0(t0)                        # Сбрасываем регистр контроля UART_0

    li t0, UART_0_DIVIDER                 # Загружаем адрес регистра делителя
    li t2, 3333                           # Установка делителя (9600)
    sw t2, 0(t0)                          # Записываем значение делителя

    li t0, UART_0_FLAGS                   # Загружаем адрес регистра флагов
    ori t1, zero, 0xFFFFFFFF              # Сброс всех флагов
    sw t1, 0(t0)                          # Записываем обновленные флаги

    li t0, UART_0_1                       # Загружаем адрес регистра контроля UART_0
    lw t1, 0(t0)                          # Читаем текущее значение регистра
    # Включаем передачу, прием и UART
    ori t1, t1, UART_1_TE_Msk | UART_1_RE_Msk | UART_1_UE_Msk
    sw t1, 0(t0)                          # Записываем новые настройки управления UART

    # Ожидание готовности к передаче
    li a0, UART_FLAGS_TEACK_Msk           # Загружаем маску готовности к передаче
    jal wait_flag_uart                    # Вызов подпрограммы ожидания

    # Отправка строки "Hello\r\n"
    la a1, message_1                      # Загружаем адрес строки для отправки

send_loop:
    lb a0, 0(a1)                          # Загрузка символа из строки
    beqz a0, end_send                     # Если символ равен 0 (конец строки), выйти из цикла
    jal uart_0_send                       # Вызов подпрограммы отправки символа
    addi a1, a1, 1                        # Переход к следующему символу
    j send_loop                           # Повторение цикла отправки

end_send:
    j halt                                 # Завершение программы

halt:
    j halt                                 # Зациклить

uart_0_send:
    li t0, UART_0_TXDATA                  # Загружаем адрес для передачи данных
    sw a0, 0(t0)                          # Отправка символа
    mv t5, ra                    # Сохраняем адрес возврата в t5
    li a0, UART_FLAGS_TC_Msk              # Загружаем маску завершения передачи
    jal wait_flag_uart                    # Ожидание завершения передачи
    mv ra, t5                    # Восстанавливаем адрес возврата из t5
    ret                                    # Возврат из подпрограммы

wait_flag_uart:
    li t0, UART_0_FLAGS                   # Загружаем адрес регистра флагов
    lw t1, 0(t0)                          # Читаем текущее значение регистра флагов
    and t1, t1, a0                        # Проверка состояния флага
    beqz t1, wait_flag_uart               # Если флаг не установлен, продолжаем ожидание
    ret                                    # Возврат из подпрограммы

message_1: 
    .string "Hello\r\n"                  # Строка для передачи
    .byte 0                               # Завершение строки
    