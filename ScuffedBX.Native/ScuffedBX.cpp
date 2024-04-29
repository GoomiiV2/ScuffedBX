#define FBXSDK_SHARED
#include "ScuffedBX.h"
#include <fbxsdk.h>
#include "FBXUtils.hpp"
#include <filesystem>
#include <map>
#include <tuple>
#include <windows.h>

#include <ScuffedBX.Native-symbols.cpp>
#include <Std-symbols.cpp>

using namespace std;
namespace fs = std::filesystem;

ScuffedBX::Vector3 FbxVector4ToVector3(FbxVector4 fbxVec)
{
    ScuffedBX::Vector3 vec(fbxVec[0], fbxVec[1], fbxVec[2]);
    return vec;
}

ScuffedBX::Vector4 FbxVector4ToVector4(FbxVector4 fbxVec)
{
    ScuffedBX::Vector4 vec(fbxVec[0], fbxVec[1], fbxVec[2], fbxVec[3]);
    return vec;
}

ScuffedBX::Vector2 FbxVector4ToVector2(FbxVector4 fbxVec)
{
    ScuffedBX::Vector2 vec(fbxVec[0], fbxVec[1]);
    return vec;
}

ScuffedBX::Vector2 FbxVector4ToVector2(FbxVector2 fbxVec)
{
    ScuffedBX::Vector2 vec(fbxVec[0], fbxVec[1]);
    return vec;
}

ScuffedBX::ColorRGBA FbxColorToColorRGBA(FbxColor fbxColor)
{
    ScuffedBX::ColorRGBA color(fbxColor.mRed, fbxColor.mGreen, fbxColor.mBlue, fbxColor.mAlpha);
    return color;
}

FbxNode* CreateFBXMesh(FbxScene* scene, ScuffedBX::Model* model, std::vector<FbxSurfacePhong*>& materials)
{
    FbxMesh* mesh = FbxMesh::Create(scene, model->Name);

    FbxNode* node = FbxNode::Create(scene, model->Name);
    node->SetNodeAttribute(mesh);
    node->SetShadingMode(FbxNode::eTextureShading);

    FbxVector4 rotVec4 = FbxVector4();
    rotVec4.SetXYZ(FbxQuaternion(model->Rotation.X, model->Rotation.Y, model->Rotation.Z, model->Rotation.W));
    node->LclTranslation.Set(FbxVector4(model->Position.X, model->Position.Y, model->Position.Z));
    node->LclRotation.Set(rotVec4);
    node->LclScaling.Set(FbxVector4(model->Scale.X, model->Scale.Y, model->Scale.Z));

    mesh->InitControlPoints(model->GetNumVerts());
    FbxVector4* controlPoints = mesh->GetControlPoints();

    for (size_t i = 0; i < model->GetNumVerts(); i++)
    {
        ScuffedBX::Vector3 pos = model->GetPosition(i);
        controlPoints[i] = FbxVector4(pos.X, pos.Y, pos.Z);
    }

    if (model->HasNormals())
    {
        FbxGeometryElementNormal* elmNormal = mesh->CreateElementNormal();
        elmNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
        elmNormal->SetReferenceMode(FbxGeometryElement::eDirect);

        for (size_t i = 0; i < model->GetNumVerts(); i++)
        {
            ScuffedBX::Vector3 norm = model->GetNormal(i);
            elmNormal->GetDirectArray().Add(FbxVector4(norm.X, norm.Y, norm.Z));
        }
    }

    if (model->HasTangents())
    {
        FbxGeometryElementTangent* elmTangent = mesh->CreateElementTangent();
        elmTangent->SetMappingMode(FbxGeometryElement::eByControlPoint);
        elmTangent->SetReferenceMode(FbxGeometryElement::eDirect);

        for (size_t i = 0; i < model->GetNumVerts(); i++)
        {
            ScuffedBX::Vector4 tan = model->GetTangent(i);
            elmTangent->GetDirectArray().Add(FbxVector4(tan.X, tan.Y, tan.Z, tan.W));
        }
    }

    if (model->HasUvs0())
    {
        FbxGeometryElementUV* uvDiffElement = mesh->CreateElementUV("DiffuseUV");
        uvDiffElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
        uvDiffElement->SetReferenceMode(FbxGeometryElement::eDirect);

        for (size_t i = 0; i < model->GetNumVerts(); i++)
        {
            ScuffedBX::Vector2 uv0 = model->GetUVs0(i);
            uvDiffElement->GetDirectArray().Add(FbxVector2(uv0.X, uv0.Y));
        }
    }

    if (model->HasColors())
    {
        FbxGeometryElementVertexColor* elmVertColor = mesh->CreateElementVertexColor();
        elmVertColor->SetMappingMode(FbxGeometryElement::eByControlPoint);
        elmVertColor->SetReferenceMode(FbxGeometryElement::eDirect);

        for (size_t i = 0; i < model->GetNumVerts(); i++)
        {
            ScuffedBX::ColorRGBA color = model->GetColor(i);
            elmVertColor->GetDirectArray().Add(FbxColor(color.R, color.G, color.B, color.A));
        }
    }

    FbxGeometryElementMaterial* materialElement = mesh->CreateElementMaterial();
    materialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
    materialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

    for (size_t i = 0; i < model->GetNumMatSections(); i++)
    {
        ScuffedBX::MatSection* matSection = model->GetMatSection(i);
        FbxSurfacePhong* mat = materials[matSection->MatIdx];
        if (node->GetMaterialIndex(mat->GetName()) == -1)
        {
            node->AddMaterial(mat);
        }

        int matIdx = node->GetMaterialIndex(mat->GetName());

        for (size_t i2 = matSection->Offset; i2 < matSection->Offset + matSection->Len; i2 += 3)
        {
            mesh->BeginPolygon(matIdx);
            for (size_t j = 0; j < 3; j++)
            {
                mesh->AddPolygon(model->GetIndice(i2 + j));
            }
            mesh->EndPolygon();
        }
    }

    return node;
}

FbxFileTexture* CreateTexture(FbxScene* pScene, const char* filePath)
{
    FbxFileTexture* gTexture = FbxFileTexture::Create(pScene, fs::path(filePath).filename().string().c_str());
    gTexture->SetFileName(filePath);
    gTexture->SetTextureUse(FbxTexture::eStandard);
    gTexture->SetMappingType(FbxTexture::eUV);
    gTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
    gTexture->SetSwapUV(false);
    gTexture->SetTranslation(0.0, 0.0);
    gTexture->SetScale(1.0, 1.0);
    gTexture->SetRotation(0.0, 0.0);

    return gTexture;
}

FbxNode* CreateSkeleton(FbxScene* pScene, ScuffedBX::Skeleton* skel, FbxPose* pose)
{
    FbxNode* skelRoot = FbxNode::Create(pScene, skel->Name);
    std::vector<FbxNode*> bones;
    for (size_t i = 0; i < skel->GetNumBones(); i++)
    {
        ScuffedBX::Bone* bone = skel->GetBone(i);
        FbxQuaternion boneRot = FbxQuaternion(bone->Rotation.X, bone->Rotation.Y, bone->Rotation.Z, bone->Rotation.W);
        FbxVector4 boneRotVec4 = FbxVector4();
        boneRotVec4.SetXYZ(boneRot);

        FbxSkeleton* skeletonAttribute = FbxSkeleton::Create(pScene, bone->Name);
        FbxNode* skeletonBone = FbxNode::Create(pScene, bone->Name);
        skeletonBone->SetNodeAttribute(skeletonAttribute);
        skeletonBone->LclTranslation.Set(FbxVector4(bone->Position.X, bone->Position.Y, bone->Position.Z));
        skeletonBone->LclRotation.Set(boneRotVec4);
        skeletonBone->LclScaling.Set(FbxVector4(bone->Scale.X, bone->Scale.Y, bone->Scale.Z));
        //skeletonBone->SetRotationOrder(FbxNode::eSourcePivot, eSphericXYZ);
        //skeletonBone->RotationPivot.Set(skeletonBone->LclTranslation.Get());

        bones.push_back(skeletonBone);
    }

    for (size_t i = 0; i < bones.size(); i++)
    {
        FbxNode* fbxBone = bones[i];
        ScuffedBX::Bone* bone = skel->GetBone(i);

        if (bone->ParentIdx == -1)
        {
            fbxBone->GetSkeleton()->SetSkeletonType(FbxSkeleton::eRoot);
            skelRoot->AddChild(fbxBone);
        }
        else
        {
            fbxBone->GetSkeleton()->SetSkeletonType(FbxSkeleton::eLimbNode);
            bones[bone->ParentIdx]->AddChild(fbxBone);
        }

        pose->Add(fbxBone, fbxBone->EvaluateGlobalTransform());
    }

    return skelRoot;
}

FbxSurfacePhong* CreateMaterial(FbxScene* pScene, ScuffedBX::MatInfo* matInfo)
{
    FbxSurfacePhong* gMaterial = FbxSurfacePhong::Create(pScene, matInfo->Name);
    gMaterial->TransparencyFactor.Set(0);
    gMaterial->ShadingModel.Set("Phong");
    gMaterial->Shininess.Set(0);

    FbxFileTexture* tex = CreateTexture(pScene, matInfo->DiffusePath);
    gMaterial->Diffuse.ConnectSrcObject(tex);

    return gMaterial;
}

void CreateSkin(FbxScene* fbxScene, FbxNode* mesh, FbxNode* skelNode, ScuffedBX::Skeleton* modelSkel, ScuffedBX::Model* model)
{
    FbxSkin* skin = FbxSkin::Create(fbxScene, "");
    std::vector<FbxCluster*> clusters;

    for (size_t i = 0; i < modelSkel->GetNumBones(); i++)
    {
        ScuffedBX::Bone* scuffedBone = modelSkel->GetBone(i);
        FbxNode* fbxBone             = skelNode->FindChild(scuffedBone->Name);
        FbxCluster* cluster          = FbxCluster::Create(fbxScene, "");
        cluster->SetLink(fbxBone);
        cluster->SetLinkMode(FbxCluster::eTotalOne);
        cluster->SetTransformMatrix(mesh->EvaluateGlobalTransform());
        cluster->SetTransformLinkMatrix(fbxBone->EvaluateGlobalTransform());

        skin->AddCluster(cluster);
        clusters.push_back(cluster);
    }

    for (size_t i = 0; i < model->GetNumVerts(); i++)
    {
        for (size_t wIdx = 0; wIdx < model->NumBonesPerVert; wIdx++)
        {
            int32_t boneWeightIdx = (i * model->NumBonesPerVert) + wIdx;
            ScuffedBX::BoneWeight weight = model->GetBoneWeight(boneWeightIdx);
            FbxCluster* cluster = clusters[weight.BoneIdx];
            cluster->AddControlPointIndex(i, weight.Weight);
        }
    }

    mesh->GetMesh()->AddDeformer(skin);
}

API void ScuffedBX::ExportAsFBX(Scene scene, const char* filePath, const FBXSettings settings)
{
    FbxManager* fbxMan = FBXUtils::GetFBXManager();

    FbxDocumentInfo* docInfo = FbxDocumentInfo::Create(fbxMan, "DocInfo");
    docInfo->mTitle = *fs::path(filePath).filename().c_str();
    docInfo->mSubject = "";
    docInfo->LastSaved_ApplicationName = "ScuffedBX";

    auto fbxScene = FbxScene::Create(fbxMan, scene.Name);
    fbxScene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit(scene.UnitScale));
    fbxScene->SetDocumentInfo(docInfo);

    std::vector<FbxSurfacePhong*> materials;
    std::vector<FbxNode*> skeletons;
    std::vector<FbxPose*> bindPoses;

    for (size_t i = 0; i < scene.GetNumMatInfos(); i++)
    {
        ScuffedBX::MatInfo* matInfo = scene.GetMatInfo(i);
        FbxSurfacePhong* mat = CreateMaterial(fbxScene, matInfo);
        materials.push_back(mat);
    }

    for (size_t i = 0; i < scene.GetNumSkeletons(); i++)
    {
        ScuffedBX::Skeleton* skel = scene.GetSkeleton(i);
        FbxPose* pose = FbxPose::Create(fbxMan, FbxString("Bindpose_") + skel->Name);
        FbxNode* skelNode = CreateSkeleton(fbxScene, skel, pose);
        fbxScene->GetRootNode()->AddChild(skelNode);
        fbxScene->AddPose(pose);

        skeletons.push_back(skelNode);
        bindPoses.push_back(pose);
    }

    for (size_t i = 0; i < scene.GetNumModels(); i++)
    {
        ScuffedBX::Model* model = scene.GetModel(i);
        FbxNode* meshNode = CreateFBXMesh(fbxScene, model, materials);
        fbxScene->GetRootNode()->AddChild(meshNode);

        if (model->SkeletonIdx != -1)
        {
            FbxNode* skelNode = skeletons[model->SkeletonIdx];
            CreateSkin(fbxScene, meshNode, skelNode, scene.GetSkeleton(model->SkeletonIdx), model);
        }
    }

    FBXUtils::ExportDoc(fbxMan, fbxScene, filePath, settings);
}

void ProcessNode(FbxNode* fbxNode, ScuffedBX::Scene* scuffedScene)
{
    if (fbxNode->GetSkeleton() != nullptr)
    {
        FbxSkeleton* fbxSkel = fbxNode->GetSkeleton();
        ScuffedBX::Skeleton* scuffedSkel = new ScuffedBX::Skeleton(fbxSkel->GetName());

        scuffedScene->AddSkeleton(scuffedSkel);
    }

    for (size_t i = 0; i < fbxNode->GetChildCount(); i++)
    {
        FbxNode* node = fbxNode->GetChild(i);
        ProcessNode(node, scuffedScene);
    }
}

void AddBoneToScuffedSkeleton(FbxNode* fbxNode, ScuffedBX::Skeleton* scuffedSkel, int depth = -1, int parentIdx = -1)
{
    for (size_t i = 0; i < fbxNode->GetChildCount(); i++)
    {
        FbxNode* skelNode = fbxNode->GetChild(i);
        if (skelNode->GetSkeleton() != nullptr)
        {
            ScuffedBX::Bone scuffedBone(skelNode->GetName(), depth);
            FbxVector4 pos = skelNode->LclTranslation.Get();
            scuffedBone.Position = ScuffedBX::Vector3(pos[0], pos[1], pos[2]);

            FbxVector4 rot = FBXUtils::DegRotToQuat(skelNode->LclRotation.Get());
            scuffedBone.Rotation = ScuffedBX::Vector4(rot[0], rot[1], rot[2], rot[3]);

            FbxVector4 scale = skelNode->LclScaling.Get();
            scuffedBone.Scale = ScuffedBX::Vector3(scale[0], scale[1], scale[2]);

            scuffedBone.ParentIdx = parentIdx;
            scuffedSkel->AddBone(scuffedBone);
        }

        AddBoneToScuffedSkeleton(skelNode, scuffedSkel, depth + 1, scuffedSkel->GetNumBones() - 1);
    }
}

int32_t GetMatIdxForName(const char* name, ScuffedBX::Scene* scuffedScene)
{
    for (size_t i = 0; i < scuffedScene->GetNumMatInfos(); i++)
    {
        ScuffedBX::MatInfo* matInfo = scuffedScene->GetMatInfo(i);
        if (strcmp(name, matInfo->Name) == 0)
        {
            return i;
        }
    }

    return -1;
}

ScuffedBX::Model* CreateScuffedModelFromFBX(FbxNode* fbxNode, ScuffedBX::Scene* scuffedScene)
{
    ScuffedBX::Model* scuffedModel = new ScuffedBX::Model();
    scuffedModel->SetName(fbxNode->GetName());
    scuffedModel->NumBonesPerVert = 0;

    scuffedModel->Position = FbxVector4ToVector3(fbxNode->LclTranslation.Get());
    scuffedModel->Rotation = FbxVector4ToVector4(FBXUtils::DegRotToQuat(fbxNode->LclRotation.Get()));
    scuffedModel->Scale    = FbxVector4ToVector3(fbxNode->LclScaling.Get());

    FbxMesh* fbxMesh                        = fbxNode->GetMesh();
    FbxGeometryElementNormal* elmNormal     = fbxMesh->GetElementNormal();
    FbxGeometryElementTangent* elTangent    = fbxMesh->GetElementTangent();
    FbxGeometryElementUV* elUvs0            = fbxMesh->GetElementUV();
    FbxGeometryElementVertexColor* elColors = fbxMesh->GetElementVertexColor();
    FbxSkin* skin                           = (FbxSkin*)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
    FbxGeometryElementMaterial* elMaterial  = fbxMesh->GetElementMaterial();

    std::map<int, std::vector<std::tuple<int, double>>> weightsMap;
    if (skin != nullptr)
    {
        int clusterCount = skin->GetClusterCount();
        for (size_t i = 0; i < clusterCount; i++)
        {
            FbxCluster* cluster = skin->GetCluster(i);
            int* indices        = cluster->GetControlPointIndices();
            double* weights     = cluster->GetControlPointWeights();

            for (size_t i2 = 0; i2 < cluster->GetControlPointIndicesCount(); i2++)
            {
                if (!weightsMap.contains(indices[i2]))
                    weightsMap.emplace(indices[i2], std::vector<std::tuple<int, double>>());

                weightsMap[indices[i2]].push_back(std::tuple<int, double>(i, weights[i2]));
            }

            if (cluster->GetLink()->GetSkeleton()->GetSkeletonType() == FbxSkeleton::eRoot)
            {
                for (size_t i = 0; i < scuffedScene->GetNumSkeletons(); i++)
                {
                    ScuffedBX::Skeleton* skel = scuffedScene->GetSkeleton(i);
                    if (strcmp(skel->Name, cluster->GetLink()->GetParent()->GetName()) == 0)
                    {
                        scuffedModel->SkeletonIdx = i;
                    }
                }
            }
        }

        for (size_t i = 0; i < weightsMap.size(); i++)
        {
            if (weightsMap[i].size() > scuffedModel->NumBonesPerVert)
            {
                scuffedModel->NumBonesPerVert = weightsMap[i].size();
            }
        }
    }

    //scuffedModel->NumBonesPerVert = 0;

    int32_t indice = -1;
    std::map<std::string, int32_t> vertDedupMap;
    std::map<int, std::vector<uint32_t>> matIndices;
    for (size_t i = 0; i < fbxMesh->GetPolygonCount() * 3; i++)
    {
        int vertIdx     = fbxMesh->GetPolygonVertex(i / 3, i % 3);
        int normalIdx   = FBXUtils::GetLayerItemIdx(vertIdx, elmNormal, i);
        int tangentsIdx = FBXUtils::GetLayerItemIdx(vertIdx, elTangent, i);
        int uvs0Idx     = FBXUtils::GetLayerItemIdx(vertIdx, elUvs0, i);
        int colorsIdx   = FBXUtils::GetLayerItemIdx(vertIdx, elColors, i);

        std::string attribId = std::format("{}_{}_{}_{}_{}", vertIdx, normalIdx, tangentsIdx, uvs0Idx, colorsIdx);
        if (vertDedupMap.contains(attribId))
            indice = vertDedupMap[attribId];
        else
            indice++;

        if (elMaterial != nullptr)
        {
            int matIdx = elMaterial->GetIndexArray().GetAt(i / 3);
            FbxSurfaceMaterial* fbxMat = fbxMesh->GetNode()->GetMaterial(matIdx);
            int32_t realIdx = GetMatIdxForName(fbxMat->GetName(), scuffedScene);

            if (!matIndices.contains(realIdx))
                matIndices.emplace(realIdx, std::vector<uint32_t>());

            matIndices[realIdx].push_back(indice);
        }

        ScuffedBX::Vector3 pos = FbxVector4ToVector3(fbxMesh->GetControlPointAt(vertIdx));
        ((std::vector<ScuffedBX::Vector3>*)scuffedModel->Positions)->push_back(pos);

        if (elmNormal != nullptr)
            ((std::vector<ScuffedBX::Vector3>*)scuffedModel->Normals)->push_back(FbxVector4ToVector3(elmNormal->GetDirectArray()[normalIdx]));

        if (elTangent != nullptr)
            ((std::vector<ScuffedBX::Vector4>*)scuffedModel->Tangents)->push_back(FbxVector4ToVector4(elTangent->GetDirectArray()[tangentsIdx]));

        if (elUvs0 != nullptr)
            ((std::vector<ScuffedBX::Vector2>*)scuffedModel->UVs0)->push_back(FbxVector4ToVector2(elUvs0->GetDirectArray()[uvs0Idx]));

        if (elColors != nullptr)
            ((std::vector<ScuffedBX::ColorRGBA>*)scuffedModel->Colors)->push_back(FbxColorToColorRGBA(elColors->GetDirectArray()[colorsIdx]));

        if (skin != nullptr)
        {
            for (size_t i2 = 0; i2 < scuffedModel->NumBonesPerVert; i2++)
            {
                if (weightsMap.contains(vertIdx) && weightsMap[vertIdx].size() > i2)
                    scuffedModel->AddBoneWeight(std::get<0>(weightsMap[vertIdx][i2]), std::get<1>(weightsMap[vertIdx][i2]));
                else
                    scuffedModel->AddBoneWeight(0, 0);
            }
        }

        /*
        printf("%i: ", indice);
        printf(attribId.c_str());
        printf("\n");
        */
    }

    for (auto val : matIndices)
    {
        int indiceOffset = scuffedModel->GetNumIndices();
        scuffedModel->AddIndices(val.second.data(), val.second.size());
        ScuffedBX::MatSection matSection = ScuffedBX::MatSection(val.first, indiceOffset, val.second.size());
        scuffedModel->AddMatSection(matSection);
    }

    /*
    for (size_t i = 0; i < fbxMesh->GetControlPointsCount(); i++)
    {
        ScuffedBX::Vector3 pos = FbxVector4ToVector3(fbxMesh->GetControlPointAt(i));

        ((std::vector<ScuffedBX::Vector3>*)scuffedModel->Positions)->push_back(pos);

        if (elmNormal != nullptr)
            ((std::vector<ScuffedBX::Vector3>*)scuffedModel->Normals)->push_back(FbxVector4ToVector3(elmNormal->GetDirectArray()[i]));

        if (elTangent != nullptr)
            ((std::vector<ScuffedBX::Vector4>*)scuffedModel->Tangents)->push_back(FbxVector4ToVector4(elTangent->GetDirectArray()[i]));

        if (elUvs0 != nullptr)
            ((std::vector<ScuffedBX::Vector2>*)scuffedModel->UVs0)->push_back(FbxVector4ToVector2(elUvs0->GetDirectArray()[i]));

        if (elColors != nullptr)
            ((std::vector<ScuffedBX::ColorRGBA>*)scuffedModel->Colors)->push_back(FbxColorToColorRGBA(elColors->GetDirectArray()[i]));

        if (skin != nullptr)
        {
            for (size_t i2 = 0; i2 < scuffedModel->NumBonesPerVert; i2++)
            {
                if (weightsMap.contains(i) && weightsMap[i].size() > i2)
                    scuffedModel->AddBoneWeight(std::get<0>(weightsMap[i][i2]), std::get<1>(weightsMap[i][i2]));
                else
                    scuffedModel->AddBoneWeight(0, 0);
            }
        }
    }
    */

    /*
    if (elMaterial != nullptr)
    {
        std::map<int, std::vector<uint32_t>> matIndices;
        for (size_t i = 0; i < fbxMesh->GetPolygonCount(); i++)
        {
            int matIdx = elMaterial->GetIndexArray().GetAt(i);
            if (!matIndices.contains(matIdx))
                matIndices.emplace(matIdx, std::vector<uint32_t>());

            for (size_t i2 = 0; i2 < fbxMesh->GetPolygonSize(i); i2++)
            {
                int indice = fbxMesh->GetPolygonVertex(i, i2);
                matIndices[matIdx].push_back(indice);
            }
        }

        for (auto val : matIndices)
        {
            int indiceOffset = scuffedModel->GetNumIndices();
            scuffedModel->AddIndices(val.second.data(), val.second.size());
            ScuffedBX::MatSection matSection = ScuffedBX::MatSection(val.first, indiceOffset, val.second.size());
            scuffedModel->AddMatSection(matSection);
        }
    }
    */

    return scuffedModel;
}

API ScuffedBX::Scene* ScuffedBX::ImportFromFBX(const char* filePath)
{
    FbxManager* fbxMan      = FBXUtils::GetFBXManager();
    FbxImporter* lImporter  = FbxImporter::Create(fbxMan, "");
    const bool importStatus = lImporter->Initialize(filePath);
    if (!importStatus) return nullptr;

    FbxScene* fbxScene = FbxScene::Create(fbxMan, "");;
    const bool importedOk = lImporter->Import(fbxScene);

    ScuffedBX::Scene* scene = new ScuffedBX::Scene();

    for (size_t i = 0; i < fbxScene->GetMaterialCount(); i++)
    {
        FbxSurfaceLambert* fbxMat = (FbxSurfaceLambert*)fbxScene->GetMaterial(i);
        FbxFileTexture* fbxTex = fbxMat->Diffuse.GetSrcObject<FbxFileTexture>();
        auto diffusePath = fbxTex != nullptr ? fbxTex->GetFileName() : "";
        ScuffedBX::MatInfo scuffedMat(fbxMat->GetName(), diffusePath);
        scene->AddMatInfo(scuffedMat);
    }


    // Expected scene layout
    // - Root Node
    //      - Skeleton 1
    //      - Model 1
    //      - Model 2
    for (size_t i = 0; i < fbxScene->GetRootNode()->GetChildCount(); i++)
    {
        FbxNode* node = fbxScene->GetRootNode()->GetChild(i);
        if (node->GetChildCount() >= 1)
        {
            FbxNode* skelNode = node;
            for (size_t i = 0; i < 2; i++)
            {
                if (skelNode->GetChild(0)->GetSkeleton() != nullptr)
                {
                    skelNode->GetChild(0)->GetSkeleton()->SetSkeletonType(FbxSkeleton::eRoot);
                    ScuffedBX::Skeleton* scuffedSkel = new ScuffedBX::Skeleton(skelNode->GetName());
                    AddBoneToScuffedSkeleton(skelNode, scuffedSkel);
                    scene->AddSkeleton(scuffedSkel);

                    break;
                }

                if (skelNode->GetChild(0)->GetChildCount() >= 1)
                    skelNode = skelNode->GetChild(0);
            }
        }
    }

    for (size_t i = 0; i < fbxScene->GetRootNode()->GetChildCount(); i++)
    {
        FbxNode* node = fbxScene->GetRootNode()->GetChild(i);
        if (node->GetMesh() != nullptr)
        {
            ScuffedBX:Model* scuffedModel = CreateScuffedModelFromFBX(node, scene);
            scene->AddModel(scuffedModel);
        }
    }

    return scene;
}

void PrintFbxNode(FbxNode* node, int depth = 1)
{
    const char* MappingModeNames[6] = { "eNone", "eByControlPoint", "eByPolygonVertex", "eByPolygon", "eByEdge", "eAllSame" };
    const char* ReferenceModeNames[3] = { "eDirect", "eIndex", "eIndexToDirect" };

    for (size_t i = 0; i < depth; i++)
        printf("  ");

    printf("-%s, %s", node->GetName(), node->GetTypeName());
    if (node->GetMesh() != nullptr)
    {
        FbxMesh* mesh = node->GetMesh();
        printf("Num Control points: %i", mesh->GetControlPointsCount());

        if (mesh->GetElementNormal() != nullptr)
            printf(" Normals (%i) %s", mesh->GetElementNormal()->GetDirectArray().GetCount(), MappingModeNames[mesh->GetElementNormal()->GetMappingMode()]);
        if (mesh->GetElementTangent() != nullptr)
            printf(", Tangents (%i) %s", mesh->GetElementTangent()->GetDirectArray().GetCount(), MappingModeNames[mesh->GetElementNormal()->GetMappingMode()]);
        if (mesh->GetElementUV() != nullptr)
            printf(", Uvs (%i) %s", mesh->GetElementUV()->GetDirectArray().GetCount(), MappingModeNames[mesh->GetElementNormal()->GetMappingMode()]);
        if (mesh->GetElementVertexColor() != nullptr)
            printf(", Colors (%i) %s", mesh->GetElementVertexColor()->GetDirectArray().GetCount(), MappingModeNames[mesh->GetElementNormal()->GetMappingMode()]);
        if (mesh->GetElementMaterial() != nullptr)
            printf(", Material");
        if (mesh->GetDeformer(0) != nullptr)
            printf(", Deformer");

        if (mesh->GetElementMaterial() != nullptr)
        {
            FbxLayerElementMaterial* elMat = mesh->GetElementMaterial();
            FbxLayerElement::EMappingMode mappingMode = elMat->GetMappingMode();
            FbxLayerElement::EReferenceMode refMode = elMat->GetReferenceMode();

            printf(" (mapping Mode: %s, Reference Mode: %s)\n", MappingModeNames[mappingMode], ReferenceModeNames[refMode]);
            std::map<int, std::vector<int>> matIndices;
            for (size_t i = 0; i < elMat->GetIndexArray().GetCount(); i++)
            {
                int matIdx = elMat->GetIndexArray().GetAt(i);
                if (!matIndices.contains(matIdx))
                    matIndices.emplace(matIdx, std::vector<int>());

                matIndices[matIdx].push_back(i);
            }

            for (auto const& item : matIndices)
            {
                for (size_t i = 0; i < depth; i++)
                    printf("  ");

                FbxSurfaceMaterial* mat = node->GetScene()->GetMaterial(item.first);
                printf("   %i: %s (%i)\n", item.first, mat->GetName(), item.second.size());
            }
        }
    }
    
    printf("\n");

    for (size_t i = 0; i < node->GetChildCount(); i++)
    {
        FbxNode* subNode = node->GetChild(i);
        PrintFbxNode(subNode, depth + 1);
    }
}

API void ScuffedBX::PrintFBXInfo(const char* filePath)
{
    FbxManager* fbxMan      = FBXUtils::GetFBXManager();
    FbxImporter* lImporter  = FbxImporter::Create(fbxMan, "");
    const bool importStatus = lImporter->Initialize(filePath);
    if (!importStatus)
    {
        printf("Error initializing importer\n");
        return;
    }

    FbxScene* fbxScene = FbxScene::Create(fbxMan, "");;
    const bool importedOk = lImporter->Import(fbxScene);
    if (!importedOk)
    {
        printf("Error importing\n");
        return;
    }

    printf("Materials\n");
    for (size_t i = 0; i < fbxScene->GetMaterialCount(); i++)
    {
        printf("  ");
        FbxSurfaceMaterial* fbxMat = fbxScene->GetMaterial(i);
        FbxString matShadingModel = fbxMat->ShadingModel.Get();
        printf("%s, %s, ", matShadingModel, fbxMat->GetName());
        if (matShadingModel == "Phong")
        {
            if (((FbxSurfacePhong*)fbxMat)->Diffuse.GetSrcObject<FbxFileTexture>() != nullptr)
                printf("%s", ((FbxSurfacePhong*)fbxMat)->Diffuse.GetSrcObject<FbxFileTexture>()->GetFileName());
        }

        printf("\n");
    }

    printf("Nodes\n");
    PrintFbxNode(fbxScene->GetRootNode());
}

API void ScuffedBX::PrintScene(Scene scene)
{
    //Sleep(1000);

    printf("%s %s \n", __DATE__, __TIME__);
    printf("%s\n", scene.Name);
    for (size_t i = 0; i < scene.GetNumModels(); i++)
    {
        auto model = scene.GetModel(i);
        printf("   - Model: %s\n", model->Name);
        printf("     - Pos: %f, %f, %f\n", model->Position.X, model->Position.Y, model->Position.Z);
        for (size_t i = 0; i < model->GetNumVerts(); i++)
        {
            auto vertPos = model->GetPosition(i);
            printf("        - Pos: %f, %f, %f\n", vertPos.X, vertPos.Y, vertPos.Z);
        }
    }
}
