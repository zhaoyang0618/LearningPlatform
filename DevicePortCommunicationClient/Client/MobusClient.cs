using DotNetty.Buffers;
using DotNetty.Transport.Bootstrapping;
using DotNetty.Transport.Channels;
using DotNetty.Transport.Channels.Sockets;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

namespace DevicePortCommunicationClient.Client
{
    class MobusClient
    {
        private static NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();
        MultithreadEventLoopGroup _group = null;
        Bootstrap _bootstrap = null;
        IChannel _clientChannel = null;
        public async void Start(string ip, int port)
        {
            try
            {
                _group = new MultithreadEventLoopGroup();
                _bootstrap = new Bootstrap();
                _bootstrap
                    .Group(_group)
                    .Channel<TcpSocketChannel>()
                    .Handler(new ActionChannelInitializer<ISocketChannel>(channel =>
                    {
                        IChannelPipeline pipeline = channel.Pipeline;
                        pipeline.AddLast(new MobusClientHandler());
                    }));

                _clientChannel = await _bootstrap.ConnectAsync(new IPEndPoint(IPAddress.Parse(ip), port));
            }
            catch (Exception exp)
            {
                if (exp.InnerException != null)
                {
                    logger.Error(exp.InnerException, "Mobus模拟客户端异常");
                }
                else
                {
                    logger.Error(exp, "Mobus模拟客户端异常");
                }
            }
        }

        public async void Stop()
        {
            if (_clientChannel != null)
            {
                await _clientChannel.CloseAsync();
                _clientChannel = null;
            }
            if (_group != null)
            {
                await _group.ShutdownGracefullyAsync();
                _group = null;
            }
        }

        public void Send(string cmd)
        {
            if (_clientChannel != null)
            {
                var data = Encoding.ASCII.GetBytes(cmd, 0, cmd.Length);
                var d = Unpooled.CopiedBuffer(data);
                _clientChannel.WriteAndFlushAsync(d);
            }
        }

        public void Send(byte[] data)
        {
            if (_clientChannel != null)
            {
                var d = Unpooled.CopiedBuffer(data);
                _clientChannel.WriteAndFlushAsync(d);
            }
        }
    }

    class MobusClientHandler : SimpleChannelInboundHandler<IByteBuffer>
    {
        /// <summary>
        /// Read0是DotNetty特有的对于Read方法的封装
        /// 封装实现了：
        /// 1. 返回的message的泛型实现
        /// 2. 丢弃非该指定泛型的信息
        /// </summary>
        /// <param name="ctx"></param>
        /// <param name="msg"></param>
        protected override void ChannelRead0(IChannelHandlerContext ctx, IByteBuffer msg)
        {
            if (msg != null)
            {
                System.Diagnostics.Debug.WriteLine("Receive From Server:" + msg.ToString(Encoding.ASCII));
            }
            //ctx.WriteAsync(Unpooled.CopiedBuffer(msg));
        }

        public override void ChannelRead(IChannelHandlerContext ctx, object msg)
        {
            if (msg != null)
            {
                var message = msg as IByteBuffer;
                var result = Unpooled.CopiedBuffer(message);
                System.Diagnostics.Debug.WriteLine(result.Array);
                System.Diagnostics.Debug.WriteLine("Receive From Server:" + message.ToString(Encoding.ASCII));
            }
        }

        public override void ChannelReadComplete(IChannelHandlerContext context)
        {
            context.Flush();
        }

        public override void ChannelActive(IChannelHandlerContext context)
        {
            //System.Diagnostics.Debug.WriteLine("发送Hello World");
            //context.WriteAndFlushAsync(Unpooled.CopiedBuffer(Encoding.UTF8.GetBytes("Hello World!")));
        }

        public override void ExceptionCaught(IChannelHandlerContext context, Exception exception)
        {
            System.Diagnostics.Debug.WriteLine(exception);
            context.CloseAsync();
        }
    }
}
