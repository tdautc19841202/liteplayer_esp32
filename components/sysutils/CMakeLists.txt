set(SRC_DIR sysutils)

set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES mbedtls)

set(COMPONENT_ADD_INCLUDEDIRS ${SRC_DIR}/include)

set(COMPONENT_SRCS
    ${SRC_DIR}/osal/esp8266/os_log.c
    ${SRC_DIR}/osal/esp8266/os_memory.c
    ${SRC_DIR}/osal/esp8266/os_thread.c
    ${SRC_DIR}/osal/esp8266/os_time.c
    ${SRC_DIR}/osal/esp8266/os_timer.c
    ${SRC_DIR}/osal/esp8266/os_misc.c
    ${SRC_DIR}/source/cutils/memdbg.c
    ${SRC_DIR}/source/cutils/mlooper.c
    ${SRC_DIR}/source/cutils/mqueue.c
    ${SRC_DIR}/source/cutils/ringbuf.c
    ${SRC_DIR}/source/cutils/swtimer.c
    ${SRC_DIR}/source/cipher/sha2.c
    ${SRC_DIR}/source/cipher/hmac_sha2.c
    ${SRC_DIR}/source/cipher/md5.c
    ${SRC_DIR}/source/cipher/base64.c
    ${SRC_DIR}/source/httpclient/httpclient.c
    ${SRC_DIR}/source/json/cJSON.c
    ${SRC_DIR}/source/json/cJSON_Utils.c
)

register_component()

target_compile_options(${COMPONENT_TARGET} PRIVATE -DOS_RTOS -DOS_FREERTOS_ESP8266 -DENABLE_HTTPCLIENT_MBEDTLS)
