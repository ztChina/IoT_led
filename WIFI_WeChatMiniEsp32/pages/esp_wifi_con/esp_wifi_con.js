//通过wifi连接esp界面
Page({
    data: {
        ESP32SSID:'',
        ESP32PWD:'',
    },
    //wifi SSID和PWD 输入
    SSIDInput(e)
    {
        this.setData({
            ESP32SSID: e.detail.value
        })
    },
    PWDInput(e)
    {
        this.setData({
            ESP32PWD: e.detail.value
        })
    },
    //点击连接
    connect_wifi()
    {
        //开启wifi
        wx.startWifi({
            success:() => {
                console.log('WiFi 模块初始化成功');
                // 连接 ESP32 的热点
                wx.connectWifi({
                    SSID: this.data.ESP32SSID,
                    password: this.data.ESP32PWD,
                    success:(res) => {
                        console.log('连接 ESP32 成功');
                        const app = getApp(); // 获取全局 app 实例
                        // 将局部变量赋值给全局变量
                        app.globalData.ESP32SSID = this.data.ESP32SSID;
                        app.globalData.ESP32PWD = this.data.ESP32PWD;
                        console.log('全局变量已设置:', app.globalData.ESP32SSID, app.globalData.ESP32PWD);
                        //跳转到esp信息配置界面
                        wx.navigateTo({
                            url: '../esp_conf/esp_conf',
                })
            },
                fail(err) {
                    console.log('连接 ESP32 失败');
                }
              })
            },
            fail(err) {
                wx.showToast({ title: 'WiFi 模块初始化失败', icon: 'none' });
            }
        })  
    },
})