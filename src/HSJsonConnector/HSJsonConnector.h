#ifndef _JSON_CONNECTOR_
#define _JSON_CONNECTOR_

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <string>

enum class HSEventType { DATA HANDSHAKE MESSAGE ERROR };

class HSJsonConnector {
 public:
  void setSensor(std::string sensor_id, std::string sensor_type);
  void setServer(std::string url);
  // returns a http response code
  // Accepts a serialized json object
  int send(HSEventType type, std::string obj);

 private:
  std::string _sensor_id;
  std::string _sensor_type;
  std::string _server_url;
  HTTPClient _client;
};

#endif
