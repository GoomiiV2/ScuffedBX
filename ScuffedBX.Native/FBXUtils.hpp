#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include "ScuffedBX.h"

namespace FBXUtils
{
    FbxManager* GetFBXManager()
    {
        FbxManager* pManager = FbxManager::Create();
        if (!pManager)
        {
            FBXSDK_printf("Error: Unable to create FBX Manager!\n");
            exit(1);
        }
        else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

        FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
        pManager->SetIOSettings(ios);

        return pManager;
    }

    void PrintFBXVerions(FbxManager*& fbxMan)
    {
        int writersCount = fbxMan->GetIOPluginRegistry()->GetWriterFormatCount();
        for (int i = 0; i < writersCount; i++)
        {
            auto desc = fbxMan->GetIOPluginRegistry()->GetWriterFormatDescription(i);
            printf("%i: %s\n", i, desc);
        }
    }

    void ExportDoc(FbxManager* man, FbxDocument* doc, const char* filePath, const ScuffedBX::FBXSettings settings)
    {
        man->GetIOSettings()->SetBoolProp(EXP_FBX_MATERIAL, true);
        man->GetIOSettings()->SetBoolProp(EXP_FBX_TEXTURE, true);
        man->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, settings.EmbedResources);
        man->GetIOSettings()->SetBoolProp(EXP_FBX_ANIMATION, true);
        man->GetIOSettings()->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

        FbxExporter* exporter = FbxExporter::Create(man, filePath);
        exporter->Initialize(filePath, settings.ExportAsBinary ? 0 : 1, man->GetIOSettings());
        exporter->Export(doc);
    }

    FbxVector4 DegRotToQuat(FbxVector4 degRot)
    {
        double cr = cos((degRot[0] * (M_PI / 180.0f)) * 0.5);
        double sr = sin((degRot[0] * (M_PI / 180.0f)) * 0.5);
        double cp = cos((degRot[1] * (M_PI / 180.0f)) * 0.5);
        double sp = sin((degRot[1] * (M_PI / 180.0f)) * 0.5);
        double cy = cos((degRot[2] * (M_PI / 180.0f)) * 0.5);
        double sy = sin((degRot[2] * (M_PI / 180.0f)) * 0.5);

        FbxVector4 quat;
        quat[3] = cr * cp * cy + sr * sp * sy;
        quat[0] = sr * cp * cy - cr * sp * sy;
        quat[1] = cr * sp * cy + sr * cp * sy;
        quat[2] = cr * cp * sy - sr * sp * cy;

        return quat;
    }

    template <class T>
    int32_t GetLayerItemIdx(int32_t vertIdx, FbxLayerElementTemplate<T>* layer, int index_id)
    {
        if (layer == nullptr)
            return -1;

        FbxLayerElement::EMappingMode mapMode = layer->GetMappingMode();
        int index = 0;
        if (mapMode == FbxLayerElement::eByControlPoint)
            index = vertIdx;
        else if (mapMode == FbxLayerElement::eByPolygonVertex)
            index = index_id;
        else
            return -1;

        FbxLayerElement::EReferenceMode refMode = layer->GetReferenceMode();
        if (refMode == FbxLayerElement::eDirect)
            return index;
        else if (refMode == FbxLayerElement::eIndexToDirect)
            return layer->GetIndexArray().GetAt(index);
        else
            return -1;
    }
}