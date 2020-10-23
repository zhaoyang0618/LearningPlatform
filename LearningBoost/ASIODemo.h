#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;

class server1
{
    int _port;
public:
	server1(int port)
        : _port(port)
	{

	}

	~server1()
	{

	}

public:
	void run()
	{
        io_service iosev;
        ip::tcp::acceptor acceptor(iosev, ip::tcp::endpoint(ip::tcp::v4(), _port));
        for (;;)
        {
            // socket对象
            ip::tcp::socket socket(iosev);

            // 等待直到客户端连接进来
            acceptor.accept(socket);

            // 显示连接进来的客户端
            std::cout << socket.remote_endpoint().address() << std::endl;

            // 向客户端发送hello world!
            boost::system::error_code ec;
            socket.write_some(buffer("hello world!"), ec);
            // 如果出错，打印出错信息
            if (ec)
            {
                std::cout <<
                    boost::system::system_error(ec).what() << std::endl;
                break;
            }

            // 与当前客户交互完成后循环继续等待下一客户连接
        }
	}
};

class client1
{
public:
    client1()
    {

    }

    ~client1()
    {

    }

public:
    int connect(const std::string& serverIp = "127.0.01", int port = 1000)
    {
        // 所有asio类都需要io_service对象
        io_service iosev;
        // socket对象
        ip::tcp::socket socket(iosev);
        // 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
        ip::tcp::endpoint ep(ip::address_v4::from_string(serverIp), port);
        // 连接服务器
        boost::system::error_code ec;
        socket.connect(ep, ec);
        // 如果出错，打印出错信息
        if (ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return -1;
        }
        // 接收数据
        char buf[100];
        size_t len = socket.read_some(buffer(buf), ec);
        std::cout.write(buf, len);
        return 0;
    }
};

struct CHelloWorld_Service {
    CHelloWorld_Service(io_service& iosev)
        :m_iosev(iosev), m_acceptor(iosev, tcp::endpoint(tcp::v4(), 1000))
    {
    }

    void start()
    {
        // 开始等待连接（非阻塞）
        std::cout << "CHelloWorld_Service::start" << std::endl;
        boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
        // 触发的事件只有error_code参数，所以用boost::bind把socket绑定进去
        m_acceptor.async_accept(*psocket,
            boost::bind(&CHelloWorld_Service::accept_handler, this, psocket, _1)
        );
        std::cout << "CHelloWorld_Service::start function end" << std::endl;
    }

    // 有客户端连接时accept_handler触发
    void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
    {
        std::cout << "CHelloWorld_Service::accept_handler" << std::endl;
        if (ec) {
            std::cout << "CHelloWorld_Service::accept_handler error: ec == true" << std::endl;
            return;
        }
        // 继续等待连接
        start();
        // 显示远程IP
        std::cout << psocket->remote_endpoint().address() << std::endl;
        // 发送信息(非阻塞)
        boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
        psocket->async_write_some(buffer(*pstr),
            boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2)
        );
        std::cout << "CHelloWorld_Service::accept_handler end" << std::endl;
    }

    // 异步写操作完成后write_handler触发
    void write_handler(boost::shared_ptr<std::string> pstr,
        error_code ec, size_t bytes_transferred)
    {
        if (ec)
            std::cout << "发送失败!" << std::endl;
        else
            std::cout << *pstr << " 已发送" << std::endl;
    }

    static void Test()
    {
        io_service iosev;
        CHelloWorld_Service sev(iosev);
        // 开始等待连接
        std::cout << "开始等待连接" << std::endl;
        sev.start();
        iosev.run();
        std::cout << "测试结束" << std::endl;
    }

private:
    io_service& m_iosev;
    ip::tcp::acceptor m_acceptor;
};
