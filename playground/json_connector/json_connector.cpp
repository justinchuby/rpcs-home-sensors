#include "json_connector.h"

json_connector::json_connector(char* sensor_id, char* sensor_type) {
  _sensor_id = sensor_id;
  _sensor_type = sensor_type;
}

bool json_connector::set_server(char* url) {
  _server_url = url;
}

bool send(event_type type, char* obj) {
  // The message is a json object in string
  // Deserialize the object and construct the json doc

  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);

  data = serialized(obj)

  doc["event_type"] = event_type;
  doc["sensor_id"] = _sensor_id;
  doc["sensor_type"] = _sensor_type;
  doc["data"] = data;

  // Send the message over http
  http.begin(_server_url);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(doc.as<String>());

}
