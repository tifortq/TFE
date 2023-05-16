#include "open62541.h"
#include "accelstepper_wrapper.h"
/* GPIO Numbers */
// #define BLINK_GPIO 2
//#define DHT22_GPIO 4
//#define RELAY_0_GPIO 2
#define RELAY_1_GPIO 16
// #define RELAY_2_GPIO 26
// #define RELAY_3_GPIO 27
extern int32_t nouv_position;
extern bool is_moving;
extern volatile bool motor_stop;
void updateTargetPosition(int32_t new_position);
void FlagTrueMouvement(void);
void FlagFalseMouvement(void);
// /* Led Method */
// UA_StatusCode
// ledProcessCallBack(UA_Server *server,
//                    const UA_NodeId *sessionId, void *sessionHandle,
//                    const UA_NodeId *methodId, void *methodContext,
//                    const UA_NodeId *objectId, void *objectContext,
//                    size_t inputSize, const UA_Variant *input,
//                    size_t outputSize, UA_Variant *output);

// void
// addLEDMethod(UA_Server *server);
/*----------*/
UA_StatusCode
setStepperPosition(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperPosition(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void 
addStepperControlNode(UA_Server *server, AccelStepperWrapper *stepper);

/*---------*/
UA_StatusCode
setStepperMaxSpeed(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperMaxSpeed(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void
addStepperMaxSpeedControlNode(UA_Server *server, AccelStepperWrapper *stepper);
/*------------------*/

UA_StatusCode
setStepperACC(UA_Server *server,
                   const UA_NodeId *sessionId, void *sessionContext,
                   const UA_NodeId *nodeId, void *nodeContext,
                   const UA_NumericRange *range, const UA_DataValue *data);
UA_StatusCode
readStepperACC(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

void
addStepperAccControlNode(UA_Server *server, AccelStepperWrapper *stepper);
/*------------------------------*/
UA_StatusCode
readCurrentSpeed(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);

UA_StatusCode addCurrentSpeed(UA_Server *server, AccelStepperWrapper *stepper);

/* Relay 1 */
UA_StatusCode
readRelay1State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue);

UA_StatusCode
setRelay1State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data);

void
addRelay1ControlNode(UA_Server *server);