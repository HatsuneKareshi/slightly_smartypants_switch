#include <Arduino.h>
#include <RtcDS1302.h>

class rtc
{
private:
    RtcDS1302<ThreeWire> *rtc_obj_ptr = 0x0;
    ThreeWire *wires_ptr = 0x0;

public:
    rtc(uint8_t dat_io, uint8_t clk, uint8_t rst_ce);
    ~rtc();
    void set_time(const RtcDateTime &dt);
    RtcDateTime get_time();
    bool get_write_protection();
    void set_write_protection(const bool &wp);
    bool get_running();
    void set_running(const bool &r);
};