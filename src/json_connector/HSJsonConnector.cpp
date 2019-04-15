// HSJsonConnector is the class that handles json translation and http communications
// HS is short for Home Sensors

#include "HSJsonConnector"

HSJsonConnector::HSJsonConnector(char* sensor_id, char* sensor_type) {
  _sensor_id = sensor_id;
  _sensor_type = sensor_type;
}

bool HSJsonConnector::set_server(char* url) {
  _server_url = url;
}

// TODO: check wifi status and reconnect if needed

bool send(event_type type, char* obj) {
  // The message is a json object in string
  // Deserialize the object and construct the json doc

  // TODO: determine the size
  const size_t capacity = JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);

  data = serialized(obj)

  doc["event_type"] = event_type;
  doc["sensor_id"] = _sensor_id;
  doc["sensor_type"] = _sensor_type;
  doc["data"] = data;

  // Send the message over http
  http.begin(_server_url);
  http.addHeader("Content-Type", "application/json");
  int resCode = http.POST(doc.as<String>());
  Serial.println(resCode, ":", doc.as<String>());
  http.end();
}
