#include "event_processor.h"
#include <iomanip>

extern "C" {

static OTF2_CallbackCode

GlobalEvtReaderCallback_Enter([[maybe_unused]] OTF2_LocationRef locationID,
                              OTF2_TimeStamp time, void *userData,
                              OTF2_AttributeList *attributeList,
                              OTF2_RegionRef region) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleEvtEnter(time, attributeList, region);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_Leave([[maybe_unused]] OTF2_LocationRef locationID,
                              OTF2_TimeStamp time, void *userData,
                              OTF2_AttributeList *attributeList,
                              OTF2_RegionRef region) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleEvtLeave(time, attributeList, region);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiSend([[maybe_unused]] OTF2_LocationRef locationID,
                                OTF2_TimeStamp time, void *userData,
                                OTF2_AttributeList *attributeList,
                                uint32_t receiver, OTF2_CommRef communicator,
                                uint32_t msgTag, uint64_t msgLength) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPISend(attributeList, time, receiver, communicator, msgTag,
                        msgLength);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiRecv([[maybe_unused]] OTF2_LocationRef locationID,
                                OTF2_TimeStamp time, void *userData,
                                OTF2_AttributeList *attributeList,
                                uint32_t sender, OTF2_CommRef communicator,
                                uint32_t msgTag, uint64_t msgLength) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIRecv(attributeList, time, sender, communicator, msgTag,
                        msgLength);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiIsend([[maybe_unused]] OTF2_LocationRef locationID,
                                 OTF2_TimeStamp time, void *userData,
                                 OTF2_AttributeList *attributeList,
                                 uint32_t receiver, OTF2_CommRef communicator,
                                 uint32_t msgTag, uint64_t msgLength,
                                 uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIISend(attributeList, time, receiver, communicator, msgTag,
                         msgLength, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiIrecv([[maybe_unused]] OTF2_LocationRef locationID,
                                 OTF2_TimeStamp time, void *userData,
                                 OTF2_AttributeList *attributeList,
                                 uint32_t sender, OTF2_CommRef communicator,
                                 uint32_t msgTag, uint64_t msgLength,
                                 uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIIRecv(attributeList, time, sender, communicator, msgTag,
                         msgLength, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiIsendComplete(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIISendComplete(attributeList, time, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiIrecvRequest(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIIRecvRequest(attributeList, time, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiRequestTest(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIRequestTest(attributeList, time, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiRequestCancelled(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t requestID) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIRequestCancelled(attributeList, time, requestID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiCollectiveBegin(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIBeginProps(attributeList, time);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_MpiCollectiveEnd(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator, uint32_t root,
    uint64_t sizeSent, uint64_t sizeReceived) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleMPIEndProps(attributeList, time, collectiveOp, communicator,
                            root, sizeSent, sizeReceived);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpFork([[maybe_unused]] OTF2_LocationRef locationID,
                                OTF2_TimeStamp time, void *userData,
                                OTF2_AttributeList *attributeList,
                                uint32_t numberOfRequestedThreads) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpFork(attributeList, time, numberOfRequestedThreads);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpJoin([[maybe_unused]] OTF2_LocationRef locationID,
                                OTF2_TimeStamp time, void *userData,
                                OTF2_AttributeList *attributeList) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpJoin(attributeList, time);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpAcquireLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint32_t lockID,
    uint32_t acquisitionOrder) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpAcquireLock(attributeList, time, lockID, acquisitionOrder);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpReleaseLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint32_t lockID,
    uint32_t acquisitionOrder) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpReleaseLock(attributeList, time, lockID, acquisitionOrder);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpTaskCreate(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t taskID) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpTaskCreate(attributeList, time, taskID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpTaskSwitch(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t taskID) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpTaskSwitch(attributeList, time, taskID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_OmpTaskComplete(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, uint64_t taskID) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleOmpTaskComplete(attributeList, time, taskID);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadFork([[maybe_unused]] OTF2_LocationRef locationID,
                                   OTF2_TimeStamp time, void *userData,
                                   OTF2_AttributeList *attributeList,
                                   OTF2_Paradigm model,
                                   uint32_t numberOfRequestedThreads) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadFork(attributeList, time, model,
                           numberOfRequestedThreads);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadJoin([[maybe_unused]] OTF2_LocationRef locationID,
                                   OTF2_TimeStamp time, void *userData,
                                   OTF2_AttributeList *attributeList,
                                   OTF2_Paradigm model) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadJoin(attributeList, time, model);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadTeamBegin(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_CommRef threadTeam) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadTeamBegin(attributeList, time, threadTeam);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadTeamEnd(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_CommRef threadTeam) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadTeamEnd(attributeList, time, threadTeam);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadAcquireLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model,
    uint32_t lockID, uint32_t acquisitionOrder) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadAcquireLock(attributeList, time, model, lockID,
                                  acquisitionOrder);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadReleaseLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_Paradigm model,
    uint32_t lockID, uint32_t acquisitionOrder) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadReleaseLock(attributeList, time, model, lockID,
                                  acquisitionOrder);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadTaskCreate(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam,
    uint32_t creatingThread, uint32_t generationNumber) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadTaskCreate(attributeList, time, threadTeam,
                                 creatingThread, generationNumber);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadTaskSwitch(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam,
    uint32_t creatingThread, uint32_t generationNumber) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadTaskCreate(attributeList, time, threadTeam,
                                 creatingThread, generationNumber);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadTaskComplete(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_CommRef threadTeam,
    uint32_t creatingThread, uint32_t generationNumber) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadTaskComplete(attributeList, time, threadTeam,
                                   creatingThread, generationNumber);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadCreate(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_CommRef threadContingent, uint64_t sequenceCount) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadCreate(attributeList, time, threadContingent,
                             sequenceCount);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadBegin(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_CommRef threadContingent, uint64_t sequenceCount) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};
  reader->handleThreadBegin(attributeList, time, threadContingent,
                            sequenceCount);
  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadWait([[maybe_unused]] OTF2_LocationRef locationID,
                                   OTF2_TimeStamp time, void *userData,
                                   OTF2_AttributeList *attributeList,
                                   OTF2_CommRef threadContingent,
                                   uint64_t sequenceCount) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadWait(attributeList, time, threadContingent,
                           sequenceCount);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_ThreadEnd([[maybe_unused]] OTF2_LocationRef locationID,
                                  OTF2_TimeStamp time, void *userData,
                                  OTF2_AttributeList *attributeList,
                                  OTF2_CommRef threadContingent,
                                  uint64_t sequenceCount) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleThreadEnd(attributeList, time, threadContingent, sequenceCount);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoCreateHandle(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    OTF2_IoAccessMode mode, OTF2_IoCreationFlag creationFlags,
    OTF2_IoStatusFlag statusFlags) {
  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoCreateHandle(attributeList, time, handle, mode, creationFlags,
                               statusFlags);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoDestroyHandle(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_IoHandleRef handle) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoDestroyHandle(attributeList, time, handle);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoDuplicateHandle(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_IoHandleRef oldHandle, OTF2_IoHandleRef newHandle,
    OTF2_IoStatusFlag statusFlags) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoHandleDuplicate(attributeList, time, oldHandle, newHandle,
                                  statusFlags);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoSeek([[maybe_unused]] OTF2_LocationRef locationID,
                               OTF2_TimeStamp time, void *userData,
                               OTF2_AttributeList *attributeList,
                               OTF2_IoHandleRef handle, int64_t offsetRequest,
                               OTF2_IoSeekOption whence,
                               uint64_t offsetResult) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoSeek(attributeList, time, handle, offsetRequest, whence,
                       offsetResult);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoChangeStatusFlags(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    OTF2_IoStatusFlag statusFlags) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoChangeStatusFlags(attributeList, time, handle, statusFlags);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoDeleteFile(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList,
    OTF2_IoParadigmRef ioParadigm, OTF2_IoFileRef file) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoDeleteFile(attributeList, time, ioParadigm, file);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoOperationBegin(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    OTF2_IoOperationMode mode, OTF2_IoOperationFlag operationFlags,
    uint64_t bytesRequest, uint64_t matchingId) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoOperationBegin(attributeList, time, handle, mode,
                                 operationFlags, bytesRequest, matchingId);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoOperationTest(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    uint64_t matchingId) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoOperationTest(attributeList, time, handle, matchingId);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoOperationIssued(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    uint64_t matchingId) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoOperationIssued(attributeList, time, handle, matchingId);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoOperationComplete(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    uint64_t bytesResult, uint64_t matchingId) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoOperationComplete(attributeList, time, handle, bytesResult,
                                    matchingId);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoOperationCancelled(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    uint64_t matchingId) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoOperationCancelled(attributeList, time, handle, matchingId);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoAcquireLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    OTF2_LockType lockType) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoAcquireLock(attributeList, time, handle, lockType);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoReleaseLock(
    [[maybe_unused]] OTF2_LocationRef locationID, OTF2_TimeStamp time,
    void *userData, OTF2_AttributeList *attributeList, OTF2_IoHandleRef handle,
    OTF2_LockType lockType) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoReleaseLock(attributeList, time, handle, lockType);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalEvtReaderCallback_IoTryLock([[maybe_unused]] OTF2_LocationRef locationID,
                                  OTF2_TimeStamp time, void *userData,
                                  OTF2_AttributeList *attributeList,
                                  OTF2_IoHandleRef handle,
                                  OTF2_LockType lockType) {

  EventProcessor *reader{static_cast<EventProcessor *>(userData)};

  reader->handleIoTryLock(attributeList, time, handle, lockType);

  return OTF2_CALLBACK_SUCCESS;
}
}

EventProcessor::EventProcessor(Maps &maps, OTF2_Reader *reader,
                               OTF2_Archive *archive, const string &traceName)
    : m_maps{maps}, m_otf2Reader{reader}, m_archive{archive}, m_traceName{
                                                                  traceName} {

  m_otf2Reader = OTF2_Reader_Open(m_traceName.c_str());
  m_evtWriter = NULL;
  OTF2_Reader_SetSerialCollectiveCallbacks(m_otf2Reader);
  m_maps.ComputeRelativeClockFactor();
}

void EventProcessor::readLocationWise() {

  auto location_tuple = m_maps.getLocationRange();

  auto start_itr = std::get<0>(location_tuple);
  auto end_itr = std::get<1>(location_tuple);

  OTF2_GlobalEvtReaderCallbacks *global_evt_callbacks =
      OTF2_GlobalEvtReaderCallbacks_New();

  OTF2_GlobalEvtReaderCallbacks_SetEnterCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_Enter);

  OTF2_GlobalEvtReaderCallbacks_SetLeaveCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_Leave);

  OTF2_GlobalEvtReaderCallbacks_SetMpiSendCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiSend);

  OTF2_GlobalEvtReaderCallbacks_SetMpiRecvCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiRecv);

  OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiIsend);

  OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiIrecv);

  OTF2_GlobalEvtReaderCallbacks_SetMpiIsendCompleteCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiIsendComplete);

  OTF2_GlobalEvtReaderCallbacks_SetMpiIrecvRequestCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiIrecvRequest);

  OTF2_GlobalEvtReaderCallbacks_SetMpiRequestTestCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiRequestTest);

  OTF2_GlobalEvtReaderCallbacks_SetMpiRequestCancelledCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiRequestCancelled);

  OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveBeginCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiCollectiveBegin);

  OTF2_GlobalEvtReaderCallbacks_SetMpiCollectiveEndCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_MpiCollectiveEnd);

  OTF2_GlobalEvtReaderCallbacks_SetOmpForkCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpFork);

  OTF2_GlobalEvtReaderCallbacks_SetOmpJoinCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpJoin);

  OTF2_GlobalEvtReaderCallbacks_SetOmpAcquireLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpAcquireLock);

  OTF2_GlobalEvtReaderCallbacks_SetOmpReleaseLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpReleaseLock);

  OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCreateCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpTaskCreate);

  OTF2_GlobalEvtReaderCallbacks_SetOmpTaskSwitchCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpTaskSwitch);

  OTF2_GlobalEvtReaderCallbacks_SetOmpTaskCompleteCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_OmpTaskComplete);

  OTF2_GlobalEvtReaderCallbacks_SetThreadForkCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadFork);

  OTF2_GlobalEvtReaderCallbacks_SetThreadJoinCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadJoin);

  OTF2_GlobalEvtReaderCallbacks_SetThreadTeamBeginCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadTeamBegin);

  OTF2_GlobalEvtReaderCallbacks_SetThreadTeamEndCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadTeamEnd);

  OTF2_GlobalEvtReaderCallbacks_SetThreadAcquireLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadAcquireLock);

  OTF2_GlobalEvtReaderCallbacks_SetThreadReleaseLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadReleaseLock);

  OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCreateCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadTaskCreate);

  OTF2_GlobalEvtReaderCallbacks_SetThreadTaskSwitchCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadTaskSwitch);

  OTF2_GlobalEvtReaderCallbacks_SetThreadTaskCompleteCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadTaskComplete);

  OTF2_GlobalEvtReaderCallbacks_SetThreadCreateCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadCreate);

  OTF2_GlobalEvtReaderCallbacks_SetThreadBeginCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadBegin);

  OTF2_GlobalEvtReaderCallbacks_SetThreadWaitCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadWait);

  OTF2_GlobalEvtReaderCallbacks_SetThreadEndCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_ThreadEnd);

  OTF2_GlobalEvtReaderCallbacks_SetIoCreateHandleCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoCreateHandle);

  OTF2_GlobalEvtReaderCallbacks_SetIoDestroyHandleCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoDestroyHandle);

  OTF2_GlobalEvtReaderCallbacks_SetIoDuplicateHandleCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoDuplicateHandle);

  OTF2_GlobalEvtReaderCallbacks_SetIoSeekCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoSeek);

  OTF2_GlobalEvtReaderCallbacks_SetIoChangeStatusFlagsCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoChangeStatusFlags);

  OTF2_GlobalEvtReaderCallbacks_SetIoDeleteFileCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoDeleteFile);

  OTF2_GlobalEvtReaderCallbacks_SetIoOperationBeginCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoOperationBegin);

  OTF2_GlobalEvtReaderCallbacks_SetIoOperationTestCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoOperationTest);

  OTF2_GlobalEvtReaderCallbacks_SetIoOperationIssuedCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoOperationIssued);

  OTF2_GlobalEvtReaderCallbacks_SetIoOperationCompleteCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoOperationComplete);

  OTF2_GlobalEvtReaderCallbacks_SetIoOperationCancelledCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoOperationCancelled);

  OTF2_GlobalEvtReaderCallbacks_SetIoAcquireLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoAcquireLock);

  OTF2_GlobalEvtReaderCallbacks_SetIoReleaseLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoReleaseLock);

  OTF2_GlobalEvtReaderCallbacks_SetIoTryLockCallback(
      global_evt_callbacks, &GlobalEvtReaderCallback_IoTryLock);

  for (auto location_itr = start_itr; location_itr != end_itr; location_itr++) {

    auto reading_location = location_itr->first;
    auto writing_location = location_itr->second;

    OTF2_Reader_SelectLocation(m_otf2Reader, reading_location);

    bool successful_open_def_files =
        OTF2_Reader_OpenDefFiles(m_otf2Reader) == OTF2_SUCCESS;
    OTF2_Reader_OpenEvtFiles(m_otf2Reader);

    if (successful_open_def_files) {
      OTF2_DefReader *def_reader =
          OTF2_Reader_GetDefReader(m_otf2Reader, reading_location);
      if (def_reader) {
        uint64_t def_reads = 0;
        OTF2_Reader_ReadAllLocalDefinitions(m_otf2Reader, def_reader,
                                            &def_reads);
        OTF2_Reader_CloseDefReader(m_otf2Reader, def_reader);
      }
    }

    [[maybe_unused]] OTF2_EvtReader *evt_reader =
        OTF2_Reader_GetEvtReader(m_otf2Reader, reading_location);

    if (successful_open_def_files) {
      OTF2_Reader_CloseDefFiles(m_otf2Reader);
    }

    OTF2_GlobalEvtReader *global_evt_reader =
        OTF2_Reader_GetGlobalEvtReader(m_otf2Reader);

    OTF2_Reader_RegisterGlobalEvtCallbacks(m_otf2Reader, global_evt_reader,
                                           global_evt_callbacks, this);

    OTF2_Archive_OpenEvtFiles(m_archive);
    m_evtWriter = OTF2_Archive_GetEvtWriter(m_archive, writing_location);

    uint64_t events_read{};
    OTF2_Reader_ReadAllGlobalEvents(m_otf2Reader, global_evt_reader,
                                    &events_read);

    OTF2_Reader_CloseGlobalEvtReader(m_otf2Reader, global_evt_reader);
  }

  OTF2_GlobalEvtReaderCallbacks_Delete(global_evt_callbacks);
}

void EventProcessor::handleEvtEnter(
    OTF2_TimeStamp time, [[maybe_unused]] OTF2_AttributeList *attributeList,
    OTF2_RegionRef region) {

  time = m_maps.getNewTimestamp(time);

  region = m_maps.getRegionID(region);

  OTF2_EvtWriter_Enter(m_evtWriter, NULL, time, region);
}

void EventProcessor::handleEvtLeave(
    OTF2_TimeStamp time, [[maybe_unused]] OTF2_AttributeList *attributeList,
    OTF2_RegionRef region) {

  time = m_maps.getNewTimestamp(time);

  region = m_maps.getRegionID(region);

  OTF2_EvtWriter_Leave(m_evtWriter, NULL, time, region);
}

void EventProcessor::handleMPISend(OTF2_AttributeList *attributeList,
                                   OTF2_TimeStamp time, uint32_t receiver,
                                   OTF2_CommRef communicator, uint32_t msgTag,
                                   uint64_t msgLength) {

  time = m_maps.getNewTimestamp(time);

  communicator = m_maps.getCommID(communicator);

  OTF2_EvtWriter_MpiSend(m_evtWriter, attributeList, time, receiver,
                         communicator, msgTag, msgLength);
}

void EventProcessor::handleMPIRecv(OTF2_AttributeList *attributeList,
                                   OTF2_TimeStamp time, uint32_t sender,
                                   OTF2_CommRef communicator, uint32_t msgTag,
                                   uint64_t msgLength) {
  time = m_maps.getNewTimestamp(time);

  communicator = m_maps.getCommID(communicator);

  OTF2_EvtWriter_MpiRecv(m_evtWriter, attributeList, time, sender, communicator,
                         msgTag, msgLength);
}

void EventProcessor::handleMPIISend(OTF2_AttributeList *attributeList,
                                    OTF2_TimeStamp time, uint32_t receiver,
                                    OTF2_CommRef communicator, uint32_t msgTag,
                                    uint64_t msgLength, uint64_t requestID) {
  time = m_maps.getNewTimestamp(time);

  communicator = m_maps.getCommID(communicator);

  OTF2_EvtWriter_MpiIsend(m_evtWriter, attributeList, time, receiver,
                          communicator, msgTag, msgLength, requestID);
}

void EventProcessor::handleMPIIRecv(OTF2_AttributeList *attributeList,
                                    OTF2_TimeStamp time, uint32_t sender,
                                    OTF2_CommRef communicator, uint32_t msgTag,
                                    uint64_t msgLength, uint64_t requestID) {

  time = m_maps.getNewTimestamp(time);

  communicator = m_maps.getCommID(communicator);

  OTF2_EvtWriter_MpiIrecv(m_evtWriter, attributeList, time, sender,
                          communicator, msgTag, msgLength, requestID);
}

void EventProcessor::handleMPIISendComplete(OTF2_AttributeList *attributeList,
                                            OTF2_TimeStamp time,
                                            uint64_t requestID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_MpiIsendComplete(m_evtWriter, attributeList, time, requestID);
}

void EventProcessor::handleMPIIRecvRequest(OTF2_AttributeList *attributeList,
                                           OTF2_TimeStamp time,
                                           uint64_t requestID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_MpiIrecvRequest(m_evtWriter, attributeList, time, requestID);
}

void EventProcessor::handleMPIRequestTest(OTF2_AttributeList *attributeList,
                                          OTF2_TimeStamp time,
                                          uint64_t requestID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_MpiRequestTest(m_evtWriter, attributeList, time, requestID);
}

void EventProcessor::handleMPIRequestCancelled(
    OTF2_AttributeList *attributeList, OTF2_TimeStamp time,
    uint64_t requestID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_MpiRequestCancelled(m_evtWriter, attributeList, time,
                                     requestID);
}

void EventProcessor::handleMPIBeginProps(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_MpiCollectiveBegin(m_evtWriter, attributeList, time);
}

void EventProcessor::handleMPIEndProps(OTF2_AttributeList *attributeList,
                                       OTF2_TimeStamp time,
                                       OTF2_CollectiveOp collectiveOp,
                                       OTF2_CommRef communicator, uint32_t root,
                                       uint64_t sizeSent,
                                       uint64_t sizeReceived) {

  time = m_maps.getNewTimestamp(time);

  communicator = m_maps.getCommID(communicator);

  OTF2_EvtWriter_MpiCollectiveEnd(m_evtWriter, attributeList, time,
                                  collectiveOp, communicator, root, sizeSent,
                                  sizeReceived);
}

void EventProcessor::handleOmpFork(OTF2_AttributeList *attributeList,
                                   OTF2_TimeStamp time,
                                   uint32_t numberOfRequestedThreads) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpFork(m_evtWriter, attributeList, time,
                         numberOfRequestedThreads);
}

void EventProcessor::handleOmpJoin(OTF2_AttributeList *attributeList,
                                   OTF2_TimeStamp time) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpJoin(m_evtWriter, attributeList, time);
}

void EventProcessor::handleOmpAcquireLock(OTF2_AttributeList *attributeList,
                                          OTF2_TimeStamp time, uint32_t lockID,
                                          uint32_t acquisitionOrder) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpAcquireLock(m_evtWriter, attributeList, time, lockID,
                                acquisitionOrder);
}

void EventProcessor::handleOmpReleaseLock(OTF2_AttributeList *attributeList,
                                          OTF2_TimeStamp time, uint32_t lockID,
                                          uint32_t acquisitionOrder) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpReleaseLock(m_evtWriter, attributeList, time, lockID,
                                acquisitionOrder);
}

void EventProcessor::handleOmpTaskCreate(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time, uint64_t taskID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpTaskCreate(m_evtWriter, attributeList, time, taskID);
}

void EventProcessor::handleOmpTaskSwitch(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time, uint64_t taskID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpTaskSwitch(m_evtWriter, attributeList, time, taskID);
}

void EventProcessor::handleOmpTaskComplete(OTF2_AttributeList *attributeList,
                                           OTF2_TimeStamp time,
                                           uint64_t taskID) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_OmpTaskComplete(m_evtWriter, attributeList, time, taskID);
}

void EventProcessor::handleThreadFork(OTF2_AttributeList *attributeList,
                                      OTF2_TimeStamp time, OTF2_Paradigm model,
                                      uint32_t numberOfRequestedThreads) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_ThreadFork(m_evtWriter, attributeList, time, model,
                            numberOfRequestedThreads);
}

void EventProcessor::handleThreadJoin(OTF2_AttributeList *attributeList,
                                      OTF2_TimeStamp time,
                                      OTF2_Paradigm model) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_ThreadJoin(m_evtWriter, attributeList, time, model);
}

void EventProcessor::handleThreadTeamBegin(OTF2_AttributeList *attributeList,
                                           OTF2_TimeStamp time,
                                           OTF2_CommRef threadTeam) {

  time = m_maps.getNewTimestamp(time);

  threadTeam = m_maps.getCommID(threadTeam);

  OTF2_EvtWriter_ThreadTeamBegin(m_evtWriter, attributeList, time, threadTeam);
}

void EventProcessor::handleThreadTeamEnd(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time,
                                         OTF2_CommRef threadTeam) {

  time = m_maps.getNewTimestamp(time);

  threadTeam = m_maps.getCommID(threadTeam);

  OTF2_EvtWriter_ThreadTeamEnd(m_evtWriter, attributeList, time, threadTeam);
}

void EventProcessor::handleThreadAcquireLock(OTF2_AttributeList *attributeList,
                                             OTF2_TimeStamp time,
                                             OTF2_Paradigm model,
                                             uint32_t lockID,
                                             uint32_t acquisitionOrder) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_ThreadAcquireLock(m_evtWriter, attributeList, time, model,
                                   lockID, acquisitionOrder);
}

void EventProcessor::handleThreadReleaseLock(OTF2_AttributeList *attributeList,
                                             OTF2_TimeStamp time,
                                             OTF2_Paradigm model,
                                             uint32_t lockID,
                                             uint32_t acquisitionOrder) {

  time = m_maps.getNewTimestamp(time);

  OTF2_EvtWriter_ThreadReleaseLock(m_evtWriter, attributeList, time, model,
                                   lockID, acquisitionOrder);
}

void EventProcessor::handleThreadTaskCreate(OTF2_AttributeList *attributeList,
                                            OTF2_TimeStamp time,
                                            OTF2_CommRef threadTeam,
                                            uint32_t creatingThread,
                                            uint32_t generationNumber) {

  time = m_maps.getNewTimestamp(time);

  threadTeam = m_maps.getCommID(threadTeam);

  OTF2_EvtWriter_ThreadTaskCreate(m_evtWriter, attributeList, time, threadTeam,
                                  creatingThread, generationNumber);
}

void EventProcessor::handleThreadTaskSwitch(OTF2_AttributeList *attributeList,
                                            OTF2_TimeStamp time,
                                            OTF2_CommRef threadTeam,
                                            uint32_t creatingThread,
                                            uint32_t generationNumber) {

  time = m_maps.getNewTimestamp(time);

  threadTeam = m_maps.getCommID(threadTeam);

  OTF2_EvtWriter_ThreadTaskSwitch(m_evtWriter, attributeList, time, threadTeam,
                                  creatingThread, generationNumber);
}

void EventProcessor::handleThreadTaskComplete(OTF2_AttributeList *attributeList,
                                              OTF2_TimeStamp time,
                                              OTF2_CommRef threadTeam,
                                              uint32_t creatingThread,
                                              uint32_t generationNumber) {
  time = m_maps.getNewTimestamp(time);

  threadTeam = m_maps.getCommID(threadTeam);

  OTF2_EvtWriter_ThreadTaskComplete(m_evtWriter, attributeList, time,
                                    threadTeam, creatingThread,
                                    generationNumber);
}

void EventProcessor::handleThreadCreate(OTF2_AttributeList *attributeList,
                                        OTF2_TimeStamp time,
                                        OTF2_CommRef threadContingent,
                                        uint64_t sequenceCount) {

  time = m_maps.getNewTimestamp(time);

  threadContingent = m_maps.getCommID(threadContingent);

  OTF2_EvtWriter_ThreadCreate(m_evtWriter, attributeList, time,
                              threadContingent, sequenceCount);
}

void EventProcessor::handleThreadBegin(OTF2_AttributeList *attributeList,
                                       OTF2_TimeStamp time,
                                       OTF2_CommRef threadContingent,
                                       uint64_t sequenceCount) {

  time = m_maps.getNewTimestamp(time);

  threadContingent = m_maps.getCommID(threadContingent);

  OTF2_EvtWriter_ThreadBegin(m_evtWriter, attributeList, time, threadContingent,
                             sequenceCount);
}

void EventProcessor::handleThreadWait(OTF2_AttributeList *attributeList,
                                      OTF2_TimeStamp time,
                                      OTF2_CommRef threadContingent,
                                      uint64_t sequenceCount) {

  time = m_maps.getNewTimestamp(time);

  threadContingent = m_maps.getCommID(threadContingent);

  OTF2_EvtWriter_ThreadWait(m_evtWriter, attributeList, time, threadContingent,
                            sequenceCount);
}

void EventProcessor::handleThreadEnd(OTF2_AttributeList *attributeList,
                                     OTF2_TimeStamp time,
                                     OTF2_CommRef threadContingent,
                                     uint64_t sequenceCount) {

  time = m_maps.getNewTimestamp(time);

  threadContingent = m_maps.getCommID(threadContingent);

  OTF2_EvtWriter_ThreadEnd(m_evtWriter, attributeList, time, threadContingent,
                           sequenceCount);
}

void EventProcessor::handleIoCreateHandle(OTF2_AttributeList *attributeList,
                                          OTF2_TimeStamp time,
                                          OTF2_IoHandleRef handle,
                                          OTF2_IoAccessMode mode,
                                          OTF2_IoCreationFlag creationFlags,
                                          OTF2_IoStatusFlag statusFlags) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoCreateHandle(m_evtWriter, attributeList, time, handle, mode,
                                creationFlags, statusFlags);
}

void EventProcessor::handleIoDestroyHandle(OTF2_AttributeList *attributeList,
                                           OTF2_TimeStamp time,
                                           OTF2_IoHandleRef handle) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoDestroyHandle(m_evtWriter, attributeList, time, handle);
}

void EventProcessor::handleIoHandleDuplicate(OTF2_AttributeList *attributeList,
                                             OTF2_TimeStamp time,
                                             OTF2_IoHandleRef oldHandle,
                                             OTF2_IoHandleRef newHandle,
                                             OTF2_IoStatusFlag statusFlags) {

  time = m_maps.getNewTimestamp(time);

  oldHandle = m_maps.getIoHandle(oldHandle);

  newHandle = m_maps.getIoHandle(newHandle);

  OTF2_EvtWriter_IoDuplicateHandle(m_evtWriter, attributeList, time, oldHandle,
                                   newHandle, statusFlags);
}

void EventProcessor::handleIoSeek(OTF2_AttributeList *attributeList,
                                  OTF2_TimeStamp time, OTF2_IoHandleRef handle,
                                  int64_t offsetRequest,
                                  OTF2_IoSeekOption whence,
                                  uint64_t offsetResult) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoSeek(m_evtWriter, attributeList, time, handle, offsetRequest,
                        whence, offsetResult);
}

void EventProcessor::handleIoChangeStatusFlags(
    OTF2_AttributeList *attributeList, OTF2_TimeStamp time,
    OTF2_IoHandleRef handle, OTF2_IoStatusFlag statusFlags) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoChangeStatusFlags(m_evtWriter, attributeList, time, handle,
                                     statusFlags);
}

void EventProcessor::handleIoDeleteFile(OTF2_AttributeList *attributeList,
                                        OTF2_TimeStamp time,
                                        OTF2_IoParadigmRef ioParadigm,
                                        OTF2_IoFileRef file) {

  time = m_maps.getNewTimestamp(time);

  ioParadigm = m_maps.getIoParadigmID(ioParadigm);

  file = m_maps.getIoFileAndDirectory(file);

  OTF2_EvtWriter_IoDeleteFile(m_evtWriter, attributeList, time, ioParadigm,
                              file);
}

void EventProcessor::handleIoOperationBegin(OTF2_AttributeList *attributeList,
                                            OTF2_TimeStamp time,
                                            OTF2_IoHandleRef handle,
                                            OTF2_IoOperationMode mode,
                                            OTF2_IoOperationFlag operationFlags,
                                            uint64_t bytesRequest,
                                            uint64_t matchingId) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoOperationBegin(m_evtWriter, attributeList, time, handle,
                                  mode, operationFlags, bytesRequest,
                                  matchingId);
}

void EventProcessor::handleIoOperationTest(OTF2_AttributeList *attributeList,
                                           OTF2_TimeStamp time,
                                           OTF2_IoHandleRef handle,
                                           uint64_t matchingId) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoOperationTest(m_evtWriter, attributeList, time, handle,
                                 matchingId);
}

void EventProcessor::handleIoOperationIssued(OTF2_AttributeList *attributeList,
                                             OTF2_TimeStamp time,
                                             OTF2_IoHandleRef handle,
                                             uint64_t matchingId) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoOperationIssued(m_evtWriter, attributeList, time, handle,
                                   matchingId);
}

void EventProcessor::handleIoOperationComplete(
    OTF2_AttributeList *attributeList, OTF2_TimeStamp time,
    OTF2_IoHandleRef handle, uint64_t bytesResult, uint64_t matchingId) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoOperationComplete(m_evtWriter, attributeList, time, handle,
                                     bytesResult, matchingId);
}

void EventProcessor::handleIoOperationCancelled(
    OTF2_AttributeList *attributeList, OTF2_TimeStamp time,
    OTF2_IoHandleRef handle, uint64_t matchingId) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoOperationCancelled(m_evtWriter, attributeList, time, handle,
                                      matchingId);
}

void EventProcessor::handleIoAcquireLock(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time,
                                         OTF2_IoHandleRef handle,
                                         OTF2_LockType lockType) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoAcquireLock(m_evtWriter, attributeList, time, handle,
                               lockType);
}

void EventProcessor::handleIoReleaseLock(OTF2_AttributeList *attributeList,
                                         OTF2_TimeStamp time,
                                         OTF2_IoHandleRef handle,
                                         OTF2_LockType lockType) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoReleaseLock(m_evtWriter, attributeList, time, handle,
                               lockType);
}

void EventProcessor::handleIoTryLock(OTF2_AttributeList *attributeList,
                                     OTF2_TimeStamp time,
                                     OTF2_IoHandleRef handle,
                                     OTF2_LockType lockType) {

  time = m_maps.getNewTimestamp(time);

  handle = m_maps.getIoHandle(handle);

  OTF2_EvtWriter_IoTryLock(m_evtWriter, attributeList, time, handle, lockType);
}

EventProcessor::~EventProcessor() {
  OTF2_Reader_CloseEvtFiles(m_otf2Reader);
  OTF2_Archive_CloseEvtFiles(m_archive);
  OTF2_Reader_Close(m_otf2Reader);
}
