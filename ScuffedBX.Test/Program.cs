
using System.Security.Cryptography;
using System.Text;

namespace ScuffedBX.Test
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("UwU :3");

            //LoopPrintTest();
            WriteTestCube();
            //TestImportFbx();
            //ScuffedBX.PrintFBXInfo(@"pl2300.mmesh.fbx");
            //ImportAndReexportTest();
        }

        private static void WriteTestCube()
        {
            var scene = new Scene();
            scene.SetName("Scuffed Test Cube");
            uint matIdx  = scene.AddMatInfo(new MatInfo("TestMat Top +Y", "TestTex.png"));
            uint mat2Idx = scene.AddMatInfo(new MatInfo("TestMat Bottom -Y", "TestTex.png"));
            uint mat3Idx = scene.AddMatInfo(new MatInfo("TestMat Left -X", "TestTex.png"));
            uint mat4Idx = scene.AddMatInfo(new MatInfo("TestMat Right +X", "TestTex.png"));
            uint mat5Idx = scene.AddMatInfo(new MatInfo("TestMat Front +Z", "TestTex.png"));
            uint mat6Idx = scene.AddMatInfo(new MatInfo("TestMat Back -Z", "TestTex.png"));

            var cube = new Model();
            cube.SetName("Cubey Cube :>");
            cube.NumBonesPerVert = 1;

            var corners = new System.Numerics.Vector3[]
            {
                new System.Numerics.Vector3(-0.5f, +0.5f, -0.5f),
                new System.Numerics.Vector3(+0.5f, +0.5f, -0.5f),
                new System.Numerics.Vector3(+0.5f, +0.5f, +0.5f),
                new System.Numerics.Vector3(-0.5f, +0.5f, +0.5f),

                new System.Numerics.Vector3(-0.5f, -0.5f, +0.5f),
                new System.Numerics.Vector3(+0.5f, -0.5f, +0.5f),
                new System.Numerics.Vector3(+0.5f, -0.5f, -0.5f),
                new System.Numerics.Vector3(-0.5f, -0.5f, -0.5f),
            };

            var normals = new System.Numerics.Vector3[]
            {
                new System.Numerics.Vector3(1, 0, 0),
                new System.Numerics.Vector3(-1, 0, 0),
                new System.Numerics.Vector3(0, 1, 0),
                new System.Numerics.Vector3(0, -1, 0),
                new System.Numerics.Vector3(0, 0, 1),
                new System.Numerics.Vector3(0, 0, -1),
            };

            var uvs = new Vector2[]
            {
                new Vector2(1, 1),
                new Vector2(0, 1),
                new Vector2(0, 0),
                new Vector2(1, 0),
            };

            var colors = new ColorRGBA[]
            {
                new ColorRGBA(1, 0, 0, 1),
                new ColorRGBA(0, 1, 0, 1),
                new ColorRGBA(0, 0, 1, 1),
                new ColorRGBA(0.5f, 0.5f, 0, 1),
            };

            cube.AddVert(corners[0], normals[2], null, uvs[0], colors[0], [new BoneWeight(2, 1f)]);
            cube.AddVert(corners[1], normals[2], null, uvs[1], colors[0], [new BoneWeight(3, 1f)]);
            cube.AddVert(corners[2], normals[2], null, uvs[2], colors[0], [new BoneWeight(4, 1f)]);
            cube.AddVert(corners[3], normals[2], null, uvs[3], colors[0], [new BoneWeight(5, 1f)]);
           
            cube.AddVert(corners[4], normals[3], null, uvs[0], colors[1], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[5], normals[3], null, uvs[1], colors[1], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[6], normals[3], null, uvs[2], colors[1], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[7], normals[3], null, uvs[3], colors[1], [new BoneWeight(6, 1f)]);

            cube.AddVert(corners[0], normals[1], null, uvs[0], colors[0], [new BoneWeight(2, 1f)]);
            cube.AddVert(corners[3], normals[1], null, uvs[1], colors[1], [new BoneWeight(5, 1f)]);
            cube.AddVert(corners[4], normals[1], null, uvs[2], colors[2], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[7], normals[1], null, uvs[3], colors[3], [new BoneWeight(6, 1f)]);

            cube.AddVert(corners[2], normals[0], null, uvs[0], colors[0], [new BoneWeight(4, 1f)]);
            cube.AddVert(corners[1], normals[0], null, uvs[1], colors[1], [new BoneWeight(3, 1f)]);
            cube.AddVert(corners[6], normals[0], null, uvs[2], colors[2], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[5], normals[0], null, uvs[3], colors[3], [new BoneWeight(6, 1f)]);

            cube.AddVert(corners[1], normals[5], null, uvs[0], colors[0], [new BoneWeight(3, 1f)]);
            cube.AddVert(corners[0], normals[5], null, uvs[1], colors[1], [new BoneWeight(2, 1f)]);
            cube.AddVert(corners[7], normals[5], null, uvs[2], colors[2], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[6], normals[5], null, uvs[3], colors[3], [new BoneWeight(6, 1f)]);

            cube.AddVert(corners[3], normals[4], null, uvs[0], colors[0], [new BoneWeight(5, 1f)]);
            cube.AddVert(corners[2], normals[4], null, uvs[1], colors[1], [new BoneWeight(4, 1f)]);
            cube.AddVert(corners[5], normals[4], null, uvs[2], colors[2], [new BoneWeight(6, 1f)]);
            cube.AddVert(corners[4], normals[4], null, uvs[3], colors[3], [new BoneWeight(6, 1f)]);

            var indices = new uint[] {
                2, 1, 0, 3, 2, 0,
                6, 5, 4, 7, 6, 4,
                10, 9, 8, 11, 10, 8,
                14, 13, 12, 15, 14, 12,
                18, 17, 16, 19, 18, 16,
                22, 21, 20, 23, 22, 20
            };
            cube.SetIndices(indices, indices.Length);

            cube.AddMatSection(new MatSection(matIdx, 0, 6)); // top
            cube.AddMatSection(new MatSection(mat2Idx, 6, 6)); // bottom
            cube.AddMatSection(new MatSection(mat3Idx, 12, 6)); // left
            cube.AddMatSection(new MatSection(mat4Idx, 18, 6)); // right
            cube.AddMatSection(new MatSection(mat5Idx, 24, 6)); // front
            cube.AddMatSection(new MatSection(mat6Idx, 30, 6)); // back

            var skel = new Skeleton("Skeleton");
            skel.AddBone(new Bone("Root", -1));
            skel.AddBone(new Bone("Top", 0) { Position = new Vector3(0, 0.5f, 0) });
            skel.AddBone(new Bone("Top 1", 1) { Position = new Vector3(-0.5f, 0, -0.5f) });
            skel.AddBone(new Bone("Top 2", 1) { Position = new Vector3(0.5f, 0, -0.5f), });
            skel.AddBone(new Bone("Top 3", 1) { Position = new Vector3(0.5f, 0, 0.5f), });
            skel.AddBone(new Bone("Top 4", 1) { Position = new Vector3(-0.5f, 0, 0.5f), });
            skel.AddBone(new Bone("Bottom", 0) { Position = new Vector3(0, -0.5f, 0), Rotation = new Vector4(0.057289f, 0.284473f, 0.188929f, 0.938136f) });
            cube.SkeletonIdx = scene.AddSkeleton(skel);

            scene.AddModel(cube);
            ScuffedBX.ExportAsFBX(scene, "ScuffedBX_Test.fbx", new FBXSettings()
            {
                EmbedResources = false,
                ExportAsBinary = true,
            });
        }

        private static void TestImportFbx()
        {
            var scene = ScuffedBX.ImportFromFBX("ScuffedBX_Test.fbx");
            bool printVerts = false;
            bool printIndices = false;

            Console.WriteLine(scene.NameStr);
            Console.WriteLine("Materials");
            for (int i = 0; i < scene.NumMatInfos; i++)
            {
                var mat = scene.GetMatInfo(i);
                Console.WriteLine($" {mat.NameStr.PadRight(50)}\t{mat.DiffusePathStr}");
            }

            Console.WriteLine("Skeletons");
            for (int i = 0; i < scene.NumSkeletons; i++)
            {
                var skel = scene.GetSkeleton(i);
                Console.WriteLine($" {skel.NameStr}");
                for (int j = 0; j < skel.NumBones; j++)
                {
                    var bone = skel.GetBone(j);
                    Console.WriteLine($"    {bone.NameStr},\t{bone.ParentIdx},\tPos: {bone.Position.X}, {bone.Position.Y}, {bone.Position.Z}    \tRot: {bone.Rotation.X}, {bone.Rotation.Y}, {bone.Rotation.Z}, {bone.Rotation.W}\t     Scale: {bone.Scale.X}, {bone.Scale.Y}, {bone.Scale.Z}");
                }
            }

            Console.WriteLine("Models");
            for (int i = 0; i < scene.NumModels; i++)
            {
                var model = scene.GetModel(i);
                //model.NumBonesPerVert = 1;
                Console.WriteLine($" {model.NameStr}, num Verts: {model.NumVerts}, Pos: {model.Position.X}, {model.Position.Y}, {model.Position.Z}    \tRot: {model.Rotation.X}, {model.Rotation.Y}, {model.Rotation.Z}, {model.Rotation.W}\t     Scale: {model.Scale.X}, {model.Scale.Y}, {model.Scale.Z}");
                Console.WriteLine($" Skeleton Idx: {model.SkeletonIdx}");

                if (printVerts)
                {
                    Console.WriteLine($"  {"Idx".PadRight(10)}{"Position".PadRight(28)}{"Normal".PadRight(25)}{"Tangent".PadRight(25)}{"Colors".PadRight(25)}{"Uvs0".PadRight(15)}{"Bones".PadRight(10)}");
                    for (uint i2 = 0; i2 < model.NumVerts; i2++)
                    {
                        var weightsSB = new StringBuilder();
                        for (int j = 0; j < model.NumBonesPerVert; j++)
                        {
                            var weight = model.GetBoneWeight((uint)((i2 * model.NumBonesPerVert) + j));
                            weightsSB.Append($"({weight.BoneIdx}, {weight.Weight}), ");
                        }
                        var weightsStr = model.NumBonesPerVert > 0 ? weightsSB.ToString().Substring(0, weightsSB.ToString().Length - 2) : "";

                        Console.WriteLine($"  {$"{i2},".PadRight(10)}({model.GetPosition(i2).X:0.###}, {model.GetPosition(i2).Y:0.###}, {model.GetPosition(i2).Z:0.###})".PadRight(40) +
                            (model.HasNormals ? $"({model.GetNormal(i2).X:0.##}, {model.GetNormal(i2).Y:0.##}, {model.GetNormal(i2).Z:0.##})".PadRight(25) : "NULL".PadRight(25)) +
                            (model.HasTangents ? $"({model.GetTangent(i2).X:0.##}, {model.GetTangent(i2).Y:0.##}, {model.GetTangent(i2).Z:0.##}, {model.GetTangent(i2).W:0.##})".PadRight(25) : "NULL".PadRight(25)) +
                            (model.HasColors ? $"({model.GetColor(i2).R:0.###}, {model.GetColor(i2).G:0.###}, {model.GetColor(i2).B:0.###}, {model.GetColor(i2).A:0.###})".PadRight(25) : "NULL".PadRight(25)) +
                            (model.HasUvs0 ? $"({model.GetUVs0(i2).X:0.###}, {model.GetUVs0(i2).Y:0.###})".PadRight(15) : "NULL\t") +
                            (model.NumBonesPerVert > 0 ? weightsStr : "NULL"));
                    }
                }

                Console.WriteLine("Mat Sections");
                for (int i2 = 0; i2 < model.NumMatSections; i2++)
                {
                    var matSection = model.GetMatSection((uint)i2);
                    Console.WriteLine($"    {matSection.MatIdx} ({scene.GetMatInfo((int)matSection.MatIdx).NameStr}), {matSection.Offset}, {matSection.Len}");
                    if (printIndices)
                    {
                        Console.Write("     ");
                        for (uint i3 = 0; i3 < matSection.Len; i3 += 3)
                        {
                            Console.Write($"({model.GetIndice(matSection.Offset + i3)}, {model.GetIndice(matSection.Offset + i3 + 1)}, {model.GetIndice(matSection.Offset + i3 + 2)}) ");
                        }
                        Console.WriteLine();
                    }
                }
            }
        }

        private static void ImportAndReexportTest()
        {
            string inputFbx = "ScuffedBX_Test.fbx";
            string outputFbx = $"{Path.GetFileNameWithoutExtension(inputFbx)}_rexport.fbx";

            var scene = ScuffedBX.ImportFromFBX(inputFbx);
            ScuffedBX.ExportAsFBX(scene, outputFbx, new FBXSettings() { });
        }

        private static void LoopPrintTest()
        {
            for (int i = 0; i < 10000; i++)
            {
                var scene = new Scene();
                scene.SetName("Scuffed Test");

                var model = new Model();
                model.SetName("Model");
                //model.Position = new Vector3(); //new System.Numerics.Vector3(0f, 2f, 1.5f);
                model.Position.X = 1;
                model.Position.Y = i;
                model.Position.Z = 1.5f;

                var pos2 = new Vector3(2.2f, 1.2f, 0);
                model.AddVert(pos2, new Vector3(0, 0, 0), null, null, null, null);
                model.AddVert(model.Position, new Vector3(0, 0, 0), null, null, null, null);

                var pos = new Vector3(0, 0, 0);
                //model.Position = pos;

                scene.AddModel(model);
                // ScuffedBX.ExportAsFBX(scene, "ScuffedBX_Test.fbx");

                ScuffedBX.PrintScene(scene);
            }
        }
    }
}
