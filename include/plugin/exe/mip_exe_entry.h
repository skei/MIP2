#ifndef mip_exe_entry_included
#define mip_exe_entry_included
//----------------------------------------------------------------------

/*
  todo: jack for audio/midi io
  implement: state, presets, ..
*/

#include "mip.h"
#include "plugin/mip_registry.h"
#include "plugin/clap/mip_clap_host_implementation.h"
#include "plugin/exe/mip_exe.h"
#include "plugin/exe/mip_exe_host_implementation.h"

#ifndef MIP_NO_GUI
  #include "gui/mip_window.h"
  #include "plugin/exe/mip_exe_window.h"
#endif // MIP_NO_GUI

//#ifdef MIP_USE_XCB
//#include "gui/xcb/mip_xcb.h"
//#endif

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

/*
  TODO: get clap_descriptor through the clap_entry
  same as a 'proper' wrapper.. so we could re-use code :-)

  idea: std argument parser for EXE...
  * cmd line to select plugin by index, or yb id, or search by name, etc..
  * override stuff..

  btw, why do we call the clap_plugin variants?
  we already have a wrapper MIP_Plugin
*/

int main(int argc, char** argv, char** env) {
  //MIP_PRINT;
  //MIP_REGISTRY.getNumDescriptors();
  uint32_t index = 0; //TODO: arg[1] select index
  uint32_t num_descriptors = MIP_REGISTRY.getNumDescriptors();
  //MIP_Print("> num_descriptors: %i\n",num_descriptors);
  //uint32_t num_descriptors = 1;
  if (num_descriptors > 0) {
    const clap_plugin_descriptor_t* descriptor = MIP_REGISTRY.getDescriptor(index);
    //const clap_plugin_descriptor_t* descriptor = &template_descriptor;
    //MIP_Print("descriptor: %p\n",descriptor);
    if (descriptor) {
      MIP_ExeHostImplementation* exe_host = new MIP_ExeHostImplementation();
      //MIP_Print("exe_host: %p\n",exe_host);
      if (exe_host) {
        const clap_host_t* host = exe_host->getHost();
        //MIP_Print("host: %p\n",host);
        if (host) {
          MIP_Plugin* plugin = MIP_CreatePlugin(index,descriptor,host);
          //MIP_Print("plugin: %p\n",plugin);
          if (plugin) {
            plugin->init();
            plugin->activate(44100,0,1024);
            plugin->start_processing();
            #ifndef MIP_NO_GUI
            const clap_plugin_t* clap_plugin = plugin->getPlugin();
            //plugin->MGui?
            clap_plugin_gui_t* gui = (clap_plugin_gui_t*)plugin->get_extension(CLAP_EXT_GUI);
            //MIP_Print("gui: %p\n",gui);
            if (gui) {
              #ifdef MIP_LINUX
              if (gui->is_api_supported(clap_plugin,CLAP_WINDOW_API_X11,false)) {
                gui->create(clap_plugin,CLAP_WINDOW_API_X11,false);
              #endif
              #ifdef MIP_WIN32
              if (gui->is_api_supported(clap_plugin,CLAP_WINDOW_API_WIN32,false)) {
                gui->create(clap_plugin,CLAP_WINDOW_API_WIN32,false);
              #endif
                gui->set_scale(clap_plugin,1.0);
                //bool can_resize = gui->can_resize(clap_plugin);
                //if (can_resize) {
                // plugin->MEditorWidth/Height
                uint32_t width, height;
                gui->get_size(clap_plugin,&width,&height);
                //}
                //else {
                //}
                //gui->set_size(clap_plugin,width,height);
                MIP_ExeWindow* exe_window = new MIP_ExeWindow(width,height,clap_plugin,gui);
                if (exe_window) {
                  exe_window->openWindow();
                  clap_window_t clap_window;
                  #ifdef MIP_LINUX
                  clap_window.api = CLAP_WINDOW_API_X11;
                  clap_window.x11 = exe_window->drawable_getXcbWindow();
                  #endif
                  #ifdef MIP_WIN32
                  clap_window.api = CLAP_WINDOW_API_WIN32;
                  clap_window.win32 = exe_window->drawable_getWin32Window();
                  #endif
                  gui->set_parent(clap_plugin,&clap_window);
                  gui->show(clap_plugin);
                  //redraw?
                  gui->set_size(clap_plugin,width,height);
                  exe_window->eventLoop();
                  gui->hide(clap_plugin);
                  gui->destroy(clap_plugin);
                  exe_window->closeWindow();
                  delete exe_window;
                }
              }
            }
            #endif // MIP_NO_GUI
            plugin->stop_processing();
            plugin->deactivate();
            plugin->destroy();
            delete plugin;
          }
        }
        delete exe_host;
      }
    }
  } // num desc > 0
  return 0;
}

//----------------------------------------------------------------------
#endif
