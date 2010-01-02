#include <Ogre.h>
#include <iostream>

// This is a test of the BSA archive handler for OGRE.

#include "../bsa_archive.cpp"

using namespace Ogre;
using namespace std;

int main()
{
  // Disable Ogre logging
  new LogManager;
  Log *log = LogManager::getSingleton().createLog("");
  log->setDebugOutputEnabled(false);

  // Set up Root
  Root *root = new Root("","","");

  // Add the archive manager
  ArchiveManager::getSingleton().addArchiveFactory( new BSAArchiveFactory );

  // Add Morrowind.bsa
  ResourceGroupManager::getSingleton().
    addResourceLocation("../../data/Morrowind.bsa", "BSA", "General");

  // Pick a sample file
  String tex = "textures\\tx_natural_cavern_wall13.dds";
  cout << "Opening file: " << tex << endl;

  // Get it from the resource system
  DataStreamPtr data = ResourceGroupManager::getSingleton().openResource(tex, "General");

  cout << "Size: " << data->size() << endl;

  return 0;
}
