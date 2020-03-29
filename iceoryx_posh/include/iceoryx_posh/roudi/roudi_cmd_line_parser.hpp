// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "iceoryx_posh/iceoryx_posh_types.hpp"
#include "iceoryx_utils/cxx/expected.hpp"
#include "iceoryx_utils/log/logcommon.hpp"

namespace iox
{
namespace roudi
{
/// @brief Controls process alive monitoring. Upon timeout, a monitored process is removed
/// and its resources are made available. The process can then start and register itself again.
/// Contrarily, unmonitored processes can be restarted but registration will fail.
/// Once Runlevel Management is extended, it will detect absent processes. Those processes can register again.
/// ON - all processes are monitored
/// OFF - no process is monitored
enum class MonitoringMode
{
    ON,
    OFF
};

class CmdLineParser
{
  public:
    enum class CmdLineArgumentParsingMode
    {
        ALL,
        ONE
    };

    CmdLineParser() noexcept = default;
    virtual ~CmdLineParser() noexcept = default;
    CmdLineParser& operator=(const CmdLineParser& other) = delete;
    CmdLineParser(const CmdLineParser& other) = delete;
    CmdLineParser& operator=(CmdLineParser&&) = delete;
    CmdLineParser(CmdLineParser&& other) = delete;

    /// @brief process the passed command line arguments
    /// @param[in] argc forwarding of command line arguments
    /// @param[in] argv forwarding of command line arguments
    /// @param[in] cmdLineParsingMode selects to parse a single option or all options
    virtual void parse(int argc,
               char* argv[],
               const CmdLineArgumentParsingMode cmdLineParsingMode = CmdLineArgumentParsingMode::ALL) noexcept;

    bool getRun() const;
    iox::log::LogLevel getLogLevel() const;
    MonitoringMode getMonitoringMode() const;

  protected:
    bool m_run{true};
    iox::log::LogLevel m_logLevel{iox::log::LogLevel::kWarn};
    MonitoringMode m_monitoringMode{MonitoringMode::ON};
};

} // namespace roudi
} // namespace iox
