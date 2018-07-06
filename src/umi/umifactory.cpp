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

#include <sstream>
#include "umifactory.h"


umiFactory* umiFactory::getInstance()
{
    static umiFactory _instance;
    return &_instance;
}

bool umiFactory::Register(const std::string &socType, UMIAudioCreate createObject)
{
    auto it = mUMIAudioMap.insert(std::pair<const std::string,UMIAudioCreate>(socType,createObject));
    return it.second;
}

bool umiFactory::UnRegister(const std::string &socType)
{
	UMIAudioMAP::const_iterator it;

    for(it = mUMIAudioMap.begin(); it != mUMIAudioMap.end(); ++it)
    {
        if(socType == it->first)
        {
            mUMIAudioMap.erase(socType);
            return true;
        }
    }

    return false;
}

umiAudio* umiFactory::createUMIAudioObj(const std::string &socType)
{
	umiAudio* audioObj = nullptr;
    std::map<std::string, UMIAudioCreate>::iterator it = mUMIAudioMap.begin();

    for( ; it != mUMIAudioMap.end(); ++it)
    {
        if(socType == it->first)
            audioObj = it->second();
    }
    return audioObj;
}
