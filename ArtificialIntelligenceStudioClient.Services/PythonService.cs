using IronPython.Hosting;
using Microsoft.Scripting.Hosting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Services
{
    /// <summary>
    /// 通过这个服务，可以使用Python语言
    /// </summary>
    public class PythonService
    {
        ScriptEngine _scriptEngine = null;
        public ScriptEngine Engine
        {
            get { return _scriptEngine; }
        }

        public PythonService()
        {
            _scriptEngine = Python.CreateEngine();
        }
    }
}
