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

#include "iceoryx_posh/roudi/roudi_cmd_line_parser_config_file_option.hpp"
#include "iceoryx_posh/internal/log/posh_logging.hpp"
#include "iceoryx_versions.hpp"

#include "iceoryx_utils/platform/getopt.hpp"
#include <iostream>

namespace iox
{
namespace roudi
{
void CmdLineParserConfigFileOption::parse(int argc,
                                          char* argv[],
                                          const CmdLineArgumentParsingMode cmdLineParsingMode) noexcept
{
    constexpr option longOptions[] = {{"help", no_argument, nullptr, 'h'},
                                      {"config-file", required_argument, nullptr, 'c'},
                                      {nullptr, 0, nullptr, 0}};

    // colon after shortOption means it requires an argument, two colons mean optional argument
    constexpr const char* shortOptions = ":hc:";
    int index;
    int opt{-1};
    while (opt = getopt_long(argc, argv, shortOptions, longOptions, &index), opt != -1)
    {
        switch (opt)
        {
        case 'h':
            // we want to parse the help option again, therefore we need to decrement the option index of getopt
            optind--;
            CmdLineParser::parse(argc, argv);
            std::cout << std::endl;
            std::cout << "Config File Option:" << std::endl;
            std::cout << "-c, --config-file                 Path to the RouDi Config File." << std::endl;
            std::cout << "                                  Have a look at the documentation for the format."
                      << std::endl;
            std::cout << "                                  If option is not given, either" << std::endl;
            std::cout << "                                  /etc/iceoryx/roudi_config.toml" << std::endl;
            std::cout << "                                  or built-in config is used." << std::endl;
            m_run = false;
            break;
        case 'c':
        {
            m_customConfigFilePath = ConfigFilePathString_t(cxx::TruncateToCapacity, optarg);
            break;
        }
        default:
        {
            // we want to parse the help option again, therefore we need to decrement the option index of getopt
            optind--;
            CmdLineParser::parse(argc, argv, CmdLineArgumentParsingMode::ONE);
        }
        };

        if (cmdLineParsingMode == CmdLineArgumentParsingMode::ONE)
        {
            break;
        }
    }
}
ConfigFilePathString_t CmdLineParserConfigFileOption::getConfigFilePath() const
{
    return m_customConfigFilePath;
}
} // namespace roudi
} // namespace iox
