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

#ifndef _CEPHMESOS_HTTPSERVER_RESTSERVER_HPP_
#define _CEPHMESOS_HTTPSERVER_RESTSERVER_HPP_

#include <string>
#include <sstream>
#include <iostream>
#include <zmq.hpp>

#include <jsoncpp/json/json.h>

extern "C"
{
#include <microhttpd.h>
}

using namespace std;

extern zmq::context_t context;

class RestServer
{
public:
  RestServer();
  RestServer(int port);
  ~RestServer();
  bool check();
  static string jsonCheck(const char* json);

private:
  MHD_Daemon* daemon;
  static int rest_request(
      void *cls,
      MHD_Connection *connection,
      const char *url,
      const char *method,
      const char *version,
      const char *upload_data,
      size_t *upload_data_size,
      void **con_cls);

  struct ConnectionData
  {
    bool is_parsing;
    stringstream read_post_data;
  };

};

int restServer(int port);

const int maxInstancesSize = 1024;
const string osd = "osd";
const string mon = "mon";

#endif
