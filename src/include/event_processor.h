#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H
#include "definition_store.h"
#include "maps.h"
#include <otf2/otf2.h>

class EventProcessor {

public:
  EventProcessor(Maps& maps, OTF2_Reader* reader, OTF2_Archive* archive,
                 const string& traceName);

  Maps& m_maps;

  void readLocationWise();

  void handleEvtEnter(OTF2_TimeStamp time, OTF2_AttributeList* attributeList,
                      OTF2_RegionRef region);

  void handleEvtLeave(OTF2_TimeStamp time, OTF2_AttributeList* attributeList,
                      OTF2_RegionRef region);

  void handleMPISend(OTF2_AttributeList* attributeList, OTF2_TimeStamp time, uint32_t receiver,
                     OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength);

  void handleMPIRecv(OTF2_AttributeList* attributeList, OTF2_TimeStamp time, uint32_t sender,
                     OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength);

  void handleMPIISend(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                      uint32_t receiver, OTF2_CommRef communicator, uint32_t msgTag,
                      uint64_t msgLength, uint64_t requestID);

  void handleMPIIRecv(OTF2_AttributeList* attributeList, OTF2_TimeStamp time, uint32_t sender,
                      OTF2_CommRef communicator, uint32_t msgTag, uint64_t msgLength,
                      uint64_t requestID);

  void handleMPIISendComplete(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                              uint64_t requestID);

  void handleMPIIRecvRequest(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                             uint64_t requestID);

  void handleMPIRequestTest(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                            uint64_t requestID);

  void handleMPIRequestCancelled(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                                 uint64_t requestID);

  void handleMPIBeginProps(OTF2_AttributeList* attributeList, OTF2_TimeStamp time);

  void handleMPIEndProps(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                         OTF2_CollectiveOp collectiveOp, OTF2_CommRef communicator,
                         uint32_t root, uint64_t sizeSent, uint64_t sizeReceived);

  void handleOmpFork(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                     uint32_t numberOfRequestedThreads);

  void handleOmpJoin(OTF2_AttributeList* attributeList, OTF2_TimeStamp time);

  void handleOmpAcquireLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                            uint32_t lockID, uint32_t acquisitionOrder);

  void handleOmpReleaseLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                            uint32_t lockID, uint32_t acquisitionOrder);

  void handleOmpTaskCreate(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                           uint64_t taskID);

  void handleOmpTaskSwitch(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                           uint64_t taskID);

  void handleOmpTaskComplete(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                             uint64_t taskID);

  void handleThreadFork(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                        OTF2_Paradigm model, uint32_t numberOfRequestedThreads);

  void handleThreadJoin(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                        OTF2_Paradigm model);

  void handleThreadTeamBegin(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                             OTF2_CommRef threadTeam);

  void handleThreadTeamEnd(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                           OTF2_CommRef threadTeam);

  void handleThreadAcquireLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                               OTF2_Paradigm model, uint32_t lockID,
                               uint32_t acquisitionOrder);

  void handleThreadReleaseLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                               OTF2_Paradigm model, uint32_t lockID,
                               uint32_t acquisitionOrder);

  void handleThreadTaskCreate(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                              OTF2_CommRef threadTeam, uint32_t creatingThread,
                              uint32_t generationNumber);

  void handleThreadTaskSwitch(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                              OTF2_CommRef threadTeam, uint32_t creatingThread,
                              uint32_t generationNumber);

  void handleThreadTaskComplete(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                                OTF2_CommRef threadTeam, uint32_t creatingThread,
                                uint32_t generationNumber);

  void handleThreadCreate(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                          OTF2_CommRef threadContingent, uint64_t sequenceCount);

  void handleThreadBegin(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                         OTF2_CommRef threadContingent, uint64_t sequenceCount);

  void handleThreadWait(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                        OTF2_CommRef threadContingent, uint64_t sequenceCount);

  void handleThreadEnd(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                       OTF2_CommRef threadContingent, uint64_t sequenceCount);

  void handleIoCreateHandle(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                            OTF2_IoHandleRef handle, OTF2_IoAccessMode mode,
                            OTF2_IoCreationFlag creationFlags, OTF2_IoStatusFlag statusFlags);

  void handleIoDestroyHandle(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                             OTF2_IoHandleRef handle);

  void handleIoHandleDuplicate(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                               OTF2_IoHandleRef oldHandle, OTF2_IoHandleRef newHandle,
                               OTF2_IoStatusFlag statusFlags);

  void handleIoSeek(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                    OTF2_IoHandleRef handle, int64_t offsetRequest, OTF2_IoSeekOption whence,
                    uint64_t offsetResult);

  void handleIoChangeStatusFlags(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                                 OTF2_IoHandleRef handle, OTF2_IoStatusFlag statusFlags);

  void handleIoDeleteFile(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                          OTF2_IoParadigmRef ioParadigm, OTF2_IoFileRef file);

  void handleIoOperationBegin(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                              OTF2_IoHandleRef handle, OTF2_IoOperationMode mode,
                              OTF2_IoOperationFlag operationFlags, uint64_t bytesRequest,
                              uint64_t matchingId);

  void handleIoOperationTest(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                             OTF2_IoHandleRef handle, uint64_t matchingId);

  void handleIoOperationIssued(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                               OTF2_IoHandleRef handle, uint64_t matchingId);

  void handleIoOperationComplete(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                                 OTF2_IoHandleRef handle, uint64_t bytesResult,
                                 uint64_t matchingId);

  void handleIoOperationCancelled(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                                  OTF2_IoHandleRef handle, uint64_t matchingId);

  void handleIoAcquireLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                           OTF2_IoHandleRef handle, OTF2_LockType lockType);

  void handleIoReleaseLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                           OTF2_IoHandleRef handle, OTF2_LockType lockType);

  void handleIoTryLock(OTF2_AttributeList* attributeList, OTF2_TimeStamp time,
                       OTF2_IoHandleRef handle, OTF2_LockType lockType);

  ~EventProcessor();

private:
  OTF2_Reader*    m_otf2Reader;
  OTF2_EvtWriter* m_evtWriter;
  OTF2_Archive*   m_archive;
  string          m_traceName;
};

#endif