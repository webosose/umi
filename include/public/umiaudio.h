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

#ifndef UMI_AUDIO_API_H
#define UMI_AUDIO_API_H

#include <cstdint>
#include <umicommon.h>

typedef uint8_t UMI_AUDIO_VOLUME_T;
typedef uint8_t SpeakerVolume;
static const SpeakerVolume MIN_VOLUME = 0;
static const SpeakerVolume MAX_VOLUME = 100;

/**
 * UMI AUDIO Resources.
 */
typedef  enum
{
    UMI_AUDIO_RESOURCE_SDEC0,               /* 0 */
    UMI_AUDIO_RESOURCE_SDEC1,               /* 1 */
    UMI_AUDIO_RESOURCE_ATP0,                /* 2 */
    UMI_AUDIO_RESOURCE_ATP1,                /* 3 */
    UMI_AUDIO_RESOURCE_ADC,                 /* 4 */
    UMI_AUDIO_RESOURCE_HDMI,                /* 5 */
    UMI_AUDIO_RESOURCE_AAD,                 /* 6 */
    UMI_AUDIO_RESOURCE_SYSTEM,              /* 7 Clip or LMF Play */
    UMI_AUDIO_RESOURCE_ADEC0,               /* 8 */
    UMI_AUDIO_RESOURCE_ADEC1,               /* 9 */
    UMI_AUDIO_RESOURCE_AENC0,               /* 10 */
    UMI_AUDIO_RESOURCE_AENC1,               /* 11 */
    UMI_AUDIO_RESOURCE_SE,                  /* 12 */
    UMI_AUDIO_RESOURCE_OUT_SPK,             /* 13 Speaker */
    UMI_AUDIO_RESOURCE_OUT_SPDIF,           /* 14 SPDIF Ouput */
    UMI_AUDIO_RESOURCE_OUT_SB_SPDIF,        /* 15 Sound Bar(SPDIF) : Mixer Output */
    UMI_AUDIO_RESOURCE_OUT_SB_PCM,          /* 16 Sound Bar(PCM)   : Mixer Output(Wireless) */
    UMI_AUDIO_RESOURCE_OUT_SB_CANVAS,       /* 17 Sound Bar(CANVAS): Sound Engine Output */
    UMI_AUDIO_RESOURCE_OUT_HP,              /* 18 Must be controlled by audio decoder.*/
    UMI_AUDIO_RESOURCE_OUT_SCART,           /* 19 Must be controlled by audio decoder.*/
    UMI_AUDIO_RESOURCE_MIXER0,              /* 20 Audio Mixer Input 0. */
    UMI_AUDIO_RESOURCE_MIXER1,              /* 21 Audio Mixer Input 1. */
    UMI_AUDIO_RESOURCE_MIXER2,              /* 22 Audio Mixer Input 2. */
    UMI_AUDIO_RESOURCE_MIXER3,              /* 23 Audio Mixer Input 3. */
    UMI_AUDIO_RESOURCE_MIXER4,              /* 24 Audio Mixer Input 4. */
    UMI_AUDIO_RESOURCE_MIXER5,              /* 25 Audio Mixer Input 5. */
    UMI_AUDIO_RESOURCE_MIXER6,              /* 26 Audio Mixer Input 6. */
    UMI_AUDIO_RESOURCE_MIXER7,              /* 27 Audio Mixer Input 7. */
    UMI_AUDIO_RESOURCE_OUT_SPDIF_ES,        /* 28 SPDIF ES Output Only */
    UMI_AUDIO_RESOURCE_HDMI0,               /* 29 Audio HDMI Input 0. */
    UMI_AUDIO_RESOURCE_HDMI1,               /* 30 Audio HDMI Input 1. */
    UMI_AUDIO_RESOURCE_HDMI2,               /* 31 Audio HDMI Input 2. */
    UMI_AUDIO_RESOURCE_HDMI3,               /* 32 Audio HDMI Input 3. */
    UMI_AUDIO_RESOURCE_SWITCH,              /* 33 Audio HDMI Input with switch. */
    UMI_AUDIO_RESOURCE_NO_CONNECTION = 0XFF,
} UMI_AUDIO_RESOURCE_T;

/**
 * UMI AUDIO Sound Output Mode.
 */
typedef  enum
{
        UMI_AUDIO_NO_OUTPUT             = 0x00,
        UMI_AUDIO_SPK                   = 0x01,
        UMI_AUDIO_SPDIF                 = 0x02,
        UMI_AUDIO_SB_SPDIF              = 0x04,
        UMI_AUDIO_SB_PCM                = 0x08,
        UMI_AUDIO_SB_CANVAS             = 0x10,
        UMI_AUDIO_HP                    = 0x20,
        UMI_AUDIO_SCART                 = 0x40,
        UMI_AUDIO_SPDIF_ES              = 0x80,
        UMI_AUDIO_AMIXER                = 0x100,
} UMI_AUDIO_SNDOUT_T;

typedef enum
{
        UMI_AUDIO_SPDIF_NONE           = 0,
        UMI_AUDIO_SPDIF_PCM            = 1,
        UMI_AUDIO_SPDIF_AUTO           = 2,
        UMI_AUDIO_SPDIF_AUTO_AAC       = 3,
        UMI_AUDIO_SPDIF_MIXED_DD       = 4,    /* Google Mixed DD(AC3 Encoding) */
} UMI_AUDIO_SPDIF_MODE_T ;

typedef enum
{
        UMI_ERROR_NONE,
        UMI_OUTPUT_NOT_SUPPORTED,
        UMI_ERROR_FAIL
} UMI_ERROR;

class umiAudio {

public:

    virtual ~umiAudio() = default;

    virtual bool initAudio() = 0;
    virtual bool deinitAudio() = 0;

    virtual UMI_ERROR connectInput(UMI_AUDIO_RESOURCE_T audioResourceId) = 0;
    virtual UMI_ERROR disconnectInput(UMI_AUDIO_RESOURCE_T audioResourceId) = 0;
    virtual UMI_ERROR setSoundOutput(UMI_AUDIO_SNDOUT_T soundOut) = 0;
    virtual UMI_ERROR setMute(UMI_AUDIO_RESOURCE_T audioResourceId, bool mute) = 0;
    virtual UMI_ERROR setOutputVolume(UMI_AUDIO_SNDOUT_T outputType, UMI_AUDIO_VOLUME_T volume) = 0;
    virtual UMI_ERROR setOutputMute(UMI_AUDIO_SNDOUT_T outputType, bool mute) = 0;
};

#endif //UMI_AUDIO_API_H
