using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace DevicePortCommunicationClient.Utils
{
    class TestThreadPool
    {
        /// <summary>
        /// 测试线程池的用法
        /// https://www.cnblogs.com/chjf2008/archive/2012/11/29/2794842.html
        /// </summary>
        public static void Test()
        {

        }

        private static void Test1()
        {
            try
            {

                ThreadPool.SetMaxThreads(3, 3); //设置最大线程数
                for (int i = 0; i < 10; i++)
                {
                    ThreadPool.QueueUserWorkItem(new WaitCallback(Auto1), i);//线程池指定线程执行Auto方法
                }
                Console.ReadLine();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        private static void Auto1(object i)//多线程执行的方法
        {
            if (string.Equals(i, 2))
            {
                Thread.Sleep(2000);
            }

            Console.WriteLine(i.ToString());
        }

        private static void Test2()
        {
            try
            {
                ThreadPool.SetMaxThreads(3, 3); //设置最大线程数
                for (int i = 0; i < 10; i++)
                {
                    ThreadPool.QueueUserWorkItem(new WaitCallback(Auto2), i);//线程池指定线程执行Auto方法
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.WriteLine("结束了"); //这句要改   
            Console.ReadLine();
        }
        private static void Auto2(object i)//多线程执行的方法
        {
            if (string.Equals(i, 2))
            {
                Thread.Sleep(2000);
            }
            Console.WriteLine(i.ToString());
        }

        private static void Test3()
        {
            while (true)
            {
                Thread.Sleep(1000);//这句写着，主要是没必要循环那么多次。去掉也可以。
                int maxWorkerThreads, workerThreads;
                int portThreads;
                ThreadPool.GetMaxThreads(out maxWorkerThreads, out portThreads);
                ThreadPool.GetAvailableThreads(out workerThreads, out portThreads);
                if (maxWorkerThreads - workerThreads == 0)
                {
                    Console.WriteLine("结束了");
                    break;
                }
            }
        }

        static object locker = new object();
        static int runningThreads = 0;

        static void Test4()
        {
            try
            {
                ThreadPool.SetMaxThreads(4, 4); //设置最大线程数 using System.Threading;

                runningThreads = 10;
                for (int i = 0; i < runningThreads; i++)
                {

                    ThreadPool.QueueUserWorkItem(new WaitCallback(Auto), i);//线程池指定线程执行Auto方法
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            lock (locker)
            {
                while (runningThreads > 0)
                {
                    Monitor.Wait(locker);
                }
            }
            Console.WriteLine("结束了");
            Console.ReadLine();
        }

        public static void Auto(object i)//多线程执行的方法
        {
            if (string.Equals(i, 2))
            {
                Thread.Sleep(2000);
            }
            lock (locker)
            {
                runningThreads--;
                Monitor.Pulse(locker);
            }
            Console.WriteLine(i.ToString());
        }

        private static void testThreads()
        {
            ManualResetEvent[] _ManualEvents = new ManualResetEvent[10];
            for (int i = 0; i < 10; i++)
            {
                _ManualEvents[i] = new ManualResetEvent(false);
                ThreadPool.QueueUserWorkItem(new WaitCallback(testMethod), _ManualEvents[i]);
            }
            WaitHandle.WaitAll(_ManualEvents);
            // 线程结束后执行后面的主线程代码  
            Console.WriteLine("结束了");
            Console.ReadLine();
        }
        private static void testMethod(object objEvent)
        {
            //TODO: Add your code here
            ManualResetEvent e = (ManualResetEvent)objEvent;
            e.Set();
        }
    }
}
