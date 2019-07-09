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

#ifndef UMI_ALSA_AUDIO_H
#define UMI_ALSA_AUDIO_H

#include <umiaudio.h>
#include "devicecapability.h"
#include "umifactory.h"

class umiAlsaAudio : public umiAudio
{
public:

    static umiAudio* createObject();
    umiAlsaAudio(umiAlsaAudio const&)    = delete;
    void operator=(umiAlsaAudio const&)  = delete;

    bool initAudio();
    bool deinitAudio();
    UMI_ERROR connectInput(UMI_AUDIO_RESOURCE_T audioResourceId);
    UMI_ERROR disconnectInput(UMI_AUDIO_RESOURCE_T audioResourceId);
    UMI_ERROR setSoundOutput(UMI_AUDIO_SNDOUT_T soundOut);
    UMI_ERROR setMute(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute);
    UMI_ERROR setOutputMute(UMI_AUDIO_SNDOUT_T outputType, bool mute);
    UMI_ERROR setOutputVolume(UMI_AUDIO_SNDOUT_T outputType, UMI_AUDIO_VOLUME_T volume);

private:
    static bool mIsObjRegistered;
    DeviceCapability mMasterDeviceCapability;
    DeviceCapability mDeviceCapability;

    umiAlsaAudio():mDeviceCapability(getConfigFilePath()){ }
    ~umiAlsaAudio();
    std::string getConfigFilePath();
    //Register Object to object factory. This is called automatically
    static bool RegisterObject()
    {
        return (umiFactory::getInstance()->Register("ALSA",
                            &umiAlsaAudio::createObject));
    }
    bool initSpeaker();
    bool setVolume(UMI_AUDIO_VOLUME_T volume);
    bool setControl(std::string cardStr, std::string idStr, std::string volumeToSet);
    UMI_ERROR resetMixerVolume(UMI_AUDIO_RESOURCE_T t, bool mute);


};
#endif
