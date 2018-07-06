// Copyright (c) 2018 LG Electronics, Inc.
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


#include "umiconfig.h"
#include "logging.h"

#define UMI_CONFIG_FILE    "umi-config.json"

using namespace pbnjson;

umiConfig* umiConfig::getInstance()
{
    static umiConfig  _umiConfigInstance;
    return &_umiConfigInstance;
}

bool umiConfig::loadUMIConfig()
{
    bool loadStatus = true;
    std::stringstream configFilePath;
    configFilePath << CONFIG_DIR_PATH << "/" << UMI_CONFIG_FILE;

    LOG_INFO(MSGID_LOAD_CONFIG, 0, "Loading umiConfig config from file %s", configFilePath.str().c_str());

    fileUMIConfig = pbnjson::JDomParser::fromFile(configFilePath.str().c_str(), JSchema::AllSchema());

    if (!fileUMIConfig.isValid() || !fileUMIConfig.isObject())
    {
        loadStatus = false;
        LOG_ERROR(MSGID_CONFIG_FILE_READ_FAILED, 0, "Failed to parse umi config file, using defaults. File: %s. Error: %s",
                configFilePath.str().c_str(),fileUMIConfig.errorString().c_str());
    }
    return loadStatus;
}

std::string umiConfig::getValue(std::string keyType)
{
    pbnjson::JValue categoryObj = fileUMIConfig[WEBOS_SOC_TYPE];

    std::string value;
    LOG_DEBUG("Need value for soc type: %s & key type: %s",WEBOS_SOC_TYPE,keyType.c_str());

    if (!categoryObj.hasKey(keyType.c_str()))
    {
        LOG_DEBUG("Did not find key type in umi config %s",keyType.c_str());
    }
    else
        value = categoryObj[keyType].asString();

    return value;
}
