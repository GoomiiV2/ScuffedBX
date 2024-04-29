using CppSharp;
using System.Xml.Linq;

namespace ScuffedBX.Gen
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var bindingGen = new BindingGen();
            ConsoleDriver.Run(bindingGen);

            var stdPath = Path.GetFullPath("../../../../../ScuffedBX/Bindings/Std.cs", Environment.CurrentDirectory);
            var txt = File.ReadAllText(stdPath);
            txt = txt.Replace("Std-symbols", "ScuffedBX.Native.dll");
            File.WriteAllText(stdPath, txt);
        }
    }
}
