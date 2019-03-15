#ifndef CONFIG_H
#define CONFIG_H
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

class Config {

public:
  Config(int argc, char** argv);
  void processArgs();

private:
  void help();

public:
  std::vector<std::string> m_trace_files;
  std::string 			   m_comm_group_pattern;
  std::string              m_archive_path;
  std::string              m_archive_name;

private:
  int    m_argc;
  char** m_argv;
};
#endif