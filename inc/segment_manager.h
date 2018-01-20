/* Copyright 2017 All Rights Reserved.
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
 *  
 * [Contact]
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef INC_SEGMENT_MANAGER_H_
#define INC_SEGMENT_MANAGER_H_

#include <stdint.h>
#include <list>
#include <mutex>
#include <condition_variable>

// For experiment
#include <sys/time.h>
#include <unistd.h>

#define kSegMaxQueueSize 104857600 // Maximum 100MB Queue
#define kSegSize 512
#define kSegThreshold 512
#define kSegQueueThreshold 50*(kSegThreshold / 512)

#define kSegFreeThreshold 256

#define kSegHeaderSize  8

#define kSegLenOffset 0
#define kSegLenMask 0x00007FFF
#define kSegFlagOffset  15
#define kSegFlagMask 0x00008000
#define mGetSegLenBits(x)(((x) & kSegLenMask) >> kSegLenOffset)
#define mGetSegFlagBits(x)(((x) & kSegFlagMask) >> kSegFlagOffset)
#define mSetSegBits(x, dest, offset, mask)  do {\
  dest |=((x << offset) & mask); \
} while (0)
#define mSetSegLenBits(x, dest) \
    mSetSegBits(x, dest, kSegLenOffset, kSegLenMask)
#define mSetSegFlagBits(x, dest) \
    mSetSegBits(x, dest, kSegFlagOffset, kSegFlagMask)

namespace cm {
/*
 *  Queue Type
 */
typedef enum {
  kSegSend = 0,
  kSegRecv = 1,
  kSegMaxQueueType = 2
} SegQueueType;
/*  Types of Flag
 */
typedef enum {
  kSegFlagMF = 1
} SegFlagVal;

/*
 * < Data Structure of Segment > - Handled by Segment Manager
 * Segment is the minimum unit of sending data through the network. 
 * The partial of the segment cannot be sent.
 *
 * (*c.f.) Segment Header (seq_no + flag_len) is delicate to memroy alignment.
 *    You need to be careful with the segment header.
 */
typedef struct {
  uint32_t seq_no;
  uint32_t flag_len;    // To present the size of the segment(consider the flag)
  uint8_t data[kSegSize + kSegHeaderSize];
} Segment;

class SegmentManager {
 public:
  static SegmentManager *get_instance(void);
  int is_changing_adapter; // 0:free | 1:increasing | 2: decreasing
  int wfd_state;


  /* Used in protocol manager */
  int send_to_segment_manager(uint8_t *data, size_t len);
  uint8_t *recv_from_segment_manager(void *proc_data_handle);

  void failed_sending(Segment *seg);
  Segment *get_failed_sending(void);
  void enqueue(SegQueueType type, Segment *seg);
  Segment *dequeue(SegQueueType type);  
  Segment *get_free_segment(void);
  void free_segment(Segment *seg);
  void free_segment_all(void);
  uint32_t queue_threshold;

  void reset(void);

  void notify_queue(void);
 private:
  SegmentManager(void);

  std::mutex seq_no_lock;
  uint32_t seq_no;
  uint32_t get_seq_no(uint32_t num_segments);

   
  uint8_t try_dequeue;  // # of try to dequeue
  int num_increase;
  int trigger, prev_trigger;
  int start_decrease;
   
  
  // for experiment
  int is_start, is_finish;
  struct timeval start,end;
  FILE *fp2;

  /* When access to queue, lock should be acquired */
  std::mutex lock[kSegMaxQueueType];
  std::mutex failed_lock;
  std::condition_variable not_empty[kSegMaxQueueType];

  std::mutex exp_lock;
  std::condition_variable exp_wait;
  uint32_t next_seq_no[kSegMaxQueueType];
  std::list<Segment *> queue[kSegMaxQueueType];
  std::list<Segment *> failed;
  std::list<Segment *> pending_queue[kSegMaxQueueType];

  std::mutex free_list_lock;
  std::list<Segment *> free_list;
  uint32_t free_list_size;

  uint32_t queue_size[kSegMaxQueueType];
  void serialize_segment_header(Segment *seg);

  void release_segment_from_free_list(uint32_t threshold);


  void reset_send_queue(void);
  void reset_recv_queue(void);
};
} /* namespace cm */
#endif  // INC_SEGMENT_MANAGER_H_
