#include "definition_reader.h"

using namespace std;

extern "C" {

static OTF2_CallbackCode

GlobalDefReaderCallback_ClockProperties(void *userData,
                                        uint64_t timerResolution,
                                        uint64_t globalOffset,
                                        uint64_t traceLength) {

  ClockProperties clock_props{timerResolution, globalOffset, traceLength, " "};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(clock_props);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_String(void *userData, OTF2_StringRef str,
                               const char *string) {

  std::string temp_string(string);

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(str, temp_string);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Paradigm(void *userData, OTF2_Paradigm paradigm,
                                 OTF2_StringRef name,
                                 OTF2_ParadigmClass paradigmClass) {

  // paradigmClass enum might need to be modified in future.
  Paradigm paradigms{paradigm, name, paradigmClass};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(paradigms);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_ParadigmProperty(void *userData, OTF2_Paradigm paradigm,
                                         OTF2_ParadigmProperty property,
                                         OTF2_Type type,
                                         OTF2_AttributeValue value) {

  ParadigmProperty paradigmProps{paradigm, property, type, value};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(paradigmProps);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoParadigm(
    void *userData, [[maybe_unused]] OTF2_IoParadigmRef self,
    OTF2_StringRef identification, OTF2_StringRef name,
    OTF2_IoParadigmClass ioParadigmClass, OTF2_IoParadigmFlag ioParadigmFlags,
    uint8_t numberOfProperties, const OTF2_IoParadigmProperty *properties,
    const OTF2_Type *types, const OTF2_AttributeValue *values) {

  IoParadigm ioParadigm{
      identification, name, ioParadigmClass, ioParadigmFlags, {}, {}, {}};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioParadigm, properties, types, values,
                           numberOfProperties);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_SystemTreeNode(void *userData,
                                       OTF2_SystemTreeNodeRef self,
                                       OTF2_StringRef name,
                                       OTF2_StringRef className,
                                       OTF2_SystemTreeNodeRef parent) {

  SystemTreeNode sysTreeNode{self, name, className, parent, false};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(sysTreeNode);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_SystemTreeNodeProperty(
    void *userData, OTF2_SystemTreeNodeRef systemTreeNode, OTF2_StringRef name,
    OTF2_Type type, OTF2_AttributeValue value) {

  SystemTreeNodeProperty sysTreeNodeProps{systemTreeNode, name, type, value};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(sysTreeNodeProps);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_SystemTreeNodeDomain(
    void *userData, OTF2_SystemTreeNodeRef systemTreeNode,
    OTF2_SystemTreeDomain systemTreeDomain) {

  SystemTreeNodeDomain sysTreeNodeDomain{systemTreeNode, systemTreeDomain};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(sysTreeNodeDomain);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Region(
    void *userData, [[maybe_unused]] OTF2_RegionRef self, OTF2_StringRef name,
    OTF2_StringRef canonicalName, OTF2_StringRef description,
    OTF2_RegionRole regionRole, OTF2_Paradigm paradigm,
    OTF2_RegionFlag regionFlags, OTF2_StringRef sourceFile,
    uint32_t beginLineNumber, uint32_t endLineNumber) {

  Region region_props{name,       canonicalName,   description,
                      regionRole, paradigm,        regionFlags,
                      sourceFile, beginLineNumber, endLineNumber};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);

  reader->handleDefinition(region_props);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_LocationGroup(
    void *userData, [[maybe_unused]] OTF2_LocationGroupRef self,
    OTF2_StringRef name, OTF2_LocationGroupType locationGroupType,
    OTF2_SystemTreeNodeRef systemTreeParent) {

  LocationGroup locationGroup_props{name, locationGroupType, systemTreeParent};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(locationGroup_props);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Location(void *userData, OTF2_LocationRef self,
                                 OTF2_StringRef name,
                                 OTF2_LocationType locationType,
                                 uint64_t numberOfEvents,
                                 OTF2_LocationGroupRef locationGroup) {

  Location location_props{self, name, locationType, numberOfEvents,
                          locationGroup};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(location_props);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Group(void *userData,
                              [[maybe_unused]] OTF2_GroupRef self,
                              OTF2_StringRef name, OTF2_GroupType groupType,
                              OTF2_Paradigm paradigm, OTF2_GroupFlag groupFlags,
                              uint32_t numberOfMembers,
                              const uint64_t *members) {

  Group group_props{name, groupType, paradigm, groupFlags, {}};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(group_props, members, numberOfMembers);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Comm(void *userData, [[maybe_unused]] OTF2_CommRef self,
                             OTF2_StringRef name, OTF2_GroupRef group,
                             [[maybe_unused]] OTF2_CommRef parent) {

  Comm comms{name, group, OTF2_UNDEFINED_UINT32};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(comms);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Parameter(void *userData,
                                  [[maybe_unused]] OTF2_ParameterRef self,
                                  OTF2_StringRef name,
                                  OTF2_ParameterType parameterType) {

  Parameter parameters{name, parameterType};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(parameters);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_Attribute(void *userData,
                                  [[maybe_unused]] OTF2_AttributeRef self,
                                  OTF2_StringRef name,
                                  OTF2_StringRef description, OTF2_Type type) {

  Attribute attributes{name, description, type};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(attributes);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoRegularFile(void *userData,
                                      [[maybe_unused]] OTF2_IoFileRef self,
                                      OTF2_StringRef name,
                                      OTF2_SystemTreeNodeRef scope) {

  IoFileDefinition ioFileDefinition{name, scope, "File", {}};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioFileDefinition);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoDirectory(void *userData,
                                    [[maybe_unused]] OTF2_IoFileRef self,
                                    OTF2_StringRef name,
                                    OTF2_SystemTreeNodeRef scope) {

  IoFileDefinition ioFileDefinition{name, scope, "Directory", {}};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioFileDefinition);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoFileProperty(void *userData, OTF2_IoFileRef ioFile,
                                       OTF2_StringRef name, OTF2_Type type,
                                       OTF2_AttributeValue value) {

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioFile, name, type, value);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoHandle(void *userData, OTF2_IoHandleRef self,
                                 OTF2_StringRef name, OTF2_IoFileRef file,
                                 OTF2_IoParadigmRef ioParadigm,
                                 OTF2_IoHandleFlag ioHandleFlags,
                                 OTF2_CommRef comm, OTF2_IoHandleRef parent) {

  IoHandle ioHandle{self, name, file, ioParadigm, ioHandleFlags, comm, parent};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioHandle);

  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode

GlobalDefReaderCallback_IoPreCreatedHandleState(void *userData,
                                                OTF2_IoHandleRef ioHandle,
                                                OTF2_IoAccessMode mode,
                                                OTF2_IoStatusFlag statusFlags) {

  IoPreCreatedHandleState ioPreCreatedHandle{ioHandle, mode, statusFlags};

  DefinitionReader *reader = static_cast<DefinitionReader *>(userData);
  reader->handleDefinition(ioPreCreatedHandle);

  return OTF2_CALLBACK_SUCCESS;
}
}

DefinitionReader::DefinitionReader(DefinitionStore &traceDefs, Maps &maps,
                                   OTF2_Reader *reader,
                                   const string &traceFileName,
                                   const string &traceName)
    : m_traceDefs{traceDefs}, m_maps{maps}, m_otf2Reader{reader},
      m_traceFileName{traceFileName}, m_traceName{traceName} {

  m_otf2Reader = OTF2_Reader_Open(m_traceFileName.c_str());

  OTF2_Reader_SetSerialCollectiveCallbacks(m_otf2Reader);

  m_otf2DefReader = OTF2_Reader_GetGlobalDefReader(m_otf2Reader);

  setup();
}

void DefinitionReader::read() {

  uint64_t definitions_read{};
  OTF2_Reader_ReadAllGlobalDefinitions(m_otf2Reader, m_otf2DefReader,
                                       &definitions_read);

  uint64_t def_anchor{};
  OTF2_Reader_GetNumberOfGlobalDefinitions(m_otf2Reader, &def_anchor);

  if (m_maps.isSerialTrace()) {
    createSerialGroup();
    m_maps.setSerialTraceCommNames();
    createSerialComm();
  }
}

void DefinitionReader::setup() {

  OTF2_GlobalDefReaderCallbacks *global_def_callbacks =
      OTF2_GlobalDefReaderCallbacks_New();

  OTF2_GlobalDefReaderCallbacks_SetClockPropertiesCallback(
      global_def_callbacks, &GlobalDefReaderCallback_ClockProperties);

  OTF2_GlobalDefReaderCallbacks_SetStringCallback(
      global_def_callbacks, &GlobalDefReaderCallback_String);

  OTF2_GlobalDefReaderCallbacks_SetParadigmCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Paradigm);

  OTF2_GlobalDefReaderCallbacks_SetParadigmPropertyCallback(
      global_def_callbacks, &GlobalDefReaderCallback_ParadigmProperty);

  OTF2_GlobalDefReaderCallbacks_SetIoParadigmCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoParadigm);

  OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeCallback(
      global_def_callbacks, &GlobalDefReaderCallback_SystemTreeNode);

  OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodePropertyCallback(
      global_def_callbacks, &GlobalDefReaderCallback_SystemTreeNodeProperty);

  OTF2_GlobalDefReaderCallbacks_SetSystemTreeNodeDomainCallback(
      global_def_callbacks, &GlobalDefReaderCallback_SystemTreeNodeDomain);

  OTF2_GlobalDefReaderCallbacks_SetRegionCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Region);

  OTF2_GlobalDefReaderCallbacks_SetLocationGroupCallback(
      global_def_callbacks, &GlobalDefReaderCallback_LocationGroup);

  OTF2_GlobalDefReaderCallbacks_SetLocationCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Location);

  OTF2_GlobalDefReaderCallbacks_SetGroupCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Group);

  OTF2_GlobalDefReaderCallbacks_SetCommCallback(global_def_callbacks,
                                                &GlobalDefReaderCallback_Comm);

  OTF2_GlobalDefReaderCallbacks_SetParameterCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Parameter);

  OTF2_GlobalDefReaderCallbacks_SetAttributeCallback(
      global_def_callbacks, &GlobalDefReaderCallback_Attribute);

  OTF2_GlobalDefReaderCallbacks_SetIoRegularFileCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoRegularFile);

  OTF2_GlobalDefReaderCallbacks_SetIoDirectoryCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoDirectory);

  OTF2_GlobalDefReaderCallbacks_SetIoFilePropertyCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoFileProperty);

  OTF2_GlobalDefReaderCallbacks_SetIoHandleCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoHandle);

  OTF2_GlobalDefReaderCallbacks_SetIoPreCreatedHandleStateCallback(
      global_def_callbacks, &GlobalDefReaderCallback_IoPreCreatedHandleState);

  OTF2_Reader_RegisterGlobalDefCallbacks(m_otf2Reader, m_otf2DefReader,
                                         global_def_callbacks, this);

  OTF2_GlobalDefReaderCallbacks_Delete(global_def_callbacks);
}

void DefinitionReader::handleDefinition(ClockProperties &clockProps) {
  clockProps.s_traceFileName = m_traceFileName;
  m_traceDefs.insertDefinition(clockProps);
}

void DefinitionReader::handleDefinition([[maybe_unused]] OTF2_StringRef str,
                                        const string &string) {
  auto string_name_id(m_traceDefs.insertDefinition(string));
  m_maps.mapString(string_name_id);
}

void DefinitionReader::handleDefinition(Paradigm &paradigm) {
  paradigm.s_name = m_maps.getUnifiedStringID(paradigm.s_name);
  m_traceDefs.insertDefinition(paradigm);
}

void DefinitionReader::handleDefinition(
    IoParadigm &ioParadigm, const OTF2_IoParadigmProperty *properties,
    const OTF2_Type *types, const OTF2_AttributeValue *values,
    size_t numberOfProperties) {
  ioParadigm.s_identification =
      m_maps.getUnifiedStringID(ioParadigm.s_identification);
  ioParadigm.s_name = m_maps.getUnifiedStringID(ioParadigm.s_name);

  for (size_t i = 0; i < numberOfProperties; i++) {
    auto current_property = *(properties + i);
    ioParadigm.s_ioProperties.push_back(current_property);

    auto current_type = *(types + i);
    ioParadigm.s_types.push_back(current_type);

    auto current_value = *(values + i);
    ioParadigm.s_values.push_back(current_value);
  }

  auto io_paradigm_id(m_traceDefs.appendIoParadigm(ioParadigm));
  m_maps.mapIoParadigm(io_paradigm_id);
}

void DefinitionReader::handleDefinition(ParadigmProperty &paradigmProps) {
  m_traceDefs.insertDefinition(paradigmProps);
}

void DefinitionReader::handleDefinition(SystemTreeNode &sysTreeNode) {
  sysTreeNode.s_name = m_maps.getUnifiedStringID(sysTreeNode.s_name);
  sysTreeNode.s_className = m_maps.getUnifiedStringID(sysTreeNode.s_className);
  sysTreeNode.s_newRootparent = false;
  auto require_abstract_node(m_traceDefs.hasAbstractNode());
  if (require_abstract_node == true) {

    const std::string new_name{"HPC"};
    auto string_name_id(m_traceDefs.insertDefinition(new_name));

    SystemTreeNode sys_tree_node{0, (uint32_t)string_name_id,
                                 sysTreeNode.s_className,
                                 OTF2_UNDEFINED_SYSTEM_TREE_NODE, false};

    m_traceDefs.insertDefinition(sys_tree_node);
  }

  else {
    if (sysTreeNode.s_parent == OTF2_UNDEFINED_SYSTEM_TREE_NODE) {
      auto parent_id(m_traceDefs.verifyAbstractParent(sysTreeNode.s_name));
      sysTreeNode.s_parent = parent_id;
    } else {
      sysTreeNode.s_parent = m_maps.getSystemTreeNodeID(sysTreeNode.s_parent);
    }
  }

  auto sys_tree_node_id(m_traceDefs.insertDefinition(sysTreeNode));
  m_maps.mapSystemTreeNode(sys_tree_node_id);
}

void DefinitionReader::handleDefinition(
    SystemTreeNodeProperty &sysTreeNodeProps) {
  sysTreeNodeProps.s_systemTreeNode =
      m_maps.getSystemTreeNodeID(sysTreeNodeProps.s_systemTreeNode);

  sysTreeNodeProps.s_name = m_maps.getUnifiedStringID(sysTreeNodeProps.s_name);
  sysTreeNodeProps.s_value.uint32 =
      m_maps.getUnifiedStringID(sysTreeNodeProps.s_value.uint32);
  m_traceDefs.insertDefinition(sysTreeNodeProps);
}

void DefinitionReader::handleDefinition(
    SystemTreeNodeDomain &sysTreeNodeDomain) {
  sysTreeNodeDomain.s_systemTreeNode =
      m_maps.getSystemTreeNodeID(sysTreeNodeDomain.s_systemTreeNode);
  m_traceDefs.insertDefinition(sysTreeNodeDomain);
}

void DefinitionReader::handleDefinition(Region &region) {
  region.s_name = m_maps.getUnifiedStringID(region.s_name);
  region.s_canonicalName = m_maps.getUnifiedStringID(region.s_canonicalName);
  region.s_description = m_maps.getUnifiedStringID(region.s_description);
  region.s_sourceFile = m_maps.getUnifiedStringID(region.s_sourceFile);

  auto region_id(m_traceDefs.insertDefinition(region));
  m_maps.mapRegion(region_id);
}

void DefinitionReader::handleDefinition(LocationGroup &locationGroup_props) {
  // violation # 1 doesnot compute values outside the defintion_store/map
  locationGroup_props.s_name =
      m_maps.getUnifiedStringID(locationGroup_props.s_name);

  auto string_tuple(m_traceDefs.getStringRange());
  auto start_itr = std::get<0>(string_tuple);
  auto new_name = *(start_itr + locationGroup_props.s_name) + "_" + m_traceName;
  auto string_name_id(m_traceDefs.insertDefinition(new_name));

  locationGroup_props.s_name = string_name_id;

  if (locationGroup_props.s_systemTreeParent !=
      OTF2_UNDEFINED_SYSTEM_TREE_NODE) {
    locationGroup_props.s_systemTreeParent =
        m_maps.getSystemTreeNodeID(locationGroup_props.s_systemTreeParent);
  }

  auto location_group_id(m_traceDefs.insertDefinition(locationGroup_props));
  m_maps.mapLocationGroup(location_group_id);
}

void DefinitionReader::handleDefinition(Location &location_props) {
  location_props.s_name = m_maps.getUnifiedStringID(location_props.s_name);
  location_props.s_locationGroup =
      m_maps.getLocationGroupID(location_props.s_locationGroup);
  auto location_id(m_traceDefs.insertDefinition(location_props));
  m_maps.mapLocation(location_props.s_locationsID, location_id);
}

void DefinitionReader::handleDefinition(Group &group, const uint64_t *members,
                                        uint32_t numberOfMembers) {
  size_t group_id;
  size_t current_member;
  group.s_name = m_maps.getUnifiedStringID(group.s_name);

  for (size_t i{}; i < numberOfMembers; i++) {
    if (group.s_groupType == OTF2_GROUP_TYPE_COMM_LOCATIONS) {
      auto member_count(m_traceDefs.getCommGroupMemberCount(group));
      m_maps.mapCommGroupMemberID(group.s_paradigm, member_count);
      current_member = m_maps.getNewLocationID(*(members + i));
    } else {
      auto member_count(m_maps.getCommGroupMemberID(group.s_paradigm));
      current_member = *(members + i) + member_count;
    }
    group.s_groupMembers.push_back(current_member);
  }

  if (group.s_groupType == OTF2_GROUP_TYPE_COMM_LOCATIONS) {
    group_id = m_traceDefs.appendGroupLocations(group);
  } else {
    group_id = m_traceDefs.insertDefinition(group);
  }
  m_maps.mapGroup(group_id);
}

void DefinitionReader::handleDefinition(Comm &comms) {
  comms.s_name = m_maps.getUnifiedStringID(comms.s_name);

  auto string_tuple(m_traceDefs.getStringRange());
  auto start_itr = std::get<0>(string_tuple);

  const std::string new_name{*(start_itr + comms.s_name) + "_" + m_traceName};
  auto string_name_id(m_traceDefs.insertDefinition(new_name));
  comms.s_name = string_name_id;
  comms.s_group = m_maps.getGroupID(comms.s_group);

  auto comm_id(m_traceDefs.insertDefinition(comms));
  m_maps.mapComm(comm_id);
}

void DefinitionReader::handleDefinition(Parameter &parameters) {
  parameters.s_name = m_maps.getUnifiedStringID(parameters.s_name);
  parameters.s_parameterType =
      m_maps.getUnifiedStringID(parameters.s_parameterType);
  m_traceDefs.insertDefinition(parameters);
}

void DefinitionReader::handleDefinition(Attribute &attributes) {
  attributes.s_name = m_maps.getUnifiedStringID(attributes.s_name);
  attributes.s_description =
      m_maps.getUnifiedStringID(attributes.s_description);
  m_traceDefs.insertDefinition(attributes);
}

void DefinitionReader::handleDefinition(IoFileDefinition &ioFileDefinition) {
  ioFileDefinition.s_name = m_maps.getUnifiedStringID(ioFileDefinition.s_name);
  ioFileDefinition.s_scope =
      m_maps.getSystemTreeNodeID(ioFileDefinition.s_scope);
  m_maps.insertIoFile(ioFileDefinition);
}

void DefinitionReader::handleDefinition(OTF2_IoFileRef ioFile,
                                        OTF2_StringRef name, OTF2_Type type,
                                        OTF2_AttributeValue value) {

  m_maps.insertIoFileProperty(
      ioFile, make_tuple(m_maps.getUnifiedStringID(name), type,
                         m_maps.getUnifiedStringID(value.uint32)));
}

void DefinitionReader::handleDefinition(IoHandle &ioHandle) {
  if (ioHandle.s_self == 0) {
    m_maps.insertIoFileDefinitionToDatabase();
  }
  ioHandle.s_name = m_maps.getUnifiedStringID(ioHandle.s_name);
  if (ioHandle.s_file != OTF2_UNDEFINED_IO_FILE) {
    ioHandle.s_file = m_maps.getIoFileAndDirectory(ioHandle.s_file);
  }

  ioHandle.s_ioParadigm = m_maps.getIoParadigmID(ioHandle.s_ioParadigm);

  if (ioHandle.s_comm != OTF2_UNDEFINED_COMM) {
    ioHandle.s_comm = m_maps.getCommID(ioHandle.s_comm);
  }

  if (ioHandle.s_parent != OTF2_UNDEFINED_IO_HANDLE) {
    ioHandle.s_parent = m_maps.getIoHandle(ioHandle.s_parent);
  }

  auto io_handle_id(m_traceDefs.insertDefinition(ioHandle));
  m_maps.mapIoHandle(io_handle_id);
}

void DefinitionReader::handleDefinition(
    IoPreCreatedHandleState &ioPreCreatedHandle) {
  ioPreCreatedHandle.s_IoHandle =
      m_maps.getIoHandle(ioPreCreatedHandle.s_IoHandle);
  m_traceDefs.insertDefinition(ioPreCreatedHandle);
}

void DefinitionReader::createSerialGroup() {
  const int NUM_OF_GROUPS{3};

  for (int i{}; i < NUM_OF_GROUPS; i++) {
    auto group(m_maps.getSerialTraceGroup(i));
    uint64_t location{};
    if (group.s_groupType == OTF2_GROUP_TYPE_COMM_SELF) {
      handleDefinition(group, &location, 0);
    } else
      handleDefinition(group, &location, 1);
  }
}

void DefinitionReader::createSerialComm() {
  const int NUM_OF_COMMS{2};

  for (int i{}; i < NUM_OF_COMMS; i++) {
    auto comm(m_maps.getSerialTraceComm(i));
    handleDefinition(comm);
  }
}

DefinitionReader::~DefinitionReader() {
  OTF2_Reader_CloseGlobalDefReader(m_otf2Reader, m_otf2DefReader);
  OTF2_Reader_Close(m_otf2Reader);
}
