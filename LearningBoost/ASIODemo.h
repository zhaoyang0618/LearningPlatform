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
            // socket����
            ip::tcp::socket socket(iosev);

            // �ȴ�ֱ���ͻ������ӽ���
            acceptor.accept(socket);

            // ��ʾ���ӽ����Ŀͻ���
            std::cout << socket.remote_endpoint().address() << std::endl;

            // ��ͻ��˷���hello world!
            boost::system::error_code ec;
            socket.write_some(buffer("hello world!"), ec);
            // ���������ӡ������Ϣ
            if (ec)
            {
                std::cout <<
                    boost::system::system_error(ec).what() << std::endl;
                break;
            }

            // �뵱ǰ�ͻ�������ɺ�ѭ�������ȴ���һ�ͻ�����
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
        // ����asio�඼��Ҫio_service����
        io_service iosev;
        // socket����
        ip::tcp::socket socket(iosev);
        // ���Ӷ˵㣬����ʹ���˱������ӣ������޸�IP��ַ����Զ������
        ip::tcp::endpoint ep(ip::address_v4::from_string(serverIp), port);
        // ���ӷ�����
        boost::system::error_code ec;
        socket.connect(ep, ec);
        // ���������ӡ������Ϣ
        if (ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return -1;
        }
        // ��������
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
        // ��ʼ�ȴ����ӣ���������
        std::cout << "CHelloWorld_Service::start" << std::endl;
        boost::shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
        // �������¼�ֻ��error_code������������boost::bind��socket�󶨽�ȥ
        m_acceptor.async_accept(*psocket,
            boost::bind(&CHelloWorld_Service::accept_handler, this, psocket, _1)
        );
        std::cout << "CHelloWorld_Service::start function end" << std::endl;
    }

    // �пͻ�������ʱaccept_handler����
    void accept_handler(boost::shared_ptr<tcp::socket> psocket, error_code ec)
    {
        std::cout << "CHelloWorld_Service::accept_handler" << std::endl;
        if (ec) {
            std::cout << "CHelloWorld_Service::accept_handler error: ec == true" << std::endl;
            return;
        }
        // �����ȴ�����
        start();
        // ��ʾԶ��IP
        std::cout << psocket->remote_endpoint().address() << std::endl;
        // ������Ϣ(������)
        boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
        psocket->async_write_some(buffer(*pstr),
            boost::bind(&CHelloWorld_Service::write_handler, this, pstr, _1, _2)
        );
        std::cout << "CHelloWorld_Service::accept_handler end" << std::endl;
    }

    // �첽д������ɺ�write_handler����
    void write_handler(boost::shared_ptr<std::string> pstr,
        error_code ec, size_t bytes_transferred)
    {
        if (ec)
            std::cout << "����ʧ��!" << std::endl;
        else
            std::cout << *pstr << " �ѷ���" << std::endl;
    }

    static void Test()
    {
        io_service iosev;
        CHelloWorld_Service sev(iosev);
        // ��ʼ�ȴ�����
        std::cout << "��ʼ�ȴ�����" << std::endl;
        sev.start();
        iosev.run();
        std::cout << "���Խ���" << std::endl;
    }

private:
    io_service& m_iosev;
    ip::tcp::acceptor m_acceptor;
};
