#include "definition_writer.h"

using namespace std;

size_t compareTraceLength( const ClockProperties& clockProps1,
                           const ClockProperties& clockProps2 );

DefinitionWriter::DefinitionWriter( OTF2_Archive* archive, DefinitionStore& traceDefs )
    : m_traceDefs( traceDefs ), m_archive( archive ) {
  m_globalDefWriter = OTF2_Archive_GetGlobalDefWriter( m_archive );
}

void DefinitionWriter::write() {
  writeClockProperties();
  writeString();
  writeParadigm();
  writeParadigmProperty();
  writeIoParadigm();
  writeSystemTreeNode();
  writeSystemTreeNodeProperty();
  writeSystemTreeNodeDomain();
  writeRegion();
  writeLocationGroup();
  writeLocation();
  writeGroup();
  writeComm();
  writeParameter();
  writeAttribute();
  writeIoFileDefinition();
  writeIoHandle();
  writeIoPreCreatedHandle();
}

void DefinitionWriter::writeClockProperties() {
  auto& clockProps( m_traceDefs.getClockProperties() );

  auto total_length_itr =
    max_element( clockProps.begin(), clockProps.end(), compareTraceLength );

  OTF2_GlobalDefWriter_WriteClockProperties( m_globalDefWriter,
                                             clockProps[ 0 ].s_timerResolution,
                                             clockProps[ 0 ].s_globalOffset,
                                             total_length_itr->s_traceLength );
}

size_t compareTraceLength( const ClockProperties& clockProps1,
                           const ClockProperties& clockProps2 ) {
  return clockProps2.s_traceLength > clockProps1.s_traceLength;
}

void DefinitionWriter::writeString() {
  auto string_tuple( m_traceDefs.getStringRange() );

  auto start_itr{std::get< 0 >( string_tuple )};
  auto end_itr{std::get< 1 >( string_tuple )};

  for ( auto string_itr = start_itr; string_itr != end_itr; string_itr++ ) {
    auto id{string_itr - start_itr};
    OTF2_GlobalDefWriter_WriteString(
      m_globalDefWriter, id, ( *string_itr ).c_str() );
  }
}

void DefinitionWriter::writeParadigm() {
  auto paradigm_tuple( m_traceDefs.getParadigmRange() );

  auto start_itr{std::get< 0 >( paradigm_tuple )};
  auto end_itr{std::get< 1 >( paradigm_tuple )};

  for ( auto paradigm_itr = start_itr; paradigm_itr != end_itr;
        paradigm_itr++ ) {

    OTF2_GlobalDefWriter_WriteParadigm( m_globalDefWriter,
                                        paradigm_itr->s_paradigm,
                                        paradigm_itr->s_name,
                                        paradigm_itr->s_paradigmClass );
  }
}

void DefinitionWriter::writeParadigmProperty() {
  auto paradigm_property_tuple( m_traceDefs.getParadigmPropertyRange() );

  auto start_itr{std::get< 0 >( paradigm_property_tuple )};
  auto end_itr{std::get< 1 >( paradigm_property_tuple )};

  for ( auto paradigm_property_itr = start_itr;
        paradigm_property_itr != end_itr;
        paradigm_property_itr++ ) {

    OTF2_GlobalDefWriter_WriteParadigmProperty(
      m_globalDefWriter,
      paradigm_property_itr->s_paradigm,
      paradigm_property_itr->s_property,
      paradigm_property_itr->s_type,
      paradigm_property_itr->s_value );
  }
}

void DefinitionWriter::writeIoParadigm() {
  auto io_paradigm_tuple( m_traceDefs.getIoParadigmRange() );

  auto start_itr{std::get< 0 >( io_paradigm_tuple )};
  auto end_itr{std::get< 1 >( io_paradigm_tuple )};

  for ( auto io_paradigm_itr = start_itr; io_paradigm_itr != end_itr;
        io_paradigm_itr++ ) {
    auto id{io_paradigm_itr - start_itr};
    vector< uint8_t >::iterator it1{io_paradigm_itr->s_ioProperties.begin()};
    uint8_t*  io_props{&( *it1 )};
    vector< uint8_t >::iterator it2{io_paradigm_itr->s_types.begin()};
    uint8_t*  types{&( *it2 )};
    vector< OTF2_AttributeValue >::iterator it3{io_paradigm_itr->s_values.begin()};
    OTF2_AttributeValue* values{&( *it3 )};

    OTF2_GlobalDefWriter_WriteIoParadigm(
      m_globalDefWriter,
      id,
      io_paradigm_itr->s_identification,
      io_paradigm_itr->s_name,
      io_paradigm_itr->s_ioParadigmClass,
      io_paradigm_itr->s_ioParadigmFlags,
      io_paradigm_itr->s_ioProperties.size(),
      io_props,
      types,
      values );
  }
}

void DefinitionWriter::writeSystemTreeNode() {
  auto sys_tree_node_tuple( m_traceDefs.getSystemTreeNodeRange() );

  auto   start_itr{std::get< 0 >( sys_tree_node_tuple )};
  auto   end_itr{std::get< 1 >( sys_tree_node_tuple )};
  size_t start_id{};
  if ( start_itr->s_newRootparent != true ) {
    start_itr = start_itr + 1;
    start_id = 1;
  } else
    ( start_itr + 1 )->s_parent = 0;

  for ( auto sys_tree_node_itr = start_itr; sys_tree_node_itr != end_itr;
        sys_tree_node_itr++ ) {
    auto id{start_id + sys_tree_node_itr - start_itr};
    OTF2_GlobalDefWriter_WriteSystemTreeNode( m_globalDefWriter,
                                              id,
                                              sys_tree_node_itr->s_name,
                                              sys_tree_node_itr->s_className,
                                              sys_tree_node_itr->s_parent );
  }
}

void DefinitionWriter::writeSystemTreeNodeProperty() {
  auto sys_tree_node_property_tuple(
    m_traceDefs.getSystemTreeNodePropertyRange() );

  auto start_itr{std::get< 0 >( sys_tree_node_property_tuple )};
  auto end_itr{std::get< 1 >( sys_tree_node_property_tuple )};

  for ( auto sys_tree_node_property_itr = start_itr;
        sys_tree_node_property_itr != end_itr;
        sys_tree_node_property_itr++ ) {

    OTF2_GlobalDefWriter_WriteSystemTreeNodeProperty(
      m_globalDefWriter,
      sys_tree_node_property_itr->s_systemTreeNode,
      sys_tree_node_property_itr->s_name,
      sys_tree_node_property_itr->s_type,
      sys_tree_node_property_itr->s_value );
  }
}

void DefinitionWriter::writeSystemTreeNodeDomain() {
  auto sys_tree_node_domain_tuple( m_traceDefs.getSystemTreeNodeDomainRange() );

  auto start_itr{std::get< 0 >( sys_tree_node_domain_tuple )};
  auto end_itr{std::get< 1 >( sys_tree_node_domain_tuple )};

  for ( auto sys_tree_node_domain_itr = start_itr;
        sys_tree_node_domain_itr != end_itr;
        sys_tree_node_domain_itr++ ) {

    OTF2_GlobalDefWriter_WriteSystemTreeNodeDomain(
      m_globalDefWriter,
      sys_tree_node_domain_itr->s_systemTreeNode,
      sys_tree_node_domain_itr->s_systemTreeDomain );
  }
}

void DefinitionWriter::writeRegion() {
  auto region_tuple( m_traceDefs.getRegionRange() );

  auto start_itr{std::get< 0 >( region_tuple )};
  auto end_itr{std::get< 1 >( region_tuple )};

  for ( auto region_itr = start_itr; region_itr != end_itr; region_itr++ ) {

    auto id{region_itr - start_itr};

    OTF2_GlobalDefWriter_WriteRegion( m_globalDefWriter,
                                      id,
                                      region_itr->s_name,
                                      region_itr->s_canonicalName,
                                      region_itr->s_description,
                                      region_itr->s_regionRole,
                                      region_itr->s_paradigm,
                                      region_itr->s_regionFlags,
                                      region_itr->s_sourceFile,
                                      region_itr->s_beginLineNumber,
                                      region_itr->s_endLineNumber );
  }
}

void DefinitionWriter::writeLocationGroup() {

  auto location_group_tuple( m_traceDefs.getLocationGroupRange() );

  auto start_itr{std::get< 0 >( location_group_tuple )};
  auto end_itr{std::get< 1 >( location_group_tuple )};

  for ( auto location_group_itr = start_itr; location_group_itr != end_itr;
        location_group_itr++ ) {

    auto id{location_group_itr - start_itr};

    OTF2_GlobalDefWriter_WriteLocationGroup(
      m_globalDefWriter,
      id,
      location_group_itr->s_name,
      location_group_itr->s_locationGroupType,
      location_group_itr->s_systemTreeParent );
  }
}

void DefinitionWriter::writeLocation() {

  auto location_tuple( m_traceDefs.getLocationRange() );

  auto start_itr{std::get< 0 >( location_tuple )};
  auto end_itr{std::get< 1 >( location_tuple )};

  for ( auto location_itr = start_itr; location_itr != end_itr;
        location_itr++ ) {
    auto id{location_itr - start_itr};
    OTF2_GlobalDefWriter_WriteLocation( m_globalDefWriter,
                                        id,
                                        location_itr->s_name,
                                        location_itr->s_locationType,
                                        location_itr->s_numberOfEvents,
                                        location_itr->s_locationGroup );
  }
}

void DefinitionWriter::writeGroup() {

  auto group_tuple( m_traceDefs.getGroupRange() );

  auto start_itr{std::get< 0 >( group_tuple )};
  auto end_itr{std::get< 1 >( group_tuple )};

  for ( auto group_itr = start_itr; group_itr != end_itr; group_itr++ ) {
    auto id{group_itr - start_itr};
    vector< size_t >::iterator it{group_itr->s_groupMembers.begin()};
    size_t* p{&( *it )};
    OTF2_GlobalDefWriter_WriteGroup( m_globalDefWriter,
                                     id,
                                     group_itr->s_name,
                                     group_itr->s_groupType,
                                     group_itr->s_paradigm,
                                     group_itr->s_groupFlags,
                                     group_itr->s_groupMembers.size(),
                                     p );
  }
}

void DefinitionWriter::writeComm() {

  auto comm_tuple( m_traceDefs.getCommRange() );

  auto start_itr{std::get< 0 >( comm_tuple )};
  auto end_itr{std::get< 1 >( comm_tuple )};

  for ( auto comm_itr = start_itr; comm_itr != end_itr; comm_itr++ ) {
    auto id{comm_itr - start_itr};
    OTF2_GlobalDefWriter_WriteComm( m_globalDefWriter,
                                    id,
                                    comm_itr->s_name,
                                    comm_itr->s_group,
                                    comm_itr->s_parent );
  }
}

void DefinitionWriter::writeParameter() {
  auto parameter_tuple( m_traceDefs.getParameterRange() );

  auto start_itr{std::get< 0 >( parameter_tuple )};
  auto end_itr{std::get< 1 >( parameter_tuple )};

  for ( auto parameter_itr = start_itr; parameter_itr != end_itr;
        parameter_itr++ ) {
    auto id{parameter_itr - start_itr};
    OTF2_GlobalDefWriter_WriteParameter( m_globalDefWriter,
                                         id,
                                         parameter_itr->s_name,
                                         parameter_itr->s_parameterType );
  }
}

void DefinitionWriter::writeAttribute() {
  auto attribute_tuple( m_traceDefs.getAttributeRange() );

  auto start_itr{std::get< 0 >( attribute_tuple )};
  auto end_itr{std::get< 1 >( attribute_tuple )};

  for ( auto attribute_itr = start_itr; attribute_itr != end_itr;
        attribute_itr++ ) {
    auto id{attribute_itr - start_itr};
    OTF2_GlobalDefWriter_WriteAttribute( m_globalDefWriter,
                                         id,
                                         attribute_itr->s_name,
                                         attribute_itr->s_description,
                                         attribute_itr->s_type );
  }
}

void DefinitionWriter::writeIoFileDefinition() {
  auto io_tuple( m_traceDefs.getIoFileDefinitionRange() );

  auto start_itr{std::get< 0 >( io_tuple )};
  auto end_itr{std::get< 1 >( io_tuple )};

  for ( auto io_itr = start_itr; io_itr != end_itr; io_itr++ ) {
    auto id{io_itr - start_itr};

    if ( io_itr->s_ioType == "Directory" ) {
      OTF2_GlobalDefWriter_WriteIoDirectory(
        m_globalDefWriter, id, io_itr->s_name, io_itr->s_scope );
    } else {
      OTF2_GlobalDefWriter_WriteIoRegularFile(
        m_globalDefWriter, id, io_itr->s_name, io_itr->s_scope );
    }

    if ( io_itr->s_ioProperty.size() > 0 ) {
      set< tuple< OTF2_StringRef, OTF2_Type, uint32_t > >::iterator set_itr;
      OTF2_AttributeValue value;
      for ( set_itr = io_itr->s_ioProperty.begin();
            set_itr != io_itr->s_ioProperty.end();
            ++set_itr ) {

        value.uint32 = get< 2 >( *set_itr );

        OTF2_GlobalDefWriter_WriteIoFileProperty( m_globalDefWriter,
                                                  id,
                                                  get< 0 >( *set_itr ),
                                                  get< 1 >( *set_itr ),
                                                  value );
      }
    }
  }
}

void DefinitionWriter::writeIoHandle() {
  auto io_tuple( m_traceDefs.getIoHandleRange() );

  auto start_itr{std::get< 0 >( io_tuple )};
  auto end_itr{std::get< 1 >( io_tuple )};

  for ( auto io_tuple = start_itr; io_tuple != end_itr; io_tuple++ ) {
    auto id{io_tuple - start_itr};
    OTF2_GlobalDefWriter_WriteIoHandle( m_globalDefWriter,
                                        id,
                                        io_tuple->s_name,
                                        io_tuple->s_file,
                                        io_tuple->s_ioParadigm,
                                        io_tuple->s_IoHandleFlags,
                                        io_tuple->s_comm,
                                        io_tuple->s_parent );
  }
}

void DefinitionWriter::writeIoPreCreatedHandle() {
  auto io_tuple{m_traceDefs.getIoPreCreatedHandle()};

  auto start_itr{std::get< 0 >( io_tuple )};
  auto end_itr{std::get< 1 >( io_tuple )};

  for ( auto io_tuple = start_itr; io_tuple != end_itr; io_tuple++ ) {
    OTF2_GlobalDefWriter_WriteIoPreCreatedHandleState(
      m_globalDefWriter,
      io_tuple->s_IoHandle,
      io_tuple->s_mode,
      io_tuple->s_statusFlags );
  }
}
