using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Modules.ImageAnnotation.Views;
using Microsoft.Extensions.Logging;
using Prism.Ioc;
using Prism.Modularity;
using Prism.Regions;
using System;

namespace ArtificialIntelligenceStudioClient.Modules.ImageAnnotation
{
    public class ImageAnnotationModule : IModule
    {
        private readonly IRegionManager _regionManager;
        private readonly ILogger _logger;

        public string Title
        {
            get;
            private set;
        }

        public ImageAnnotationModule(ILogger logger, IRegionManager regionManager)
        {
            _logger = logger;
            _regionManager = regionManager;
            this.Title = "Í¼Ïñ±ê×¢";
        }

        public void OnInitialized(IContainerProvider containerProvider)
        {
            //_regionManager.RequestNavigate(RegionNames.ContentRegion, "AnnotationWnd");
            //_regionManager.RegisterViewWithRegion(RegionNames.ContentRegion, typeof(AnnotationWnd));
        }

        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            _logger.LogInformation("ImageAnnotationModule.RegisterTypes");
            containerRegistry.RegisterForNavigation<AnnotationWnd>();
        }
    }
}
