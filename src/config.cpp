#include <config.h>

Config::Config(int argc, char **argv) : m_argc{argc}, m_argv{argv} {};

void Config::help() {
  std::cout << "OTF2 Merger v1.0 unifies multiple input traces into absolute "
               "one. \n\n";

  std::cout
      << "Commands:\n\n"
         "  --traceFile <tfile>: Set trace files with absloute paths. (req.)\n"
         "  --commGroupPattern <pname>: Provide a pattern name, to be followed "
         "by comm and group defintions in the unified trace. (req.)\n"
         "  --outputPath <path>: Set an output trace path. (req.)\n"
         "  --mergedTraceName <tname>: Set an output trace name, by default "
         "\"traces\". \n"
         "  --version: Display otf2 merger version.\n"
         "  --help: Show help. \n\n";
  std::cout << "Required (req.)\n";
  exit(1);
}

void Config::processArgs() {

  std::string traceFile{" "};
  std::string traceName{" "};
  std::string outputPath{" "};
  std::string mergedTraceName{" "};
  m_archive_name = "traces";

  const char *const short_opts = "f:n:o::m:v:h";

  const option long_opts[] = {
      {"traceFile", required_argument, nullptr, 'f'},
      {"commGroupPattern", required_argument, nullptr, 'n'},
      {"outputPath", required_argument, nullptr, 'o'},
      {"mergedTraceName", required_argument, nullptr, 'm'},
      {"version", no_argument, nullptr, 'v'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, no_argument, nullptr, 0}};

  while (true) {
    const auto opt =
        getopt_long_only(m_argc, m_argv, short_opts, long_opts, nullptr);

    if (-1 == opt)
      break;

    switch (opt) {
    case 'f':
      m_trace_files.push_back(std::string(optarg));
      break;

    case 'n':
      m_comm_group_pattern = std::string(optarg);
      break;

    case 'o':
      m_archive_path = std::string(optarg);
      break;

    case 'm':
      m_archive_name = std::string(optarg);
      break;

    case 'v':
      std::cout << " OTF2_MERGER v1.0 \n";
      exit(0);
      break;

    case 'h': // -h or --help
    case '?': // Unrecognized option

    default:
      help();
      break;
    }
  }
  if (m_trace_files.size() == 0)
    help();
}