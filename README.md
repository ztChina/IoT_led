# IoT_led
物联网led测试

## bluetooth_led_WXmini
bluetooth_led是基于蓝牙的esp控制led亮灭的程序,
手机可以搜索到esp的蓝牙并连接，再使用蓝牙调试助手APP就可以通信控制esp。

bluetooth_weixin是微信小程序，手机可以使用这个小程序通过选择并蓝牙控制esp，用来替代蓝牙调试助手。

## wifi_led_WXmini
wifi_led:先使esp的WIFI模块启动为AP模式，并开启一个Web服务。
当手机通过WLAN连接此wifi之后，向此wed服务端输入esp需要连接的路由器WIFI账号和密码，ESP处理好手机客户端请求之后，连接路由器WIFI并关闭web服务。
然后创建一个MQTT客户端，走wifi通道，订阅和发布消息，根据消息控制led。

server_weixin:服务器nginx、emqx服务
  -docker-compose.yml容器配置文件,
  -nginx.conf服务配置文件。

WIFI_WeChatMiniEsp32：手机wifi连接esp并设置esp，基于MQTT通信的微信小程序

