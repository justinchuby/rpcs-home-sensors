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

int HSJsonConnector::send(HSEvent type, String obj) {
  // The message is a json object in string
  // Deserialize the object and construct the json doc

  const size_t capacity = JSON_OBJECT_SIZE(MAX_JSON_SIZE);
  DynamicJsonDocument doc(capacity);

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
  doc["data"] = serialized(obj);
#ifdef DEBUG
  serializeJsonPretty(doc, Serial);
  Serial.println("");
#endif

  // Send the message over http
  _client.begin(_server_url);

  _client.addHeader("Content-Type", "application/json");
  String serialized_doc;
  serializeJson(doc, serialized_doc);
  int resCode = _client.POST(serialized_doc);

#ifdef DEBUG
  if (resCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", resCode);
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n",
                  _client.errorToString(resCode).c_str());
  }
#endif
  Serial.println(serialized_doc);
  _client.end();
  return resCode;
}
