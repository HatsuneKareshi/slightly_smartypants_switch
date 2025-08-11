#include <Arduino.h>

class relay
{
private:
    uint8_t sig_pin;
    bool NO_closed = false;
    bool _previous_state = false;

public:
    relay(uint8_t sig_pin);
    void turn_on();
    void turn_off();
    void toggle();
    bool get_state_NO(); // for normally OPEN relay mode
    bool get_changed();  // i hope this workss
};