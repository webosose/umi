// Copyright (c) 2017-2024 LG Electronics, Inc.
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

#ifndef DEVICE_CAPABILITY_H
#define DEVICE_CAPABILITY_H

#include <string>
#include <pbnjson/cxx/JValue.h>
#include "umicommon.h"

class DeviceCapability
{
public:
    DeviceCapability() {}
    DeviceCapability(const std::string &configFilePath);
    ~DeviceCapability();

    DeviceCapability &operator=(const DeviceCapability &) = delete;
    DeviceCapability(const DeviceCapability &) = delete;
    std::string getCardName() const {
        return mAudioDefaults.card;
    }
    std::string getMuteControlName() const {
        return mAudioDefaults.muteControlName;
    }
    std::string getVolumeControlName() const {
        return mAudioDefaults.volumeControlName;
    }

private:
    class AudioDefaults
    {
    public:
        AudioDefaults()
        {
            if ("RPI" == WEBOS_SOC_TYPE)
            {
                card = "hw:Headphones";
                muteControlName = "\'PCM Playback Switch\'";
                volumeControlName = "\'PCM Playback Volume\'";
            }
            if ("qemux86" == WEBOS_SOC_TYPE)
            {
                card = "default";
                muteControlName = "\'Master Playback Switch\'";
                volumeControlName = "\'Master Playback Volume\'";
            }
        }
        ~AudioDefaults(){}
        std::string card;
        std::string muteControlName;
        std::string volumeControlName;

    };

    AudioDefaults mAudioDefaults;
    void loadDeviceConfig(const std::string& configFilePath);
    void parseAudioDefaults(pbnjson::JValue element);
    void setCardName(std::string cardName) {
        mAudioDefaults.card = cardName;
    }
    void setMuteControlName(std::string muteName) {
        mAudioDefaults.muteControlName = muteName;
    }
    void setVolumeControlName(std::string volumeName) {
        mAudioDefaults.volumeControlName = volumeName;
    }
};
#endif
