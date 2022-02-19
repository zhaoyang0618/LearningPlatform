using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.ViewModels;
using Microsoft.Extensions.Configuration;
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
            moduleCatalog.AddModule<Modules.HomeModule>();
            moduleCatalog.AddModule<Modules.ImageAnnotation.ImageAnnotationModule>(InitializationMode.OnDemand);
            return;
            //读取配置文件:modules.json
            //获取当前app所在的目录
            var assembly = System.Reflection.Assembly.GetExecutingAssembly();
            var fi = new System.IO.FileInfo(assembly.Location);
            var builderModules = new ConfigurationBuilder().SetBasePath(fi.DirectoryName)
                .AddJsonFile("modules.json", optional: false, reloadOnChange: true);
            var modulesConfig = builderModules.Build();
            //然后加载模块
            var list = new List<ModuleItem>();
            var modulesDir = System.IO.Path.Combine(fi.DirectoryName, "Modules");
            modulesConfig.GetSection("modules").Bind(list);

            //
            var context = Container.Resolve<LocalAppContext>();
            foreach (var v in list)
            {
                //
                //string path = System.IO.Path.Combine(fi.DirectoryName, v.assemblyFile);
                //var dll = System.Reflection.Assembly.LoadFile(path);
                //var type = dll.GetType(v.moduleType);
                //moduleCatalog.AddModule(new ModuleInfo
                //{
                //    ModuleName = type.Name,
                //    ModuleType = type.AssemblyQualifiedName,
                //    Ref = new Uri(path, UriKind.RelativeOrAbsolute).AbsoluteUri
                //});
                //
                var m = new ModuleInfo()
                {
                    ModuleName = v.moduleName,
                    ModuleType = v.moduleType,
                    Ref = String.Format("file://{0}/{1}", fi.DirectoryName, v.assemblyFile),
                    InitializationMode = v.startupLoaded ? InitializationMode.WhenAvailable : InitializationMode.OnDemand,
                };
                if (context != null)
                {
                    context.Modules.Add(m);
                }
                moduleCatalog.AddModule(m);

            }
        }

        //protected override IModuleCatalog CreateModuleCatalog()
        //{
        //    var path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
        //    var directories = new string[] { "Modules"};//, "Services" 

        //    List<IModuleCatalogItem> components = new List<IModuleCatalogItem>();

        //    foreach (var dir in directories)
        //    {
        //        var dirCatelog = new DirectoryModuleCatalog() { ModulePath = $"{path}\\{dir}" };
        //        dirCatelog.Initialize();

        //        components.AddRange(dirCatelog.Items);
        //    }

        //    var catelog = new ModuleCatalog();

        //    foreach (var com in components)
        //    {
        //        catelog.Items.Add(com);
        //    }

        //    return catelog;
        //    //return new DirectoryModuleCatalog() { ModulePath = @".\modules" };
        //    //return base.CreateModuleCatalog();
        //}
    }
}
