#include "guitest.h"
#include "gui.h"
#include <GL/glut.h>

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
  // Construct the four classes required by the innards of the logic simulator
  nmz = new names();
  netz = new network(nmz);
  dmz = new devices(nmz, netz);
  mmz = new monitor(nmz, netz);
  devlink d;
  name sw1 = nmz->newname("Switch1");
  name sw2 = nmz->newname("Switch2");
  bool ok;
  dmz->makedevice(aswitch,sw1, 0, ok);
  dmz->makedevice(aswitch,sw2, 1, ok);
  mmz->makemonitor(sw1,blankname,ok);
  mmz->makemonitor(sw2,blankname,ok);
  // glutInit cannot cope with Unicode command line arguments, so we pass
  // it some fake ASCII ones instead
  char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
  // Construct the GUI
  MyFrame *frame = new MyFrame(NULL, wxT("Logic simulator"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz);
  frame->Show(true);
  return(true); // enter the GUI event loop
}
