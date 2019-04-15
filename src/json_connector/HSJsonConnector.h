#ifndef _JSON_CONNECTOR_
#define _JSON_CONNECTOR_

#include <ArduinoJson.h>
#include <HTTPClient.h>

enum class event_type {
    DATA
    HANDSHAKE
    MESSAGE
    ERROR
};

class HSJsonConnector {
public:
    void set_sensor(char* sensor_id, char* sensor_type);
    void set_server(char* url)
    // returns a http response code
    // Accepts a serialized json object
    int send(event_type type, char* obj) 

private:
    char* _sensor_id
    char* _sensor_type
    char* _server_url
    HTTPClient _client
};


#endif