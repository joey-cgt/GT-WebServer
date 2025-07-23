
#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <ctime>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);  // 同步连接（阻塞）

        while (true) {
            // 获取当前时间字符串
            std::time_t now = std::time(nullptr);
            char buf[64];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
            std::string message = std::string("Time: ") + buf;
            std::cout << "Sending: " << message << std::endl;
            boost::asio::write(socket, boost::asio::buffer(message));

            // 可选：接收服务器回复
            char reply[1024];
            boost::system::error_code ec;
            size_t length = socket.read_some(boost::asio::buffer(reply), ec);
            if (!ec) {
                std::cout << "Received: " << std::string(reply, length) << std::endl;
            } else {
                std::cerr << "Receive error: " << ec.message() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}