//
// Created by grigory on 28.01.20.
//

#ifndef OPEN62541_SERVER_NODESET_AUTOID_H
#define OPEN62541_SERVER_NODESET_AUTOID_H

#define CONNECTED_TAG_ID    0x6a37
#define NUM_TAGS            3

#define TAG_0               0x6a26
#define TAG_1               0x6a28
#define TAG_2               0x6a57

typedef struct {
    UA_Int32 tagId;
    UA_Boolean isAlive;
    UA_DateTime timestamp;
    struct {
        UA_Double x;
        UA_Double y;
        UA_Double z;
    } coordinates;
    struct {
        UA_Double yaw;
        UA_Double roll;
        UA_Double pitch;
    } rotation;
} TagData;

TagData *tags;

#endif //OPEN62541_SERVER_NODESET_AUTOID_H
