## 项目概述
GT-WebServer是一个高性能的异步Web服务器（Demo），基于C++和boost::asio库开发。服务器采用主从Reactor设计模式，结合线程池技术，能够有效处理大量并发连接

## 主要特性
- **高并发架构**：实现主从Reactor模式，主Reactor负责监听新连接，从Reactor处理I/O事件
- **异步I/O**：基于boost::asio库实现高效的异步I/O操作
- **线程池管理**：内置线程池处理业务逻辑，避免频繁线程创建销毁开销

## 环境要求
- Ubuntu 24.04.2 LTS
- cmake 3.28.3
- g++ 13.3.0
- Boost::Asio 1.83

## 编译与安装

### Linux平台

```bash
# 克隆仓库
git clone https://github.com/joey-cgt/GT-Thread.git
cd GT-WebServer

# 运行构建脚本
chmod +x build.sh
./build.sh
```

## 使用方法

### 启动服务器

```bash
# Linux
cd build
./GTWebServer
```

默认情况下，服务器会监听127.0.0.1:12345端口。

### 客户端示例

项目提供了一个简单的客户端程序用于测试：

```bash
cd build/gt_client
./gt_client
```

客户端会每5秒向服务器发送当前时间信息，并接收服务器响应。

## 架构设计

### 主从Reactor模式

- **主Reactor**：负责监听TCP端口，接受新连接，并将连接分配给从Reactor
- **从Reactor**：管理已连接的客户端，处理I/O事件
- **线程池**：处理业务逻辑，与Reactor线程解耦，提高并发处理能力

![](/assets/pic_1.png "架构设计示意图")

### 核心组件

- `MainReactor`：主反应堆实现，管理 acceptor 和从反应堆
- `SubReactor`：从反应堆实现，管理IO事件
- `Session`：处理单个客户端连接
- `ThreadPool`：线程池实现，处理业务逻辑