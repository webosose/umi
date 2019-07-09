// Copyright (c) 2018-2019 LG Electronics, Inc.
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

#include <alsa/asoundlib.h>
#include "logging.h"
#include "umialsaaudio.h"
#include "devicecapability.h"
#include "umiclient.h"
#include "config.h"

using namespace std;

bool umiAlsaAudio::mIsObjRegistered = umiAlsaAudio::RegisterObject();

umiAudio* umiAlsaAudio::createObject()
{
    if (umiAlsaAudio::mIsObjRegistered)
    {
        static umiAlsaAudio  _alsaInstance;
        return &_alsaInstance;
    }
    else
        return nullptr;
}

umiAlsaAudio::~umiAlsaAudio()
{
    umiFactory::getInstance()->UnRegister("ALSA");
    LOG_DEBUG("UnRegister umiAlsaAudio ...");
}

bool umiAlsaAudio::initSpeaker()
{
    return true;
}

bool umiAlsaAudio::initAudio()
{
    bool success = true;

    success = initSpeaker();
    return success;
}

bool umiAlsaAudio::deinitAudio()
{
    return true;
}

string umiAlsaAudio::getConfigFilePath()
{
    stringstream configPath;
    configPath << CONFIG_DIR_PATH << "/" << "device-cap.json";
    LOG_DEBUG("UMI Configuration file path: %s", configPath.str().c_str());
    return configPath.str();
}

UMI_ERROR umiAlsaAudio::connectInput(UMI_AUDIO_RESOURCE_T audioResourceId)
{
    LOG_DEBUG("connectInput  %d", audioResourceId);

    return resetMixerVolume(audioResourceId, false);

}

UMI_ERROR umiAlsaAudio::disconnectInput(UMI_AUDIO_RESOURCE_T audioResourceId)
{
    return resetMixerVolume(audioResourceId, true);

}

UMI_ERROR umiAlsaAudio::resetMixerVolume(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute)
{
    LOG_DEBUG("reset volume  resource=%d mute=%d", audioResourceId, mute);

    //TODO::This is a temporary hack. Fix this when we implement different output types
    string card = mDeviceCapability.getCardName();
    string controlId = "name=";
    controlId += mDeviceCapability.getMuteControlName();
    string volumeStr = "0%";
    if (!mute)
    {
        volumeStr = "100%";
    }

    if (!setControl(card, controlId, volumeStr))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR, 0, "Failed to set control %s to %s for card:%s",controlId.c_str(), volumeStr.c_str(), card);
        return UMI_ERROR_FAIL;
    }

    card = mMasterDeviceCapability.getCardName();
    controlId = "name=";
    controlId += mMasterDeviceCapability.getMuteControlName();

    if (!setControl(card, controlId, volumeStr))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR, 0, "Failed to set control %s to %s for card:%s",controlId.c_str(), volumeStr.c_str(), card.c_str());
        return UMI_ERROR_FAIL;
    }
    return UMI_ERROR_NONE;
}

UMI_ERROR umiAlsaAudio::setMute(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute)
{
    return resetMixerVolume(audioResourceId, mute);
}

UMI_ERROR umiAlsaAudio::setSoundOutput(UMI_AUDIO_SNDOUT_T soundOut)
{
    return UMI_ERROR_NONE;
}

UMI_ERROR umiAlsaAudio::setOutputMute(UMI_AUDIO_SNDOUT_T outputType, bool mute)
{
    LOG_DEBUG("In %s, setting mute to %d", __func__, mute);

    //TODO::This is a temporary hack. Fix this when we implement different output types
    string card = mDeviceCapability.getCardName();
    string idstr = "name=";
    idstr += mDeviceCapability.getMuteControlName();
    string mutestr = mute?"off":"on";
    if (!setControl(card, idstr, mutestr))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR,0, "Failed to set mute %s for card:%s, control:%s", mutestr.c_str(), card.c_str(), idstr.c_str());
        return UMI_ERROR_FAIL;
    }

    card = mMasterDeviceCapability.getCardName();
    idstr = "name=";
    idstr += mMasterDeviceCapability.getMuteControlName();
    if (!setControl(card, idstr, mutestr))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR,0, "Failed to set mute %s for card:%s, control:%s", mutestr.c_str(), card.c_str(), idstr.c_str());
        return UMI_ERROR_FAIL;
    }
    return UMI_ERROR_NONE;
}

UMI_ERROR umiAlsaAudio::setOutputVolume(UMI_AUDIO_SNDOUT_T outputType, UMI_AUDIO_VOLUME_T volume)
{
    //TODO: Add volume mapping curve - currently linear so no mapping needed
    if(!setVolume(volume))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR, 0, "Failed setting volume to %d", volume);
        return UMI_ERROR_FAIL;
    }

    return UMI_ERROR_NONE;
}

bool umiAlsaAudio::setVolume(UMI_AUDIO_VOLUME_T volume)
{
    string card = mDeviceCapability.getCardName();
    string idstr = "name=";
    idstr += mDeviceCapability.getVolumeControlName();

    string volumeToSet = to_string(volume);
    volumeToSet += "%";

    LOG_DEBUG("In %s, setting volume to %s", __func__, volumeToSet.c_str());
    if (!setControl(card, idstr, volumeToSet))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR, 0, "Failed to set control %s to %s for card:%s",idstr.c_str(), volumeToSet.c_str(), card);
        return false;
    }

    card = mMasterDeviceCapability.getCardName();
    idstr = "name=";
    idstr += mMasterDeviceCapability.getVolumeControlName();

    if (!setControl(card, idstr, volumeToSet))
    {
        LOG_ERROR(MSGID_SET_VOLUME_ERROR, 0, "Failed to set control %s to %s for card:%s",idstr.c_str(), volumeToSet.c_str(), card.c_str());
        return false;
    }

    return true;
}

bool umiAlsaAudio::setControl(string card, string idstr, string controlValue){
    int err;
    static snd_ctl_t *handle = nullptr;
    snd_ctl_elem_info_t *info;
    snd_ctl_elem_id_t *id;
    snd_ctl_elem_value_t *control;

    if(""==card || ""==idstr || ""==controlValue)
    {
        LOG_DEBUG("Wrong input parameter\n");
        return false;
    }
    snd_ctl_elem_info_alloca(&info);
    snd_ctl_elem_id_alloca(&id);
    snd_ctl_elem_value_alloca(&control);

    if (snd_ctl_ascii_elem_id_parse(id, idstr.c_str()))
    {
        LOG_DEBUG("Wrong control identifier: %s\n", idstr.c_str());
        return false;
    }

    LOG_DEBUG("snd_ctl_open on card %s, control %s value=%s\n", card.c_str(), idstr.c_str(), controlValue.c_str());
    if ( nullptr == handle &&
        (err = snd_ctl_open(&handle, card.c_str(), 0)) < 0)
    {
        LOG_DEBUG("Control %s open error: %s\n", card.c_str(), snd_strerror(err));
        return false;
    }

    snd_ctl_elem_info_set_id(info, id);
    if ((err = snd_ctl_elem_info(handle, info)) < 0)
    {
        LOG_DEBUG("Cannot find the given element from control %s, error %s\n", card.c_str(), snd_strerror(err));
        snd_ctl_close(handle);
        handle = nullptr;
        return false;
    }

    err = snd_ctl_ascii_value_parse(handle, control, info, controlValue.c_str());
    if (err < 0)
    {
        LOG_DEBUG("Control %s parse error: %s\n", card.c_str(), snd_strerror(err));
        snd_ctl_close(handle);
        handle = nullptr;
        return false;
    }

    if ((err = snd_ctl_elem_write(handle, control)) < 0) {
        LOG_DEBUG("Control %s element write error: %s\n", card.c_str(), snd_strerror(err));
        snd_ctl_close(handle);
        handle = nullptr;
        return false;
    }

    snd_ctl_close(handle);
    handle = nullptr;
    return true;
}
