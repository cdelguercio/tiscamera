/*
 * Copyright 2021 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "AravisDevice.h"
#include "aravis_property_impl.h"
#include "logging.h"

namespace tcam
{

void AravisDevice::index_properties(const char* name)
{
    ArvGcNode* node = arv_gc_get_node(genicam, name);

    if (ARV_IS_GC_FEATURE_NODE(node)
        && arv_gc_feature_node_is_implemented(ARV_GC_FEATURE_NODE(node), NULL)
        && arv_gc_feature_node_is_available(ARV_GC_FEATURE_NODE(node), NULL))
    {

        if (ARV_IS_GC_CATEGORY(node))
        {
            const GSList* features;
            const GSList* iter;

            std::string node_name = arv_gc_feature_node_get_name(ARV_GC_FEATURE_NODE(node));

            if (node_name == "DeviceControl"
                || node_name == "TransportLayerControl"
                || node_name == "ImageFormatControl")
            {
                return;
            }

            features = arv_gc_category_get_features(ARV_GC_CATEGORY(node));

            for (iter = features; iter != NULL; iter = iter->next)
            {
                index_properties((char*)iter->data);
            }
            return;
        }
    }

    static std::vector<std::string> private_settings = {
        "TLParamsLocked",
        "GevSCPSDoNotFragment",
        "GevTimestampTickFrequency",
        "GevTimeSCPD",
        "GevSCPD",
        "PayloadSize",
        "PayloadPerFrame",
        "PayloadPerPacket",
        "TotalPacketSize",
        "PacketsPerFrame",
        "PacketTimeUS",
        "GevSCPSPacketSize",
        "GevSCPSFireTestPacket",
        "DeviceVendorName",
        "DeviceType",
        "DeviceModelType",
        "DeviceVersion",
        "DeviceSerialNumber",
        "DeviceUserID",
        "DeviceSFNCVersionMajor",
        "DeviceSFNCVersionMinor",
        "DeviceTLType",
        "DeviceTLTypeMajor",
        "DeviceTLTypeMinor",
        "DeviceTLTypeSubMinor",
        "DeviceLinkSelector",
        "WidthMax",
        "HeightMax",
        "ChunkModeActive",
        "ChunkImage",
        "ChunkBlockId",
        "ActionDeviceKey",
        "ActionSelector",
        "ActionGroupMask",
        "ActionGroupKey",
        "UserSetSelector",
        "UserSetLoad",
        "UserSetSave",
        "UserSetDefault",
        "DeviceScanType",
        "StringReg",
        "DeviceModelName",
        "DeviceSFNCVersionSubMinor",
        "MaskedIntReg",
        "DeviceTLVersionMajor",
        "MaskedIntReg",
        "DeviceTLVersionMinor",
        "DeviceTLVersionSubMinor",
        "DeviceLinkHeartbeatTimeout",
        "DeviceStreamChannelCount",
        "DeviceStreamChannelSelector",
        "DeviceStreamChannelType",
        "DeviceStreamChannelLink",
        "DeviceStreamChannelEndianness",
        "DeviceStreamChannelPacketSize",
        "DeviceEventChannelCount",
        "DeviceTemperatureConverter",
        "IMX174HardwareWDRShutterMode",
        "IMX174HardwareWDREnable",
        "IMX174WDRShutter2",
        "ChunkIMX174FrameSet",
        "ChunkIMX174FrameId",
        "SensorPixelHeight",
        "SensorPixelWidth",
        "AcquisitionStart",
        "AcquisitionStop",
        "AcquisitionMode",
        // "Binning",
        "SensorWidth",
        "SensorHeight",
        "Width",
        "Height",
        "FPS",
        "AcquisitionFrameRate",
        "PixelFormat"
    };

    if (std::find(private_settings.begin(), private_settings.end(), arv_gc_feature_node_get_name(ARV_GC_FEATURE_NODE(node)))
        != private_settings.end())
    {
        //SPDLOG_INFO("Private setting {}", arv_gc_feature_node_get_name(ARV_GC_FEATURE_NODE(node)));
        return;
    }

    if (strcmp(arv_dom_node_get_node_name(ARV_DOM_NODE(node)), "Float") == 0)
     {
        m_properties.push_back(
            std::make_shared<tcam::property::AravisPropertyDoubleImpl>(arv_camera, node, m_backend));

     }
    else if (strcmp(arv_dom_node_get_node_name(ARV_DOM_NODE(node)), "Integer") == 0)
    {
        m_properties.push_back(
            std::make_shared<tcam::property::AravisPropertyIntegerImpl>(arv_camera, node, m_backend));
    }
    else if (strcmp(arv_dom_node_get_node_name(ARV_DOM_NODE(node)), "Boolean") == 0)
    {
        m_properties.push_back(
            std::make_shared<tcam::property::AravisPropertyBoolImpl>(arv_camera, node, m_backend));
    }
    else if (strcmp(arv_dom_node_get_node_name(ARV_DOM_NODE(node)), "Command") == 0)
    {
        m_properties.push_back(
            std::make_shared<tcam::property::AravisPropertyCommandImpl>(node, m_backend));
    }
    else if (strcmp(arv_dom_node_get_node_name(ARV_DOM_NODE(node)), "Enumeration") == 0)
    {
        m_properties.push_back(
            std::make_shared<tcam::property::AravisPropertyEnumImpl>(arv_camera, node, m_backend));
    }
    //m_properties.push_back();
}

} // namespace tcam
