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
#include "open62541/autoid_nodeids.h"

#include "server_nodeset_autoid.h"

#include "nodeset/mqtt-c/include/mqtt.h"
#include "nodeset/mqtt-c/include/posix_sockets.h"

#include "nodeset/json/cJSON.h"

#include <signal.h>
#include <stdlib.h>
#include <open62541/types.h>

#define ADDRESS     "localhost"
#define PORT        "1883"
#define TOPIC       "tags"

void publish_callback(void** unused, struct mqtt_response_publish *published);
void client_refresher(UA_Server *server, void* client);
void updateTagsArray(const char* json);
UA_UInt16 getTagArrayIndex(UA_Int32 tagId);
UA_Boolean tagInArray(UA_Int32 tagId);
static size_t getNumberAliveTags(void);

UA_UInt16 tagsPointer = 0;

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


static UA_NodeId addPozyx(UA_Server *server){
    UA_NodeId pozyxTagNodeId;
    UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;

    object_attr.displayName = UA_LOCALIZEDTEXT("en-US", "Pozyx system");

    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(2, 5001),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Pozyx system"),
                            UA_NODEID_NUMERIC((autoidNamespace = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/AutoID/")), 1012),
                            object_attr, NULL, &pozyxTagNodeId);

    UA_NodeId currentChild;
    UA_Variant val;

    /*
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
     */

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "Model"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_LocalizedText model = UA_LOCALIZEDTEXT("en-US", "Creator kit");
    UA_Variant_setScalar(&val, &model, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "Manufacturer"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_LocalizedText manufacturer = UA_LOCALIZEDTEXT("en-US", "Pozyx");
    UA_Variant_setScalar(&val, &manufacturer, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeValue(server, currentChild, val);

    /*
    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "HardwareRevision"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String hardwareRevision = UA_STRING("v1.3");
    UA_Variant_setScalar(&val, &hardwareRevision, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "DeviceRevision"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceRevision = UA_STRING("v1.5");
    UA_Variant_setScalar(&val, &deviceRevision, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);
     */

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(3, "DeviceName"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceName = UA_STRING("Pozyx Creator kit");
    UA_Variant_setScalar(&val, &deviceName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, currentChild, val);

    currentChild = findSingleChildNode(server, UA_QUALIFIEDNAME(2, "DeviceManual"), UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), pozyxTagNodeId);
    UA_String deviceManual = UA_STRING("https://www.pozyx.io/shop/product/creator-kit-65");
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

    return pozyxTagNodeId;
}

static UA_StatusCode getSupportedLocationTypes(UA_Server *server,
                                               const UA_NodeId *sessionId, void *sessionHandle,
                                               const UA_NodeId *methodId, void *methodContext,
                                               const UA_NodeId *objectId, void *objectContext,
                                               size_t inputSize, const UA_Variant *input,
                                               size_t outputSize, UA_Variant *output) {

    /* without ExtensionObject, because enum is an UInt32 internally */
    UA_LocationTypeEnumeration type;
    UA_LocationTypeEnumeration_init(&type);
    type = UA_LOCATIONTYPEENUMERATION_LOCAL;

    UA_Variant_setArrayCopy(output, &type, 1, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_LOCATIONTYPEENUMERATION]);

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
scanPozyxWithoutOpt(UA_Server *server,
          const UA_NodeId *sessionId, void *sessionHandle,
          const UA_NodeId *methodId, void *methodContext,
          const UA_NodeId *objectId, void *objectContext,
          size_t inputSize, const UA_Variant *input,
          size_t outputSize, UA_Variant *output) {

    UA_ExtensionObject *settingsWithoutOptEo = (UA_ExtensionObject *) input[0].data;
    UA_ScanSettingsWithoutOpt settingsWithoutOpt;
    memset(&settingsWithoutOpt, 0, sizeof(UA_ScanSettingsWithoutOpt));
    size_t settingsWithoutOptOffset = 0;
    UA_ByteString byteString = settingsWithoutOptEo->content.encoded.body;
    UA_ScanSettingsWithoutOpt_decodeBinary(&byteString, &settingsWithoutOptOffset, &settingsWithoutOpt);

    UA_ScanSettings settings;
    UA_ScanSettings_init(&settings);
    settings.hasLocationType = true;
    settings.duration = settingsWithoutOpt.duration;
    settings.cycles = settingsWithoutOpt.cycles;
    settings.dataAvailable = settingsWithoutOpt.dataAvailable;
    settings.locationType = settingsWithoutOpt.locationType;

    UA_ByteString *settingsBuf = UA_ByteString_new();
    size_t settingsMsgSize = UA_ScanSettings_calcSizeBinary(&settings);
    UA_ByteString_allocBuffer(settingsBuf, settingsMsgSize);
    memset(settingsBuf->data, 0, settingsMsgSize);
    UA_Byte *settingsBufPos = settingsBuf->data;
    const UA_Byte *settingsBufEnd = &settingsBuf->data[settingsBuf->length];
    UA_ScanSettings_encodeBinary(&settings, &settingsBufPos, settingsBufEnd);

    UA_ExtensionObject settingsEo;
    settingsEo.encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
    settingsEo.content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, UA_TYPES_AUTOID[UA_TYPES_AUTOID_SCANSETTINGS].binaryEncodingId);
    settingsEo.content.encoded.body = *settingsBuf;
    UA_Variant settingsInput;
    UA_Variant_setScalar(&settingsInput, &settingsEo, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);

    UA_CallMethodRequest callMethodRequest;
    UA_CallMethodRequest_init(&callMethodRequest);
    callMethodRequest.inputArgumentsSize = 1;
    callMethodRequest.objectId = *objectId;
    callMethodRequest.methodId = UA_NODEID_NUMERIC(autoidNamespace, UA_AUTOIDID_RTLSDEVICETYPE_SCAN);
    callMethodRequest.inputArguments = &settingsInput;
    UA_CallMethodResult methodResult;
    methodResult = UA_Server_call(server, &callMethodRequest);

    if(methodResult.statusCode) return methodResult.statusCode;

    UA_AutoIdOperationStatusEnumeration *status = (UA_AutoIdOperationStatusEnumeration *) methodResult.outputArguments[1].data;
    if(*status != UA_AUTOIDOPERATIONSTATUSENUMERATION_SUCCESS){
        output[1] = methodResult.outputArguments[1];
        return UA_STATUSCODE_GOOD;
    }
    UA_Variant resultsVariant = methodResult.outputArguments[0];
    size_t resultNum = resultsVariant.arrayLength;
    size_t resultsWithoutOptOffset = 0;
    UA_ExtensionObject *resultsEo = (UA_ExtensionObject *) resultsVariant.data;
    UA_ExtensionObject *resultsWithoutOpt = UA_Array_new(resultNum, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    UA_RtlsLocationResult result;
    UA_RtlsLocationResultWithoutOpt resultWithoutOpt;
    for(size_t i = 0; i < resultNum; i++){
        UA_ByteString resultBuf = resultsEo[i].content.encoded.body;
        UA_RtlsLocationResult_decodeBinary(&resultBuf, &resultsWithoutOptOffset, &result);
        resultsWithoutOptOffset = 0;
        resultWithoutOpt.codeType = result.codeType;
        resultWithoutOpt.scanData = result.scanData.string;
        resultWithoutOpt.timestamp = result.timestamp;
        resultWithoutOpt.location = result.location.local;
        resultWithoutOpt.speed = result.speed;
        resultWithoutOpt.heading = result.heading;
        resultWithoutOpt.rotation = result.rotation;
        resultWithoutOpt.receiveTime = result.receiveTime;

        size_t resultWithoutOptMsgSize = UA_RtlsLocationResultWithoutOpt_calcSizeBinary(&resultWithoutOpt);
        UA_ByteString *resultWithoutOptBuf = &resultsWithoutOpt[i].content.encoded.body;
        UA_ByteString_allocBuffer(resultWithoutOptBuf, resultWithoutOptMsgSize);
        memset(resultWithoutOptBuf->data, 0, resultWithoutOptMsgSize);
        UA_Byte *resultWithoutOptPos = resultWithoutOptBuf->data;
        const UA_Byte *resultWithoutOptEnd = &resultWithoutOptBuf->data[resultWithoutOptBuf->length];

        resultsWithoutOpt[i].encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
        resultsWithoutOpt[i].content.encoded.typeId = UA_NODEID_NUMERIC(3, UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULTWITHOUTOPT].binaryEncodingId);
        UA_RtlsLocationResultWithoutOpt_encodeBinary(&resultWithoutOpt, &resultWithoutOptPos, resultWithoutOptEnd);
    }
    UA_Variant_setArrayCopy(output, resultsWithoutOpt, resultNum, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    UA_Variant_setScalarCopy(output+1, status, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_AUTOIDOPERATIONSTATUSENUMERATION]);

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
scanPozyx(UA_Server *server,
          const UA_NodeId *sessionId, void *sessionHandle,
          const UA_NodeId *methodId, void *methodContext,
          const UA_NodeId *objectId, void *objectContext,
          size_t inputSize, const UA_Variant *input,
          size_t outputSize, UA_Variant *output) {

    UA_ExtensionObject *eo = (UA_ExtensionObject *) input[0].data;
    UA_ScanSettings settings;
    UA_ScanSettings_init(&settings);
    size_t scanSettingsOffset = 0;
    UA_ByteString byteString = eo->content.encoded.body;
    UA_ScanSettings_decodeBinary(&byteString, &scanSettingsOffset, &settings);

    /*
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

    size_t numAliveTags = getNumberAliveTags();
    if(numAliveTags == 0) {
        UA_AutoIdOperationStatusEnumeration st = UA_AUTOIDOPERATIONSTATUSENUMERATION_NO_IDENTIFIER;
        UA_Variant_setScalarCopy(output + 1, &st,
                                 &UA_TYPES_AUTOID[UA_TYPES_AUTOID_AUTOIDOPERATIONSTATUSENUMERATION]);
        return UA_STATUSCODE_GOOD;
    }
    UA_ExtensionObject* eoTags = UA_Array_new(numAliveTags, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    for(size_t i = 0, j = 0; i < tagsPointer; i++) {
        if(tags[i].isAlive){
            UA_RtlsLocationResult res;
            UA_RtlsLocationResult_init(&res);

            res.hasLocation = true;
            res.codeType = UA_STRING("CUSTOM:TAGID");
            char sTagId[6];
            sprintf(sTagId, "%d", tags[i].tagId);

            res.scanData.switchField = 2;
            res.scanData.string = UA_STRING(sTagId);
            res.timestamp = UA_DateTime_now();
            res.location.switchField = 2;
            res.location.local.x = tags[i].coordinates.x;
            res.location.local.y = tags[i].coordinates.y;
            res.location.local.z = tags[i].coordinates.z;
            res.location.local.timestamp = tags[i].timestamp;
            res.rotation.yaw = tags[i].rotation.yaw;
            res.rotation.pitch = tags[i].rotation.pitch;
            res.rotation.roll = tags[i].rotation.roll;

            UA_ByteString *buf = UA_ByteString_new();
            size_t msgSize = UA_RtlsLocationResult_calcSizeBinary(&res);
            UA_ByteString_allocBuffer(buf, msgSize);
            memset(buf->data, 0, msgSize);
            UA_Byte *bufPos = buf->data;
            const UA_Byte *bufEnd = &buf->data[buf->length];
            UA_RtlsLocationResult_encodeBinary(&res, &bufPos, bufEnd);

            eoTags[j].encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
            eoTags[j].content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULT].binaryEncodingId);
            eoTags[j].content.encoded.body = *buf;
            j++;
        }
    }


    UA_AutoIdOperationStatusEnumeration st = UA_AUTOIDOPERATIONSTATUSENUMERATION_SUCCESS;

    UA_Variant_setArrayCopy(output, eoTags, numAliveTags, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    //UA_Variant_setArrayCopy(output, (UA_Variant*)e, 1, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULT]);
    UA_Variant_setScalarCopy(output+1, &st, &UA_TYPES_AUTOID[UA_TYPES_AUTOID_AUTOIDOPERATIONSTATUSENUMERATION]);

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
getLocationPozyx(UA_Server *server,
          const UA_NodeId *sessionId, void *sessionHandle,
          const UA_NodeId *methodId, void *methodContext,
          const UA_NodeId *objectId, void *objectContext,
          size_t inputSize, const UA_Variant *input,
          size_t outputSize, UA_Variant *output) {

    size_t scOffset = 0;
    UA_ExtensionObject *scanDataEo = (UA_ExtensionObject *) input[0].data;
    UA_ScanData scanData;
    UA_ScanData_init(&scanData);
    UA_ByteString sdByteString = scanDataEo->content.encoded.body;
    UA_ScanData_decodeBinary(&sdByteString, &scOffset, &scanData);

    UA_LocationTypeEnumeration locationType = *(UA_LocationTypeEnumeration *) input[1].data;

    UA_String codeType = *(UA_String *) input[2].data;

    if(strcmp((const char*) codeType.data, "CUSTOM:TAGID") || scanData.switchField != 2 || locationType != UA_LOCATIONTYPEENUMERATION_LOCAL)
        return UA_STATUSCODE_BADINVALIDARGUMENT;

    char* ptr;
    UA_Int32 tagId = (UA_Int32) strtol((const char *) scanData.string.data, &ptr, 10);
    if(tagId == 0) return UA_STATUSCODE_BADINVALIDARGUMENT;

    UA_UInt16 i = getTagArrayIndex(tagId);
    if(i == tagsPointer) return UA_STATUSCODE_BADINVALIDSTATE;
    if(!tags[i].isAlive) return UA_STATUSCODE_BADINVALIDSTATE;

    UA_RtlsLocationResult res;
    UA_RtlsLocationResult_init(&res);

    res.hasLocation = true;
    res.codeType = UA_STRING("CUSTOM:TAGID");

    res.scanData.switchField = 2;
    res.scanData.string = scanData.string;
    res.timestamp = UA_DateTime_now();
    res.location.switchField = 2;
    res.location.local.x = tags[i].coordinates.x;
    res.location.local.y = tags[i].coordinates.y;
    res.location.local.z = tags[i].coordinates.z;
    res.location.local.timestamp = tags[i].timestamp;
    res.rotation.yaw = tags[i].rotation.yaw;
    res.rotation.pitch = tags[i].rotation.pitch;
    res.rotation.roll = tags[i].rotation.roll;

    UA_ByteString *buf = UA_ByteString_new();
    size_t msgSize = UA_RtlsLocationResult_calcSizeBinary(&res);
    UA_ByteString_allocBuffer(buf, msgSize);
    memset(buf->data, 0, msgSize);
    UA_Byte *bufPos = buf->data;
    const UA_Byte *bufEnd = &buf->data[buf->length];
    UA_RtlsLocationResult_encodeBinary(&res, &bufPos, bufEnd);

    UA_ExtensionObject *resEo = UA_ExtensionObject_new();
    UA_ExtensionObject_init(resEo);
    resEo->encoding = UA_EXTENSIONOBJECT_ENCODED_BYTESTRING;
    resEo->content.encoded.typeId = UA_NODEID_NUMERIC(autoidNamespace, UA_TYPES_AUTOID[UA_TYPES_AUTOID_RTLSLOCATIONRESULT].binaryEncodingId);
    resEo->content.encoded.body = *buf;

    UA_Variant_setScalarCopy(output, &resEo, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);

    return UA_STATUSCODE_GOOD;
}

static size_t getNumberAliveTags(void) {
    size_t num = 0;
    for(int i = 0; i < tagsPointer; i++){
        if(tags[i].isAlive) num++;
    }
    return num;
}


void publish_callback(void** unused, struct mqtt_response_publish *published)
{
    //cJSON *jsonTag = cJSON_Parse((const char*) published->application_message);
    //printf("%s\n", cJSON_Print(jsonTag));
    updateTagsArray((const char*) published->application_message);
    //cJSON_Delete(jsonTag);
}

void updateTagsArray(const char* json){
    cJSON *jsonTag = cJSON_Parse(json);
    cJSON *tagData = cJSON_GetArrayItem(jsonTag, 0);
    cJSON *success = cJSON_GetObjectItemCaseSensitive(tagData, "success");
    cJSON *alive = cJSON_GetObjectItemCaseSensitive(tagData, "alive");

    cJSON *tagId = cJSON_GetObjectItemCaseSensitive(tagData, "tagId");
    if(cJSON_IsString(tagId)) {
        UA_Int32 tagIdNum = atoi(tagId->valuestring);
        if (tagIdNum != CONNECTED_TAG_ID) {
            UA_UInt16 index = getTagArrayIndex(tagIdNum);
            if (!tagInArray(tagIdNum)) {
                tagsPointer++;
            }
            if (cJSON_IsTrue(success) && cJSON_IsTrue(alive)) {

                tags[index].tagId = tagIdNum;
                tags[index].isAlive = true;
                tags[index].timestamp = (UA_DateTime) (cJSON_GetObjectItemCaseSensitive(tagData,
                                                                                        "timestamp"))->valuedouble;

                cJSON *coordinates = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(tagData, "data"),
                                                                      "coordinates");
                tags[index].coordinates.x = (UA_Double) cJSON_GetObjectItemCaseSensitive(coordinates, "x")->valueint;
                tags[index].coordinates.y = (UA_Double) cJSON_GetObjectItemCaseSensitive(coordinates, "y")->valueint;
                tags[index].coordinates.z = (UA_Double) cJSON_GetObjectItemCaseSensitive(coordinates, "z")->valueint;

                cJSON *rotation = cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(tagData, "data"),
                                                                   "orientation");
                tags[index].rotation.yaw = cJSON_GetObjectItemCaseSensitive(rotation, "yaw")->valuedouble;
                tags[index].rotation.roll = cJSON_GetObjectItemCaseSensitive(rotation, "roll")->valuedouble;
                tags[index].rotation.pitch = cJSON_GetObjectItemCaseSensitive(rotation, "pitch")->valuedouble;

            } else {
                tags[index].tagId = tagIdNum;
                tags[index].isAlive = false;
                tags[index].timestamp = 0;
                tags[index].coordinates.x = 0;
                tags[index].coordinates.y = 0;
                tags[index].coordinates.z = 0;
                tags[index].rotation.yaw = 0;
                tags[index].rotation.roll = 0;
                tags[index].rotation.pitch = 0;
            }
        }
    }
    cJSON_Delete(jsonTag);
}

UA_Boolean tagInArray(UA_Int32 tagId) {
    for(UA_UInt16 i = 0; i < tagsPointer; i++){
        if(tags[i].tagId == tagId) return true;
    }
    return false;
}


UA_UInt16 getTagArrayIndex(UA_Int32 tagId) {
    for(UA_UInt16 i = 0; i < tagsPointer; i++){
        if(tags[i].tagId == tagId) return i;
    }
    return tagsPointer;
}


void client_refresher(UA_Server *server, void* client)
{
    mqtt_sync((struct mqtt_client*) client);
}


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

    tags = UA_calloc(NUM_TAGS, sizeof(TagData));

    int sockfd = open_nb_socket(ADDRESS, PORT);

    if(sockfd == -1){
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Failed to open socket for MQTT connection");
        return EXIT_FAILURE;
    }
    struct mqtt_client client;
    // setup a client
    uint8_t sendbuf[2048]; // sendbuf should be large enough to hold multiple whole mqtt messages
    uint8_t recvbuf[4096]; // recvbuf should be large enough any whole mqtt message expected to be received
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
    // Create an anonymous session
    const char* client_id = NULL;
    // Ensure we have a clean session
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    // Send connection request to the broker.
    mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);

    // check that we don't have any errors
    if (client.error != MQTT_OK) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Failed to create MQTT client: %s", mqtt_error_str(client.error));
        return EXIT_FAILURE;
    }


    // start a thread to refresh the client (handle egress and ingree client traffic)
    /*pthread_t client_daemon;
    if(pthread_create(&client_daemon, NULL, client_refresher, &client)) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Failed to to start MQTT client daemon");
        return -1;
    }*/

    addPozyx(server);

    // subscribe
    mqtt_subscribe(&client, TOPIC, 0);

    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7055), scanPozyx);
    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7058), getSupportedLocationTypes);
    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7056), getLocationPozyx);
    UA_Server_setMethodNode_callback(server, UA_NODEID_NUMERIC(autoidNamespace, 7059), scanPozyxWithoutOpt);

    UA_Server_addRepeatedCallback(server, client_refresher, &client, 200, NULL);

    retval = UA_Server_run(server, &serverRunning);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
