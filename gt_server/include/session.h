#ifndef __SESSION_H__
#define __SESSION_H__
#include "typedefine.h"
#include "subreactor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <boost/asio.hpp>

namespace gt
{   
namespace reactor
{

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket);
    ~Session();
    void Start();
private:
    void DoRead();
    void HandleRead(const char* data, size_t length);

private:
    tcp::socket m_socket;
    std::vector<char> m_data;
};


} // namespace reactor
} // namespace gt


#endif // __SESSION_H__