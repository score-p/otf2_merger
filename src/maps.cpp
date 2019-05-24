#include "maps.h"

Maps::Maps(DefinitionStore& traceDefs)
    : m_traceDefs{traceDefs}, m_serialTrace{true}, m_relativeClockFactor{} {};

void Maps::ComputeRelativeClockFactor() {
  auto&  clockProps(m_traceDefs.getClockProperties());
  size_t clock_id{clockProps.size() - 1};

  m_relativeClockFactor =
    (double(clockProps[0].s_timerResolution) / double(clockProps[clock_id].s_timerResolution));
}

size_t Maps::getNewTimestamp(size_t oldTimestamp) {
  auto&  clockProps(m_traceDefs.getClockProperties());
  size_t clock_id{clockProps.size() - 1};

  size_t new_timeStamp =
    (oldTimestamp - clockProps[clock_id].s_globalOffset) * m_relativeClockFactor +
    clockProps[0].s_globalOffset;

  return new_timeStamp;
}

void Maps::mapString(size_t stringID) {
  m_stringMap.push_back(stringID);
}

size_t Maps::getUnifiedStringID(size_t oldStringID) {
  if (oldStringID < m_stringMap.size()) {
    return m_stringMap[oldStringID];
  } else
    return 0;
}

void Maps::mapRegion(size_t regionID) {
  m_regionMap.push_back(regionID);
}

size_t Maps::getRegionID(size_t oldRegionID) {
  if (oldRegionID < m_regionMap.size()) {
    return m_regionMap[oldRegionID];
  } else
    return 0;
}

void Maps::mapIoParadigm(size_t ioParadigmID) {
  m_IoParadigmMap.push_back(ioParadigmID);
}

size_t Maps::getIoParadigmID(size_t oldID) {
  if (oldID < m_IoParadigmMap.size()) {
    return m_IoParadigmMap[oldID];
  } else
    return 0;
}

void Maps::mapLocationGroup(size_t locationGroupID) {
  m_locationGroupIdMap.push_back(locationGroupID);
}

size_t Maps::getLocationGroupID(size_t oldLocationGroupID) {
  if (oldLocationGroupID < m_locationGroupIdMap.size()) {
    return m_locationGroupIdMap[oldLocationGroupID];
  } else
    return 0;
}

void Maps::mapLocation(size_t locationID, size_t newLocationID) {
  m_locationIdMap.insert({locationID, newLocationID});
}

size_t Maps::getNewLocationID(size_t oldLocationID) {
  return m_locationIdMap.at(oldLocationID);
}

std::tuple<map<size_t, size_t>::iterator, map<size_t, size_t>::iterator>
Maps::getLocationRange() {
  auto start_itr = m_locationIdMap.begin();
  auto end_itr = m_locationIdMap.end();
  return make_tuple(start_itr, end_itr);
}

void Maps::mapGroup(size_t groupID) {
  m_groupMap.push_back(groupID);
  m_serialTrace = false;
}

void Maps::mapCommGroupMemberID(uint8_t paradigm, size_t memberCount) {
  m_CommGroupMemberID.insert({paradigm, memberCount});
}

size_t Maps::getCommGroupMemberID(uint8_t paradigm) {
  return m_CommGroupMemberID.at(paradigm);
}

size_t Maps::getGroupID(size_t oldGroupID) {
  if (oldGroupID < m_groupMap.size()) {
    return m_groupMap[oldGroupID];
  } else
    return 0;
}

void Maps::mapComm(size_t commID) {
  m_commMap.push_back(commID);
}

size_t Maps::getCommID(size_t oldCommID) {
  if (oldCommID < m_groupMap.size()) {
    return m_commMap[oldCommID];
  } else
    return 0;
}

void Maps::mapSystemTreeNode(size_t sysID) {
  m_sysTreeNodeMap.push_back(sysID);
}

size_t Maps::getSystemTreeNodeID(size_t oldSysID) {
  if (oldSysID < m_sysTreeNodeMap.size()) {
    return m_sysTreeNodeMap[oldSysID];
  } else
    return 0;
}

void Maps::insertIoFile(IoFileDefinition& ioFileDefinition) {
  m_ioFileDefinition.push_back(ioFileDefinition);
}

void Maps::insertIoFileProperty(const OTF2_IoFileRef&                             ioFileRef,
                                const tuple<OTF2_StringRef, OTF2_Type, uint32_t>& ioProperty) {
  m_ioFileDefinition[ioFileRef].s_ioProperty.insert(ioProperty);
}

void Maps::insertIoFileDefinitionToDatabase() {
  vector<IoFileDefinition>::iterator iter;
  for (iter = m_ioFileDefinition.begin(); iter != m_ioFileDefinition.end(); ++iter) {
    auto id = m_traceDefs.insertDefinition(*iter);
    mapIoFileAndDirectory(id);
  }
}

void Maps::mapIoFileAndDirectory(size_t fileOrDirId) {
  m_ioFileDirectoryMap.push_back(fileOrDirId);
}

size_t Maps::getIoFileAndDirectory(size_t oldFileOrDirID) {
  if (oldFileOrDirID < m_ioFileDirectoryMap.size()) {
    return m_ioFileDirectoryMap[oldFileOrDirID];
  } else
    return 0;
}

void Maps::mapIoHandle(size_t ioHandleID) {
  m_ioHandleMap.push_back(ioHandleID);
}

size_t Maps::getIoHandle(size_t oldHandleID) {
  if (oldHandleID < m_ioHandleMap.size()) {
    return m_ioHandleMap[oldHandleID];
  } else
    return 0;
}

bool Maps::isSerialTrace() {
  return m_serialTrace;
}

Group Maps::getSerialTraceGroup(int groupID) {
  Group group;

  group.s_name = 0;
  group.s_groupType = groupID + OTF2_GROUP_TYPE_COMM_LOCATIONS;
  group.s_paradigm = OTF2_PARADIGM_MPI;
  group.s_groupFlags = OTF2_GROUP_FLAG_NONE;

  return group;
}

void Maps::setSerialTraceCommNames() {
  m_serialGroupNames.push_back("MPI_COMM_WORLD");
  m_serialGroupNames.push_back("MPI_COMM_SELF");
}

Comm Maps::getSerialTraceComm(int CommID) {
  Comm comm;

  m_serialGroupNames[CommID];
  auto string_name_id(m_traceDefs.insertDefinition(m_serialGroupNames[CommID]));

  comm.s_name = string_name_id;
  comm.s_group = CommID + 1;
  comm.s_parent = OTF2_UNDEFINED_COMM;

  return comm;
}
