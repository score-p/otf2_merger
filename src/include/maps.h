#ifndef MAPS_H
#define MAPS_H
#include "definition_store.h"
#include <otf2/otf2.h>

class Maps {

public:
  Maps(DefinitionStore &traceDefs);

  DefinitionStore &m_traceDefs;

  void ComputeRelativeClockFactor();
  size_t getNewTimestamp(size_t oldTimestamp);

  void mapString(size_t stringID);
  size_t getUnifiedStringID(size_t oldStringID);

  void mapRegion(size_t regionID);
  size_t getRegionID(size_t oldRegionID);

  void mapIoParadigm(size_t ioParadigmID);
  size_t getIoParadigmID(size_t oldIoParadigmID);

  void mapLocationGroup(size_t locationGroupID);
  size_t getLocationGroupID(size_t oldLocationGroupID);

  void mapLocation(size_t locationID, size_t newLocationID);
  size_t getNewLocationID(size_t oldLocationID);
  std::tuple<map<size_t, size_t>::iterator, map<size_t, size_t>::iterator>
  getLocationRange();

  void mapGroup(size_t groupID);
  void mapCommGroupMemberID(uint8_t paradigm, size_t memberCount);
  size_t getCommGroupMemberID(uint8_t paradigm);
  size_t getGroupID(size_t oldGroupID);

  void mapComm(size_t commID);
  size_t getCommID(size_t oldCommID);

  void mapSystemTreeNode(size_t sysID);
  size_t getSystemTreeNodeID(size_t oldSysID);

  void insertIoFile(IoFileDefinition &ioFileDefinition);
  void insertIoFileProperty(
      const OTF2_IoFileRef &ioFileRef,
      const tuple<OTF2_StringRef, OTF2_Type, uint32_t> &ioProperty);
  void insertIoFileDefinitionToDatabase();

  void mapIoFileAndDirectory(size_t fileOrDirID);
  size_t getIoFileAndDirectory(size_t oldFileOrDirID);

  void mapIoHandle(size_t ioHandleID);
  size_t getIoHandle(size_t oldHandleID);

  bool isSerialTrace();
  Group getSerialTraceGroup(int groupID);

  void setSerialTraceCommNames();
  Comm getSerialTraceComm(int commID);

private:
  bool m_serialTrace;
  double m_relativeClockFactor;
  vector<string> m_serialGroupNames;
  vector<size_t> m_stringMap;
  vector<size_t> m_regionMap;
  vector<size_t> m_IoParadigmMap;
  vector<size_t> m_locationGroupIdMap;
  map<size_t, size_t> m_locationIdMap;
  vector<size_t> m_originalLocationId;
  map<uint8_t, size_t> m_CommGroupMemberID;
  vector<size_t> m_groupMap;
  vector<size_t> m_commMap;
  vector<size_t> m_sysTreeNodeMap;
  vector<size_t> m_ioFileDirectoryMap;
  vector<IoFileDefinition> m_ioFileDefinition;
  vector<size_t> m_ioHandleMap;
};

#endif