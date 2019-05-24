#include "config.h"
#include "definition_reader.h"
#include "definition_store.h"
#include "definition_writer.h"
#include "event_processor.h"
#include "maps.h"
#include <iostream>
using namespace std;

extern "C" {

static OTF2_FlushType

pre_flush([[maybe_unused]] void *userData,
          [[maybe_unused]] OTF2_FileType fileType,
          [[maybe_unused]] OTF2_LocationRef location,
          [[maybe_unused]] void *callerData, [[maybe_unused]] bool final) {

  return OTF2_FLUSH;
}

static OTF2_TimeStamp

post_flush([[maybe_unused]] void *userData,
           [[maybe_unused]] OTF2_FileType fileType,
           [[maybe_unused]] OTF2_LocationRef location) {

  return 0;
}

static OTF2_FlushCallbacks

    flush_callbacks = {.otf2_pre_flush = pre_flush, .otf2_post_flush = NULL};
}

int main(int argc, char **argv) {

  Config args(argc, argv);
  args.processArgs();

  OTF2_Reader *reader = NULL;
  const uint64_t CHUNK_SIZE_EVENTS = 1024 * 1024;
  const uint64_t CHUNK_SIZE_DEFS = 4 * 1024 * 1024;
  const OTF2_FileSubstrate file_substrate = OTF2_SUBSTRATE_POSIX;
  const OTF2_Compression compression = OTF2_COMPRESSION_NONE;

  OTF2_Archive *archive = OTF2_Archive_Open(
      args.m_archive_path.c_str(), args.m_archive_name.c_str(),
      OTF2_FILEMODE_WRITE, CHUNK_SIZE_EVENTS, CHUNK_SIZE_DEFS, file_substrate,
      compression);

  OTF2_Archive_SetSerialCollectiveCallbacks(archive);
  OTF2_Archive_SetFlushCallbacks(archive, &flush_callbacks, NULL);

  DefinitionStore traceDefs;

  for (size_t i{}; i < args.m_trace_files.size(); i++) {
    Maps maps(traceDefs);

    DefinitionReader defReader(traceDefs, maps, reader, args.m_trace_files[i],
                               args.m_comm_group_pattern + "_" + to_string(i));

    defReader.read();

    EventProcessor evtProcessor(maps, reader, archive, args.m_trace_files[i]);

    evtProcessor.readLocationWise();
  }

  DefinitionWriter defWriter(archive, traceDefs);

  defWriter.write();

  OTF2_Archive_Close(archive);

  return EXIT_SUCCESS;
}
