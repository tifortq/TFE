#include "open62541.h"
#include "model.h"
#include "DHT22.h"
#include "driver/gpio.h"
#include "accelstepper_wrapper.h"

static void
configureGPIO();

/* GPIO Configuration */
static void
configureGPIO(void) {
    //gpio_set_direction(RELAY_0_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(RELAY_1_GPIO, GPIO_MODE_INPUT_OUTPUT);

}
int32_t nouv_position = 0;



void updateTargetPosition(int32_t new_position) {
    nouv_position = new_position;
    
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
    int32_t current_position = accelstepper_current_position(stepper);
    //UA_StatusCode status = UA_STATUSCODE_GOOD; //: UA_STATUSCODE_BADINTERNALERROR;
    //(current_position == new_position)
    UA_StatusCode status = current_position == new_position ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
    


}
/*------------------*/

/*------------------------------*/
void addStepperControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor Position");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor Position");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor Position");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource stepperDataSource;
    stepperDataSource.read = readStepperPosition;
    stepperDataSource.write = setStepperPosition;

   /*UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        stepperDataSource, NULL, NULL);*/
    //UA_StatusCode retval = UA_Server_addDataSourceVariableNode                                    
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, stepperDataSource, (void *)stepper, NULL);
                                                        //(void *)stepper
    }                                                    /*-----*/
/*--------------*/
UA_StatusCode 
readStepperSpeed(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    UA_Float speed = 0.0; // ou une autre valeur par défaut
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
    UA_Float new_speed = *(UA_Float *)data->value.data;
    accelstepper_set_max_speed(stepper, new_speed);
    ESP_LOGI("Step Init", "VITESSE: %f", new_speed);
    return UA_STATUSCODE_GOOD;
}
UA_StatusCode addStepperSpeedControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor Speed");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor Speed");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor Speed");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource SpeedDataSource;
    SpeedDataSource.read = readStepperSpeed;
    SpeedDataSource.write = setStepperSpeed;

                                   
    return  UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, SpeedDataSource, (void *)stepper, NULL);
     
                                                        
    }                                                   
/*-------------*/

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