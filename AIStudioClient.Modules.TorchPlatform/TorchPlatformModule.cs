using AIStudioClient.Modules.TorchPlatform.Views;
using ArtificialIntelligenceStudioClient.Core;
using Microsoft.Extensions.Logging;
using Prism.Ioc;
using Prism.Modularity;
using Prism.Regions;
using System;

namespace AIStudioClient.Modules.TorchPlatform
{
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
