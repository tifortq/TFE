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
int32_t position=0;
int32_t vit_position =0;
UA_Float new_acc = 0.0;
UA_Float new_speed = 0.0;
bool is_moving = false;
volatile bool motor_stop = false;
void updateTargetPosition(int32_t new_position) {
    nouv_position = new_position;
    
}
void FlagTrueMouvement(void) {
    is_moving = true;
    
}
void FlagFalseMouvement(void) {
    is_moving = false;  
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
   /* if (is_moving == true){
     position = accelstepper_current_position(stepper);}
    else if (is_moving == false){
    position = 0;
    }*/
    position = accelstepper_current_position(stepper);
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

    /*----------------------------*/
    accelstepper_move_to(stepper, new_position);
    /*-----------------------------*/


    // Vérifiez si la position du moteur a été mise à jour correctement
    //int32_t current_position = accelstepper_current_position(stepper);
    //UA_StatusCode status = current_position == new_position ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return UA_STATUSCODE_GOOD;
    


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
readStepperMaxSpeed(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
     // ou une autre valeur par défaut
    UA_Variant_setScalarCopy(&dataValue->value, &new_speed, &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;

}
UA_StatusCode 
setStepperMaxSpeed(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    new_speed = *(UA_Float *)data->value.data;
    accelstepper_set_max_speed(stepper, new_speed);
    ESP_LOGI("Step Init", "VITESSE: %f", new_speed);
    return UA_STATUSCODE_GOOD;
}
void addStepperMaxSpeedControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor MaxSpeed");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor MaxSpeed");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor MaxSpeed");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource SpeedDataSource;
    SpeedDataSource.read = readStepperMaxSpeed;
    SpeedDataSource.write = setStepperMaxSpeed;

                                   
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, SpeedDataSource, (void *)stepper, NULL);
     
                                                        
    }                                                   
/*-------------*/

UA_StatusCode 
readStepperACC(UA_Server *server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    // ou une autre valeur par défaut
    UA_Variant_setScalarCopy(&dataValue->value, &new_acc, &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;

}
UA_StatusCode 
setStepperACC(UA_Server *server,
                               const UA_NodeId *sessionId, void *sessionContext,
                               const UA_NodeId *nodeId, void *nodeContext,
                               const UA_NumericRange *range, const UA_DataValue *data) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    new_acc = *(UA_Float *)data->value.data;
    accelstepper_set_acceleration(stepper, new_acc);
    ESP_LOGI("Step Init", "ACC: %f", new_acc);
    return UA_STATUSCODE_GOOD;
}
void addStepperAccControlNode(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "stepmotor Acc");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "stepmotor Acc");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "stepmotor Acc");
     UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource ACCDataSource;
    ACCDataSource.read = readStepperACC;
    ACCDataSource.write = setStepperACC;

                                   
    UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, ACCDataSource, (void *)stepper, NULL);
     
                                                        
    }
    /*---------------------------------*/
    UA_StatusCode
    readCurrentSpeed(UA_Server*server,
                                  const UA_NodeId *sessionId, void *sessionContext,
                                  const UA_NodeId *nodeId, void *nodeContext,
                                  UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                                  UA_DataValue *dataValue) {
    AccelStepperWrapper *stepper = (AccelStepperWrapper *)nodeContext;
    
    vit_position = accelstepper_current_position(stepper);
    UA_Variant_setScalarCopy(&dataValue->value, &vit_position, &UA_TYPES[UA_TYPES_INT32]);
    dataValue->hasValue = true; 
    return UA_STATUSCODE_GOOD;
} 



UA_StatusCode
addCurrentSpeed(UA_Server *server, AccelStepperWrapper *stepper) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Vitesse actuelle");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Vitesse actuelle");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Vitesse actuelle");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource SpeedActDataSource;
    SpeedActDataSource.read = readCurrentSpeed;
    return UA_Server_addDataSourceVariableNode(server, currentNodeId,
                                                        parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId,attr, SpeedActDataSource, (void *)stepper, NULL);
}
/*-----------------------------------------------*/

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
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
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
    
    UA_Variant_setScalarCopy(&dataValue->value, &motor_stop,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRelay1State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {
    motor_stop = *(UA_Boolean *)data->value.data;
    
    
    //UA_StatusCode status = relay1_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return UA_STATUSCODE_GOOD;
}

void
addRelay1ControlNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "STOP");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "STOP");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "STOP");
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