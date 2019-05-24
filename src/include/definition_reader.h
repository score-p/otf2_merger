#ifndef DEFINITION_READER_H
#define DEFINITION_READER_H

#include "definition_store.h"
#include "maps.h"
#include <otf2/otf2.h>

class DefinitionReader {

public:
  DefinitionReader(DefinitionStore &traceDefs, Maps &maps, OTF2_Reader *reader,
                   const string &traceFileName, const string &traceName);

  DefinitionStore &m_traceDefs;
  Maps &m_maps;

  void read();

  void handleDefinition(ClockProperties &clockProps);

  void handleDefinition(OTF2_StringRef str, const string &string);

  void handleDefinition(Paradigm &paradigm);

  void handleDefinition(ParadigmProperty &paradigmProps);

  void handleDefinition(IoParadigm &ioParadigm,
                        const OTF2_IoParadigmProperty *properties,
                        const OTF2_Type *types,
                        const OTF2_AttributeValue *values,
                        size_t numberOfProperties);

  void handleDefinition(SystemTreeNode &sysTreeNode);

  void handleDefinition(SystemTreeNodeProperty &sysTreeNodeProperty);

  void handleDefinition(SystemTreeNodeDomain &sysTreeNodeDomain);

  void handleDefinition(Region &region);

  void handleDefinition(LocationGroup &locationGroup_props);

  void handleDefinition(Location &location);

  void handleDefinition(Group &group, const uint64_t *members,
                        uint32_t numberOfMembers);

  void handleDefinition(Comm &comm);

  void handleDefinition(Parameter &parameters);

  void handleDefinition(Attribute &attributes);

  void handleDefinition(IoFileDefinition &ioFileDefinition);

  void handleDefinition(OTF2_IoFileRef ioFile, OTF2_StringRef name,
                        OTF2_Type type, OTF2_AttributeValue value);

  void handleDefinition(IoHandle &ioHandle);

  void handleDefinition(IoPreCreatedHandleState &ioPreCreatedHandle);

  ~DefinitionReader();

private:
  void setup();

  void createSerialGroup();

  void createSerialComm();

private:
  OTF2_Reader *m_otf2Reader;
  OTF2_GlobalDefReader *m_otf2DefReader;
  const string m_traceFileName;
  const string m_traceName;
};

#endif