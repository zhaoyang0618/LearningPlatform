#include "ASIO2Exam.h"
#include "../LearningCpp/TimeLost.h"

void SendData(asio2::tcp_client& client, TimeLost& tl)
{
	tl.reset();
	// 发送数据
	// 01 03 00 00 00 07 04 08;七合一
	// 01 03 00 06 00 01 64 0B;一氧化碳
	std::string sss;
	sss += (char)(0x01);
	sss += (char)(0x03);
	sss += (char)(0x00);
	sss += (char)(0x06);
	sss += (char)(0x00);
	sss += (char)(0x01);
	sss += (char)(0x64);
	sss += (char)(0x0B);
	auto count = sss.size();
	auto ret = client.send(sss.c_str(), count);
	if (ret)
	{
		printf("send successed\r\n");
	}
	else
	{
		printf("send failed\r\n");
	}
}

/// <summary>
/// 从七合一设备读取数据
/// </summary>
void ReadDataFrom71Device()
{
//#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)
//	// Detected memory leaks on windows system
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif

	std::string_view host = "192.168.225.51";
	std::string_view port = "10002";

	TimeLost tl;
	asio2::tcp_client client;

	client.bind_connect([&](asio::error_code ec)
		{
			if (asio2::get_last_error())
				printf("connect failure : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
			else
			{
				printf("connect success : %s %u\n", client.local_address().c_str(), client.local_port());
				tl.reset();
				// 发送数据
				// 01 03 00 00 00 07 04 08;七合一
				// 01 03 00 06 00 01 64 0B;一氧化碳
				std::string sss;
				sss += (char)(0x01);
				sss += (char)(0x03);
				sss += (char)(0x00);
				sss += (char)(0x06);
				sss += (char)(0x00);
				sss += (char)(0x01);
				sss += (char)(0x64);
				sss += (char)(0x0B);
				auto count = sss.size();
				auto ret = client.send(sss.c_str(), count);
				if (ret)
				{
					printf("send successed\r\n");
				}
				else
				{
					printf("send failed\r\n");
				}
			}

			//std::string s;
			//s += '<';
			//int len = 128 + std::rand() % (300);
			//for (int i = 0; i < len; i++)
			//{
			//	s += (char)((std::rand() % 26) + 'a');
			//}
			////s += '>';
			//s += "\r\n";

			//client.send(s);


		}).bind_disconnect([](asio::error_code ec)
			{
				printf("disconnect : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
			}).bind_recv([&](std::string_view sv)
				{
					//打印出接收到的数据
					printf("recv : %u %.*s\n", (unsigned)sv.size(), (int)sv.size(), sv.data());
					auto delta = tl.elspsed();
					printf("time lost: %d\n", (int)delta);

					//std::string s;
					//s += '<';
					//int len = 128 + std::rand() % (300);
					//for (int i = 0; i < len; i++)
					//{
					//	s += (char)((std::rand() % 26) + 'a');
					//}
					////s += '>';
					//s += "\r\n";

					//// demo of force a packet of data to be sent twice 
					//client.send(s.substr(0, s.size() / 2), []() {});
					//std::this_thread::sleep_for(std::chrono::milliseconds(10));
					//client.send(s.substr(s.size() / 2), [](std::size_t bytes_sent) {});

					//休息1秒钟，然后马上发送下一条数据
					std::this_thread::sleep_for(std::chrono::seconds(1));
					SendData(client, tl);

				}).bind_init([](asio::error_code ec)
					{
						printf("init : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
					});

			//client.start(host, port, '>');
			client.async_start(host, port);

			//等待
			while (std::getchar() != '\n');
}

void testASIO2Client()
{
	ReadDataFrom71Device();
}
