#ifndef _JSON_CONNECTOR_
#define _JSON_CONNECTOR_

#include <ArduinoJson.h>
#include <HTTPClient.h>

#define MAX_JSON_SIZE 100

enum class HSEvent { DATA, HANDSHAKE, MESSAGE, ERROR };

class HSJsonConnector {
 public:
  HSJsonConnector();
  void setSensor(const char* sensor_id, const char* sensor_type);

  void setServer(const char* url);
  // returns a http response code
  // Accepts a serialized json object
  int send(HSEvent type, String obj);

 private:
  String _sensor_id;
  String _sensor_type;
  String _server_url;
  HTTPClient _client;
};

#endif
