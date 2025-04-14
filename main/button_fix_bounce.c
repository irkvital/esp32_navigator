#include "button_fix_bounce.h"

butt_data_t butt_data;

gpio_num_t gpio_button[BUTTONS_NUM] = {
                                                GPIO_NUM_13,
                                                GPIO_NUM_12,
                                                GPIO_NUM_11,
                                            };

QueueHandle_t button_queue[BUTTONS_NUM];


void initPinInp(int pin) {
    printf("%d\n", pin);
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    gpio_set_level(pin, 1);
}

// обработчик по нажатию на кнопку
static void IRAM_ATTR buttonHandle(void* button_num)
{
    // Переменные для переключения контекста
    BaseType_t xHigherPriorityTaskWoken, xResult;
    xHigherPriorityTaskWoken = pdFALSE;
    bool pressed;
    if (gpio_get_level(gpio_button[(int)button_num]) == 0) {
        pressed = true;
    } else {
        pressed = false;
    }
    // Отправляем в очередь задачи событие "кнопка нажата"
    xResult = xQueueSendFromISR(button_queue[(int)button_num], &pressed, &xHigherPriorityTaskWoken);
    // Если высокоприоритетная задача ждет этого события, переключаем управление
    if (xResult == pdPASS) {
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void taskButton(void* button_num) {
    ESP_LOGI("MAIN", "START TASK %d", (int)button_num);
    bool state = false;
    bool last_button_state = false;
    bool tmp = false;
    TickType_t prew = xTaskGetTickCount();
    TickType_t curr;

    while (1) {
        if (xQueueReceive(button_queue[(int)button_num], &state, portMAX_DELAY)) {
            curr = xTaskGetTickCount();
            // проверка на дребезг контактов (на событие нажатия)
            if (state == true && last_button_state == false && (curr - prew) > WAIT_BOUNCE_MS) {
                last_button_state = true;
                buttonFixPressed((int)button_num);
            // проверка на дребезг контактов (на сыбытие отжатия)
            } else if (state == false && last_button_state == true && !xQueuePeek(button_queue[(int)button_num], &tmp, WAIT_BOUNCE_MS)) {
                last_button_state = false;
                buttonFixReleased((int)button_num);
            }
            prew = curr;
        }
    }
    vTaskDelete(NULL);
}

void buttonFixInit() {
    // Устанавливаем сервис GPIO ISR service
    esp_err_t err = gpio_install_isr_service(0);
    if (err == ESP_ERR_INVALID_STATE) {
        ESP_LOGW("ISR", "GPIO isr service already installed");
    };
    // инициализация каждого gpio   
    for (int i = 0; i < BUTTONS_NUM; i++) {
        if (button_queue[i] == NULL) {
            button_queue[i] = xQueueCreate(20, sizeof(bool));
        }
        initPinInp(gpio_button[i]);

        // Регистрируем обработчик прерывания на нажатие кнопки
        gpio_isr_handler_add(gpio_button[i], buttonHandle, (void*) i);
        gpio_set_intr_type(gpio_button[i], GPIO_INTR_ANYEDGE);
        // Разрешаем использование прерываний
        gpio_intr_enable(gpio_button[i]);

        xTaskCreatePinnedToCore(taskButton, "taskButton", 3072, (void*) i, 20, NULL, tskNO_AFFINITY);
    }
}

void buttonFixPressed(int button_num) {
    butt_data.key = button_num;
    butt_data.status = 1;
    // ESP_LOGI("MAIN", "Button %d is pressed", button_num);
    // ESP_LOGI("BUTT", "button %d status %d", butt_data.key, butt_data.status);
}

void buttonFixReleased(int button_num) {
    butt_data.key = button_num;
    butt_data.status = 0;
    // ESP_LOGI("MAIN", "Button %d is released\n", button_num);
    // ESP_LOGI("BUTT", "button %d status %d", butt_data.key, butt_data.status);
}

int get_key() {
    return butt_data.key;
}
int get_status() {
    return butt_data.status;
}