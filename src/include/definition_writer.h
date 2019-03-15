#ifndef DEFINITION_WRITER_H
#define DEFINITION_WRITER_H

#include "definition_store.h"
#include <otf2/otf2.h>

class DefinitionWriter {

public:
  DefinitionWriter( OTF2_Archive* archive, DefinitionStore& traceDefs);
  void write();

private:
  void writeClockProperties();

  void writeString();

  void writeParadigm();

  void writeParadigmProperty();

  void writeIoParadigm();

  void writeSystemTreeNode();

  void writeSystemTreeNodeProperty();

  void writeSystemTreeNodeDomain();

  void writeRegion();

  void writeLocationGroup();

  void writeLocation();

  void writeGroup();

  void writeComm();

  void writeParameter();

  void writeAttribute();

  void writeIoFileDefinition();

  void writeIoHandle();

  void writeIoPreCreatedHandle();

private:
  DefinitionStore&      m_traceDefs;
  string                m_archivePath;
  string                m_archiveName;
  OTF2_Archive*         m_archive;
  OTF2_GlobalDefWriter* m_globalDefWriter;
};

#endif