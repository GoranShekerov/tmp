/* Hints:
The following UML diagram provides the design of the tested solution. 
Use it to refactor the Exercise.h.

<<OpenClosed-Fix.drawio.png>>

*/

/* Solution
<<The content of OCPExercise-Solution.h>>
*/


#include "OCPDependencies.h"
#include <vector>
#include <iomanip>

using namespace std;


struct AQIParameter{
                 // example
    string name; // "co2_level"
    int value;   // 520
    string unit; // ppm
};


// Abstract interface that will be used to utilize the Open Closed principle
class AQISensor {
public:
    virtual ~AQISensor() = default;
    virtual bool connect(string connectionData) = 0;
    virtual void setRoomName(string roomName) = 0;
    virtual string roomName() const = 0;
    virtual vector<AQIParameter> getData() const = 0; // Strategy Design Pattern 
                                                     //  (dynamic-dispatching)
};


// Adapter Design Pattern
class XAQISensor_Adapter : public AQISensor {
private:
    // composition over inheritance, delegation.
    XAQISensor sensor_;
public:
    bool connect(string connectionData) override {
        return sensor_.connect(connectionData);
    }

    void setRoomName(string roomName) override {
        return sensor_.setRoomName(roomName);
    }

    string roomName() const override {
        return sensor_.roomName();
    }

    vector<AQIParameter> getData() const override {
        vector<AQIParameter> data;

        AQIParameter air_quality_index;
        air_quality_index.name = "AQI";
        air_quality_index.value = sensor_.airQualityIndex();
        air_quality_index.unit = "";

        data.push_back(air_quality_index);
        
        return data;
    }
};


// Adapter Design Pattern
class JAQISensor_Adapter : public AQISensor {
private:
    // composition over inheritance, delegation.
    JAQISensor sensor_;
    AQIJSONParser parser_;
public:

    bool connect(string connectionData) override {
        return sensor_.connect(connectionData);
    }

    void setRoomName(string roomName) override {
        return sensor_.setRoomName(roomName);
    }

    string roomName() const override {
        return sensor_.roomName();
    }

    vector<AQIParameter> getData() const override {
        string jsonData = sensor_.jsonData();
        //cout << jsonData << endl;

        AQIParameter air_quality_index;
        AQIParameter co2level;
        AQIParameter temperature;
        AQIParameter humidity;
        
        air_quality_index.name = "AQI";
        air_quality_index.value = parser_.airQualityIndex(jsonData);
        air_quality_index.unit = "";

        co2level.name = "CO2";
        co2level.value = parser_.co2level(jsonData);
        co2level.unit = "ppm";

        temperature.name = "Temperature";
        temperature.value = parser_.temperature(jsonData);
        temperature.unit = "Celsius";

        humidity.name = "Humidity";
        humidity.value = parser_.humidity(jsonData);
        humidity.unit = "%";

        vector<AQIParameter> data;
        data.push_back(air_quality_index);
        data.push_back(co2level);
        data.push_back(temperature);
        data.push_back(humidity);

        return data;
    }
};


class ConnectionManager {
private:
    QRScanner qr_scanner_;
public:
    // Mocked simplified version of the connection process.
    bool connect(AQISensor& sensor) {
        auto connection_data = qr_scanner_.getData();
        return sensor.connect(connection_data);
    }
    // bool disconnect(XAQISensor&)
    // bool isConnected(XAQISensor&)
};


class AQISensorDisplay {
private: 
     void renderAirQualityIndex(string roomName, int airQualityIndex) const {
        cout << "\n";
        // set AQI text color
        if (airQualityIndex <= 50) {
            cout << "\033[32m"; // set output to green
        } else if (airQualityIndex <= 150) {
            cout << "\033[33m"; // set output to yellow
        } else {
            cout << "\033[31m"; // set output to red
        }
        cout << left << setw(10) << roomName << " : AQI  "
             << airQualityIndex;
        if (airQualityIndex > 150) {
            cout << "  - Open a window!";
        }     
        cout << "\033[0m"; // default output colour
    }


    void renderAdditionalParameter(string paramName, int value, string unit) const {
        cout << "\n" << "             " << left << setw(13) << paramName
             << left << setw(6) << value
             << left << unit;
    }

public:
    void renderParameters(const AQISensor& sensor, bool showDetails = true) const {
        cout << "\n"; 
        // Print the overall Air Quality Index in color. Print the room name in color as well.
        // The additional AQI data will be printed in white
        vector<AQIParameter> sensor_data = sensor.getData();
        auto air_quality_index_parameter = sensor_data.front();
        if (air_quality_index_parameter.name == "AQI") {
            renderAirQualityIndex(sensor.roomName(), air_quality_index_parameter.value);
        } else {
            cout << "Wrong data formatting.\n\n";
            return;
        }
        // Loop through the rest of the additional sensor parameters.
        // Print them in white text color, vertically left indented to AQI.
        if (showDetails) {
            for (auto parameters_iterator = sensor_data.begin() + 1; 
                parameters_iterator != sensor_data.end(); 
                ++parameters_iterator) {
                    renderAdditionalParameter(parameters_iterator->name,
                        parameters_iterator->value,
                        parameters_iterator->unit);
            }        
        }        
    }
};

int main () {
    /* 
     Important clarification!
     
     The AQISensorDisplay and the ConnectionManager are enough to demonstrate 
     the SOLID Principles.

     Managing the sensors: add, remove, keep track.. deserves a separate class, 
     but this functionality is left in main() for simplicity. 
     As well as setting and editing parameters on the sensors, ex. setRoomName(), 
     deserves a separate class/es. Intentionally left in main() for simplicity.

     Make the examples As Simple As Possible.
    */

    AQISensorDisplay aqi_sensor_display;
    ConnectionManager connection_manager;

    vector<unique_ptr<AQISensor>> sensors;

    // Simulating, the user is adding X-type sensor in the LivingRoom:
    unique_ptr<XAQISensor_Adapter> sensor_living_room = make_unique<XAQISensor_Adapter>();
    if (connection_manager.connect(*sensor_living_room)) {
            sensor_living_room->setRoomName("LivingRoom");
            sensors.push_back(move(sensor_living_room));
    } 

    // Simulating, the user is adding X-type sensor in the BadRoom:
    unique_ptr<XAQISensor_Adapter> sensor_bad_room = make_unique<XAQISensor_Adapter>();
    if (connection_manager.connect(*sensor_bad_room)) {
            sensor_bad_room->setRoomName("BadRoom");
            sensors.push_back(move(sensor_bad_room));
    } 
    
    // user adding J-type sensor in the OfficeRoom:
    unique_ptr<JAQISensor_Adapter> sensor_office_room = make_unique<JAQISensor_Adapter>();
    if (connection_manager.connect(*sensor_office_room)) {
            sensor_office_room->setRoomName("OfficeRoom");
            sensors.push_back(move(sensor_office_room));
    } 

    // user adding J-type sensor in the KidsRoom1:
    unique_ptr<JAQISensor_Adapter> sensor_kids_room_1 = make_unique<JAQISensor_Adapter>();
    if (connection_manager.connect(*sensor_kids_room_1)) {
            sensor_kids_room_1->setRoomName("KidsRoom1");
            sensors.push_back(move(sensor_kids_room_1));
    } 

    // Maybe loop and refresh every 10 seconds.
    for (const auto& sensor : sensors) {
        aqi_sensor_display.renderParameters(*sensor);
    }

    cout << "\n\n";
    return 0;
}; 