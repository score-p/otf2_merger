#define protected public
#define private public
#include "definition_store.h"
#include "maps.h"
#undef protected
#undef private
#include "catch.hpp"

void printFileDefinitionFromMaps(Maps &maps);
void insertIoFileDefinitionToDatabase(Maps &maps, DefinitionIntegrator &di);

SCENARIO("IO TEST CASE ID #1 : To evaluate the IO File and its property "
         "related functions/features in the src code.") {
  DefinitionIntegrator di;

  GIVEN(" The files and their properties from trace id 1") {
    WHEN("The files are given") {

      Maps maps(&di);

      IoFileDefinition io_file_def1{10, 0, "File"};
      maps.insertIoFile(io_file_def1);

      IoFileDefinition io_file_def2{11, 0, "File"};
      maps.insertIoFile(io_file_def2);

      IoFileDefinition io_file_def3{12, 0, "File"};
      maps.insertIoFile(io_file_def3);

      IoFileDefinition io_file_def4{12, 0, "File"};
      maps.insertIoFile(io_file_def4);

      IoFileDefinition io_file_def5{1, 0, "File"};
      maps.insertIoFile(io_file_def5);

      REQUIRE(maps.m_ioFileDefinition.size() == 5);

      THEN("Reading the respective file properties and insert the data to the "
           "DefinitionIntegrator") {

        maps.insertIoFileProperty(0, make_tuple(1, -1, 2));
        maps.insertIoFileProperty(0, make_tuple(2, -1, 2));
        maps.insertIoFileProperty(0, make_tuple(3, -1, 2));
        REQUIRE(maps.m_ioFileDefinition[0].s_IoProperty.size() == 3);

        maps.insertIoFileProperty(1, make_tuple(1, -1, 3));
        maps.insertIoFileProperty(1, make_tuple(2, -1, 3));
        REQUIRE(maps.m_ioFileDefinition[1].s_IoProperty.size() == 2);

        maps.insertIoFileProperty(2, make_tuple(1, -1, 4));
        maps.insertIoFileProperty(2, make_tuple(2, -1, 4));
        REQUIRE(maps.m_ioFileDefinition[2].s_IoProperty.size() == 2);

        maps.insertIoFileProperty(3, make_tuple(1, -1, 4));
        maps.insertIoFileProperty(3, make_tuple(2, -1, 4));
        REQUIRE(maps.m_ioFileDefinition[3].s_IoProperty.size() == 2);

        maps.insertIoFileProperty(4, make_tuple(0, -1, 2));
        maps.insertIoFileProperty(4, make_tuple(0, -1, 2));
        REQUIRE(maps.m_ioFileDefinition[4].s_IoProperty.size() == 1);

        maps.insertIoFileDefinitionToDatabase();

        REQUIRE(di.m_ioFileDefinitionDb.size() == 4);
        REQUIRE(di.m_ioFileDefinitionDb[0].s_Name == 10);
        REQUIRE(di.m_ioFileDefinitionDb[1].s_Name == 11);
        REQUIRE(di.m_ioFileDefinitionDb[0].s_IoProperty.size() == 3);
        REQUIRE(di.m_ioFileDefinitionDb[2].s_IoProperty.size() == 2);

        Maps maps1(&di);

        IoFileDefinition io_file_def6{10, 0, "File"};
        maps1.insertIoFile(io_file_def6);

        IoFileDefinition io_file_def7{21, 0, "File"};
        maps1.insertIoFile(io_file_def7);

        IoFileDefinition io_file_def8{12, 0, "File"};
        maps1.insertIoFile(io_file_def8);

        IoFileDefinition io_file_def9{22, 0, "File"};
        maps1.insertIoFile(io_file_def9);

        IoFileDefinition io_file_def10{23, 0, "File"};
        maps1.insertIoFile(io_file_def10);

        REQUIRE(maps1.m_ioFileDefinition.size() == 5);
        maps1.insertIoFileProperty(0, make_tuple(0, -1, 2));
        maps1.insertIoFileProperty(0, make_tuple(2, -1, 2));
        maps1.insertIoFileProperty(0, make_tuple(3, -1, 2));
        REQUIRE(maps1.m_ioFileDefinition[0].s_IoProperty.size() == 3);

        maps1.insertIoFileProperty(1, make_tuple(1, -1, 3));
        maps1.insertIoFileProperty(1, make_tuple(2, -1, 3));
        REQUIRE(maps1.m_ioFileDefinition[1].s_IoProperty.size() == 2);

        maps1.insertIoFileProperty(2, make_tuple(1, -1, 4));
        maps1.insertIoFileProperty(2, make_tuple(2, -1, 4));
        REQUIRE(maps1.m_ioFileDefinition[2].s_IoProperty.size() == 2);

        maps1.insertIoFileProperty(3, make_tuple(1, -1, 4));
        maps1.insertIoFileProperty(3, make_tuple(2, -1, 4));
        REQUIRE(maps1.m_ioFileDefinition[3].s_IoProperty.size() == 2);

        maps1.insertIoFileProperty(4, make_tuple(0, -1, 2));
        maps1.insertIoFileProperty(4, make_tuple(0, -1, 2));
        REQUIRE(maps1.m_ioFileDefinition[4].s_IoProperty.size() == 1);

        maps1.insertIoFileDefinitionToDatabase();

        REQUIRE(di.m_ioFileDefinitionDb.size() == 8);
        REQUIRE(di.m_ioFileDefinitionDb[4].s_Name == 10);
        REQUIRE(di.m_ioFileDefinitionDb[5].s_Name == 21);
        REQUIRE(di.m_ioFileDefinitionDb[4].s_IoProperty.size() == 3);
        REQUIRE(di.m_ioFileDefinitionDb[7].s_IoProperty.size() == 1);
      }
    }
  }
}
