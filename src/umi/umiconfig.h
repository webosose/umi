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

#ifndef UMI_CONFIG_H
#define UMI_CONFIG_H

#include <string>
#include <pbnjson/cxx/JValue.h>
#include <pbnjson/cxx/JDomParser.h>
#include "config.h"

class umiConfig
{
public:

    bool loadUMIConfig();
    static umiConfig* getInstance();
    std::string getValue(std::string keyType);

private:
    pbnjson::JValue fileUMIConfig;
    umiConfig(){ }
    ~umiConfig(){ }
    umiConfig(umiConfig const&)    = delete;
    void operator=(umiConfig const&)  = delete;

};
#endif
