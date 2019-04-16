// HSJsonConnector is the class that handles json translation and http
// communications HS is short for Home Sensors

#include "HSJsonConnector.h"

HSJsonConnector::HSJsonConnector() {}

void HSJsonConnector::setServer(const char* url) {
  // TODO: check param type and copy the values
  _server_url = url;
}

void HSJsonConnector::setSensor(const char* sensor_id,
                                const char* sensor_type) {
  _sensor_id = sensor_id;
  _sensor_type = sensor_type;
}

int send(HSEventType type, const String& obj) {
  // The message is a json object in string
  // Deserialize the object and construct the json doc

  // TODO: determine the size
  const size_t capacity = JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);

  data = serialized(obj);

  switch (type) {
    case HSEvent::DATA:
      doc["event_type"] = "data";
      break;
    case HSEvent::HANDSHAKE:
      doc["event_type"] = "handshake";
      break;
    case HSEvent::MESSAGE:
      doc["event_type"] = "message";
      break;
    case HSEvent::ERROR:
      doc["event_type"] = "error";
      break;
    default:
      break;
  }
  doc["sensor_id"] = _sensor_id;
  doc["sensor_type"] = _sensor_type;
  doc["data"] = data;

  // Send the message over http
  _client.begin(_server_url);
  _client.addHeader("Content-Type", "application/json");

  int resCode = _client.POST(doc.as<String>());
  if (resCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", resCode);
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n",
                  _client.errorToString(resCode).c_str());
  }

  Serial.println(resCode, ":", doc.as<String>());
  _client.end();
  return resCode
}
