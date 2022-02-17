using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Modules.ImageAnnotation.Views;
using Prism.Ioc;
using Prism.Modularity;
using Prism.Regions;
using System;

namespace ArtificialIntelligenceStudioClient.Modules.ImageAnnotation
{
    public class ImageAnnotationModule : IModule
    {
        private readonly IRegionManager _regionManager;

        public ImageAnnotationModule(IRegionManager regionManager)
        {
            _regionManager = regionManager;
        }

        public void OnInitialized(IContainerProvider containerProvider)
        {
            _regionManager.RequestNavigate(RegionNames.ContentRegion, "AnnotationWnd");
        }

        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            containerRegistry.RegisterForNavigation<AnnotationWnd>();
        }
    }
}
