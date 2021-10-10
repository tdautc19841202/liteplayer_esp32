// Copyright (c) 2021 Qinglong<sysu.zqlong@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "osal/os_thread.h"
#include "cutils/log_helper.h"
#include "cutils/memory_helper.h"
#include "liteplayer_main.h"

#include "source_httpclient_wrapper.h"
#include "source_esp32_flashtone_wrapper.h"
#include "sink_esp32_i2s_wrapper.h"

#include "audio_tone_uri.h"
#include "board.h"
#include "driver/i2s.h"
#include "board_pins_config.h"

#define TAG "liteplayer_demo"

#define I2S_PORT  ( I2S_NUM_0 )
#define MCLK_GPIO ( GPIO_NUM_0 )
#define DEFAULT_VOLUME 50

static enum liteplayer_state g_player_state = LITEPLAYER_IDLE;
static liteplayer_handle_t g_player_handle = NULL;

static int liteplayer_demo_state_listener(enum liteplayer_state state, int errcode, void *priv)
{
    bool state_sync = true;
    switch (state) {
    case LITEPLAYER_IDLE:
        OS_LOGD(TAG, "-->LITEPLAYER_IDLE");
        break;
    case LITEPLAYER_INITED:
        OS_LOGD(TAG, "-->LITEPLAYER_INITED");
        break;
    case LITEPLAYER_PREPARED:
        OS_LOGD(TAG, "-->LITEPLAYER_PREPARED");
        break;
    case LITEPLAYER_STARTED:
        OS_LOGD(TAG, "-->LITEPLAYER_STARTED");
        break;
    case LITEPLAYER_PAUSED:
        OS_LOGD(TAG, "-->LITEPLAYER_PAUSED");
        break;
    case LITEPLAYER_NEARLYCOMPLETED:
        OS_LOGD(TAG, "-->LITEPLAYER_NEARLYCOMPLETED");
        state_sync = false;
        break;
    case LITEPLAYER_COMPLETED:
        OS_LOGD(TAG, "-->LITEPLAYER_COMPLETED");
        break;
    case LITEPLAYER_STOPPED:
        OS_LOGD(TAG, "-->LITEPLAYER_STOPPED");
        break;
    case LITEPLAYER_ERROR:
        OS_LOGE(TAG, "-->LITEPLAYER_ERROR: %d", errcode);
        break;
    default:
        OS_LOGE(TAG, "-->LITEPLAYER_UNKNOWN: %d", state);
        state_sync = false;
        break;
    }
    if (state_sync)
        g_player_state = state;
    return 0;
}

static void *liteplayer_demo(void *arg)
{
    const char *url = (const char *)arg;
    OS_LOGI(TAG, "liteplayer playing: %s", url);

    if (liteplayer_set_data_source(g_player_handle, url) != 0) {
        OS_LOGE(TAG, "Failed to set data source");
        goto test_done;
    }

    if (liteplayer_prepare_async(g_player_handle) != 0) {
        OS_LOGE(TAG, "Failed to prepare player");
        goto test_done;
    }
    while (g_player_state != LITEPLAYER_PREPARED && g_player_state != LITEPLAYER_ERROR) {
        os_thread_sleep_msec(100);
    }
    if (g_player_state == LITEPLAYER_ERROR) {
        OS_LOGE(TAG, "Failed to prepare player");
        goto test_done;
    }

    if (liteplayer_start(g_player_handle) != 0) {
        OS_LOGE(TAG, "Failed to start player");
        goto test_done;
    }
    OS_MEMORY_DUMP();
    while (g_player_state != LITEPLAYER_COMPLETED && g_player_state != LITEPLAYER_ERROR) {
        os_thread_sleep_msec(100);
    }

    if (liteplayer_stop(g_player_handle) != 0) {
        OS_LOGE(TAG, "Failed to stop player");
        goto test_done;
    }
    while (g_player_state != LITEPLAYER_STOPPED) {
        os_thread_sleep_msec(100);
    }

test_done:
    liteplayer_reset(g_player_handle);
    while (g_player_state != LITEPLAYER_IDLE) {
        os_thread_sleep_msec(100);
    }
    return NULL;
}

void app_main()
{
    printf("Hello liteplayer!\n");

    OS_LOGI(TAG, "Start audio codec chip");
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_DECODE, AUDIO_HAL_CTRL_START);
    audio_hal_set_volume(board_handle->audio_hal, DEFAULT_VOLUME);

    OS_LOGI(TAG, "Start i2s driver");
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
        .dma_buf_count = 3,
        .dma_buf_len = 300,
        .use_apll = true,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0,
    };
    if (i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL) != 0) {
        OS_LOGE(TAG, "i2s_set_clk failed");
        return;
    }
    i2s_pin_config_t i2s_pin_cfg = {0};
    get_i2s_pins(I2S_PORT, &i2s_pin_cfg);
    if (i2s_set_pin(I2S_PORT, &i2s_pin_cfg) != 0) {
        OS_LOGE(TAG, "i2s_set_pin failed");
        i2s_driver_uninstall(I2S_PORT);
        return;
    }
    i2s_mclk_gpio_select(I2S_PORT, MCLK_GPIO);

    OS_LOGI(TAG, "Create liteplayer");
    g_player_handle = liteplayer_create();
    if (g_player_handle == NULL)
        return;
    liteplayer_register_state_listener(g_player_handle, liteplayer_demo_state_listener, NULL);
    struct sink_wrapper sink_ops = {
        .priv_data = NULL,
        .name = esp32_i2s_out_wrapper_name,
        .open = esp32_i2s_out_wrapper_open,
        .write = esp32_i2s_out_wrapper_write,
        .close = esp32_i2s_out_wrapper_close,
    };
    liteplayer_register_sink_wrapper(g_player_handle, &sink_ops);
    struct source_wrapper flashtone_ops = {
        .async_mode = false,
        .buffer_size = 2*1024,
        .priv_data = NULL,
        .url_protocol = esp32_flashtone_wrapper_url_protocol,
        .open = esp32_flashtone_wrapper_open,
        .read = esp32_flashtone_wrapper_read,
        .content_pos = esp32_flashtone_wrapper_content_pos,
        .content_len = esp32_flashtone_wrapper_content_len,
        .seek = esp32_flashtone_wrapper_seek,
        .close = esp32_flashtone_wrapper_close,
    };
    liteplayer_register_source_wrapper(g_player_handle, &flashtone_ops);

    struct os_thread_attr attr = {
        .name = "liteplayer_demo",
        .priority = OS_THREAD_PRIO_NORMAL,
        .stacksize = 8192,
        .joinable = true,
    };
    for (int i = 0; i < TONE_TYPE_MAX; i++) {
        os_thread tid = os_thread_create(&attr, liteplayer_demo, (void *)tone_uri[i]);
        os_thread_join(tid, NULL);
    }

    OS_LOGI(TAG, "Destroy liteplayer");
    liteplayer_destroy(g_player_handle);

    while (1)
        os_thread_sleep_msec(100);
}
