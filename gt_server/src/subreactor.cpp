#include "subreactor.h"

namespace gt
{
namespace reactor
{

SubReactor::SubReactor()
    : m_ioContext()  // 初始化IO上下文
    , m_pWork(std::make_unique<boost::asio::io_context::work>(m_ioContext))  // 保持io_context不退出
    , m_bRunning(false)  // 初始状态为未运行
{

}

SubReactor::~SubReactor()
{
    Stop();  // 确保在销毁时停止运行
}

void SubReactor::Start()
{
    if (!m_bRunning) 
    {
        m_bRunning = true;
        m_pThread = std::make_shared<std::thread>([this]() {
            m_ioContext.run();  // 启动IO上下文
        });
    }
}

void SubReactor::Stop()
{
    if (m_bRunning) 
    {
        m_bRunning = false;
        m_pWork.reset();  // 销毁work对象，允许io_context退出
        if (m_pThread && m_pThread->joinable()) 
        {
            m_pThread->join();  // 等待线程结束
        }
    }
}

boost::asio::io_context& SubReactor::GetIoContext()
{
    return m_ioContext;  // 返回IO上下文
}


}}