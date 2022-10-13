using Architecture.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace Architecture.Web.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class PrizeController : ControllerBase
    {
        [HttpGet]
        public List<Prize> List()
        {
            return new List<Prize>() { new Prize() { Id=1, Name="AAA" }, new Prize() { Id=2, Name="BBBBBB" } };
        }
    }
}
