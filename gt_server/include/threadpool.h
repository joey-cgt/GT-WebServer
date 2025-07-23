#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <future>
#include <functional>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include "typedefine.h"

namespace gt
{
namespace thread
{

class ThreadPool
{
public:
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;  // 禁止拷贝构造
    ThreadPool& operator=(const ThreadPool&) = delete;  // 禁止拷贝赋值
    static std::shared_ptr<ThreadPool> GetInstance();
    
    void Start();
    void Stop();

    template<typename F>
    void PostTask(F&& f);

    template<typename F, typename... Args>
    auto PostTaskWithResult(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    ThreadPool(UINT32 numThreads);

private:
    UINT32 m_numThreads;  // 线程池中的线程数量
    std::mutex m_mutex;  // 保护单例实例的互斥锁
    std::vector<std::thread> m_threads;  // 线程池中的线程
    boost::asio::io_context m_ioContext;  // IO上下文
    std::unique_ptr<boost::asio::io_context::work> m_pWork;  // 保持IO上下文不退出
};

template<typename F>
void ThreadPool::PostTask(F&& f)
{
    boost::asio::post(m_ioContext, std::forward<F>(f));
}

template<typename F, typename... Args>
auto ThreadPool::PostTaskWithResult(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    using task_type = std::packaged_task<return_type()>;
    auto task = std::make_shared<task_type>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> future = task->get_future();
    boost::asio::post(m_ioContext, [task]() {
        (*task)();
    });
    return future;
}

}  // namespace thread
}  // namespace gt


#endif // __THREADPOOL_H__