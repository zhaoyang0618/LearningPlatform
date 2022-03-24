//var builder = WebApplication.CreateBuilder(args);
//var app = builder.Build();

//app.MapGet("/", () => "Hello World!");

//app.Run();
using App;
using Microsoft.AspNetCore.Server.Kestrel.Core;
using Server01;

var builder = WebApplication.CreateBuilder(args);
builder.WebHost.ConfigureKestrel(kestrel => kestrel.ConfigureEndpointDefaults(endpoint => endpoint.Protocols = HttpProtocols.Http2));
builder.Services.AddGrpc();
var app = builder.Build();
app.MapGrpcService<CalculatorService>();
app.Run();
