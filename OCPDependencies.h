#pragma once

#include <string>
#include <ctime>
#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//*** 3rd party XAQISensor API

// Provided API from the XAQISensor manufacturer.
class XAQISensor {

public:

    XAQISensor();

    // For demo purposes. Oversimplified connection over wifi
    bool connect(string connection_data);

    void setRoomName(string room_name);
    string roomName() const;

    int airQualityIndex() const;


//To focus on the SOLID Principles well keep everything in this header file.
//In real life all the implementation/definitions provided by 3rd parties is 
//almost exclusively closed in a library.
private:
    string room_name_{};
    bool connected_{false};
};


XAQISensor::XAQISensor(){
    // init pseudo-random number generator
    srand(time(NULL));
}

// just for demo purposes
bool XAQISensor::connect(string connection_data) {
    // the connection data can be taken by QR scan
    auto password = connection_data.substr(connection_data.length() - 3);
    if (password != "123") {
        cout << "connection refused: " << connection_data << endl;
        return false;
    } 
    bool is_sensor_reachable = true;
    if (is_sensor_reachable) {
        connected_ = true;
    }
    return true;
}


void XAQISensor::setRoomName(string room_name) {
    if (connected_ == false) {
        return;
    }
    room_name_ = room_name;
}

string XAQISensor::roomName() const {
    return room_name_;
}

int XAQISensor::airQualityIndex() const {
    if (connected_ == false) {
        return -1;
    }
    int air_quality_index_ = 10 + rand() % 200; // pseudo-random number between 10 and 209
    return air_quality_index_;
}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//*** 3rd party JAQISensor API

// Provided API from the JAQISensor manufacturer.
class JAQISensor {

public:

    JAQISensor();

    // For demo purposes. Oversimplified connection over wifi
    bool connect(string connection_data);

    void setRoomName(string room_name);
    string roomName() const;

    string jsonData() const;

//To focus on the SOLID Principles well keep everything in this header file.
//In real life all the implementation/definitions provided by 3rd parties is 
//almost exclusively closed in a library.
private:
    string room_name_{};
    bool connected_{false};
};


JAQISensor::JAQISensor(){
    // init pseudo-random number generator
    srand(time(NULL));
}

// just for demo purposes
bool JAQISensor::connect(string connection_data) {
    // the connection data can be taken by QR scan
    auto password = connection_data.substr(connection_data.length() - 3);
    if (password != "123") {
        cout << "connection refused: " << connection_data << endl;
        return false;
    } 
    bool is_sensor_reachable = true;
    if (is_sensor_reachable) {
        connected_ = true;
    }
    return true;
}


void JAQISensor::setRoomName(string room_name) {
    if (connected_ == false) {
        return;
    }
    room_name_ = room_name;
}

string JAQISensor::roomName() const {
    return room_name_;
}

string JAQISensor::jsonData() const {
    if (connected_ == false) {
        return {};
    }
    int air_quality_index_ = 10 + rand() % 100; // pseudo random number between 10 and 109
    int co2_level_ = 400 + rand() % 2000; // pseudo random number between 400 and 2399
    int temperature_ = 16 + rand() % 10;
    int humidity_ = 20 + rand() % 40;
    return
        "{\n"
        "\"SensorType\" : \"BrandJAirQualitySensor\",\n"
        "\"AirQualityIndex\" : " + to_string(air_quality_index_) + ",\n"
        "\"CO2Level\" : " + to_string(co2_level_) + ",\n"
        "\"Temperature\" : " + to_string(temperature_) + ",\n"
        "\"Humidity\" : " + to_string(humidity_) + "\n"
        "}"
        ;
}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//*** AQIJsonSensorParser
class AQIJSONParser {
public: 
    int airQualityIndex(const string& jsonData) const;
    int co2level(const string& jsonData) const;
    int temperature(const string& jsonData) const;
    int humidity(const string& jsonData) const;

private:
    int getValueFromKey(string key, const string& jsonData) const;
};




/*
Quick hack for demo purposes, not important to understanding SOLID
Just to parse the example json string, like
{
"SensorType" : "BrandJAirQualitySensor",
"AirQualityIndex" : 65,
"CO2Level" : 2180,
"Temperature" : 21,
"Humidity" : 23
} 
*/
int AQIJSONParser::getValueFromKey(string key, const string& jsonData) const {
    // This process is quite complex and usually available in a 3rd party library.
    string start_string{key};
    string json_syntax_connector{"\" : \""};
    auto start_index = jsonData.find(start_string) + start_string.length() +
                       json_syntax_connector.length();
    auto end_index = jsonData.find(",", start_index);
    string return_value = jsonData.substr(start_index - 1, end_index - start_index + 1);
    return stoi(return_value);
}


int AQIJSONParser::airQualityIndex(const string& jsonData) const {
    return getValueFromKey("AirQualityIndex", jsonData);
}

int AQIJSONParser::co2level(const string& jsonData) const {
    return getValueFromKey("CO2Level", jsonData);
}

int AQIJSONParser::temperature(const string& jsonData) const {
    return getValueFromKey("Temperature", jsonData);
}

int AQIJSONParser::humidity(const string& jsonData) const {
    return getValueFromKey("Humidity", jsonData);
}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
// QRScanner mock
class QRScanner {
public:
    QRScanner();
    string getData() const;
};

QRScanner::QRScanner() {
    // init pseudo-random number generator
    srand(time(NULL));
}

string QRScanner::getData() const {
    // Mocking the return of data by scanning the QR Code on the label on the Sensor.
    int random_two_mac_address_digits = rand() % 100; // pseudo-random number between 0 and 99
    return "00:11:22:33:44:" + to_string(random_two_mac_address_digits) + "pwd123";
}