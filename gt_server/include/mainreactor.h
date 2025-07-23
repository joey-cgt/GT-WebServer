#ifndef __MAIN_REACTOR_H__
#define __MAIN_REACTOR_H__

#include "typedefine.h"
#include "subreactor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


namespace gt
{
namespace reactor
{
using boost::asio::ip::tcp;

class MainReactor
{
public:
    MainReactor(SHORT port, UINT8 numSubReactors);
    ~MainReactor();
    void Start();
    void Stop();
private:
    void DoAccept(); 

private:
    boost::asio::io_context m_ioContext;
    std::unique_ptr<boost::asio::io_context::work> m_pWork;
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    std::vector<std::shared_ptr<SubReactor>> m_subReactors;
    UINT8 m_numSubReactors;
    UINT32 m_nextReactorIndex = 0;  // 用于轮询选择下一个SubReactor
};


}  // namespace reactor
}  // namespace gt

#endif // __MAIN_REACTOR_H__