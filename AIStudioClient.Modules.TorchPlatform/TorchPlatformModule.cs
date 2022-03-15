using AIStudioClient.Modules.TorchPlatform.Views;
using ArtificialIntelligenceStudioClient.Core;
using Microsoft.Extensions.Logging;
using Prism.Ioc;
using Prism.Modularity;
using Prism.Regions;
using System;

namespace AIStudioClient.Modules.TorchPlatform
{
    /// <summary>
    /// 一些神经网络相关的例子
    /// 原来是使用python编写的，争取把它们转化为C#代码
    /// https://blog.csdn.net/kobayashi_/article/details/108951993
    /// 
    /// https://blog.csdn.net/asialee_bird/article/details/94836962
    /// https://www.codeproject.com/Articles/5182988/Solving-Iris-classification-using-XGBoost-and-Csha
    /// 
    /// 图像处理
    /// https://www.cnblogs.com/hellotim/p/14023632.html
    /// </summary>
    public class TorchPlatformModule : IModule
    {
        private readonly IRegionManager _regionManager;
        private readonly ILogger _logger;

        public string Title
        {
            get;
            private set;
        }

        public TorchPlatformModule(ILogger logger, IRegionManager regionManager)
        {
            _logger = logger;
            _regionManager = regionManager;
            this.Title = "Torch学习";
        }

        public void OnInitialized(IContainerProvider containerProvider)
        {
            //_regionManager.RequestNavigate(RegionNames.ContentRegion, this.Title);
        }

        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            _logger.LogInformation("TorchPlatformModule.RegisterTypes");
            containerRegistry.RegisterForNavigation<TorchMainWnd>();
        }
    }

}
