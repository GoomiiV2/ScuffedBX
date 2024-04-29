// ScuffedBX.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>

#define API __declspec(dllexport)
#define STRING_LEN_MAX 1024

namespace ScuffedBX
{
	struct API Vector2
	{
		float X = 0;
		float Y = 0;

		Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}
	};

	struct API Vector3
	{
		float X = 0;
		float Y = 0;
		float Z = 0;

		Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	};

	struct API Vector4
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;

		Vector4(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
	};

	struct API ColorRGBA
	{
		float R;
		float G;
		float B;
		float A;

		ColorRGBA(uint32_t color)
		{
			uint8_t r = (color >> 24) & 0xFF;
			uint8_t g = (color >> 16) & 0xFF;
			uint8_t b = (color >> 8) & 0xFF;
			uint8_t a = color & 0xFF;
		}

		ColorRGBA(float r, float g, float b, float a)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}
	};

	struct API Bone
	{
		char Name[STRING_LEN_MAX];
		Vector3 Position = { 0, 0, 0 };
		Vector4 Rotation = { 0, 0, 0, 1 };
		Vector3 Scale = { 1, 1, 1 };
		int ParentIdx;

		Bone(const char* name, int parentIdx)
		{
			SetName(name);
			ParentIdx = parentIdx;
		}

		Bone(const char* name, int parentIdx, Vector3 pos, Vector4 rot, Vector3 scale)
		{
			SetName(name);
			ParentIdx = parentIdx;

			Position = pos;
			Rotation = Rotation;
			Scale    = scale;
		}

		void SetName(const char* name) { strcpy_s(Name, STRING_LEN_MAX, name); }
	};

	class API Skeleton
	{
	public:
		char Name[STRING_LEN_MAX];
		void* Bones;

		Skeleton(const char* name)
		{
			SetName(name);
			Bones = new std::vector<Bone>();
		}

		~Skeleton()
		{
			free(Bones);
		}

		void SetName(const char* name) { strcpy_s(Name, STRING_LEN_MAX, name); }
		void AddBone(const Bone bone) { reinterpret_cast<std::vector<Bone>*>(Bones)->push_back(bone); }
		Bone* GetBone(const int idx) { return &reinterpret_cast<std::vector<Bone>*>(Bones)->at(idx); }
		const int GetNumBones() { return reinterpret_cast<std::vector<Bone>*>(Bones)->size(); }
	};

	class API MatSection
	{
	public:
		char Name[STRING_LEN_MAX];
		uint32_t MatIdx;
		uint32_t Offset;
		uint32_t Len;

		MatSection(uint32_t matIdx, uint32_t offset, uint32_t len)
		{
			MatIdx = matIdx;
			Offset = offset;
			Len = len;
		}
	};

	class API MatInfo
	{
	public:
		char Name[STRING_LEN_MAX];
		char DiffusePath[STRING_LEN_MAX];

		MatInfo(const char* name, const char* diffusePath)
		{
			strcpy_s(Name, STRING_LEN_MAX, name);
			strcpy_s(DiffusePath, STRING_LEN_MAX, diffusePath);
		}
	};

	struct API BoneWeight
	{
		int32_t BoneIdx;
		float Weight;

		BoneWeight(int32_t boneIdx, float weight)
		{
			BoneIdx = boneIdx;
			Weight = weight;
		}
	};

	class API Model
	{
	public:
		char Name[STRING_LEN_MAX];

		Vector3 Position { 0,0,0 };
		Vector4 Rotation{ 0,0,0,0};
		Vector3 Scale { 1, 1, 1 };
		int32_t SkeletonIdx = -1;
		int32_t NumBonesPerVert = 4;

		void* Positions;
		void* Normals;
		void* Tangents;
		void* UVs0;
		void* Colors;
		void* Indices;
		void* MatSections;
		void* BoneWeights;

		Model()
		{
			Positions   = new std::vector<Vector3>();
			Normals     = new std::vector<Vector3>();
			Tangents    = new std::vector<Vector4>();
			UVs0        = new std::vector<Vector2>();
			Colors      = new std::vector<ColorRGBA>();
			Indices     = new std::vector<uint32_t>();
			MatSections = new std::vector<MatSection>();
			BoneWeights = new std::vector<BoneWeight>();
		}

		~Model()
		{
			free(Positions);
			free(Normals);
			free(Tangents);
			free(UVs0);
			free(Colors);
			free(Indices);
			free(MatSections);
			free(BoneWeights);
		}

		void SetName(const char* name) { strcpy_s(Name, STRING_LEN_MAX, name); };
		const int GetNumVerts() { return ((std::vector<Vector3>*)Positions)->size(); }
		const int AddVert(Vector3* pos, Vector3* normal, Vector4* tangent, Vector2* uv0, ColorRGBA* color, BoneWeight boneWeights[])
		{
			uint32_t count = GetNumVerts();

			((std::vector<Vector3>*)Positions)->push_back(*pos);

			if (normal != nullptr)
				((std::vector<Vector3>*)Normals)->push_back(*normal);

			if (tangent != nullptr)
				((std::vector<Vector4>*)Tangents)->push_back(*tangent);

			if (uv0 != nullptr)
				((std::vector<Vector2>*)UVs0)->push_back(*uv0);

			if (color != nullptr)
				((std::vector<ColorRGBA>*)Colors)->push_back(*color);

			if (boneWeights != nullptr)
			{
				for (size_t i = 0; i < NumBonesPerVert; i++)
				{
					AddBoneWeight(boneWeights[i].BoneIdx, boneWeights[i].Weight);
				}
			}

			return count;
		}

		void AddIndice(const uint32_t indice)
		{
			((std::vector<uint32_t>*)Indices)->push_back(indice);
		}

		void AddIndices(const uint32_t* indices, int numIndices)
		{
			std::vector<uint32_t>* indicesVec = ((std::vector<uint32_t>*)Indices);
			indicesVec->insert(indicesVec->end(), indices, indices + numIndices);
		}

		void AddTri(const uint32_t i1, const uint32_t i2, const uint32_t i3)
		{
			AddIndice(i1);
			AddIndice(i2);
			AddIndice(i3);
		}

		void SetIndices(uint32_t indices[], const int32_t len)
		{
			((std::vector<uint32_t>*)Indices)->clear();
			((std::vector<uint32_t>*)Indices)->insert(((std::vector<uint32_t>*)Indices)->begin(), indices, indices + len);
		}

		void AddMatSection(MatSection matSection) { ((std::vector<MatSection>*)MatSections)->push_back(matSection); }
		const uint32_t GetNumMatSections() { return ((std::vector<MatSection>*)MatSections)->size(); }
		MatSection* GetMatSection(const uint32_t idx) { return &((std::vector<MatSection>*)MatSections)->at(idx); }

		const int GetNumIndices() { return reinterpret_cast<std::vector<uint32_t>*>(Indices)->size(); }
		const Vector3 GetPosition(const uint32_t idx) { return reinterpret_cast<std::vector<Vector3>*>(Positions)->at(idx); }
		const Vector3 GetNormal(const uint32_t idx) { return reinterpret_cast<std::vector<Vector3>*>(Normals)->at(idx); }
		const Vector4 GetTangent(const uint32_t idx) { return reinterpret_cast<std::vector<Vector4>*>(Tangents)->at(idx); }
		const Vector2 GetUVs0(const uint32_t idx) { return reinterpret_cast<std::vector<Vector2>*>(UVs0)->at(idx); }
		const ColorRGBA GetColor(const uint32_t idx) { return reinterpret_cast<std::vector<ColorRGBA>*>(Colors)->at(idx); }
		const uint32_t GetIndice(const uint32_t idx) { return reinterpret_cast<std::vector<uint32_t>*>(Indices)->at(idx); }
		const uint32_t* GetIndices(const uint32_t idx) { return reinterpret_cast<std::vector<uint32_t>*>(Indices)->data(); }

		const bool HasNormals() { return reinterpret_cast<std::vector<Vector3>*>(Normals)->size() > 0; }
		const bool HasTangents() { return reinterpret_cast<std::vector<Vector4>*>(Tangents)->size() > 0; }
		const bool HasUvs0() { return reinterpret_cast<std::vector<Vector2>*>(UVs0)->size() > 0; }
		const bool HasColors() { return reinterpret_cast<std::vector<ColorRGBA>*>(Colors)->size() > 0; }

		const BoneWeight GetBoneWeight(const uint32_t idx) { return reinterpret_cast<std::vector<BoneWeight>*>(BoneWeights)->at(idx); }
		void AddBoneWeight(const int32_t boneIdx, const float weight) { reinterpret_cast<std::vector<BoneWeight>*>(BoneWeights)->push_back(BoneWeight(boneIdx, weight)); }
		void AddBoneWeights(int32_t boneIdxs[], float weights[])
		{
			for (size_t i = 0; i < NumBonesPerVert; i++)
			{
				AddBoneWeight(boneIdxs[i], weights[i]);
			}
		}
	};

	class API Scene
	{
	public:
		char Name[STRING_LEN_MAX];
		void* Models = 0;
		void* Materials = 0;
		void* Skeletons = 0;

		double UnitScale = 100.0f;

		Scene()
		{
			Models = new std::vector<Model*>;
			Materials = new std::vector<MatInfo>;
			Skeletons = new std::vector<Skeleton*>();
		}

		~Scene()
		{
			free(Models);
			free(Materials);
			free(Skeletons);
		}

		void SetName(const char* name) { strcpy_s(Name, STRING_LEN_MAX, name); };
		const int GetNumModels() const { return ((std::vector<Model*>*)Models)->size(); }
		Model* GetModel(int idx) { return ((std::vector<Model*>*)Models)->at(idx); }
		void AddModel(Model* model) { ((std::vector<Model*>*)Models)->push_back(model); }

		const int GetNumMatInfos() const { return ((std::vector<MatInfo>*)Materials)->size(); }
		MatInfo* GetMatInfo(int idx) { return &((std::vector<MatInfo>*)Materials)->at(idx); }
		uint32_t AddMatInfo(MatInfo matInfo)
		{
			uint32_t count = GetNumMatInfos();
			((std::vector<MatInfo>*)Materials)->push_back(matInfo);

			return count;
		}

		Skeleton* GetSkeleton(const int idx) { return reinterpret_cast<std::vector<Skeleton*>*>(Skeletons)->at(idx); }
		const int GetNumSkeletons() { return reinterpret_cast<std::vector<Skeleton*>*>(Skeletons)->size(); }
		int32_t AddSkeleton(Skeleton* skeleton)
		{
			uint32_t idx = GetNumSkeletons();
			reinterpret_cast<std::vector<Skeleton*>*>(Skeletons)->push_back(skeleton);
			return idx;
		}
	};

	struct API FBXSettings
	{
		bool EmbedResources = false;
		bool ExportAsBinary = true;
	};

	API void ExportAsFBX(const Scene scene, const char* filePath, const FBXSettings settings);
	API Scene* ImportFromFBX(const char* filePath);
	API void PrintFBXInfo(const char* filePath);
	API void PrintScene(const Scene scene);
}

