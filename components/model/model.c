#include "open62541.h"
#include "model.h"
#include "DHT22.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "accelstepper_wrapper.h"



#define PWM_GPIO 2
#define LEDC_TIMER 0
#define LEDC_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_FREQUENCY 5000
#define LEDC_RESOLUTION LEDC_TIMER_12_BIT

int32_t nouv_position = 0;

void updateTargetPosition(int32_t new_position) {
    nouv_position = new_position;
}
static void
configureGPIO();

/* GPIO Configuration */
static void
configureGPIO(void) {
    gpio_set_direction(RELAY_0_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(RELAY_1_GPIO, GPIO_MODE_INPUT_OUTPUT);

}

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


/*Control du moteur*/
/*UA_StatusCode 
readStepperSpeed(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                               UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    float speed = accelstepper_max_speed(stepper);

    UA_Variant_setScalarCopy(&dataValue->value, &speed, &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode 
setStepperSpeed(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    float new_speed = *(UA_Float *)data->value.data;
    accelstepper_set_max_speed(stepper, new_speed);

    return UA_STATUSCODE_GOOD;
}


UA_StatusCode 
setStepperAcceleration(UA_Server *server,
                                     const UA_NodeId *sessionId, void *sessionContext,
                                     const UA_NodeId *nodeId, void *nodeContext,
                                     const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    float new_acceleration = *(UA_Float *)data->value.data;
    accelstepper_set_acceleration(stepper, new_acceleration);

    return UA_STATUSCODE_GOOD;
}*/
/*
UA_StatusCode 
setStepperSpeed(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    float new_speed = *(UA_Float *)data->value.data;
    accelstepper_set_max_speed(stepper, new_speed);

    return UA_STATUSCODE_GOOD;
}*/

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

    updateTargetPosition(new_position);
    
    // Vérifiez si la position du moteur a été mise à jour correctement
    //int32_t current_position = accelstepper_current_position(stepper);
    UA_StatusCode status = UA_STATUSCODE_GOOD; //: UA_STATUSCODE_BADINTERNALERROR;
    //(current_position == new_position)

    return status;

    /*---------------------------*/
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
/*
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        stepperDataSource, NULL, NULL);*/
    UA_StatusCode retval = UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                                        UA_QUALIFIEDNAME(1, "Control Stepper Position"),
                                                        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),attr, stepperDataSource, (void *)stepper, NULL);
                                                        //(void *)stepper
                                     

    ESP_LOGI("retval", "%08x", retval);
if (retval != UA_STATUSCODE_GOOD) {
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                   "Failed to add stepper position node, error: 0x%08x", retval);
} else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Stepper position node added successfully");
}

}
/*--------------------------------------------------*/
/*UA_StatusCode readStepperPosition(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    // Lire la position actuelle du moteur pas à pas
    // Utilisez votre bibliothèque de moteur pas à pas préférée pour lire la position actuelle
    UA_Int32 currentPosition =  Votre fonction pour lire la position ;

    UA_Variant_setScalarCopy(&dataValue->value, &currentPosition,
                             &UA_TYPES[UA_TYPES_INT32]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode setStepperPosition(UA_Server *server,
                                 const UA_NodeId *sessionId, void *sessionContext,
                                 const UA_NodeId *nodeId, void *nodeContext,
                                 const UA_NumericRange *range, const UA_DataValue *data) {
    UA_Int32 newPosition = *(UA_Int32 *)data->value.data;

    // Utilisez newPosition pour définir la position du moteur pas à pas
    // en utilisant votre bibliothèque de moteur pas à pas préférée.
    // Vous devrez peut-être récupérer l'instance du moteur pas à pas à partir de nodeContext ou handle.

    // Vérifiez si la position a été mise à jour correctement et renvoyez un code d'état approprié
    UA_StatusCode status =  Vérifiez si la mise à jour a réussi  ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}*/
    /*

    void addStepperControlNodes(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr_speed = UA_VariableAttributes_default;
    attr_speed.displayName = UA_LOCALIZEDTEXT("en-US", "Stepper Speed");
    attr_speed.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr_speed.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_VariableAttributes attr_acceleration = UA_VariableAttributes_default;
    attr_acceleration.displayName = UA_LOCALIZEDTEXT("en-US", "Stepper Acceleration");
    attr_acceleration.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr_acceleration.accessLevel = UA_ACCESSLEVELMASK_WRITE;

    UA_VariableAttributes attr_position = UA_VariableAttributes_default;
    attr_position.displayName = UA_LOCALIZEDTEXT("en-US", "Stepper Position");
    attr_position.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr_position.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    // Constants for NodeIds
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    // Speed Node
    UA_NodeId speedNodeId = UA_NODEID_NUMERIC(1, "Stepper_speed");
    UA_QualifiedName speedNodeName = UA_QUALIFIEDNAME(1, "Stepper_speed");
    UA_DataSource speedDataSource;
    speedDataSource.read = readStepperSpeed;
    speedDataSource.write = setStepperSpeed;
    UA_Server_addDataSourceVariableNode(server, speedNodeId, parentNodeId,
                                        parentReferenceNodeId, speedNodeName,
                                        variableTypeNodeId, attr_speed,
                                        speedDataSource, stepper, NULL);

    // Acceleration Node
    UA_NodeId accelerationNodeId = UA_NODEID_NUMERIC(1, "Stepper_Accel");
    UA_QualifiedName accelerationNodeName = UA_QUALIFIEDNAME(1, "Stepper_Accel");
    UA_DataSource accelerationDataSource;
    
    accelerationDataSource.write = setStepperAcceleration;
    UA_Server_addDataSourceVariableNode(server, accelerationNodeId, parentNodeId,
                                        parentReferenceNodeId, accelerationNodeName,
                                        variableTypeNodeId, attr_acceleration,
                                        accelerationDataSource, stepper, NULL);

    // Position Node
    UA_NodeId positionNodeId = UA_NODEID_NUMERIC(1, "Stepper_position");
    UA_QualifiedName positionNodeName = UA_QUALIFIEDNAME(1, "Stepper_position");
    UA_DataSource positionDataSource;
    positionDataSource.read = readStepperPosition;
    positionDataSource.write = setStepperPosition;
    UA_Server_addDataSourceVariableNode(server, positionNodeId, parentNodeId,
                                        parentReferenceNodeId, positionNodeName,
                                        variableTypeNodeId, attr_position,
                                        positionDataSource, stepper, NULL);
}*/


/*Sortie analogique*/
UA_StatusCode
setAnalogOutput(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                const UA_NumericRange *range, const UA_DataValue *data) {
    // Lisez la valeur actuelle du PWM
    uint32_t current_duty = ledc_get_duty(LEDC_MODE, LEDC_CHANNEL_0);

    // Mettez à jour la sortie PWM avec la nouvelle valeur
    uint32_t new_duty = *(UA_UInt32 *)data->value.data;
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_0, new_duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_0);

    // Vérifiez si la sortie PWM a été mise à jour correctement
    uint32_t pwm_output_after_write = ledc_get_duty(LEDC_MODE, LEDC_CHANNEL_0);
    UA_StatusCode status = (pwm_output_after_write == new_duty) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;

    return status;
}
UA_StatusCode
readAnalogOutput(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {
    uint32_t current_duty = ledc_get_duty(LEDC_MODE, LEDC_CHANNEL_0);

    UA_Variant_setScalarCopy(&dataValue->value, &current_duty,
                             &UA_TYPES[UA_TYPES_UINT32]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}
void
addAnalogControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Red");
    attr.dataType = UA_TYPES[UA_TYPES_UINT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Red color");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Red color");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource red0;
    //Configure GPIOs just before adding relay 0 - not a good practice.
   
    red0.read = readAnalogOutput;
    red0.write = setAnalogOutput;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        red0, NULL, NULL);
}



/* Relay 0 */

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
}

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