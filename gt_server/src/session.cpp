#include "session.h"

namespace gt {
namespace reactor {

Session::Session(tcp::socket socket)
    : m_socket(std::move(socket))  // 移动socket以避免拷贝
{
    m_data.resize(1024);  // 设置缓冲区大小，可以根据需要调整
}


Session::~Session() 
{

}

void Session::Start() 
{
    DoRead();
}

void Session::DoRead() 
{
    auto self(shared_from_this());  // 创建一个shared_ptr以保持Session对象在异步操作期间有效
    m_socket.async_read_some(boost::asio::buffer(m_data),
        [this, self](const boost::system::error_code& error, size_t bytes_transferred) {
            if (!error) 
            {
                HandleRead(m_data.data(), bytes_transferred);
                m_data.resize(1024);
                DoRead();
            } else 
            {
                std::cerr << "Read error: " << error.message() << std::endl;
                // 此处不再继续 DoRead，连接出错时自动结束
            }
        }
    );
}


void Session::HandleRead(const char* data, size_t length) 
{
    std::string recv_msg(data, length);
    std::cout << "Received: " << recv_msg << std::endl;

    // 构造回复消息
    std::string reply = "Server received: " + recv_msg;
    auto self(shared_from_this());
    boost::asio::async_write(m_socket, boost::asio::buffer(reply),
        [this, self](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
            if (error) {
                std::cerr << "Send reply error: " << error.message() << std::endl;
            }
        }
    );
}

} // namespace reactor
} // namespace gt
