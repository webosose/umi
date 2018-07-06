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

#ifndef UMI_CLIENT_H
#define UMI_CLIENT_H

#include <umiaudio.h>

class umiClient
{
public:

    static umiClient* getInstance();
    umiClient(umiClient const&)      = delete;
    void operator=(umiClient const&)  = delete;

    bool initialize();
    bool deinitialize();

    UMI_ERROR connectInput(UMI_AUDIO_RESOURCE_T audioResourceId);
    UMI_ERROR disconnectInput(UMI_AUDIO_RESOURCE_T audioResourceId);
    UMI_ERROR setSoundOutput(UMI_AUDIO_SNDOUT_T soundOut);
    UMI_ERROR setMute(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute);
    UMI_ERROR setOutputMute(UMI_AUDIO_SNDOUT_T outputType, bool mute);
    UMI_ERROR setOutputVolume(UMI_AUDIO_SNDOUT_T outputType, UMI_AUDIO_VOLUME_T volume);
    UMI_AUDIO_VOLUME_T getDefaultVolume();

private:
    umiAudio* mAudioObj ;
    UMI_AUDIO_VOLUME_T mDefaultVolume ;
    umiClient():mAudioObj(nullptr) { }
    ~umiClient() { }
    umiAudio* getAudioObj();

};
#endif
