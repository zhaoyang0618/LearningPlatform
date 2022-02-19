using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Views;
using Microsoft.Extensions.Logging;
using Prism.Ioc;
using Prism.Modularity;
using Prism.Regions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Modules
{
    internal class HomeModule : IModule
    {
        private readonly IRegionManager _regionManager;
        private readonly ILogger _logger;

        public string Title
        {
            get;
            private set;
        }

        public HomeModule(ILogger logger, IRegionManager regionManager)
        {
            _logger = logger;
            _regionManager = regionManager;
            this.Title = "首页";
        }

        public void OnInitialized(IContainerProvider containerProvider)
        {
            _regionManager.RequestNavigate(RegionNames.ContentRegion, "HomeWnd");
        }

        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            _logger.LogInformation("ImageAnnotationModule.RegisterTypes");
            containerRegistry.RegisterForNavigation<HomeWnd>();
        }
    }

}
