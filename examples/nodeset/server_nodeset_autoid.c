//
// Created by grigory on 21.11.19.
//

#ifdef UA_ENABLE_AMALGAMATION
#include "open62541.h"
#else
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#endif

#include "open62541/namespace_di_generated.h"
#include "open62541/namespace_autoid_generated.h"
#include "open62541/types_autoid_generated_encoding_binary.h"
#include "open62541/types_autoid_generated_handling.h"

#include "paho.mqtt.c/MQTTClient.h"

#include "json/cJSON.h"

#include <signal.h>
#include <stdlib.h>
#include <open62541/types.h>

#define ADDRESS     "localhost"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "tags"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

/*
static int pozyx_mqtt_config(void);
void conn_lost(void *context, char *cause);
int msg_arrvd(void *context, char *topicname, int topicLen, MQTTClient_message *message);
void delivered(void *context, MQTTClient_deliveryToken dt);

volatile MQTTClient_deliveryToken deliveredtoken;*/
UA_UInt16 autoidNamespace;



static UA_NodeId findSingleChildNode(UA_Server *server, UA_QualifiedName targetName, UA_NodeId referenceTypeId, UA_NodeId startingNode){
    UA_NodeId resultNodeId = UA_NODEID_NULL;
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = referenceTypeId;
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = targetName;

    UA_BrowsePath bp;
    UA_BrowsePath_init(&bp);
    bp.startingNode = startingNode;
    bp.relativePath.elementsSize = 1;
    bp.relativePath.elements = &rpe;

    UA_BrowsePathResult bpr = UA_Server_translateBrowsePathToNodeIds(server, &bp);
    if(bpr.statusCode != UA_STATUSCODE_GOOD || bpr.targetsSize < 1){
        return UA_NODEID_NULL;
    }
    if(UA_NodeId_copy(&bpr.targets[0].targetId.nodeId, &resultNodeId) != UA_STATUSCODE_GOOD){
        UA_BrowsePathResult_deleteMembers(&bpr);
        return UA_NODEID_NULL;
    }
    UA_BrowsePathResult_deleteMembers(&bpr);

    return resultNodeId;
}


static void addPozyxTag(UA_Server *server, char *name, UA_String tagId){
    UA_NodeId pozyxTagNodeId;
    UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;

    object_attr.displayName = UA_LOCALIZEDTEXT("en-US", name);

    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(2, 5001),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, name),
                            UA_NODEID_NUMERIC((autoidNamespace = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/AutoID/")), 1012),
                            object_attr, NULL, &pozyxTagNodeId);

    UA_NodeId currentChild;
    UA_Variant val;

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "SoftwareRevision"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String softwareRevision = UA_STRING("v2.0");
    UA_Variant_setScalar(&val, &softwareRevision, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "SerialNumber"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String serialNumber = tagId;
    UA_Variant_setScalar(&val, &serialNumber, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "RevisionCounter"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_Int32 revisionCounter = 1;
    UA_Variant_setScalar(&val, &revisionCounter, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "Model"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_LocalizedText model = UA_LOCALIZEDTEXT("en-US", "Developer Tag");
    UA_Variant_setScalar(&val, &model, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "Manufacturer"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_LocalizedText manufacturer = UA_LOCALIZEDTEXT("en-US", "Pozyx");
    UA_Variant_setScalar(&val, &manufacturer, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "HardwareRevision"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String hardwareRevision = UA_STRING("v1.3");
    UA_Variant_setScalar(&val, &hardwareRevision, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "DeviceRevision"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceRevision = UA_STRING("v1.5");
    UA_Variant_setScalar(&val, &deviceRevision, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "DeviceName"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceName = UA_STRING("Pozyx Developer Tag");
    UA_Variant_setScalar(&val, &deviceName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "DeviceManual"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceManual = UA_STRING("https://www.pozyx.io/shop/product/developer-tag-68");
    UA_Variant_setScalar(&val, &deviceManual, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "DeviceInfo"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceInfo = UA_STRING("no device info");
    UA_Variant_setScalar(&val, &deviceInfo, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "AutoIdModelVersion"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String autoIdModelVersion = UA_STRING("1.00");
    UA_Variant_setScalar(&val, &autoIdModelVersion, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "LengthUnit"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_EUInformation lengthUnit;
    UA_EUInformation_init(&lengthUnit);
    lengthUnit.namespaceUri = UA_STRING("https://opcfoundation.org/UA/units/un/cefact");
    lengthUnit.unitId = 5066068;
    lengthUnit.displayName = UA_LOCALIZEDTEXT("en-US", "mm");
    lengthUnit.description = UA_LOCALIZEDTEXT("en-US", "millimetre");
    UA_Variant_setScalar(&val, &lengthUnit, &UA_TYPES[UA_TYPES_EUINFORMATION]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "RotationalUnit"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_EUInformation rotationalUnit;
    UA_EUInformation_init(&rotationalUnit);
    rotationalUnit.namespaceUri = UA_STRING("https://opcfoundation.org/UA/units/un/cefact");
    rotationalUnit.unitId = 17476;
    rotationalUnit.displayName = UA_LOCALIZEDTEXT("en-US", "°");
    rotationalUnit.description = UA_LOCALIZEDTEXT("en-US", "degree [unit of angle]");
    UA_Variant_setScalar(&val, &rotationalUnit, &UA_TYPES[UA_TYPES_EUINFORMATION]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "GeographicalUnit"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_EUInformation geographicalUnit;
    UA_EUInformation_init(&geographicalUnit);
    geographicalUnit.namespaceUri = UA_STRING("https://opcfoundation.org/UA/units/un/cefact");
    geographicalUnit.unitId = 17476;
    geographicalUnit.displayName = UA_LOCALIZEDTEXT("en-US", "°");
    geographicalUnit.description = UA_LOCALIZEDTEXT("en-US", "degree [unit of angle]");
    UA_Variant_setScalar(&val, &geographicalUnit, &UA_TYPES[UA_TYPES_EUINFORMATION]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "SpeedUnit"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_EUInformation speedUnit;
    UA_EUInformation_init(&speedUnit);
    speedUnit.namespaceUri = UA_STRING("https://opcfoundation.org/UA/units/un/cefact");
    speedUnit.unitId = 5067859;
    speedUnit.displayName = UA_LOCALIZEDTEXT("en-US", "m/s");
    speedUnit.description = UA_LOCALIZEDTEXT("en-US", "metre per second");
    UA_Variant_setScalar(&val, &speedUnit, &UA_TYPES[UA_TYPES_EUINFORMATION]);
    UA_Server_writeValue(server, currentChild, val);

}
static UA_StatusCode getSupportedLocationTypes(UA_Server *server,
                                               const UA_NodeId *sessionId, void *sessionHandle,
                                               const UA_NodeId *methodId, void *methodContext,
                                               const UA_NodeId *objectId, void *objectContext,
                                               size_t inputSize, const UA_Variant *input,
                                               size_t outputSize, UA_Variant *output) {
    UA_LocationTypeEnumeration type;
    UA_LocationTypeEnumeration_init(&type);
    type = UA_LOCATIONTYPEENUMERATION_LOCAL;
    /*UA_ByteString *buf = UA_ByteString_new();
    size_t msgSize = UA_LocationTypeEnumeration_calcSizeBinary(&type);
    printf("\n%zu\n", msgSize);
    UA_ByteString_allocBuffer(buf, msgSize);
    memset(buf->data, 0, msgSize);
    UA_Byte *bufPos = buf->data;
    const UA_Byte *bufEnd = &buf->data[buf->length];
    UA_LocationTypeEnumeration_encodeBinary(&type, &bufPos, bufEnd);*/

    /*UA_ExtensionObject eoInput;
    eoInput.content.encoded.body = *buf;
    eoInput.content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, 3009);
    eoInput.encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;*/
    UA_Variant_setArrayCopy(output, &type, 1, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_LOCATIONTYPEENUMERATION]);


    return UA_STATUSCODE_GOOD;
}



static UA_StatusCode
scanPozyxTag(UA_Server *server,
              const UA_NodeId *sessionId, void *sessionHandle,
              const UA_NodeId *methodId, void *methodContext,
              const UA_NodeId *objectId, void *objectContext,
              size_t inputSize, const UA_Variant *input,
              size_t outputSize, UA_Variant *output) {

    printf("rfidScanStarted\n");
    UA_ExtensionObject *eo = (UA_ExtensionObject *) input[0].data;
    UA_ScanSettings settings;
    UA_ScanSettings_init(&settings);
    size_t scanSettingsOffset = 0;
    UA_ByteString byteString = eo->content.encoded.body;

    printf("%d, %d\n", eo->content.encoded.typeId.namespaceIndex, eo->content.encoded.typeId.identifier.numeric);
    printf("%s", input[0].type->typeName);
    UA_ScanSettings_decodeBinary(&byteString, &scanSettingsOffset, &settings);

    printf("hasLocationType: %d\n", settings.hasLocationType);
    printf("duration: %f\n", settings.duration);
    printf("cycles: %d\n", settings.cycles);
    printf("dataAvailable: %d\n", settings.dataAvailable);
    printf("locationType: %d\n", settings.hasLocationType);

    /*
    printf("%zu\n", byteString.length);
    for(size_t i=0; i<byteString.length; i++){
        printf("%x", byteString.data[i]);
    }
    printf("\n");

    if(eo->encoding == UA_EXTENSIONOBJECT_ENCODED_BYTESTRING) {

        // workaround for optional field issue if optional field not
        // included append empty bits to match expected decode size
        if (eo->content.encoded.body.length <= 17) {
            UA_Byte *newBufferReceive = (UA_Byte *) UA_calloc(21, sizeof(UA_Byte));
            memcpy(newBufferReceive, eo->content.encoded.body.data, 17);
            eo->content.encoded.body.data = newBufferReceive;
            eo->content.encoded.body.length = 21;
            byteString = eo->content.encoded.body;
            byteString.length = 21;
        }
        UA_ScanSettings_decodeBinary(&byteString, &scanSettingsOffset, &settings);
    }*/
    UA_RtlsLocationResult res;
    UA_RtlsLocationResult_init(&res);

    res.hasLocation = true;
    res.codeType = UA_STRING("RAW:BYTES");
    res.scanData.switchField = 1;
    res.scanData.byteString = UA_BYTESTRING("AD-CD-03-04");
    res.timestamp = UA_DateTime_now();
    res.location.local.x = 1.0;
    res.location.local.y = 2.0;
    res.location.local.z = 4.0;
    res.location.local.timestamp = UA_DateTime_now();
    res.speed = 8.0;
    res.heading = 16.0;
    res.rotation.yaw = 32.0;
    res.rotation.pitch = 64.0;
    res.rotation.roll = 128.0;
    res.receiveTime = UA_DateTime_now();


    UA_ByteString *buf = UA_ByteString_new();
    size_t msgSize = UA_RtlsLocationResult_calcSizeBinary(&res);
    UA_ByteString_allocBuffer(buf, msgSize);
    const UA_Byte *bufEnd = &buf->data[buf->length];
    UA_RtlsLocationResult_encodeBinary(&res, &buf->data, bufEnd);


    printf("%zu\n", buf->length);
    for(size_t j=0; j<buf->length; j++){
        printf("%02X", buf->data[j]);
    }

    printf("\n");

    UA_ExtensionObject *e = UA_ExtensionObject_new();
    e->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
    e->content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULT].binaryEncodingId);
    e->content.encoded.body = *buf;


    UA_AutoIdOperationStatusEnumeration st;
    UA_AutoIdOperationStatusEnumeration_init(&st);
    st = UA_AUTOIDOPERATIONSTATUSENUMERATION_READ_ERROR;

    UA_Variant_setScalar(output, e, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    //UA_Variant_setArrayCopy(output, (UA_Variant*)e, 1, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULT]);
    UA_Variant_setScalarCopy(output+1, &st, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_AUTOIDOPERATIONSTATUSENUMERATION]);

    return UA_STATUSCODE_GOOD;
}

/*
void conn_lost(void *context, char *cause){
    printf("\nConnection lost. Cause: %s\n", cause);
}

int msg_arrvd(void *context, char *topicname, int topicLen, MQTTClient_message *message){
    char* payloadptr;
    payloadptr = (char*) message->payload;
    cJSON *json = cJSON_Parse(payloadptr);
    printf("%s\n", cJSON_Print(json));
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicname);
    return 1;
}

void delivered(void *context, MQTTClient_deliveryToken dt){
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int pozyx_mqtt_config(void){
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, conn_lost, msg_arrvd, delivered);

    if((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS){
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Failed to connect to the MQTT Broker, return code %d", rc);
        return rc;
    }
    printf("Subscribing to topic %s for client %s using QoS%d\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
    return rc;
}*/

UA_Boolean serverRunning = true;

static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    serverRunning = false;
}

int main(int argc, char** argv) {

    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    /* create nodes from nodeset */
    UA_StatusCode retval = namespace_di_generated(server);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Adding the DI namespace failed. Please check previous error output.");
        UA_Server_delete(server);
        return EXIT_FAILURE;
    }


    retval |= namespace_autoid_generated(server);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Adding the AutoID namespace failed. Please check previous error output.");
        UA_Server_delete(server);
        return EXIT_FAILURE;
    }



    addPozyxTag(server, "Pozyx Tag 0x6a26", UA_STRING("0x6a26"));
    addPozyxTag(server, "Pozyx Tag 0x6a37", UA_STRING("0x6a37"));
    addPozyxTag(server, "Pozyx Tag 0x6a28", UA_STRING("0x6a28"));
    addPozyxTag(server, "Pozyx Tag 0x6a57", UA_STRING("0x6a57"));

    //pozyx_mqtt_config();

    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7055), scanPozyxTag);
    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7058), getSupportedLocationTypes);

    /*
    UA_RtlsLocationResult r;
    UA_ByteString *buf = UA_ByteString_new();
    size_t msgSize = UA_RtlsLocationResult_calcSizeBinary(&r);
    UA_ByteString_allocBuffer(buf, msgSize);
    memset(buf->data, 0, msgSize);

    UA_Byte *bufSettingsPos = buf->data;
    const UA_Byte *bufEnd = &buf->data[buf->length];

    UA_RtlsLocationResult_encodeBinary(&r, &bufSettingsPos, bufEnd);
*/
/*
    UA_ScanSettings *testSettings = (UA_ScanSettings *) UA_calloc(1, sizeof(UA_ScanSettings));
    testSettings->duration = 1.7976931348623157E+308;
    testSettings->cycles = 2147483647;
    testSettings->dataAvailable = UA_TRUE;
    testSettings->locationType = UA_LOCATIONTYPEENUMERATION_LOCAL;

    UA_ByteString *buf = UA_ByteString_new();
    size_t msgSize = UA_ScanSettings_calcSizeBinary(testSettings);
    printf("%zu\n\n", msgSize);
    UA_ByteString_allocBuffer(buf, msgSize);
    memset(buf->data, 0, msgSize);
    UA_Byte *bufSettingsPos = buf->data;
    const UA_Byte *bufEnd = &buf->data[buf->length];
    UA_ScanSettings_encodeBinary(testSettings, &bufSettingsPos, bufEnd);

    for(size_t i = 0; i < buf->length; i++){
        printf("%02X", buf->data[i]);
    }
    printf("\n %zu \n", buf->length);

    UA_ExtensionObject eoInput;
    eoInput.content.encoded.body = *buf;
    eoInput.content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, 3010);
    eoInput.encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
    UA_Variant params;
    UA_Variant_setScalar(&params, &eoInput, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);

    UA_CallMethodRequest callMethodRequest;
    UA_CallMethodRequest_init(&callMethodRequest);
    callMethodRequest.inputArgumentsSize = 1;

    UA_NodeId n = findSingleChildNode(server, UA_QUALIFIEDNAME(1, "Pozyx Tag 0x6a26"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_NODEID_NUMERIC(2, 5001));
    printf("NodeId: %d, %d", n.namespaceIndex, n.identifier.numeric);

    callMethodRequest.objectId = n;
    callMethodRequest.methodId = UA_NODEID_NUMERIC(autoidNamespace, 7055);
    callMethodRequest.inputArguments = &params;
    UA_Server_call(server, &callMethodRequest);
*/
    retval = UA_Server_run(server, &serverRunning);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
