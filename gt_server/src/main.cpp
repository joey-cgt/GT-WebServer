#include "mainreactor.h"


int main()
{
    gt::reactor::MainReactor reactor(12345, 4);  // 创建MainReactor，监听端口12345，使用4个SubReactor
    reactor.Start();  // 启动MainReactor

    return 0;  // 程序结束
}