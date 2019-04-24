#ifndef _JSON_CONNECTOR_
#define _JSON_CONNECTOR_

#include <ArduinoJson.h>
#include <HTTPClient.h>

#define MAX_JSON_SIZE 100
#define DEBUG

enum class HSEvent { DATA, HANDSHAKE, MESSAGE, ERROR };

class HSJsonConnector {
 public:
  HSJsonConnector();
  void setSensor(const char* sensor_id, const char* sensor_type);

  void setServer(const char* url);
  void setServerAuth(const char* auth);
  // returns a http response code
  // Accepts a serialized json object
  int send(HSEvent type, String obj);

 private:
  const char* _sensor_id;
  const char* _sensor_type;
  const char* _server_url;
  const char* _server_auth;
  bool _require_auth = false;
  HTTPClient _client;
};

#endif
