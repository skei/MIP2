#ifndef preset_included
#define preset_included
//----------------------------------------------------------------------

char      preset_buffer[65536] = {0};
int32_t   preset_size = 0;
int32_t   stream_pos = 0;

// returns the number of bytes read. 0 for end of file. -1 on error.
// to: error checking, safety, etc, etc...

int64_t preset_stream_read(struct clap_istream *stream, void *buffer, uint64_t size) {
  printf("stream read: plugin asking for %i bytes\n",(int)size);
  if (stream_pos >= preset_size) return 0;
  int num = preset_size - stream_pos;
  if (num >= (int)size) {
    memcpy(buffer,&preset_buffer[stream_pos],size);
    stream_pos += size;
    return size;
  }
  else {
    memcpy(buffer,&preset_buffer[stream_pos],num);
    stream_pos += num;
    return num;
  }
}

//----------

clap_istream_t preset_stream = {
  nullptr,
  preset_stream_read
};

//------------------------------
//
//------------------------------

bool load_preset_state(const clap_plugin_t* plugin, const char* arg_preset_file) {
  const clap_plugin_state_t* state = (const clap_plugin_state_t*)plugin->get_extension(plugin,CLAP_EXT_STATE);
  if (state) printf("state extension supported\n");
  FILE* fp = nullptr;
  fp = fopen(arg_preset_file,"rb");
  fseek(fp,0,SEEK_END);
  preset_size = ftell(fp);
  printf("filesize: %i\n",preset_size);
  //rewind(fp);
  fseek(fp,0,SEEK_SET);
  fread(preset_buffer,1,preset_size,fp);
  fclose(fp);
  stream_pos = 0;
  if (state->load(plugin,&preset_stream)) {
    printf("preset '%s' loaded\n",arg_preset_file);
    return true;
  }
  else {
    printf("error loading preset '%s'\n",arg_preset_file);
    return false;
  }
}

//----------

bool load_preset_file(const clap_plugin_t* plugin, const char* arg_preset_file) {
  const clap_plugin_preset_load_t* preset_load = (const clap_plugin_preset_load_t*)plugin->get_extension(plugin,CLAP_EXT_PRESET_LOAD);
  if (preset_load) {
    return preset_load->from_file(plugin,arg_preset_file);
  }
  return false;
}

//------------------------------
//
//------------------------------

bool load_preset(const clap_plugin_t* plugin, const char* arg_preset_file) {
  printf("loading preset: '%s'\n",arg_preset_file);
  if (plugin->get_extension(plugin,CLAP_EXT_PRESET_LOAD)) {
    printf("preset-load extension supported\n");
    return load_preset_file(plugin,arg_preset_file);
  }
  else {
    printf("preset-load extension not supported\n");
    return load_preset_state(plugin,arg_preset_file);
  }
}


//----------------------------------------------------------------------
#endif
