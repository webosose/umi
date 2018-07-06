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

#include "umiclient.h"
#include "umifactory.h"
#include "umialsaaudio.h"
#include "config.h"
#include "logging.h"
#include "umiconfig.h"

static const char* const logContextName = "umi";
static const char* const logPrefix = "[umi]";
PmLogContext umiLogContext;


umiClient* umiClient::getInstance()
{
    static umiClient  _instance;
    return &_instance;
}

bool umiClient::initialize()
{
    //Setup logging context
    PmLogErr error = PmLogGetContext(logContextName, &umiLogContext);

    if (error != kPmLogErr_None)
    {
        std::cerr << logPrefix << "Failed to setup up log context " << logContextName << std::endl;
        return false;
    }
    umiConfig * umiConfigInstance = umiConfig::getInstance();
    if(nullptr == umiConfigInstance)
    {
        LOG_DEBUG("Falied to get umiConfig instance");
        return false;
    }

    if(!umiConfigInstance->loadUMIConfig())
    {
        LOG_DEBUG("Falied to load UMI Config file");
        return false;
    }
    std::string socUMIType = umiConfigInstance->getValue("umiAudio");
    if(socUMIType == "")
    {
        LOG_ERROR(MSGID_INVALID_UMI_TYPE, 0,"Failed to get umi type from config");
        return false;
    }
    std::string defaultVolume = umiConfigInstance->getValue("umiDefaultVolume");
    try
    {
        mDefaultVolume = std::stoi(defaultVolume);
    }
    catch (const std::exception& e)
    {
        LOG_WARNING(MSGID_INVALID_DEFAULT_VOLUME, 0,"exception %s, set mDefaultVolume as 0",e.what());
        mDefaultVolume = 0;
    }
    LOG_DEBUG("Default volume %d",mDefaultVolume);

    umiFactory * umiFactoryInstance = umiFactory::getInstance();
    if(nullptr == umiFactoryInstance)
    {
        LOG_ERROR(MSGID_INVALID_UMI_FACTORY, 0,"Falied to get umiFactory instance");
        return false;
    }

    UMIAudioMAP UMIAudios = umiFactoryInstance->getRegisteredUMIAudios();

    UMIAudioMAP::iterator it;

    it = UMIAudios.find(socUMIType.c_str());;

    if(it != UMIAudios.end())
        mAudioObj = umiFactoryInstance->createUMIAudioObj(it->first);

    if(nullptr == mAudioObj)
        return false;

    LOG_DEBUG("In umiClient::initialize %s", socUMIType.c_str());

    if(!mAudioObj->initAudio())
        return false;

    return true;
}

bool umiClient::deinitialize()
{
    LOG_DEBUG("In umiClient::deinitialize");
    if(nullptr == mAudioObj)
        return false;

    if(!mAudioObj->deinitAudio())
            return false;

    return true;
}

UMI_AUDIO_VOLUME_T umiClient::getDefaultVolume()
{
    return mDefaultVolume;
}

umiAudio* umiClient::getAudioObj()
{
    return mAudioObj;
}

UMI_ERROR umiClient::connectInput(UMI_AUDIO_RESOURCE_T audioResourceId)
{
    LOG_DEBUG("In umiClient::connectInput");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;

    return audioObj->connectInput(audioResourceId);
}

UMI_ERROR umiClient::disconnectInput(UMI_AUDIO_RESOURCE_T audioResourceId)
{
    LOG_DEBUG("In umiClient::disconnectInput");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;

    return audioObj->disconnectInput(audioResourceId);
}

UMI_ERROR umiClient::setSoundOutput(UMI_AUDIO_SNDOUT_T soundOut)
{
    LOG_DEBUG("In umiClient::setSoundOutput");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;
    return audioObj->setSoundOutput(soundOut);
}

UMI_ERROR umiClient::setMute(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute)
{
    LOG_DEBUG("In umiClient::setMute");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;

    return audioObj->setMute(audioResourceId, mute);
}

UMI_ERROR umiClient::setOutputMute(UMI_AUDIO_SNDOUT_T outputType, bool mute)
{
    LOG_DEBUG("In umiClient::setOutputMute");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;

    return audioObj->setOutputMute(outputType, mute);
}

UMI_ERROR umiClient::setOutputVolume(UMI_AUDIO_SNDOUT_T outputType, UMI_AUDIO_VOLUME_T volume)
{
    LOG_DEBUG("In umiClient::setOutputVolume");
    umiAudio* audioObj = nullptr;

    audioObj = umiClient::getAudioObj();
    if(audioObj == nullptr)
         return UMI_ERROR_FAIL;

    return audioObj->setOutputVolume(outputType, volume);
}
