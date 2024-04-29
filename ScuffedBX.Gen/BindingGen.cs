using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
using CppSharp.Generators.CSharp;
using CppSharp.Passes;
using CppSharp.Types;
using Mono.Options;
using Std.Vector;

namespace ScuffedBX.Gen
{
    public class BindingGen : ILibrary
    {
        public void Postprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Preprocess(Driver driver, ASTContext ctx)
        {
            
        }

        public void Setup(Driver driver)
        {
            var baseDir = Path.GetFullPath("../../../../../", Environment.CurrentDirectory);

            var options = driver.Options;
            options.GeneratorKind = GeneratorKind.CSharp;
            var module = options.AddModule("ScuffedBX.Native");
            module.OutputNamespace = "";
            module.Headers.Add(Path.Combine(baseDir, "ScuffedBX.Native", "ScuffedBX.h"));
            options.OutputDir = Path.Combine(baseDir, "ScuffedBX", "Bindings");
        }

        public void SetupPasses(Driver driver)
        {
            var macroPass = new CheckMacroPass();
            driver.AddTranslationUnitPass(macroPass);
            driver.AddGeneratorOutputPass(new TypeConversionPass());
        }
    }

    //[TypeMap("Vector3", GeneratorKind = GeneratorKind.CSharp)]
    public class ScuffedBX_Vector3 : TypeMap
    {

    }

    public class TypeConversionPass : GeneratorOutputPass
    {
        public override void VisitClass(Block block)
        {
            var nearEndBlock = block.Blocks[^1];
            nearEndBlock.WriteLine("");

            if (block.Blocks[0].Text.ToString().Contains("Vector3"))
            {
                nearEndBlock.WriteLine("public static implicit operator System.Numerics.Vector3(Vector3 vec) => new System.Numerics.Vector3(vec.X, vec.Y, vec.Z);");
                nearEndBlock.WriteLine("public static implicit operator Vector3(System.Numerics.Vector3 vec) => new Vector3(vec.X, vec.Y, vec.Z);");
            }
            else if (block.Blocks[0].Text.ToString().Contains("Vector2"))
            {
                nearEndBlock.WriteLine("public static implicit operator System.Numerics.Vector2(Vector2 vec) => new System.Numerics.Vector2(vec.X, vec.Y);");
                nearEndBlock.WriteLine("public static implicit operator Vector2(System.Numerics.Vector2 vec) => new Vector2(vec.X, vec.Y);");
            }
            else if (block.Blocks[0].Text.ToString().Contains("Vector4"))
            {
                nearEndBlock.WriteLine("public static implicit operator System.Numerics.Vector4(Vector4 vec) => new System.Numerics.Vector4(vec.X, vec.Y, vec.Z, vec.W);");
                nearEndBlock.WriteLine("public static implicit operator Vector4(System.Numerics.Vector4 vec) => new Vector4(vec.X, vec.Y, vec.Z, vec.W);");

                nearEndBlock.WriteLine("public static implicit operator System.Numerics.Quaternion(Vector4 vec) => new System.Numerics.Quaternion(vec.X, vec.Y, vec.Z, vec.W);");
                nearEndBlock.WriteLine("public static implicit operator Vector4(System.Numerics.Quaternion vec) => new Vector4(vec.X, vec.Y, vec.Z, vec.W);");
            }

            var props = block.Blocks.Where(x => x.Kind == BlockKind.Property);
            foreach (var prop in props)
            {
                var strParts = prop.Blocks[0].Text.ToString().TrimStart(' ').Split(' ');
                var propType = strParts[1];
                if (propType == "sbyte[]")
                {
                    var propName = strParts[2].TrimEnd(['\r', '\n']);
                    nearEndBlock.WriteLine($"public string {propName}Str => Utils.GimmeTheDammString({propName});");
                }
            }

            base.VisitClass(block);
        }



        public override void VisitGeneratorOutput(GeneratorOutput output)
        {
            base.VisitGeneratorOutput(output);
        }
    }
}
