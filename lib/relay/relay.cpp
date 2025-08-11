#include "relay.h"

relay::relay(uint8_t sig_pin)
{
    pinMode(sig_pin, OUTPUT);
    this->sig_pin = sig_pin;
    this->NO_closed = false;
}

void relay::turn_on()
{
    this->_previous_state = this->NO_closed;
    this->NO_closed = true;
    digitalWrite(this->sig_pin, HIGH);
}

void relay::turn_off()
{
    this->_previous_state = this->NO_closed;
    this->NO_closed = false;
    digitalWrite(this->sig_pin, LOW);
}

void relay::toggle()
{
    this->_previous_state = this->NO_closed;
    this->NO_closed = !this->NO_closed;
    digitalWrite(this->sig_pin, this->NO_closed);
}

bool relay::get_state_NO()
{
    return this->NO_closed;
}

bool relay::get_changed()
{
    return this->_previous_state != this->NO_closed;
}