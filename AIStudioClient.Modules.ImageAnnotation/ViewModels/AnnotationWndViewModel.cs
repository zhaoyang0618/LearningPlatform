using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Taiji.Utils;

namespace AIStudioClient.Modules.ImageAnnotation.ViewModels
{
    public class AnnotationWndViewModel : BindableBase
    {
        public string ImageFolder
        {
            get
            {
                return _imageFolder;
            }
            set
            {
                SetProperty(ref _imageFolder, value);
                if(!string.IsNullOrEmpty(_imageFolder))
                {
                    EnumImageFiles(_imageFolder);
                }
            }
        }
        public ObservableCollection<ImageLabelViewModel> ImageFiles
        {
            get
            {
                return _listImageFiles; 
            }
        }

        public AnnotationWndViewModel()
        {
            //或者有一个默认值
            this.ImageFolder = string.Empty;
        }

        string _imageFolder = null;
        ObservableCollection<ImageLabelViewModel> _listImageFiles = new ObservableCollection<ImageLabelViewModel>();
        void EnumImageFiles(string folder)
        {
            var files = System.IO.Directory.GetFiles(folder);
            _listImageFiles.Clear();
            foreach (var file in files)
            {
                if(file.ToLower().EndsWith(".jpg") || file.ToLower().EndsWith(".png"))
                {
                    var item = new ImageLabelViewModel()
                    {
                        Name = FileOperatorHelper.FileName(file),
                        ImageFilePath = file,
                    };
                    _listImageFiles.Add(item);
                }
            }
        }
    }
}
