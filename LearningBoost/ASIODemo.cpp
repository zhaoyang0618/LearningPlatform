#include "ASIODemo.h"

/// 
/// https://zhuanlan.zhihu.com/p/499630880
/// 
#include <memory> // std::shared_ptr std::make_shared
#include <string> // std::string
#include <vector> // std::vector
#include <boost/asio.hpp> // boost::asio
//#include <boost/system.hpp> // boost::system::error_code

//使用协程: 需要C++20 
#include <coroutine> // std::coroutine_handle std::suspend_never

#ifndef HAS_SPDLOG
#  define HAS_SPDLOG 0
#endif

#if HAS_SPDLOG
#  include <spdlog/spdlog.h>
#else
#  include <iostream>
#  define SPDLOG_INFO(x) std::cout << (x) << "\n";
#endif

namespace asio = boost::asio;
using error_code_t = boost::system::error_code;

//* 回调版本
void server_callback(std::shared_ptr<asio::ip::tcp::socket> _socket)
{
    auto vec = std::make_shared<std::vector<char>>(1024, '\0');
    _socket->async_read_some(
        asio::buffer(vec->data(), vec->size()),
        [_socket, vec](error_code_t _error, std::size_t _bytes)
        {
            if (_error)
            {
                SPDLOG_INFO(_error.message());
                return;
            }
            SPDLOG_INFO(_bytes);

            SPDLOG_INFO(vec->data());

            auto str = std::make_shared<std::string>("server.");
            _socket->async_write_some(
                asio::buffer(str->c_str(), str->size()),
                [_socket, str](error_code_t _error, std::size_t _bytes)
                {
                    if (_error)
                    {
                        SPDLOG_INFO(_error.message());
                        return;
                    }
                    SPDLOG_INFO(_bytes);
                });
        });
}

void on_accept(
    error_code_t _error,
    asio::ip::tcp::acceptor& _acceptor,
    std::shared_ptr<asio::ip::tcp::socket> _socket)
{
    if (_error)
    {
        SPDLOG_INFO(_error.message());
        return;
    }

    auto newSocket = std::make_shared<asio::ip::tcp::socket>(_acceptor.get_executor());
    _acceptor.async_accept(
        *newSocket,
        [&_acceptor, newSocket](error_code_t error)
        {
            on_accept(error, _acceptor, newSocket);
        });

    auto remote_endpoint = _socket->remote_endpoint();
    SPDLOG_INFO(remote_endpoint.address().to_string());
    SPDLOG_INFO(remote_endpoint.port());

    server_callback(_socket); // 回調版本
}
//*

//* 协程版本
namespace lite
{
    struct task
    {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;
        struct promise_type
        {
            task get_return_object()
            {
                return { handle_type::from_promise(*this) };
            }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::exit(1); }
        };
        handle_type handle; // 協程句柄
};

    template<typename P, typename F>
        requires std::invocable<F, std::coroutine_handle<P>&>
    struct awaiter
    {
        using handle_type = std::coroutine_handle<P>;
        awaiter(F _f) : f(_f) {}
        bool await_ready() { return false; }
        void await_suspend(handle_type _handle) // _handle為傳入的task::handle
        {
            f(_handle);
        }
        void await_resume() {}
        F f;
    };

    template<typename P, std::invocable F>
    inline auto await(std::coroutine_handle<P>& _handle, F _f)
    {
        using handle_type = std::coroutine_handle<P>;
        auto fn = [&_handle, _f](handle_type& _hdl)
        {
            _handle = _hdl; // 把協程句柄傳出去
            _f();
        };
        return awaiter<P, decltype(fn)>{ fn };
    }
}

lite::task server_coro(std::shared_ptr<asio::ip::tcp::socket> _socket)
{
    auto& socket = *_socket;
    using promise_type = lite::task::promise_type;
    std::coroutine_handle<promise_type> handle; // 用於保存協程句柄

    error_code_t error;
    std::size_t bytes;

    std::vector<char> vec(1024, '\0');
    co_await lite::await(
        handle,
        [&handle, &socket, &error, &bytes, &vec]
        {
            socket.async_read_some(
                asio::buffer(vec.data(), vec.size()),
                [&handle, &error, &bytes](error_code_t _error, std::size_t _bytes)
                {
                    error = _error;
                    bytes = _bytes;
                    handle.resume();
                });
        });
    if (error)
    {
        SPDLOG_INFO(error.message());
        co_return;
    }
    SPDLOG_INFO(bytes);
    SPDLOG_INFO(vec.data());

    std::string str{ "server." };
    co_await lite::await(
        handle,
        [&handle, &socket, &error, &bytes, &str]
        {
            socket.async_write_some(
                asio::buffer(str.c_str(), str.size()),
                [&handle, &error, &bytes](error_code_t _error, std::size_t _bytes)
                {
                    error = _error;
                    bytes = _bytes;
                    handle.resume();
                });
        });
    if (error)
    {
        SPDLOG_INFO(error.message());
        co_return;
    }
    SPDLOG_INFO(bytes);
    SPDLOG_INFO("write finished.");
}

void on_accept2(
    error_code_t _error,
    asio::ip::tcp::acceptor& _acceptor,
    std::shared_ptr<asio::ip::tcp::socket> _socket)
{
    if (_error)
    {
        SPDLOG_INFO(_error.message());
        return;
    }

    auto newSocket = std::make_shared<asio::ip::tcp::socket>(_acceptor.get_executor());
    _acceptor.async_accept(
        *newSocket,
        [&_acceptor, newSocket](error_code_t error)
        {
            on_accept(error, _acceptor, newSocket);
        });

    auto remote_endpoint = _socket->remote_endpoint();
    SPDLOG_INFO(remote_endpoint.address().to_string());
    SPDLOG_INFO(remote_endpoint.port());

    server_coro(_socket); // 協程包裹回調
}
//*

void testCallback()
{
#if HAS_SPDLOG
    spdlog::set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] [t:%6t] [p:%6P] [%-20!!:%4#] %v");
#endif

    asio::io_context io_context;

    asio::ip::tcp::acceptor acceptor(
        io_context,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8888));

    {
        auto socket = std::make_shared<asio::ip::tcp::socket>(io_context);
        acceptor.async_accept(
            *socket,
            [&acceptor, socket](error_code_t error)
            {
                //on_accept(error, acceptor, socket);
                on_accept2(error, acceptor, socket);
            });
    }

    io_context.run();
}
