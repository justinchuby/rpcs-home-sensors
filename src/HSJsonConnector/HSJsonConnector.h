#ifndef _JSON_CONNECTOR_
#define _JSON_CONNECTOR_

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <string>

enum class HSEvent { DATA, HANDSHAKE, MESSAGE, ERROR };

class HSJsonConnector {
 public:
  void setSensor(const char* sensor_id, const char* sensor_type);

  void setServer(const char* url);
  // returns a http response code
  // Accepts a serialized json object
  int send(HSEvent type, const std::string &obj);

 private:
  std::string _sensor_id;
  std::string _sensor_type;
  std::string _server_url;
  HTTPClient _client;
};

#endif
