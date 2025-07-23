#include "threadpool.h"

namespace gt
{
namespace thread
{

std::shared_ptr<ThreadPool> ThreadPool::GetInstance()
{
    static std::shared_ptr<ThreadPool> instance(new ThreadPool(std::thread::hardware_concurrency()));
    return instance;
}

ThreadPool::ThreadPool(UINT32 numThreads)
    : m_pWork(std::make_unique<boost::asio::io_context::work>(m_ioContext))
    , m_numThreads(numThreads)  // 初始化线程池中的线程数量
{
    Start();
}

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::Start()
{
    for (UINT32 i = 0; i < m_numThreads; ++i) 
    {
        m_threads.emplace_back([this]() {
            m_ioContext.run();
        });
    }
}

void ThreadPool::Stop()
{
    m_pWork.reset();  // 销毁work对象，允许io_context退出
    m_ioContext.stop();  // 停止IO上下文
    for (auto& thread : m_threads) 
    {
        if (thread.joinable()) 
        {
            thread.join();  // 等待线程结束
        }
    }
}

}   // namespace thread
}   // namespace gt