/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * [Contact]
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __NETWORK_INITIALIZER_H__
#define __NETWORK_INITIALIZER_H__

#include <string>

namespace sc {
class NetworkInitializer {
public:
  void initialize(void);
private:
  int kill_dhcpd(void);
  int ping_wpa_cli(char ret[], size_t len);
  void retrieve_wpa_interface_name(std::string &wpaIntfName);
}; /* class NetworkInitializer */
} /* namespace sc */

#endif /* !defined(__NETWORK_INITIALIZER_H__) */