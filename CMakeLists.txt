cmake_minimum_required(VERSION 2.8)
project(mqtt_pub)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/main.c
    ${CMAKE_SOURCE_DIR}/transport.c

    ${CMAKE_SOURCE_DIR}/MQTTClient-C/src/linux/MQTTLinux.c

    ${CMAKE_SOURCE_DIR}/MQTTClient-C/src/MQTTClient.c

    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTConnectClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTConnectServer.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTDeserializePublish.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTFormat.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTPacket.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTSerializePublish.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTSubscribeClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTSubscribeServer.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTUnsubscribeClient.c
    ${CMAKE_SOURCE_DIR}/MQTTPacket/src/MQTTUnsubscribeServer.c
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/MQTTClient-C/src ${CMAKE_SOURCE_DIR}/MQTTClient-C/src/linux ${CMAKE_SOURCE_DIR}/MQTTPacket/src)

add_executable(mqtt_pub ${SOURCE_FILES})

#add_executable(mqtt_pub
#    ${CMAKE_SOURCE_DIR}/main.c
#    ${CMAKE_SOURCE_DIR}/MQTTClient-C/src/MQTTClient.c
#    ${CMAKE_SOURCE_DIR}/MQTTClient-C/src/linux/MQTTLinux.c
#)

# target_link_libraries(mqtt_pub)