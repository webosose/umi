// Copyright (c) 2016-2020 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

/* This file is intended only for ALSA */

#include "devicecapability.h"
#include "logging.h"
#include <pbnjson/cxx/JDomParser.h>
#include "config.h"
using namespace pbnjson;

DeviceCapability::DeviceCapability(const std::string& configFilePath)
{
        loadDeviceConfig(configFilePath);
}

void DeviceCapability::loadDeviceConfig(const std::string& configFilePath)
{
    // Load static DeviceCapability
    LOG_INFO(MSGID_LOAD_CONFIG, 0, "Loading static DeviceCapability from file %s", configFilePath.c_str());
    JValue configJson = JDomParser::fromFile(configFilePath.c_str(), JSchema::AllSchema());

    if (!configJson.isValid() || !configJson.isObject())
    {
        LOG_ERROR(MSGID_CONFIG_FILE_READ_FAILED, 0, "Failed to parse DeviceCapability file, using defaults. File: %s. Error: %s",
                  configFilePath.c_str(),configJson.errorString().c_str());
        return;
    }

    JValue categoryObject = configJson[WEBOS_SOC_TYPE];
    LOG_DEBUG("Value for soc type: %s",WEBOS_SOC_TYPE);

    // Parse the value, overriding defaults, so all values are optional.
    if (categoryObject.hasKey("audioMasterDefault"))
    {
        LOG_DEBUG("Found audioMasterDefault");
        parseAudioDefaults(categoryObject["audioMasterDefault"]);
    }
    else {
        LOG_DEBUG("Did not find  audioMasterDefault");
    }

}

void DeviceCapability::parseAudioDefaults(pbnjson::JValue object)
{
    if (!object.isObject())
    {
        LOG_ERROR(MSGID_CONFFILE_MISCONFIGURED, 0, "Failed to read resolution. using defaults.");
        return;
    }

    if (object.hasKey("card") && object.hasKey("muteControlName") && object.hasKey("volumeControlName"))
    {
        LOG_DEBUG("Found card muteControlName and volumeControlName");
        setCardName(object["card"].asString());
        setMuteControlName(object["muteControlName"].asString());
        setVolumeControlName(object["volumeControlName"].asString());
    }
}

DeviceCapability::~DeviceCapability()
{
    LOG_DEBUG("Destroy DeviceCapability");
}
