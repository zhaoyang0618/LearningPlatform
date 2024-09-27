// See https://aka.ms/new-console-template for more information
// https://www.cnblogs.com/wucy/p/18111091/sementic-kernel-image-to-text
// 

using Microsoft.SemanticKernel.ImageToText;
using Microsoft.SemanticKernel;

Console.WriteLine("Hello, World!");
var kernel = Kernel.CreateBuilder().AddHuggingFaceImageToText("Salesforce/blip-image-captioning-base").Build();
IImageToTextService service = kernel.GetRequiredService<IImageToTextService>();
var imageBinary = File.ReadAllBytes(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "demo.png"));
var imageContent = new ImageContent(imageBinary) { MimeType = "image/jpeg" };
var textContent = await service.GetTextContentAsync(imageContent);
Console.WriteLine($"已识别图片中描述的内容: {textContent.Text}");
