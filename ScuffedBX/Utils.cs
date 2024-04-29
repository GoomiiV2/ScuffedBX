using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Formats.Asn1.AsnWriter;

namespace ScuffedBX
{
    public static class Utils
    {
        public unsafe static string GimmeTheDammString(sbyte[] arr)
        {
            fixed (sbyte* nameBytes = arr)
            {
                var len = Array.IndexOf(arr, (sbyte)0);
                var str = Encoding.ASCII.GetString((byte*)nameBytes, len);
                return str;
            }
        }
    }
}
