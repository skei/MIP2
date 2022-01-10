
//#define KODE_GUI_CAIRO
#define KODE_NO_GUI
//#define KODE_GUI_XCB
#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/kode.socket

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"

//#include "sa_botage_effects.h"

#include "sa_botage_descriptor.h"
#include "sa_botage_editor.h"
#include "sa_botage_process.h"
#include "sa_botage_instance.h"

//----------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
