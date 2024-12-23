#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Exercise-Solution.h"

TEST(OpenClosed, ConnectionManager_connect_XAQISensor) {
    ConnectionManager connection_manager;
    XAQISensor_Adapter x_sensor;
    EXPECT_TRUE(connection_manager.connect(x_sensor));
}


TEST(OpenClosed, ConnectionManager_connect_JAQISensor) {
    ConnectionManager connection_manager;
    JAQISensor_Adapter j_sensor;
    EXPECT_TRUE(connection_manager.connect(j_sensor));
}


TEST(OpenClosed, CheckAQISensorInterfaceInheritance_X) {
   // XAQISensor_Adapter should inherit from the abstract AQISensor interface
   unique_ptr<AQISensor> x_sensor = make_unique<XAQISensor_Adapter>();
}


TEST(OpenClosed, CheckAQISensorInterfaceInheritance_J) {
   // JAQISensor_Adapter should inherit from the abstract AQISensor interface
   unique_ptr<AQISensor> j_sensor = make_unique<JAQISensor_Adapter>();
}


TEST(OpenClosed, AQISensorDisplay_renderParametersXAQISensor) {
    AQISensorDisplay sensor_display;
    ConnectionManager connection_manager;
    XAQISensor_Adapter x_sensor;
    if (connection_manager.connect(x_sensor)) {
        x_sensor.setRoomName("LivingRoom");
    }
    testing::internal::CaptureStdout();
    sensor_display.renderParameters(x_sensor);
    auto stdout = testing::internal::GetCapturedStdout();
    EXPECT_THAT(stdout, testing::HasSubstr("LivingRoom"));
    EXPECT_THAT(stdout, testing::HasSubstr("AQI"));
}


TEST(OpenClosed, AQISensorDisplay_renderParametersJAQISensor) {
    AQISensorDisplay sensor_display;
    ConnectionManager connection_manager;
    JAQISensor_Adapter j_sensor;
    if (connection_manager.connect(j_sensor)) {
        j_sensor.setRoomName("LivingRoom");
    }
    testing::internal::CaptureStdout();
    sensor_display.renderParameters(j_sensor);
    auto stdout = testing::internal::GetCapturedStdout();
    EXPECT_THAT(stdout, testing::HasSubstr("LivingRoom"));
    EXPECT_THAT(stdout, testing::HasSubstr("AQI"));
    EXPECT_THAT(stdout, testing::HasSubstr("CO2"));
    EXPECT_THAT(stdout, testing::HasSubstr("Temperature"));
    EXPECT_THAT(stdout, testing::HasSubstr("Humidity"));
}