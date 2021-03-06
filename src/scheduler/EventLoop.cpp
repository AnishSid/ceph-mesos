/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "EventLoop.hpp"

EventLoop::EventLoop()
{
  loopTag = true;
  recvThread = new thread(&EventLoop::recvData, this);
}

EventLoop::~EventLoop()
{
  loopTag = false;
  recvThread->join();
  recvThread = NULL;
}

void EventLoop::recvData()
{
  zmq::socket_t receiver(context, ZMQ_PULL);
  receiver.bind("inproc://flexUp");
  try{
    while (loopTag) {
      zmq::message_t message;
      string event_data;
      receiver.recv(&message);
      istringstream iss(static_cast<char*>(message.data()));
      iss >> event_data;
      LOG(INFO) << "EventLoop.recvData got flexUp data";
      processData(event_data);
    }
    receiver.close();
  } catch (const std::exception& e){
    LOG(INFO) << "EventLoop.recvData " << e.what();
  }
}

void EventLoop::processData(string data)
{
  const char* json = data.c_str();
  Json::Reader reader;
  Json::Value value;
  if (reader.parse(json, value)) {
    if ("osd" == value["profile"].asString()){
      if (value["instances"].asInt() != 0){
        pendingOSD.push_back(value["instances"].asInt());
      }
      LOG(INFO) << "pendingOSD flexUp request Num: " << pendingOSD.size();
    } else {
      LOG(INFO) << "EventLoop.processData not profile:osd";
    }
  } else {
    LOG(INFO) << "EventLoop.processData json error" << endl;
  }
}
