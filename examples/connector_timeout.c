#include "example_config.h"

#if COMPILE_CONNECTOR_TIMEOUT

#include "knet.h"

/* 客户端 - 连接器回调 */
void connector_cb(kchannel_ref_t* channel, knet_channel_cb_event_e e) {
    if (e & channel_cb_event_connect_timeout) { /* 连接成功 */
        knet_channel_ref_close(channel);
        /* 连接超时，退出循环 */
        knet_loop_exit(knet_channel_ref_get_loop(channel));
    } else if (e & channel_cb_event_close) {
        /* 发生错误，退出循环 */
        printf("connect failed!\n");
        knet_loop_exit(knet_channel_ref_get_loop(channel));
    }
}

int main() {
    /* 创建循环 */
    kloop_t* loop = knet_loop_create();
    /* 创建客户端 */
    kchannel_ref_t* connector = knet_loop_create_channel(loop, 8, 1024);
    /* 设置回调 */
    knet_channel_ref_set_cb(connector, connector_cb);
    /* 连接 */
    if (error_ok != knet_channel_ref_connect(connector, "127.0.0.1", 8000, 2)) {
        printf("remote unreachable\n");
    } else {
        /* 启动 */
        knet_loop_run(loop);
    }
    /* 销毁, connector, acceptor不需要手动销毁 */
    knet_loop_destroy(loop);
    return 0;
}

#endif /* COMPILE_CONNECTOR_TIMEOUT */
