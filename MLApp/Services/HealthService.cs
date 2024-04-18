using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace MLApp.Services
{
    internal class HealthService
    {
        HttpClient _client;
        JsonSerializerOptions _serializerOptions;

        //public List<TodoItem> Items { get; private set; }

        public HealthService()
        {
            _client = new HttpClient();
            _client.DefaultRequestHeaders.Add("token", "{\"agentId\":\"nsJ3of7aG\",\"appKey\":\"78f6234ddn2fncc0G4G4\",\"appSecret\":\"f81bb7d385fcd8d8704fa6b935561c28bdbc48a6d021c51f764910b3bb044b89\"}");
            //_client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            _client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/oapi.zyws.v1.0+json"));
            _serializerOptions = new JsonSerializerOptions
            {
                PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                WriteIndented = true
            };
        }

        public async void Confirm()
        {
            var info = new Info();
            var task = await _client.PutAsJsonAsync("https://oapi.wkzwy.cn/api/healthCheckAppointment/confirm", info).ContinueWith(
                async (postTask) =>
                {
                    try
                    {
                        HttpResponseMessage response = postTask.Result;
                        await response.EnsureSuccessStatusCodeAsync();
                        await response.Content.ReadAsStringAsync().ContinueWith((readTask) =>
                        {
                            System.Diagnostics.Debug.WriteLine(readTask.Result);
                        });
                    }
                    catch (Exception exp)
                    {
                        System.Diagnostics.Debug.Write(exp.ToString());
                    }
                });
            await task;
        }
    }

    public class Info
    {
        public string appointmentId { get; set; } = "BgKCOCB9P";
        public string startTime { get; set; }
        public string endTime{ get; set; }
    }
    public static class HttpResponseMessageExtensions
    {
        public static async Task EnsureSuccessStatusCodeAsync(this HttpResponseMessage response)
        {
            if (response == null)
            {
                return;
            }

            if (response.IsSuccessStatusCode)
            {
                return;
            }

            if (response.Content == null)
            {
                throw new SimpleHttpResponseException(response.StatusCode, "返回数据为空!");
            }

            var content = await response.Content.ReadAsStringAsync();

            if (response.Content != null)
                response.Content.Dispose();

            throw new SimpleHttpResponseException(response.StatusCode, content);
        }
    }
    public class SimpleHttpResponseException : Exception
    {
        public HttpStatusCode StatusCode { get; private set; }

        public SimpleHttpResponseException(HttpStatusCode statusCode, string content) : base(content)
        {
            StatusCode = statusCode;
        }
    }
}
