/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef INC_COMMUNICATOR_H_
#define INC_COMMUNICATOR_H_

#include <Communicator.h>
#include <ServerAdapter.h>

class Communicator;
class ServerAdapter;

namespace cm {
inline void start_communication(void) {
  NetworkSwitcher::get_instance()->run();
  Communicator::get_instance()->start();
}

inline void stop_communication(void) {
  Communicator::get_instance()->stop();
}

inline void register_control_adapter(ServerAdapter* adapter) {
  Communicator::get_instance()->register_control_adapter(adapter);
}

inline void register_data_adapter(ServerAdapter* adapter) {
  Communicator::get_instance()->register_data_adapter(adapter);
}

/**
 * If data size is big, it is recommanded to use following
 * libraries in a thread
 */
inline int send(const void *buf, uint32_t len) {
  Communicator::get_instance()->send(buf, len);
}

/**
 * @param len: IN buffer length
 * @param buf: OUT buffer read
 * @return: Received bytes(<0 if error)
 */
inline int receive(void **buf) {
  Communicator::get_instance()->receive(buf);
}

} /* namespace cm */
#endif  /* INC_COMMUNICATOR_H_ */
