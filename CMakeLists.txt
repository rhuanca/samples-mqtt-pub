cmake_minimum_required(VERSION 2.8)
project(mqtt_pub)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/transport.c

    ${CMAKE_SOURCE_DIR}/MQTTClient-C/MQTTLinux.c
    ${CMAKE_SOURCE_DIR}/MQTTClient-C/MQTTClient.c

    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTConnectClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTConnectServer.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTDeserializePublish.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTFormat.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTPacket.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTSerializePublish.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTSubscribeClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTSubscribeServer.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTUnsubscribeClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/MQTTUnsubscribeServer.c
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/MQTTClient-C ${CMAKE_SOURCE_DIR}/MQTTClient-C ${CMAKE_SOURCE_DIR}/MQTTPacket)

add_executable(mqtt_pub ${SOURCE_FILES})
