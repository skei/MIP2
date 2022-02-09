#ifndef mip_json_included
#define mip_json_included
//----------------------------------------------------------------------

// https://github.com/stbachmann/xjson

//----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wsign-compare"

  #define XJSON_H_IMPLEMENTATION
  #include "extern/xjson.h"

#pragma GCC diagnostic pop

//----------

#include "plugin/clap/mip_clap.h"

//----------------------------------------------------------------------
// test

typedef struct vec2 {
  float x, y;
} vec2;

typedef struct simple_struct {
  uint32_t a;
  float b[3];
  char* c;
  bool d;
  vec2 pos;
  vec2 delta;
} simple_struct;

const char* json_sample = "{ \"a\": 20, \"b\": [2.0, 1.0, 3.0], \"c\": \"A test string!\", \"d\": false, \"pos\": { \"x\": 4, \"y\": 10.5 }, \"delta\": { \"x\": 20.3331, \"y\": 8 }}";

//----------

  clap_param_info_t param1 = {
    31,           // id
    85,           // flags
    (void*)666,   // cookie
    "myPlugin",   // name
    "myModule",   // module
    0.0,          // min
    1.0,          // max
    0.5           // def
  };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------


class MIP_Json {

//------------------------------
private:
//------------------------------

  xjson*    MJson             = nullptr;
  char*     MBuffer           = nullptr;
  uint32_t  MBufferSize       = 0;
  bool      MBufferAllocated  = false;

//------------------------------
public:
//------------------------------

  MIP_Json() {

    MJson = (xjson*)malloc(sizeof(xjson));
    memset(MJson,0,sizeof(xjson));
    xjson_set_string_allocator(MJson, allocate_string);

    MBufferSize       = 2048;
    MBuffer           = (char*)malloc(MBufferSize);
    MBufferAllocated  = true;

  }

  //----------

  virtual ~MIP_Json() {
    if (MBuffer && MBufferAllocated) free(MBuffer);
  }

//------------------------------
private:
//------------------------------

  static
  char* allocate_string(const char* str, size_t str_len, void* mem_ctx) {
    char* new_str = (char*)malloc(str_len + 1); // zero-terminate!
    memcpy(new_str, str, str_len);
    new_str[str_len] = '\0';
    return new_str;
  }

//------------------------------
public:
//------------------------------

  void test() {

//    simple_struct obj = {
//      .a = 10,
//      .b = { 0.1, 10.0, 15.0 },
//      .c = (char *)"Test String!",
//      .d = true,
//      .pos = { .x=1.0, .y=2.0 },
//      .delta = { .x=10, .y=10 }
//    };
//
//    bool read = false;
//    char json_str[2048];
//
//    xjson* json = (xjson*)malloc(sizeof(xjson));
//    memset(json,0,sizeof(xjson));
//    xjson_set_string_allocator(json, mip_json_allocate_string);
//
//    if (read) {
//      xjson_setup_read(json,json_sample,strlen(json_sample));
//    }
//    else {
//      xjson_setup_write(json,true,json_str,2048);
//    }

    //xjson_setup_read(MJson,json_sample,strlen(json_sample));
    xjson_setup_write(MJson,true,MBuffer,MBufferSize);

    //----------

    xjson_object_begin(MJson,nullptr);
    {

      uint64_t  cookie  = (uint64_t)param1.cookie;
      char*     name    = param1.name;//(char *)"Test String!";
      char*     module  = param1.module;//(char *)"Test String!";
      uint32_t  flags   = param1.flags;

      bool      is_stepped        = flags & CLAP_PARAM_IS_STEPPED;
      bool      is_per_note       = flags & CLAP_PARAM_IS_PER_NOTE;
      bool      is_per_channel    = flags & CLAP_PARAM_IS_PER_CHANNEL;
      bool      is_per_port       = flags & CLAP_PARAM_IS_PER_PORT;
      bool      is_periodic       = flags & CLAP_PARAM_IS_PERIODIC;
      bool      is_hidden         = flags & CLAP_PARAM_IS_HIDDEN;
      bool      is_bypass         = flags & CLAP_PARAM_IS_BYPASS;
      bool      is_readonly       = flags & CLAP_PARAM_IS_READONLY;
      bool      is_modulatable    = flags & CLAP_PARAM_IS_MODULATABLE;
      bool      requires_process  = flags & CLAP_PARAM_REQUIRES_PROCESS;

      xjson_u32(MJson,"id",&param1.id);

      xjson_u32(MJson,"flags",&param1.flags);
      xjson_array_begin(MJson, "flags_ext");
      xjson_bool(MJson,"CLAP_PARAM_IS_STEPPED",&is_stepped);
      xjson_bool(MJson,"CLAP_PARAM_IS_PER_NOTE",&is_per_note);
      xjson_bool(MJson,"CLAP_PARAM_IS_PER_CHANNEL",&is_per_channel);
      xjson_bool(MJson,"CLAP_PARAM_IS_PER_PORT",&is_per_port);
      xjson_bool(MJson,"CLAP_PARAM_IS_PERIODIC",&is_periodic);
      xjson_bool(MJson,"CLAP_PARAM_IS_HIDDEN",&is_hidden);
      xjson_bool(MJson,"CLAP_PARAM_IS_BYPASS",&is_bypass);
      xjson_bool(MJson,"CLAP_PARAM_IS_READONLY",&is_readonly);
      xjson_bool(MJson,"CLAP_PARAM_IS_MODULATABLE",&is_modulatable);
      xjson_bool(MJson,"CLAP_PARAM_REQUIRES_PROCESS",&requires_process);
      xjson_array_end(MJson);

      xjson_u64(MJson,"cookie",&cookie);
      xjson_string(MJson,"name",(const char**)&name);
      xjson_string(MJson,"module",(const char**)&module);
      xjson_double(MJson,"min_value",&param1.min_value);
      xjson_double(MJson,"max_value",&param1.max_value);
      xjson_double(MJson,"default_value",&param1.default_value);

//      xjson_array_begin(MJson, "b");
//      for (int i=0; !xjson_array_reached_end(MJson, i, 3); i++) {
//        xjson_float(MJson, NULL, &obj.b[i]);
//      }
//      xjson_array_end(MJson);
//      xjson_string(MJson, "c", (const char **)&obj.c);
//      xjson_bool(MJson, "d", &obj.d);
//      xjson_object_begin(MJson, "pos");
//      {
//        xjson_float(MJson, "x", &obj.pos.x);
//        xjson_float(MJson, "y", &obj.pos.y);
//      }
//      xjson_object_end(MJson);
//      xjson_object_begin(MJson, "delta");
//      {
//        char* key_x = (char *)"x";
//        char* key_y = (char *)"y";
//        xjson_key(MJson, (const char **)&key_x);
//        xjson_float(MJson, NULL, &obj.delta.x);
//        xjson_key(MJson, (const char **)&key_y);
//        xjson_float(MJson, NULL, &obj.delta.y);
//      }
//      xjson_object_end(MJson);
    }
    xjson_object_end(MJson);

    //----------

//    if (!read) {

      puts(MBuffer);
//    }
//    if (MJson->error) {
//      puts(MJson->error_message);
//    }

    //----------

  }

};

//----------------------------------------------------------------------
#endif
