#ifndef DEFINITION_STORE_H
#define DEFINITION_STORE_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <otf2/otf2.h>
#include <set>
#include <stdlib.h>
#include <string.h>
#include <tuple>
#include <vector>

using namespace std;

struct ClockProperties {

  uint64_t s_timerResolution;
  uint64_t s_globalOffset;
  uint64_t s_traceLength;
  string s_traceFileName;
};

struct Region {

  OTF2_StringRef s_name;
  OTF2_StringRef s_canonicalName;
  OTF2_StringRef s_description;
  OTF2_RegionRole s_regionRole;
  OTF2_Paradigm s_paradigm;
  OTF2_RegionFlag s_regionFlags;
  OTF2_StringRef s_sourceFile;
  uint32_t s_beginLineNumber;
  uint32_t s_endLineNumber;

  bool operator==(const Region &rhs) {
    return (rhs.s_name == this->s_name &&
            rhs.s_description == this->s_description &&
            rhs.s_canonicalName == this->s_canonicalName &&
            rhs.s_sourceFile == this->s_sourceFile &&
            rhs.s_beginLineNumber == this->s_beginLineNumber &&
            rhs.s_endLineNumber == this->s_endLineNumber);
  }
};

struct Paradigm {

  OTF2_Paradigm s_paradigm;
  OTF2_StringRef s_name;
  OTF2_ParadigmClass s_paradigmClass;

  bool operator==(const Paradigm &rhs) {
    return (rhs.s_paradigm == this->s_paradigm);
  }
};

struct ParadigmProperty {

  OTF2_Paradigm s_paradigm;
  OTF2_ParadigmProperty s_property;
  OTF2_Type s_type;
  OTF2_AttributeValue s_value;

  bool operator==(const ParadigmProperty &rhs) {
    return (rhs.s_paradigm == this->s_paradigm &&
            rhs.s_property == this->s_property && rhs.s_type == this->s_type);
  }
};

struct IoParadigm {

  OTF2_StringRef s_identification;
  OTF2_StringRef s_name;
  OTF2_IoParadigmClass s_ioParadigmClass;
  OTF2_IoParadigmFlag s_ioParadigmFlags;
  vector<uint8_t> s_ioProperties;
  vector<uint8_t> s_types;
  vector<OTF2_AttributeValue> s_values;

  bool operator==(const IoParadigm &rhs) {
    return (rhs.s_identification == this->s_identification &&
            rhs.s_name == this->s_name &&
            rhs.s_ioParadigmClass == this->s_ioParadigmClass &&
            rhs.s_ioParadigmFlags == this->s_ioParadigmFlags &&
            rhs.s_ioProperties == this->s_ioProperties &&
            rhs.s_types == this->s_types);
  }
};

struct SystemTreeNode {

  OTF2_SystemTreeNodeRef s_self;
  OTF2_StringRef s_name;
  OTF2_StringRef s_className;
  OTF2_SystemTreeNodeRef s_parent;
  bool s_newRootparent;

  bool operator==(const SystemTreeNode &rhs) {
    return (rhs.s_name == this->s_name &&
            rhs.s_className == this->s_className &&
            rhs.s_parent == this->s_parent);
  }
};

struct SystemTreeNodeProperty {

  OTF2_SystemTreeNodeRef s_systemTreeNode;
  OTF2_StringRef s_name;
  OTF2_Type s_type;
  OTF2_AttributeValue s_value;

  bool operator==(const SystemTreeNodeProperty &rhs) {
    return (rhs.s_systemTreeNode == this->s_systemTreeNode &&
            rhs.s_name == this->s_name && rhs.s_type == this->s_type &&
            rhs.s_value.uint32 == this->s_value.uint32);
  }
};

struct SystemTreeNodeDomain {

  OTF2_SystemTreeNodeRef s_systemTreeNode;
  OTF2_SystemTreeDomain s_systemTreeDomain;

  bool operator==(const SystemTreeNodeDomain &rhs) {
    return (rhs.s_systemTreeNode == this->s_systemTreeNode &&
            rhs.s_systemTreeDomain == this->s_systemTreeDomain);
  }
};

struct LocationGroup {

  OTF2_StringRef s_name;
  OTF2_LocationGroupType s_locationGroupType;
  OTF2_SystemTreeNodeRef s_systemTreeParent;
};

struct Location {

  OTF2_LocationRef s_locationsID;
  OTF2_StringRef s_name;
  OTF2_LocationType s_locationType;
  uint64_t s_numberOfEvents;
  OTF2_LocationGroupRef s_locationGroup;
};

struct Group {

  OTF2_StringRef s_name;
  OTF2_GroupType s_groupType;
  OTF2_Paradigm s_paradigm;
  OTF2_GroupFlag s_groupFlags;
  vector<size_t> s_groupMembers;

  bool operator==(const Group &rhs) {
    return (rhs.s_name == this->s_name &&
            rhs.s_groupType == this->s_groupType &&
            rhs.s_paradigm == this->s_paradigm &&
            // checking all group members
            rhs.s_groupMembers == this->s_groupMembers);
  }
};

struct Comm {

  OTF2_StringRef s_name;
  OTF2_GroupRef s_group;
  OTF2_CommRef s_parent;
};

struct Parameter {

  OTF2_StringRef s_name;
  OTF2_ParameterType s_parameterType;

  bool operator==(const Parameter &rhs) {
    return (rhs.s_name == this->s_name &&
            rhs.s_parameterType == this->s_parameterType);
  }
};

struct Attribute {

  OTF2_StringRef s_name;
  OTF2_StringRef s_description;
  OTF2_Type s_type;

  bool operator==(const Attribute &rhs) {
    return (rhs.s_name == this->s_name &&
            rhs.s_description == this->s_description);
  }
};

struct IoFileDefinition {

  OTF2_StringRef s_name;
  OTF2_SystemTreeNodeRef s_scope;
  string s_ioType;

  set<tuple<OTF2_StringRef, OTF2_Type, uint32_t>> s_ioProperty;

  bool operator==(const IoFileDefinition &rhs) const {
    return std::tie(this->s_name, this->s_scope, this->s_ioType,
                    this->s_ioProperty) ==
           std::tie(rhs.s_name, rhs.s_scope, rhs.s_ioType, rhs.s_ioProperty);
  }
};

struct IoHandle {

  OTF2_IoHandleRef s_self;
  OTF2_StringRef s_name;
  OTF2_IoFileRef s_file;
  OTF2_IoParadigmRef s_ioParadigm;
  OTF2_IoHandleFlag s_IoHandleFlags;
  OTF2_CommRef s_comm;
  OTF2_IoHandleRef s_parent;

  bool operator==(const IoHandle &rhs) {
    return (rhs.s_name == this->s_name && rhs.s_file == this->s_file &&
            rhs.s_ioParadigm == this->s_ioParadigm &&
            rhs.s_IoHandleFlags == this->s_IoHandleFlags &&
            rhs.s_comm == this->s_comm && rhs.s_parent == this->s_parent);
  }
};

struct IoPreCreatedHandleState {

  OTF2_IoHandleRef s_IoHandle;
  OTF2_IoAccessMode s_mode;
  OTF2_IoStatusFlag s_statusFlags;

  bool operator==(const IoPreCreatedHandleState &rhs) {
    return (rhs.s_IoHandle == this->s_IoHandle && rhs.s_mode == this->s_mode &&
            rhs.s_statusFlags == this->s_statusFlags);
  }
};

class DefinitionStore {

public:
  DefinitionStore();

  void insertDefinition(const ClockProperties &clockProps);
  const std::vector<ClockProperties> &getClockProperties() const;

  size_t insertDefinition(const string &string);
  std::tuple<vector<string>::iterator, vector<string>::iterator>
  getStringRange();

  size_t insertDefinition(const Region &region);
  std::tuple<vector<Region>::iterator, vector<Region>::iterator>
  getRegionRange();

  void insertDefinition(const Paradigm &paradigm);
  std::tuple<vector<Paradigm>::iterator, vector<Paradigm>::iterator>
  getParadigmRange();

  void insertDefinition(const ParadigmProperty &paradigmProps);
  std::tuple<vector<ParadigmProperty>::iterator,
             vector<ParadigmProperty>::iterator>
  getParadigmPropertyRange();

  size_t insertDefinition(const IoParadigm &ioParadigm);
  size_t appendIoParadigm(const IoParadigm &ioParadigm);
  std::tuple<vector<IoParadigm>::iterator, vector<IoParadigm>::iterator>
  getIoParadigmRange();

  bool hasAbstractNode();
  size_t insertDefinition(SystemTreeNode &sysTreeNode);
  uint32_t getAbstractParent(const size_t sysNodeName);

  std::tuple<vector<SystemTreeNode>::iterator, vector<SystemTreeNode>::iterator>
  getSystemTreeNodeRange();

  void insertDefinition(const SystemTreeNodeProperty &sysTreeNodeProperty);
  std::tuple<vector<SystemTreeNodeProperty>::iterator,
             vector<SystemTreeNodeProperty>::iterator>
  getSystemTreeNodePropertyRange();

  void insertDefinition(const SystemTreeNodeDomain &sysTreeNodeDomain);
  std::tuple<vector<SystemTreeNodeDomain>::iterator,
             vector<SystemTreeNodeDomain>::iterator>
  getSystemTreeNodeDomainRange();

  size_t insertDefinition(const LocationGroup &locationgroup);
  std::tuple<vector<LocationGroup>::iterator, vector<LocationGroup>::iterator>
  getLocationGroupRange();

  size_t insertDefinition(const Location &location);
  std::tuple<vector<Location>::iterator, vector<Location>::iterator>
  getLocationRange();

  size_t insertDefinition(const Group &group);
  size_t appendGroupLocations(const Group &group);
  size_t getCommGroupMemberCount(const Group &group);
  std::tuple<vector<Group>::iterator, vector<Group>::iterator> getGroupRange();

  size_t insertDefinition(const Comm &comm);
  std::tuple<vector<Comm>::iterator, vector<Comm>::iterator> getCommRange();

  void insertDefinition(const Parameter &parameters);
  std::tuple<vector<Parameter>::iterator, vector<Parameter>::iterator>
  getParameterRange();

  void insertDefinition(const Attribute &attributes);
  std::tuple<vector<Attribute>::iterator, vector<Attribute>::iterator>
  getAttributeRange();

  size_t insertDefinition(const IoFileDefinition &ioFileDefinition);
  std::tuple<vector<IoFileDefinition>::iterator,
             vector<IoFileDefinition>::iterator>
  getIoFileDefinitionRange();

  size_t insertDefinition(const IoHandle &ioHandle);
  std::tuple<vector<IoHandle>::iterator, vector<IoHandle>::iterator>
  getIoHandleRange();

  void insertDefinition(const IoPreCreatedHandleState &ioPreCreatedHandle);
  std::tuple<vector<IoPreCreatedHandleState>::iterator,
             vector<IoPreCreatedHandleState>::iterator>
  getIoPreCreatedHandle();

private:
  vector<ClockProperties> m_clockProps;
  vector<string> m_stringName;
  vector<Region> m_regionDb;
  vector<Paradigm> m_paradigmDb;
  vector<ParadigmProperty> m_paradigmPropertyDb;
  vector<IoParadigm> m_ioParadigmDb;
  vector<SystemTreeNode> m_sysTreeNodeDb;
  bool m_reviewSystemRoot;
  vector<SystemTreeNodeProperty> m_sysTreeNodePropertyDb;
  vector<SystemTreeNodeDomain> m_sysTreeNodeDomainDb;
  vector<LocationGroup> m_locationGroupDb;
  vector<Location> m_locationDb;
  vector<double> m_relativeClockFactor;
  vector<Group> m_groupDb;
  vector<Comm> m_commDb;
  vector<Parameter> m_parameterDb;
  vector<Attribute> m_attributeDb;
  vector<IoFileDefinition> m_ioFileDefinitionDb;
  vector<IoHandle> m_ioHandleDb;
  vector<IoPreCreatedHandleState> m_ioPreCreatedHandleDb;
};

#endif