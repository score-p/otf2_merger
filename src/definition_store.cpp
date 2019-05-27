#include "definition_store.h"

DefinitionStore::DefinitionStore() : m_reviewSystemRoot{false} {};

void DefinitionStore::insertDefinition(const ClockProperties &clockProps) {
  m_clockProps.push_back(clockProps);
}

const std::vector<ClockProperties> &
DefinitionStore::getClockProperties() const {
  return m_clockProps;
}

size_t DefinitionStore::insertDefinition(const std::string &string) {
  auto string_name_itr = find(m_stringName.begin(), m_stringName.end(), string);
  if (string_name_itr != m_stringName.end()) {
    return string_name_itr - m_stringName.begin();
  }
  m_stringName.push_back(string);
  return m_stringName.size() - 1;
}

std::tuple<vector<string>::iterator, vector<string>::iterator>
DefinitionStore::getStringRange() {
  auto start_itr = m_stringName.begin();
  auto end_itr = m_stringName.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const Region &region) {
  auto region_itr = find(m_regionDb.begin(), m_regionDb.end(), region);
  if (region_itr != m_regionDb.end()) {
    return region_itr - m_regionDb.begin();
  }
  m_regionDb.push_back(region);
  return m_regionDb.size() - 1;
}

std::tuple<vector<Region>::iterator, vector<Region>::iterator>
DefinitionStore::getRegionRange() {
  auto start_itr = m_regionDb.begin();
  auto end_itr = m_regionDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(const Paradigm &paradigm) {
  auto paradigm_itr = find(m_paradigmDb.begin(), m_paradigmDb.end(), paradigm);
  if (paradigm_itr == m_paradigmDb.end()) {
    m_paradigmDb.push_back(paradigm);
  }
}

std::tuple<vector<Paradigm>::iterator, vector<Paradigm>::iterator>
DefinitionStore::getParadigmRange() {
  auto start_itr = m_paradigmDb.begin();
  auto end_itr = m_paradigmDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(const ParadigmProperty &paradigmProps) {
  auto paradigm_itr = find(m_paradigmPropertyDb.begin(),
                           m_paradigmPropertyDb.end(), paradigmProps);
  if (paradigm_itr == m_paradigmPropertyDb.end()) {
    m_paradigmPropertyDb.push_back(paradigmProps);
  }
}

std::tuple<vector<ParadigmProperty>::iterator,
           vector<ParadigmProperty>::iterator>
DefinitionStore::getParadigmPropertyRange() {
  auto start_itr = m_paradigmPropertyDb.begin();
  auto end_itr = m_paradigmPropertyDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const IoParadigm &ioParadigm) {
  auto io_itr = find(m_ioParadigmDb.begin(), m_ioParadigmDb.end(), ioParadigm);
  if (io_itr != m_ioParadigmDb.end()) {
    return io_itr - m_ioParadigmDb.begin();
  }
  m_ioParadigmDb.push_back(ioParadigm);
  return m_ioParadigmDb.size() - 1;
}

size_t DefinitionStore::appendIoParadigm(
    const IoParadigm &ioParadigm) { // lambda function as parameter are used
  auto io_itr =
      find_if(m_ioParadigmDb.begin(), m_ioParadigmDb.end(),
              [&ioParadigm](const IoParadigm &existingioParadigm) -> bool {
                if (existingioParadigm.s_identification ==
                        ioParadigm.s_identification &&
                    existingioParadigm.s_name == ioParadigm.s_name &&
                    existingioParadigm.s_ioParadigmClass ==
                        ioParadigm.s_ioParadigmClass &&
                    existingioParadigm.s_ioParadigmFlags ==
                        ioParadigm.s_ioParadigmFlags) {
                  return true;
                } else
                  return false;
              });
  if (io_itr == m_ioParadigmDb.end()) {
    auto io_paradigm_id = insertDefinition(ioParadigm);
    return io_paradigm_id;
  } else {
    io_itr->s_ioProperties.insert(io_itr->s_ioProperties.end(),
                                  ioParadigm.s_ioProperties.begin(),
                                  ioParadigm.s_ioProperties.end());

    io_itr->s_types.insert(io_itr->s_types.end(), ioParadigm.s_types.begin(),
                           ioParadigm.s_types.end());

    io_itr->s_values.insert(io_itr->s_values.end(), ioParadigm.s_values.begin(),
                            ioParadigm.s_values.end());

    return io_itr - m_ioParadigmDb.begin();
  }
}

std::tuple<vector<IoParadigm>::iterator, vector<IoParadigm>::iterator>
DefinitionStore::getIoParadigmRange() {
  auto start_itr = m_ioParadigmDb.begin();
  auto end_itr = m_ioParadigmDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(SystemTreeNode &sysTreeNode) {
  auto sysTree_itr =
      find(m_sysTreeNodeDb.begin(), m_sysTreeNodeDb.end(), sysTreeNode);
  if (sysTree_itr != m_sysTreeNodeDb.end()) {
    return sysTree_itr - m_sysTreeNodeDb.begin();
  }
  sysTreeNode.s_self = m_sysTreeNodeDb.size();
  m_sysTreeNodeDb.push_back(sysTreeNode);
  return m_sysTreeNodeDb.size() - 1;
}

bool DefinitionStore::hasAbstractNode() {
  if (m_sysTreeNodeDb.size() == 0) {
    m_reviewSystemRoot = true;
    return true;
  }
  return false;
}

uint32_t DefinitionStore::getAbstractParent(const size_t sysNodeName) {
  if (m_sysTreeNodeDb.size() > 1) {
    if (m_sysTreeNodeDb[1].s_name != sysNodeName) {
      m_sysTreeNodeDb[0].s_newRootparent = true;
      return 0;
    }
  }
  return OTF2_UNDEFINED_SYSTEM_TREE_NODE;
}

std::tuple<vector<SystemTreeNode>::iterator, vector<SystemTreeNode>::iterator>
DefinitionStore::getSystemTreeNodeRange() {
  auto start_itr = m_sysTreeNodeDb.begin();
  auto end_itr = m_sysTreeNodeDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(
    const SystemTreeNodeProperty &sysTreeNodeProperty) {
  auto sysTree_itr = find(m_sysTreeNodePropertyDb.begin(),
                          m_sysTreeNodePropertyDb.end(), sysTreeNodeProperty);
  if (sysTree_itr == m_sysTreeNodePropertyDb.end()) {
    m_sysTreeNodePropertyDb.push_back(sysTreeNodeProperty);
  }
}

std::tuple<vector<SystemTreeNodeProperty>::iterator,
           vector<SystemTreeNodeProperty>::iterator>
DefinitionStore::getSystemTreeNodePropertyRange() {
  auto start_itr = m_sysTreeNodePropertyDb.begin();
  auto end_itr = m_sysTreeNodePropertyDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(
    const SystemTreeNodeDomain &sysTreeNodeDomain) {
  auto sysTree_itr = find(m_sysTreeNodeDomainDb.begin(),
                          m_sysTreeNodeDomainDb.end(), sysTreeNodeDomain);
  if (sysTree_itr == m_sysTreeNodeDomainDb.end()) {
    m_sysTreeNodeDomainDb.push_back(sysTreeNodeDomain);
  }
}

std::tuple<vector<SystemTreeNodeDomain>::iterator,
           vector<SystemTreeNodeDomain>::iterator>
DefinitionStore::getSystemTreeNodeDomainRange() {
  auto start_itr = m_sysTreeNodeDomainDb.begin();
  auto end_itr = m_sysTreeNodeDomainDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const LocationGroup &locationGroup) {
  m_locationGroupDb.push_back(locationGroup);
  return m_locationGroupDb.size() - 1;
}

std::tuple<vector<LocationGroup>::iterator, vector<LocationGroup>::iterator>
DefinitionStore::getLocationGroupRange() {
  auto start_itr = m_locationGroupDb.begin();
  auto end_itr = m_locationGroupDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const Location &location) {
  m_locationDb.push_back(location);
  return m_locationDb.size() - 1;
}

std::tuple<vector<Location>::iterator, vector<Location>::iterator>
DefinitionStore::getLocationRange() {
  auto start_itr = m_locationDb.begin();
  auto end_itr = m_locationDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const Group &group) {
  auto group_itr = find(m_groupDb.begin(), m_groupDb.end(), group);
  if (group_itr != m_groupDb.end()) {
    return group_itr - m_groupDb.begin();
  }
  m_groupDb.push_back(group);
  return m_groupDb.size() - 1;
}

size_t DefinitionStore::appendGroupLocations(
    const Group &group) { // lambda function as parameter has been used
  auto group_itr =
      find_if(m_groupDb.begin(), m_groupDb.end(),
              [&group](const Group &existingGroup) -> bool {
                if (existingGroup.s_name == group.s_name &&
                    existingGroup.s_groupType == group.s_groupType &&
                    existingGroup.s_paradigm == group.s_paradigm) {
                  return true;
                } else
                  return false;
              });
  if (group_itr == m_groupDb.end()) {
    auto group_id = insertDefinition(group);
    return group_id;
  } else {
    group_itr->s_groupMembers.insert(group_itr->s_groupMembers.end(),
                                     group.s_groupMembers.begin(),
                                     group.s_groupMembers.end());
    return group_itr - m_groupDb.begin();
  }
}

size_t DefinitionStore::getCommGroupMemberCount(const Group &group) {
  vector<Group>::reverse_iterator group_itr =
      find_if(m_groupDb.rbegin(), m_groupDb.rend(),
              [&group](const Group &existingGroup) -> bool {
                if (existingGroup.s_groupType == group.s_groupType &&
                    existingGroup.s_paradigm == group.s_paradigm) {
                  return true;
                } else
                  return false;
              });
  if (group_itr == m_groupDb.rend()) {
    return 0;
  } else {
    return group_itr->s_groupMembers.size();
  }
}

std::tuple<vector<Group>::iterator, vector<Group>::iterator>
DefinitionStore::getGroupRange() {
  auto start_itr = m_groupDb.begin();
  auto end_itr = m_groupDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const Comm &comm) {
  m_commDb.push_back(comm);
  return m_commDb.size() - 1;
}

std::tuple<vector<Comm>::iterator, vector<Comm>::iterator>
DefinitionStore::getCommRange() {
  auto start_itr = m_commDb.begin();
  auto end_itr = m_commDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(const Parameter &parameters) {
  auto parameter_itr =
      find(m_parameterDb.begin(), m_parameterDb.end(), parameters);
  if (parameter_itr == m_parameterDb.end()) {
    m_parameterDb.push_back(parameters);
  }
}

std::tuple<vector<Parameter>::iterator, vector<Parameter>::iterator>
DefinitionStore::getParameterRange() {
  auto start_itr = m_parameterDb.begin();
  auto end_itr = m_parameterDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(const Attribute &attributes) {
  auto attribute_itr =
      find(m_attributeDb.begin(), m_attributeDb.end(), attributes);
  if (attribute_itr == m_attributeDb.end()) {
    m_attributeDb.push_back(attributes);
  }
}

std::tuple<vector<Attribute>::iterator, vector<Attribute>::iterator>
DefinitionStore::getAttributeRange() {
  auto start_itr = m_attributeDb.begin();
  auto end_itr = m_attributeDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t
DefinitionStore::insertDefinition(const IoFileDefinition &ioFileDefinition) {

  auto io_itr = find(m_ioFileDefinitionDb.begin(), m_ioFileDefinitionDb.end(),
                     ioFileDefinition);
  if (io_itr != m_ioFileDefinitionDb.end()) {
    return io_itr - m_ioFileDefinitionDb.begin();
  }
  m_ioFileDefinitionDb.push_back(ioFileDefinition);
  return m_ioFileDefinitionDb.size() - 1;
}

std::tuple<vector<IoFileDefinition>::iterator,
           vector<IoFileDefinition>::iterator>
DefinitionStore::getIoFileDefinitionRange() {
  auto start_itr = m_ioFileDefinitionDb.begin();
  auto end_itr = m_ioFileDefinitionDb.end();
  return make_tuple(start_itr, end_itr);
}

size_t DefinitionStore::insertDefinition(const IoHandle &ioHandle) {
  auto io_itr = find(m_ioHandleDb.begin(), m_ioHandleDb.end(), ioHandle);
  if (io_itr != m_ioHandleDb.end()) {
    return io_itr - m_ioHandleDb.begin();
  }
  m_ioHandleDb.push_back(ioHandle);
  return m_ioHandleDb.size() - 1;
}

std::tuple<vector<IoHandle>::iterator, vector<IoHandle>::iterator>
DefinitionStore::getIoHandleRange() {
  auto start_itr = m_ioHandleDb.begin();
  auto end_itr = m_ioHandleDb.end();
  return make_tuple(start_itr, end_itr);
}

void DefinitionStore::insertDefinition(
    const IoPreCreatedHandleState &ioPreCreatedHandle) {
  auto io_itr = find(m_ioPreCreatedHandleDb.begin(),
                     m_ioPreCreatedHandleDb.end(), ioPreCreatedHandle);
  if (io_itr == m_ioPreCreatedHandleDb.end()) {
    m_ioPreCreatedHandleDb.push_back(ioPreCreatedHandle);
  }
}

std::tuple<vector<IoPreCreatedHandleState>::iterator,
           vector<IoPreCreatedHandleState>::iterator>
DefinitionStore::getIoPreCreatedHandle() {
  auto start_itr = m_ioPreCreatedHandleDb.begin();
  auto end_itr = m_ioPreCreatedHandleDb.end();
  return make_tuple(start_itr, end_itr);
}
