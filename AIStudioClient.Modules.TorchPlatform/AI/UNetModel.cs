using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TorchSharp;
using static TorchSharp.torch;

namespace AIStudioClient.Modules.TorchPlatform.AI
{
    /// <summary>
    /// 原来是使用python编写的，争取把它们转化为C#代码
    /// https://blog.csdn.net/kobayashi_/article/details/108951993
    /// </summary>
    internal class UNetModel : nn.Module
    {
        public UNetModel(int features, int hidden, int outputs)
        : base(nameof(BPNetModel))
        {
            RegisterComponents();
        }

        public override Tensor forward(Tensor input)
        {
            throw new NotImplementedException();
        }
    }

    /// class DownsampleLayer(nn.Module):
    /// def __init__(self, in_ch, out_ch):
    ///     super(DownsampleLayer, self).__init__()
    ///     self.Conv_BN_ReLU_2=nn.Sequential(
    ///         nn.Conv2d(in_channels=in_ch,out_channels=out_ch,kernel_size=3,stride=1,padding=1),
    ///         nn.BatchNorm2d(out_ch),
    ///         nn.ReLU(),
    ///         nn.Conv2d(in_channels=out_ch, out_channels=out_ch, kernel_size=3, stride=1,padding=1),
    ///         nn.BatchNorm2d(out_ch),
    ///         nn.ReLU()
    ///     )
    ///     self.downsample=nn.Sequential(
    ///         nn.Conv2d(in_channels=out_ch,out_channels=out_ch,kernel_size=3,stride=2,padding=1),
    ///         nn.BatchNorm2d(out_ch),
    ///         nn.ReLU()
    ///     )

    /// def forward(self, x):
    ///     """
    ///     :param x:
    ///     :return: out输出到深层，out_2输入到下一层，
    ///     """
    ///     out=self.Conv_BN_ReLU_2(x)
    ///     out_2 = self.downsample(out)
    ///     return out,out_2
    class DownSampleLayer : nn.Module
    {
        nn.Module Conv_BN_ReLU_2;
        nn.Module downsample;
        public DownSampleLayer(int in_ch, int out_ch)
        : base(nameof(DownSampleLayer))
        {
            RegisterComponents();
            Conv_BN_ReLU_2 = nn.Sequential(
                nn.Conv2d(inputChannel: in_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU(),
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
            downsample = nn.Sequential(
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 2, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
        }

        public (Tensor, Tensor) forward(Tensor input)
        {
            var out_1 = this.Conv_BN_ReLU_2.forward(input);
            var out_2 = this.downsample.forward(out_1);
            return (out_1, out_2);//这一步的返回值似乎存在问题
        }
    }

    //class UpSampleLayer(nn.Module):
    //def __init__(self, in_ch, out_ch):
    //    # 512-1024-512
    //    # 1024-512-256
    //    # 512-256-128
    //    # 256-128-64
    //    super(UpSampleLayer, self).__init__()
    //    self.Conv_BN_ReLU_2 = nn.Sequential(
    //        nn.Conv2d(in_channels=in_ch, out_channels=out_ch*2, kernel_size=3, stride=1,padding=1),
    //        nn.BatchNorm2d(out_ch*2),
    //        nn.ReLU(),
    //        nn.Conv2d(in_channels=out_ch*2, out_channels=out_ch*2, kernel_size=3, stride=1,padding=1),
    //        nn.BatchNorm2d(out_ch*2),
    //        nn.ReLU()
    //    )
    //    self.upsample=nn.Sequential(
    //        nn.ConvTranspose2d(in_channels=out_ch*2,out_channels=out_ch,kernel_size=3,stride=2,padding=1,output_padding=1),
    //        nn.BatchNorm2d(out_ch),
    //        nn.ReLU()
    //    )

    //def forward(self, x, out):
    //    '''
    //    :param x: 输入卷积层
    //    :param out:与上采样层进行cat
    //    :return:
    //    '''
    //    x_out=self.Conv_BN_ReLU_2(x)
    //    x_out = self.upsample(x_out)
    //    cat_out=torch.cat((x_out,out),dim=1)
    //    return cat_out
    class UpSampleLayer : nn.Module
    {
        nn.Module Conv_BN_ReLU_2;
        nn.Module downsample;
        public UpSampleLayer(int in_ch, int out_ch)
        : base(nameof(DownSampleLayer))
        {
            RegisterComponents();
            Conv_BN_ReLU_2 = nn.Sequential(
                nn.Conv2d(inputChannel: in_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU(),
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
            downsample = nn.Sequential(
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 2, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
        }

        public override Tensor forward(Tensor input)
        {
            var out_1 = this.Conv_BN_ReLU_2.forward(input);
            var out_2 = this.downsample.forward(out_1);
            return out_2;//这一步的返回值似乎存在问题
        }
    }

    //class UNet(nn.Module):
    //def __init__(self):
    //    super(UNet, self).__init__()
    //    out_channels=[2**(i+6) for i in range(5)] #[64, 128, 256, 512, 1024]
    //    #下采样
    //    self.d1=DownsampleLayer(3, out_channels[0])#3-64
    //    self.d2=DownsampleLayer(out_channels[0], out_channels[1])#64-128
    //    self.d3=DownsampleLayer(out_channels[1], out_channels[2])#128-256
    //    self.d4=DownsampleLayer(out_channels[2], out_channels[3])#256-512
    //    #上采样
    //    self.u1=UpSampleLayer(out_channels[3], out_channels[3])#512-1024-512
    //    self.u2=UpSampleLayer(out_channels[4], out_channels[2])#1024-512-256
    //    self.u3=UpSampleLayer(out_channels[3], out_channels[1])#512-256-128
    //    self.u4=UpSampleLayer(out_channels[2], out_channels[0])#256-128-64
    //    #输出
    //    self.o=nn.Sequential(
    //        nn.Conv2d(out_channels[1], out_channels[0], kernel_size=3,stride=1,padding=1),
    //        nn.BatchNorm2d(out_channels[0]),
    //        nn.ReLU(),
    //        nn.Conv2d(out_channels[0], out_channels[0], kernel_size=3, stride=1, padding=1),
    //        nn.BatchNorm2d(out_channels[0]),
    //        nn.ReLU(),
    //        nn.Conv2d(out_channels[0],3,3,1,1),
    //        nn.Sigmoid(),
    //        # BCELoss
    //    )
    //def forward(self, x):
    //    out_1,out1=self.d1(x)
    //    out_2, out2 = self.d2(out1)
    //    out_3,out3=self.d3(out2)
    //    out_4, out4 = self.d4(out3)
    //    out5=self.u1(out4, out_4)
    //    out6 = self.u2(out5, out_3)
    //    out7=self.u3(out6, out_2)
    //    out8 = self.u4(out7, out_1)
    //    out=self.o(out8)
    //    return out
    class UNet : nn.Module
    {
        nn.Module Conv_BN_ReLU_2;
        nn.Module downsample;
        public UNet(int in_ch, int out_ch)
        : base(nameof(DownSampleLayer))
        {
            RegisterComponents();
            Conv_BN_ReLU_2 = nn.Sequential(
                nn.Conv2d(inputChannel: in_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU(),
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 1, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
            downsample = nn.Sequential(
                nn.Conv2d(inputChannel: out_ch, outputChannel: out_ch, kernelSize: 3, stride: 2, padding: 1),
                nn.BatchNorm2d(out_ch),
                nn.ReLU());
        }

        public override Tensor forward(Tensor input)
        {
            var out_1 = this.Conv_BN_ReLU_2.forward(input);
            var out_2 = this.downsample.forward(out_1);
            return out_2;//这一步的返回值似乎存在问题
        }
    }

    //    class SEGData(Dataset):
    //    def __init__(self):
    //        '''
    //        根据标注文件去取图片
    //        '''
    //        self.img_path=IMG_PATH
    //        self.label_path=SEGLABE_PATH
    //        self.label_data= os.listdir(self.label_path)
    //        self.totensor= torchvision.transforms.ToTensor()
    //        self.resizer= torchvision.transforms.Resize((256, 256))
    //    def __len__(self):
    //        return len(self.label_data)
    //    def __getitem__(self, item):
    //        '''
    //        由于输出的图片的尺寸不同，我们需要转换为相同大小的图片。首先转换为正方形图片，然后缩放的同样尺度(256*256)。
    //        否则dataloader会报错。
    //        '''
    //        # 取出图片路径
    //        img_name = os.path.join(self.label_path, self.label_data[item])
    //        img_name = os.path.split(img_name)
    //        img_name = img_name[-1]
    //        img_name = img_name.split('.')
    //        img_name = img_name[0] + '.jpg'
    //        img_data = os.path.join(self.img_path, img_name)
    //        label_data = os.path.join(self.label_path, self.label_data[item])
    //        # 将图片和标签都转为正方形
    //        img = Image.open(img_data)
    //        label = Image.open(label_data)
    //        w, h = img.size
    //# 以最长边为基准，生成全0正方形矩阵
    //        slide = max(h, w)
    //        black_img = torchvision.transforms.ToPILImage() (torch.zeros(3, slide, slide))
    //        black_label = torchvision.transforms.ToPILImage() (torch.zeros(3, slide, slide))
    //        black_img.paste(img, (0, 0, int (w), int (h)))  # patse在图中央和在左上角是一样的
    //        black_label.paste(label, (0, 0, int (w), int (h)))
    //        # 变为tensor,转换为统一大小256*256
    //        img = self.resizer(black_img)
    //        label = self.resizer(black_label)
    //        img = self.totensor(img)
    //        label = self.totensor(label)
    //        return img,label
    class SEGData : utils.data.Dataset
    {
        public SEGData()
        {

        }

        public override long Count => throw new NotImplementedException();

        public override Dictionary<string, Tensor> GetTensor(long index)
        {
            throw new NotImplementedException();
        }
    }
}
