using MaterialDesignThemes.Wpf;
using Microsoft.Xaml.Behaviors.Core;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.ViewModels
{
    internal class ItemMenu 
    {
        public ItemMenu(string header, PackIconKind icon, ActionCommand cmd)
        {
            Header = header;
            Icon = icon;
            Command = cmd;
        }

        public string Header { get; private set; }
        public PackIconKind Icon { get; private set; }
        public ActionCommand Command { get; private set; }
    }
}
