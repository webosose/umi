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

#ifndef UMI_FACTORY_H
#define UMI_FACTORY_H

#include <map>
#include <string>
#include "umifactory.h"
#include "umiaudio.h"

typedef umiAudio* (*UMIAudioCreate)();
using UMIAudioMAP = std::map< std::string, UMIAudioCreate > ;

class umiFactory
{
public:

    static umiFactory* getInstance();
    umiFactory(umiFactory const&) = delete;
    void operator=(umiFactory const&) =  delete;

    bool Register(const std::string &socType, UMIAudioCreate createObject);
    bool UnRegister(const std::string &socType);
    umiAudio* createUMIAudioObj(const std::string &socType);
    UMIAudioMAP getRegisteredUMIAudios()
    {
        return mUMIAudioMap;
    }

private:
    UMIAudioMAP mUMIAudioMap;
    umiFactory(){}
    ~umiFactory() { }
};
#endif
