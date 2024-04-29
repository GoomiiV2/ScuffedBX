#include <D:\NonWindows\Projects\Relink\ScuffedBX\ScuffedBX.Native\ScuffedBX.h>
#include <new>

extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Vector2_Vector2_f_f(void* __instance, float x, float y) { ::new (__instance) ScuffedBX::Vector2(x, y); }
struct ScuffedBX::Vector2& (ScuffedBX::Vector2::*_0)(struct ScuffedBX::Vector2&&) = &ScuffedBX::Vector2::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Vector3_Vector3_f_f_f(void* __instance, float x, float y, float z) { ::new (__instance) ScuffedBX::Vector3(x, y, z); }
struct ScuffedBX::Vector3& (ScuffedBX::Vector3::*_1)(struct ScuffedBX::Vector3&&) = &ScuffedBX::Vector3::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Vector4_Vector4_f_f_f_f(void* __instance, float x, float y, float z, float w) { ::new (__instance) ScuffedBX::Vector4(x, y, z, w); }
struct ScuffedBX::Vector4& (ScuffedBX::Vector4::*_2)(struct ScuffedBX::Vector4&&) = &ScuffedBX::Vector4::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_ColorRGBA_ColorRGBA_i(void* __instance, unsigned int color) { ::new (__instance) ScuffedBX::ColorRGBA(color); }
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_ColorRGBA_ColorRGBA_f_f_f_f(void* __instance, float r, float g, float b, float a) { ::new (__instance) ScuffedBX::ColorRGBA(r, g, b, a); }
struct ScuffedBX::ColorRGBA& (ScuffedBX::ColorRGBA::*_3)(struct ScuffedBX::ColorRGBA&&) = &ScuffedBX::ColorRGBA::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Bone_Bone__1C_I(void* __instance, const char* name, int parentIdx) { ::new (__instance) ScuffedBX::Bone(name, parentIdx); }
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Bone_Bone__1C_I___N_ScuffedBX_S_Vector3___N_ScuffedBX_S_Vector4_S1(void* __instance, const char* name, int parentIdx, ScuffedBX::Vector3 pos, ScuffedBX::Vector4 rot, ScuffedBX::Vector3 scale) { ::new (__instance) ScuffedBX::Bone(name, parentIdx, pos, rot, scale); }
void (ScuffedBX::Bone::*_4)(const char*) = &ScuffedBX::Bone::SetName;
struct ScuffedBX::Bone& (ScuffedBX::Bone::*_5)(struct ScuffedBX::Bone&&) = &ScuffedBX::Bone::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Skeleton_Skeleton__1C(void* __instance, const char* name) { ::new (__instance) ScuffedBX::Skeleton(name); }
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Skeleton__Skeleton(ScuffedBX::Skeleton*__instance) { __instance->~Skeleton(); }
void (ScuffedBX::Skeleton::*_6)(const char*) = &ScuffedBX::Skeleton::SetName;
void (ScuffedBX::Skeleton::*_7)(const struct ScuffedBX::Bone) = &ScuffedBX::Skeleton::AddBone;
struct ScuffedBX::Bone* (ScuffedBX::Skeleton::*_8)(const int) = &ScuffedBX::Skeleton::GetBone;
const int (ScuffedBX::Skeleton::*_9)() = &ScuffedBX::Skeleton::GetNumBones;
class ScuffedBX::Skeleton& (ScuffedBX::Skeleton::*_10)(const class ScuffedBX::Skeleton&) = &ScuffedBX::Skeleton::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_MatSection_MatSection_i_i_i(void* __instance, unsigned int matIdx, unsigned int offset, unsigned int len) { ::new (__instance) ScuffedBX::MatSection(matIdx, offset, len); }
class ScuffedBX::MatSection& (ScuffedBX::MatSection::*_11)(class ScuffedBX::MatSection&&) = &ScuffedBX::MatSection::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_MatInfo_MatInfo__1C_S0(void* __instance, const char* name, const char* diffusePath) { ::new (__instance) ScuffedBX::MatInfo(name, diffusePath); }
class ScuffedBX::MatInfo& (ScuffedBX::MatInfo::*_12)(class ScuffedBX::MatInfo&&) = &ScuffedBX::MatInfo::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_BoneWeight_BoneWeight_I_f(void* __instance, int boneIdx, float weight) { ::new (__instance) ScuffedBX::BoneWeight(boneIdx, weight); }
struct ScuffedBX::BoneWeight& (ScuffedBX::BoneWeight::*_13)(struct ScuffedBX::BoneWeight&&) = &ScuffedBX::BoneWeight::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Model_Model(void* __instance) { ::new (__instance) ScuffedBX::Model(); }
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Model__Model(ScuffedBX::Model*__instance) { __instance->~Model(); }
void (ScuffedBX::Model::*_14)(const char*) = &ScuffedBX::Model::SetName;
const int (ScuffedBX::Model::*_15)() = &ScuffedBX::Model::GetNumVerts;
const int (ScuffedBX::Model::*_16)(struct ScuffedBX::Vector3*, struct ScuffedBX::Vector3*, struct ScuffedBX::Vector4*, struct ScuffedBX::Vector2*, struct ScuffedBX::ColorRGBA*, struct ScuffedBX::BoneWeight[]) = &ScuffedBX::Model::AddVert;
void (ScuffedBX::Model::*_17)(const unsigned int) = &ScuffedBX::Model::AddIndice;
void (ScuffedBX::Model::*_18)(const unsigned int*, int) = &ScuffedBX::Model::AddIndices;
void (ScuffedBX::Model::*_19)(const unsigned int, const unsigned int, const unsigned int) = &ScuffedBX::Model::AddTri;
void (ScuffedBX::Model::*_20)(unsigned int[], const int) = &ScuffedBX::Model::SetIndices;
void (ScuffedBX::Model::*_21)(class ScuffedBX::MatSection) = &ScuffedBX::Model::AddMatSection;
const unsigned int (ScuffedBX::Model::*_22)() = &ScuffedBX::Model::GetNumMatSections;
class ScuffedBX::MatSection* (ScuffedBX::Model::*_23)(const unsigned int) = &ScuffedBX::Model::GetMatSection;
const int (ScuffedBX::Model::*_24)() = &ScuffedBX::Model::GetNumIndices;
const struct ScuffedBX::Vector3 (ScuffedBX::Model::*_25)(const unsigned int) = &ScuffedBX::Model::GetPosition;
const struct ScuffedBX::Vector3 (ScuffedBX::Model::*_26)(const unsigned int) = &ScuffedBX::Model::GetNormal;
const struct ScuffedBX::Vector4 (ScuffedBX::Model::*_27)(const unsigned int) = &ScuffedBX::Model::GetTangent;
const struct ScuffedBX::Vector2 (ScuffedBX::Model::*_28)(const unsigned int) = &ScuffedBX::Model::GetUVs0;
const struct ScuffedBX::ColorRGBA (ScuffedBX::Model::*_29)(const unsigned int) = &ScuffedBX::Model::GetColor;
const unsigned int (ScuffedBX::Model::*_30)(const unsigned int) = &ScuffedBX::Model::GetIndice;
const unsigned int* (ScuffedBX::Model::*_31)(const unsigned int) = &ScuffedBX::Model::GetIndices;
const bool (ScuffedBX::Model::*_32)() = &ScuffedBX::Model::HasNormals;
const bool (ScuffedBX::Model::*_33)() = &ScuffedBX::Model::HasTangents;
const bool (ScuffedBX::Model::*_34)() = &ScuffedBX::Model::HasUvs0;
const bool (ScuffedBX::Model::*_35)() = &ScuffedBX::Model::HasColors;
const struct ScuffedBX::BoneWeight (ScuffedBX::Model::*_36)(const unsigned int) = &ScuffedBX::Model::GetBoneWeight;
void (ScuffedBX::Model::*_37)(const int, const float) = &ScuffedBX::Model::AddBoneWeight;
void (ScuffedBX::Model::*_38)(int[], float[]) = &ScuffedBX::Model::AddBoneWeights;
class ScuffedBX::Model& (ScuffedBX::Model::*_39)(const class ScuffedBX::Model&) = &ScuffedBX::Model::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Scene_Scene(void* __instance) { ::new (__instance) ScuffedBX::Scene(); }
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_Scene__Scene(ScuffedBX::Scene*__instance) { __instance->~Scene(); }
void (ScuffedBX::Scene::*_40)(const char*) = &ScuffedBX::Scene::SetName;
const int (ScuffedBX::Scene::*_41)() const = &ScuffedBX::Scene::GetNumModels;
class ScuffedBX::Model* (ScuffedBX::Scene::*_42)(int) = &ScuffedBX::Scene::GetModel;
void (ScuffedBX::Scene::*_43)(class ScuffedBX::Model*) = &ScuffedBX::Scene::AddModel;
const int (ScuffedBX::Scene::*_44)() const = &ScuffedBX::Scene::GetNumMatInfos;
class ScuffedBX::MatInfo* (ScuffedBX::Scene::*_45)(int) = &ScuffedBX::Scene::GetMatInfo;
unsigned int (ScuffedBX::Scene::*_46)(class ScuffedBX::MatInfo) = &ScuffedBX::Scene::AddMatInfo;
class ScuffedBX::Skeleton* (ScuffedBX::Scene::*_47)(const int) = &ScuffedBX::Scene::GetSkeleton;
const int (ScuffedBX::Scene::*_48)() = &ScuffedBX::Scene::GetNumSkeletons;
int (ScuffedBX::Scene::*_49)(class ScuffedBX::Skeleton*) = &ScuffedBX::Scene::AddSkeleton;
class ScuffedBX::Scene& (ScuffedBX::Scene::*_50)(const class ScuffedBX::Scene&) = &ScuffedBX::Scene::operator=;
extern "C" __declspec(dllexport) void c__N_ScuffedBX_S_FBXSettings_FBXSettings(void* __instance) { ::new (__instance) ScuffedBX::FBXSettings(); }
struct ScuffedBX::FBXSettings& (ScuffedBX::FBXSettings::*_51)(struct ScuffedBX::FBXSettings&&) = &ScuffedBX::FBXSettings::operator=;
