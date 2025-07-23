#ifndef __SUBREACTOR_H__
#define __SUBREACTOR_H__

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include "typedefine.h"

namespace gt
{
namespace reactor
{

class SubReactor
{
public:
    SubReactor();
    ~SubReactor();
    void Start();
    void Stop();
    boost::asio::io_context& GetIoContext();
private:
    boost::asio::io_context m_ioContext;  // 每个SubReactor都有自己的IO上下文
    std::unique_ptr<boost::asio::io_context::work> m_pWork;  // 保持IO上下文不退出
    std::shared_ptr<std::thread> m_pThread;  // 运行SubReactor的线程
    bool m_bRunning = false;  // 标记SubReactor是否在运行

};
} // namespace reactor
} // namespace gt#endif

#endif // __SUBREACTOR_H__