// Wrapper for libComedi I/O.
// These functions provide and interface to libComedi limited to use in
// the real time lab.
//
// 2006, Martin Korsgaard
#ifndef __INCLUDE_IO_H__
#define __INCLUDE_IO_H__

#define N_FLOORS 4
#define N_BUTTONS 3
/*
This module is initialized with one of the following ElevatorTypes:
*/
typedef enum  {
    // Interfaces with the hardware at the Real-time Lab using libcomedi
    ET_comedi=0,

    // Communicates with simulator_interface, that displays the
    // elevator state and handles user input
    ET_simulation=1
} ElevatorType;


/**
  Initialize libComedi in "Sanntidssalen"
  @return Non-zero on success and 0 on failure
*/
// Returns 0 on init failure
int io_init(ElevatorType type);


/**
  Sets a digital channel bit.
  @param channel Channel bit to set.
*/
void io_set_bit(int channel);



/**
  Clears a digital channel bit.
  @param channel Channel bit to set.
*/
void io_clear_bit(int channel);



/**
  Writes a value to an analog channel.
  @param channel Channel to write to.
  @param value Value to write.
*/
void io_write_analog(int channel, int value);



/**
  Reads a bit value from a digital channel.
  @param channel Channel to read from.
  @return Value read.
*/
int io_read_bit(int channel);




/**
  Reads a bit value from an analog channel.
  @param channel Channel to read from.
  @return Value read.
*/
int io_read_analog(int channel);

#endif // #ifndef __INCLUDE_IO_H__

