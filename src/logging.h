// Copyright (c) 2017-2018 LG Electronics, Inc.
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

#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <exception>
#include <sstream>
#include <memory>
#include <PmLogLib.h>

extern PmLogContext umiLogContext;
#define LOG_CRITICAL(msgid, kvcount, ...) \
    PmLogCritical(umiLogContext, msgid, kvcount, ##__VA_ARGS__)

#define LOG_ERROR(msgid, kvcount, ...) \
    PmLogError(umiLogContext, msgid, kvcount,##__VA_ARGS__)

#define LOG_WARNING(msgid, kvcount, ...) \
    PmLogWarning(umiLogContext, msgid, kvcount, ##__VA_ARGS__)

#define LOG_INFO(msgid, kvcount, ...) \
    PmLogInfo(umiLogContext, msgid, kvcount, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
    PmLogDebug(umiLogContext, "%s:%s() " fmt, __FILE__, __FUNCTION__, ##__VA_ARGS__)

#define LOG_ESCAPED_ERRMSG(msgid, errmsg) \
    do { \
    gchar *escaped_errtext = g_strescape(errmsg, NULL); \
     LOG_ERROR(msgid, 1, PMLOGKS("Error", escaped_errtext), ""); \
    g_free(escaped_errtext); \
    } while(0)

static std::string string_format_valist(const std::string& fmt_str, va_list ap)
{
    size_t n = fmt_str.size() * 2;
    std::unique_ptr<char[]> formatted(new char[n]);
    va_list apCopy;
    va_copy(apCopy, ap);

    int final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
    if (final_n < 0 || final_n >= (int)n)
    {
            /* There was not enough space, retry */
            /* MS implements < 0 as not large enough */
            n = (size_t) (abs(final_n) + 1);

            formatted.reset(new char[n]);
            vsnprintf(&formatted[0], n, fmt_str.c_str(), apCopy);
    }
    va_end(apCopy);

    return std::string(formatted.get());
}

class FatalException : public std::exception
{
public:
    FatalException(const char *file, int line, const char* format, ...)
    {
        std::stringstream s;
        va_list args;
        va_start(args, format);
        s << file << ":" << line << ": " << string_format_valist(format, args);
        va_end(args);
        mMessage = s.str();
        LOG_ERROR("FATAL_ERROR", 0, "%s", mMessage.c_str());
        va_end(args);
    }
    const char* what() const noexcept override
    {
        return mMessage.c_str();
    }

private:
    std::string mMessage;
};

#define THROW_FATAL_EXCEPTION(...)  throw FatalException(__FILE__, __LINE__, __VA_ARGS__)

#define MSGID_SET_VOLUME_ERROR           "SET_VOLUME_ERROR"
#define MSGID_SET_MUTE_ERROR             "SET_MUTE_ERROR"
#define MSGID_INVALID_DEFAULT_VOLUME     "INVALID_DEFAULT_VOLUME"
#define MSGID_INVALID_UMI_TYPE           "INVALID_UMI_TYPE"
#define MSGID_INVALID_UMI_FACTORY        "INVALID_UMI_FACTOY"

//config file releated
#define MSGID_LOAD_CONFIG                "LOAD_CONFIG"
#define MSGID_CONFIG_FILE_READ_FAILED    "CONFIG_FILE_READ_FAILED"
#define MSGID_CONFFILE_MISCONFIGURED     "CONFIG_FILE_MISCONFIGURED"

#define MSGID_HAL_ERROR                  "HAL_ERROR"
#endif
