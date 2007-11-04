#include "class.h"
#include <iostream>
#include "rtmidi/RtMidi.h"

class Midi : public Node
{
public:
  bool init (const Params& p)
  { }
  
  
  void sendMessage(const Signal& sig)
  {
    // set from void pointer
    // ...
  }
  
  
  // print a list of possible inputs
  static void inputs(std::ostream * pOutput, const Params& p) {
    RtMidiIn *midiin = 0;
    unsigned int i,nPorts;
    std::string portName;

    // RtMidiOut constructor
    try {
      midiin = new RtMidiIn();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return;
    }

    // Check outputs.
    
    nPorts = midiin->getPortCount();
    *pOutput << "Midi in ports (" << nPorts << "):" << std::endl;
    
    for ( i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << " : " ;
      try {
        portName = midiin->getPortName(i);
        *pOutput << portName << std::endl;
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
      }
    }
  }
  
  // print a list of possible outputs
  static void outputs(std::ostream * pOutput, const Params& p) {
    RtMidiOut *midiout = 0;
    unsigned int i,nPorts;
    std::string portName;

    // RtMidiOut constructor
    try {
      midiout = new RtMidiOut();
    }
    catch (RtError &error) {
      *pOutput << error.getMessageString() << std::endl;
      return;
    }

    // Check outputs.
    
    nPorts = midiout->getPortCount();
    *pOutput << "Midi out ports (" << nPorts << "):" << std::endl;
    
    for ( i=0; i<nPorts; i++ ) {
      *pOutput << "  " << i << " : " ;
      try {
        portName = midiout->getPortName(i);
        *pOutput << portName << std::endl;
      }
      catch (RtError &error) {
        *pOutput << error.getMessageString() << std::endl;
      }
    }
  }
private:
  /* data */
};
/*
// params: portNumber
static VALUE t_initialize(VALUE self, VALUE rPort) {
  
  // RtMidiOut constructor
  try {
    midiout = new RtMidiOut();
  }
  catch (RtError &error) {
    error.printMessage();
    return Qfalse;
  }

  if (rPort == Qnil) {
    // Call function to select port.
    try {
      midiout->openVirtualPort();
    }
    catch (RtError &error) {
      error.printMessage();
      t_close(self);
      return Qnil;
    }
    
  } else {
    // Call function to select port.
    try {
      midiout->openPort( NUM2INT(rPort) );
    }
    catch (RtError &error) {
      error.printMessage();
      t_close(self);
      return Qnil;
    }
    
  }
  return self;
}


static VALUE t_sendMessage(VALUE self, VALUE rByte1, VALUE rByte2, VALUE rByte3) {
  std::vector<unsigned char> message(3);
  
  message[0] = NUM2INT(rByte1);
  message[1] = NUM2INT(rByte2);
  message[2] = NUM2INT(rByte3);
  midiout->sendMessage( &message );

  return Qtrue;
}

// Send note on message
static VALUE t_noteOn(VALUE self, VALUE rChannel, VALUE rNote, VALUE rVelocity) {
  std::vector<unsigned char> message(3);
  message[0] = 0x90 + ((NUM2INT(rChannel) + 15) % 16);
  message[1] = NUM2INT(rNote) % 128;
  message[2] = NUM2INT(rVelocity) % 128;
  midiout->sendMessage( &message );

  return Qtrue;
}

// Send note off message
static VALUE t_noteOff(VALUE self, VALUE rChannel, VALUE rNote, VALUE rVelocity) {
  std::vector<unsigned char> message(3);
  // Send out a series of MIDI messages.
  message[0] = 0x80 + ((NUM2INT(rChannel) + 15) % 16);
  message[1] = NUM2INT(rNote) % 128;
  message[2] = NUM2INT(rVelocity) % 128;
  midiout->sendMessage( &message );

  return Qtrue;
}
*/
extern "C" void init() {
  Class * klass = Class::declare<Midi>("Midi");
  
  klass->add_class_method("outputs", &Midi::outputs);
  klass->add_class_method("inputs", &Midi::inputs);
  
  // rk_cRtMidi = rb_define_class("RtMidi", rb_cObject);
  // rb_define_singleton_method(rk_cRtMidi, "outputs", (VALUE(*)(...))c_outputs, 0);
  // rb_define_method(rk_cRtMidi, "initialize", (VALUE(*)(...))t_initialize, 1);
  // rb_define_method(rk_cRtMidi, "close", (VALUE(*)(...))t_close, 0);
  // rb_define_method(rk_cRtMidi, "sendMessage", (VALUE(*)(...))t_sendMessage, 3);
  // rb_define_method(rk_cRtMidi, "noteOn", (VALUE(*)(...))t_noteOn, 3); // channel, note, velocity
  // rb_define_method(rk_cRtMidi, "noteOff", (VALUE(*)(...))t_noteOff, 3); // channel, note, velocity
}

