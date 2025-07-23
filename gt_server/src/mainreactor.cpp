#include "mainreactor.h"
#include "session.h"

namespace gt
{
namespace reactor
{

MainReactor::MainReactor(SHORT port, UINT8 numSubReactors)
    : m_ioContext()  // 初始化IO上下文
    , m_pWork(std::make_unique<boost::asio::io_context::work>(m_ioContext))  // 保持io_context不退出
    , m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port))  // 绑定端口
    , m_socket(m_ioContext)  // 初始化socket
    , m_numSubReactors(numSubReactors)  // 初始化子反应堆数量
{
    // 初始化SubReactor池
    for(UINT8 i = 0; i < m_numSubReactors; ++i) 
    {
        m_subReactors.emplace_back(std::make_shared<SubReactor>());
    }

    // 启动所有SubReactor
    for(auto& pReactor : m_subReactors) 
    {
        pReactor->Start();
    }

    // 开始监听连接
    DoAccept();
}
    
MainReactor::~MainReactor()
{

}

void MainReactor::Start()
{
    // 启动IO上下文
    m_ioContext.run();
}

void MainReactor::Stop()
{
    // 销毁work对象，允许io_context退出
    m_pWork.reset();
    // 停止IO上下文
    m_ioContext.stop();
    // 停止所有SubReactor
    for(auto& pReactor : m_subReactors) 
    {
        pReactor->Stop();
    }
}

void MainReactor::DoAccept()
{
    if(!m_acceptor.is_open()) 
    {
        std::cerr << "Acceptor is not open." << std::endl;
        return;
    }

    std::shared_ptr<SubReactor>& nextReactor = m_subReactors[m_nextReactorIndex++ % m_subReactors.size()];
    boost::asio::io_context& subIoContext = nextReactor->GetIoContext();
    auto newSocket = std::make_shared<tcp::socket>(subIoContext);
    m_acceptor.async_accept(
        *newSocket, 
        [this, newSocket](const boost::system::error_code& error) mutable {
            if (!error) 
            {
                std::make_shared<Session>(std::move(*newSocket))->Start();
                std::cout << "New session create." << std::endl;
            } 
            else 
            {
                std::cerr << "Error on accept: " << error.message() << std::endl;
            }

            DoAccept();
        });
}

} // namespace reactor
}  // namespace gt