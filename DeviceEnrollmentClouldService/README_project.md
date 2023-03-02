## Device enrollment Cloud service

基于linux平台下的测试用tcp服务



基本配置

日志文件路径：

/var/log/DeviceEnrollmentCloudService/

项目配置路径

/etc/DeviceEnrollmentCloudService/DeviceEnrollmentCloudService.config



DeviceEnrollmentCloudService.config

```ini
# 默认ip端口在程序中为 127.0.0.1:26758
[server]
# ip
ip = 127.0.0.1
# 监听端口
port = 9981


[epoll-set]
# 用户态最大epoll_event buffer,程序默认为1000
max_event = 1000
```



注1：在服务启动前需要使用

```bash
ulimit -n 1000000
```

来调整单个端口最大可连接数量