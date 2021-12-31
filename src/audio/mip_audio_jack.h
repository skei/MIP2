#ifndef mip_audio_jack_included
#define mip_audio_jack_included
//----------------------------------------------------------------------

#include <jack/jack.h>
#include <jack/midiport.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//----------------------------------------------------------------------


//----------------------------------------------------------------------

class MIP_AudioIOListener {
};

//----------------------------------------------------------------------

// https://github.com/jackaudio/example-clients/blob/master/midiseq.c

class MIP_AudioJack {

//------------------------------
private:
//------------------------------

  MIP_AudioIOListener*  MListener         = nullptr;

  jack_client_t*        MJackClient       = nullptr;
  jack_port_t*          MAudioInputPort1   = nullptr;
  jack_port_t*          MAudioInputPort2   = nullptr;
  jack_port_t*          MAudioOutputPort1  = nullptr;
  jack_port_t*          MAudioOutputPort2  = nullptr;
  jack_port_t*          MMidiInputPort    = nullptr;
  jack_port_t*          MMidiOutputPort   = nullptr;

  jack_nframes_t        MSampleRate       = 0;
  jack_nframes_t        MBufferSize       = 0;

//------------------------------
private:
//------------------------------

static
int jack_process_callback(jack_nframes_t nframes, void* arg) {
//  void* port_buf = jack_port_get_buffer(output_port, nframes);
//  unsigned char* buffer;
//  jack_midi_clear_buffer(port_buf);
//  //memset(buffer, 0, nframes*sizeof(jack_default_audio_sample_t));
//  for (int i=0; i<nframes; i++) {
//    for (int j=0; j<num_notes; j++) {
//      if (note_starts[j] == loop_index) {
//        buffer = jack_midi_event_reserve(port_buf, i, 3);
//        // printf("wrote a note on, port buffer = 0x%x, event buffer = 0x%x\n", port_buf, buffer);
//        buffer[2] = 64;		/* velocity */
//        buffer[1] = note_frqs[j];
//        buffer[0] = 0x90;	/* note on */
//      }
//      else if(note_starts[j] + note_lengths[j] == loop_index) {
//        buffer = jack_midi_event_reserve(port_buf, i, 3);
//        // printf("wrote a note off, port buffer = 0x%x, event buffer = 0x%x\n", port_buf, buffer);
//        buffer[2] = 64;		/* velocity */
//        buffer[1] = note_frqs[j];
//        buffer[0] = 0x80;	/* note off */
//      }
//    }
//    loop_index = loop_index+1 >= loop_nsamp ? 0 : loop_index+1;
//  }
  return 0;
}

//------------------------------
public:
//------------------------------

    //bool config(float ASampleRate, uint32_t ABlockSize, uint32_t APeriods, uint32_t ANumInputs, uint32_t ANumOutputs) {
    //  MSampleRate = ASampleRate;
    //  MBlockSize  = ABlockSize;
    //  MPeriods    = APeriods;
    //  MNumInputs  = ANumInputs;
    //  MNumOutputs = ANumOutputs;
    //  return true;
    //}

    //----------

    bool init(MIP_AudioIOListener* AListener) {
      MListener   = AListener;
      jack_status_t status;// = (jack_status_t)0;

      MJackClient = jack_client_open("MIP",JackNullOption,&status);
      jack_set_process_callback(MJackClient,jack_process_callback,this);

      MAudioInputPort1  = jack_port_register(MJackClient,"audio_in1", JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
      MAudioInputPort2  = jack_port_register(MJackClient,"audio_in2", JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
      MAudioOutputPort1 = jack_port_register(MJackClient,"audio_out1",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
      MAudioOutputPort2 = jack_port_register(MJackClient,"audio_out2",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
      MMidiInputPort    = jack_port_register(MJackClient,"midi_in",   JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
      MMidiOutputPort   = jack_port_register(MJackClient,"midi_out",  JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput,0);

      MSampleRate = jack_get_sample_rate(MJackClient);
      MBufferSize = jack_get_buffer_size(MJackClient);

      return true;
    }

    //----------

    void exit() {
      jack_client_close(MJackClient);
    }

    //----------

    bool start() {
      jack_activate(MJackClient);
      return true;
    }

    //----------

    void stop() {
      jack_deactivate(MJackClient);
    }

};

//----------------------------------------------------------------------

#endif




#if 0

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/midiport.h>

jack_port_t *input_port;
jack_port_t *output_port;
jack_default_audio_sample_t ramp=0.0;
jack_default_audio_sample_t note_on;
unsigned char note = 0;
jack_default_audio_sample_t note_frqs[128];

void calc_note_frqs(jack_default_audio_sample_t srate) {
	int i;
	for(i=0; i<128; i++) 	{
		note_frqs[i] = (2.0 * 440.0 / 32.0) * pow(2, (((jack_default_audio_sample_t)i - 9.0) / 12.0)) / srate;
	}
}

int process(jack_nframes_t nframes, void *arg) {
	int i;
	void* port_buf = jack_port_get_buffer(input_port, nframes);
	jack_default_audio_sample_t *out = (jack_default_audio_sample_t *) jack_port_get_buffer (output_port, nframes);
	jack_midi_event_t in_event;
	jack_nframes_t event_index = 0;
	jack_nframes_t event_count = jack_midi_get_event_count(port_buf);
	if(event_count > 1) 	{
		printf(" midisine: have %d events\n", event_count);
		for(i=0; i<event_count; i++) 		{
			jack_midi_event_get(&in_event, port_buf, i);
			printf("    event %d time is %d. 1st byte is 0x%x\n", i, in_event.time, *(in_event.buffer));
		}
    /*printf("1st byte of 1st event addr is %p\n", in_events[0].buffer);*/
	}
	jack_midi_event_get(&in_event, port_buf, 0);
	for(i=0; i<nframes; i++) 	{
		if((in_event.time == i) && (event_index < event_count)) {
			if( ((*(in_event.buffer) & 0xf0)) == 0x90 ) {
				/* note on */
				note = *(in_event.buffer + 1);
				note_on = 1.0;
			}
			else if( ((*(in_event.buffer)) & 0xf0) == 0x80 )
			{
				/* note off */
				note = *(in_event.buffer + 1);
				note_on = 0.0;
			}
			event_index++;
			if(event_index < event_count)
				jack_midi_event_get(&in_event, port_buf, event_index);
		}
		ramp += note_frqs[note];
		ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
		out[i] = note_on*sin(2*M_PI*ramp);
	}
	return 0;
}

int srate(jack_nframes_t nframes, void *arg) {
	printf("the sample rate is now %" PRIu32 "/sec\n", nframes);
	calc_note_frqs((jack_default_audio_sample_t)nframes);
	return 0;
}

void jack_shutdown(void *arg) {
	exit(1);
}

int main(int narg, char **args) {
	jack_client_t *client;
	if ((client = jack_client_open ("midisine", JackNullOption, NULL)) == 0) {
		fprintf(stderr, "jack server not running?\n");
		return 1;
	}
	calc_note_frqs(jack_get_sample_rate (client));
	jack_set_process_callback (client, process, 0);
	jack_set_sample_rate_callback (client, srate, 0);
	jack_on_shutdown (client, jack_shutdown, 0);
	input_port = jack_port_register (client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	output_port = jack_port_register (client, "audio_out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if (jack_activate (client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	/* run until interrupted */
	while(1) {
		sleep(1);
	}
	jack_client_close(client);
	exit (0);
}

#endif // 0
