#include "open62541.h"
#include "model.h"
#include "DHT22.h"
#include "driver/gpio.h"


static void
configureGPIO();

/* GPIO Configuration */
/*static void
configureGPIO(void) {
    gpio_set_direction(RELAY_0_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(RELAY_1_GPIO, GPIO_MODE_INPUT_OUTPUT);

}*/
int32_t nouv_position = 0;
bool is_new_position_set = false; 


void updateTargetPosition(int32_t new_position) {
    nouv_position = new_position;
    is_new_position_set = true;
}
/*------*/
UA_StatusCode 
readStepperPosition(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;

    if (stepper == NULL) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stepper pointer is NULL");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    int32_t position = accelstepper_current_position(stepper);
    UA_Variant_setScalarCopy(&dataValue->value, &position, &UA_TYPES[UA_TYPES_INT32]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setStepperPosition(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;

    // Obtenez la nouvelle position à partir des données
    int32_t new_position = *(UA_Int32 *)data->value.data;

    //updateTargetPosition(new_position);
    updateTargetPosition(new_position);
    
    // Vérifiez si la position du moteur a été mise à jour correctement
    //int32_t current_position = accelstepper_current_position(stepper);
    UA_StatusCode status = UA_STATUSCODE_GOOD; //: UA_STATUSCODE_BADINTERNALERROR;
    //(current_position == new_position)

    return status;


}
/*------------------------------*/
void addStepperControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Stepper Position");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Stepper Position");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Stepper Position");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource stepperDataSource;
    stepperDataSource.read = readStepperPosition;
    stepperDataSource.write = setStepperPosition;

   /* UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        stepperDataSource, NULL, NULL);*/
    UA_StatusCode retval = UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                                        UA_QUALIFIEDNAME(1, "Control Stepper Position"),
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),attr, stepperDataSource, (void *)stepper, NULL);
                                                        //(void *)stepper
    }                                                    /*-----*/
// /* LED Method */

// UA_StatusCode
// ledProcessCallBack(UA_Server *server,
//                    const UA_NodeId *sessionId, void *sessionHandle,
//                    const UA_NodeId *methodId, void *methodContext,
//                    const UA_NodeId *objectId, void *objectContext,
//                    size_t inputSize, const UA_Variant *input,
//                    size_t outputSize, UA_Variant *output)
// {
//     UA_Int32 i = 0;
//     UA_Int32 *inputVal = (UA_Int32 *)input->data;
//     UA_String tmp = UA_STRING_ALLOC("Data Received");
//     if (*inputVal > 0)
//     {
//         tmp.data = (UA_Byte *)UA_realloc(tmp.data, tmp.length);
//         while (i < *inputVal + 1)
//         {
//             gpio_pad_select_gpio(BLINK_GPIO);
//             gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
//             gpio_set_level(BLINK_GPIO, 1);
//             vTaskDelay(500 / portTICK_PERIOD_MS);
//             gpio_set_level(BLINK_GPIO, 0);
//             vTaskDelay(500 / portTICK_PERIOD_MS);
//             i++;
//         }
//     }
//     UA_String_clear(&tmp);
//     return UA_STATUSCODE_GOOD;
// }

// void
// addLEDMethod(UA_Server *server)
// {

//     UA_NodeId createdNodeId;
//     UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;

//     object_attr.description = UA_LOCALIZEDTEXT("en-US", "A pump!");
//     object_attr.displayName = UA_LOCALIZEDTEXT("en-US", "Pump1");

//     UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
//                             UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
//                             UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
//                             UA_QUALIFIEDNAME(1, "Pump1"),
//                             UA_NODEID_NUMERIC(2, 1002),
//                             object_attr, NULL, &createdNodeId);

//     UA_Argument inputArgument;
//     UA_Argument_init(&inputArgument);
//     inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Number of times to blink LED!");
//     inputArgument.name = UA_STRING("Blink Count");
//     inputArgument.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
//     inputArgument.valueRank = -1; /* scalar */

//     /* And output argument for a void method is not logical, check here !!! */
//     UA_Argument outputArgument;
//     UA_Argument_init(&outputArgument);
//     outputArgument.description = UA_LOCALIZEDTEXT("en-US", "LED Blinked");
//     outputArgument.name = UA_STRING("Led Blink Method Output");
//     outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
//     outputArgument.valueRank = UA_VALUERANK_ONE_DIMENSION;

//     UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
//     helloAttr.description = UA_LOCALIZEDTEXT("en-US", "Enter the number of times you want LED to blin!");
//     helloAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Blink");
//     helloAttr.executable = true;
//     helloAttr.userExecutable = true;
//     UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1, 62541),
//                             UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
//                             UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
//                             UA_QUALIFIEDNAME(1, "Blink"),
//                             helloAttr, &ledProcessCallBack,
//                             1, &inputArgument, 1, &outputArgument, NULL, &createdNodeId);
// }


/* Temperature */

UA_StatusCode
readCurrentTemperature(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Float temperature = 2;
    UA_Variant_setScalarCopy(&dataValue->value, &temperature,
                             &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}


void
addCurrentTemperatureDataSourceVariable(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperature");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "temperature");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Ambient Temperature");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource timeDataSource;
    timeDataSource.read = readCurrentTemperature;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        timeDataSource, NULL, NULL);
}


/* Relay 0 */
/*
UA_StatusCode
readRelay0State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean relay0_State = gpio_get_level(RELAY_0_GPIO);
    UA_Variant_setScalarCopy(&dataValue->value, &relay0_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRelay0State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    UA_Boolean currentState = gpio_get_level(RELAY_0_GPIO);
    int level = currentState == true ? 0:1;
    gpio_set_level(RELAY_0_GPIO, level);
    UA_Boolean relay0_state_after_write = gpio_get_level(RELAY_0_GPIO);
    UA_StatusCode status = relay0_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

void
addRelay0ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Relay0");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Relay number 0.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Relay number 0.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource relay0;
    //Configure GPIOs just before adding relay 0 - not a good practice.
    configureGPIO();
    relay0.read = readRelay0State;
    relay0.write = setRelay0State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        relay0, NULL, NULL);
}*/

/* Relay 1 */

UA_StatusCode
readRelay1State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    UA_Boolean relay1_State = gpio_get_level(RELAY_1_GPIO);
    UA_Variant_setScalarCopy(&dataValue->value, &relay1_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRelay1State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    UA_Boolean currentState = gpio_get_level(RELAY_1_GPIO);
    int level = currentState == true ? 0:1;
    gpio_set_level(RELAY_1_GPIO, level);
    UA_Boolean relay1_state_after_write = gpio_get_level(RELAY_1_GPIO);
    UA_StatusCode status = relay1_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

void
addRelay1ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Relay1");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Relay number 1.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Relay number 1.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource relay1;
    relay1.read = readRelay1State;
    relay1.write = setRelay1State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        relay1, NULL, NULL);
}