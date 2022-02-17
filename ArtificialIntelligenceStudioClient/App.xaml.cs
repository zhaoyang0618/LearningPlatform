using ArtificialIntelligenceStudioClient.Modules.ImageAnnotation;
using ArtificialIntelligenceStudioClient.Services;
using Microsoft.Extensions.Logging;
using Prism.Ioc;
using Prism.Modularity;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace ArtificialIntelligenceStudioClient
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// https://blog.csdn.net/dxm809/article/details/121049754
    /// </summary>
    public partial class App 
    {
        protected override Window CreateShell()
        {
            return Container.Resolve<MainWindow>();
        }

        protected override void RegisterTypes(IContainerRegistry containerRegistry)
        {
            //注入日志
            var factory = new NLog.Extensions.Logging.NLogLoggerFactory();
            ILogger logger = factory.CreateLogger("");
            containerRegistry.RegisterInstance<ILogger>(logger);
            //保存一些用户信息
            containerRegistry.RegisterSingleton<LocalAppContext>();
        }

        protected override void ConfigureModuleCatalog(IModuleCatalog moduleCatalog)
        {
            //所有的模块通过modules注册，然后按需加载
            moduleCatalog.AddModule<ImageAnnotationModule>();
        }
    }
}
