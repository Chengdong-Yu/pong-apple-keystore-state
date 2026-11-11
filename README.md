# AppleKeyStoreState / Pong 22H374

保留 UniXNU 的 `NativeKeyStoreEvents.cpp` 和许可证作为同版协议实现来源。Pong复用其有界DER状态编码，通过 `PongKeyStoreState.h` 接入当前内核的唯一AppleKeyStore服务，并校验外部方法的实际输入/输出指针。

上游原始provider仅支持状态和通知记账，不提供keybag/wrap。当前Pong keychain、Wi-Fi保存凭据和USB配对仍依赖既有开发软件wrap格式，因此不同时注册第二个AppleKeyStore，也不将上游的“不支持wrap”覆盖当前格式。

保持当前无密码开发状态和既有wrap字节格式，不声明SEP、硬件数据保护或安全密钥存储。完整上游provider源码已合并，但只编译与当前服务兼容的共享codec。
