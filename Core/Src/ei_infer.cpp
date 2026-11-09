/* Edge Impulse inference bridge.
 * Compiled as C++ (this file holds the C++-only EI headers);
 * main.c stays pure C and calls my_run_inference().
 */
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

extern "C" {
    int16_t ai_get_accel_sample(uint32_t idx);
    void uart_send_str(const char *s);
}

static int get_signal_data(size_t offset, size_t length, float *out_ptr)
{
    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = (float)ai_get_accel_sample((uint32_t)(offset + i));
    }
    return 0;
}

extern "C" void my_run_inference(void)
{
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;   /* 200 * 3 = 600 */
    signal.get_data = &get_signal_data;

    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

    if (res == EI_IMPULSE_OK) {
        float max_val = 0.0f;
        int max_idx = 0;
        for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
            if (result.classification[i].value > max_val) {
                max_val = result.classification[i].value;
                max_idx = (int)i;
            }
        }
        uart_send_str("AI Result: ");
        uart_send_str(result.classification[max_idx].label);
        uart_send_str("\r\n");
    } else {
        uart_send_str("Inference Error!\r\n");
    }
}