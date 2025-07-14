#include "rtc.h"

rtc::rtc(uint8_t dat_io, uint8_t clk, uint8_t rst_ce)
{
    this->wires_ptr = new ThreeWire(dat_io, clk, rst_ce);
    this->rtc_obj_ptr = new RtcDS1302<ThreeWire>(*this->wires_ptr);
    this->rtc_obj_ptr->Begin();
    RtcDateTime construction_time = RtcDateTime(__DATE__, __TIME__);
    this->rtc_obj_ptr->SetIsRunning(true);
    this->rtc_obj_ptr->SetDateTime(construction_time);
}

rtc::~rtc()
{
    delete this->wires_ptr;
    delete this->rtc_obj_ptr;
}

void rtc::set_time(const RtcDateTime &dt)
{
    this->rtc_obj_ptr->SetDateTime(dt);
}

RtcDateTime rtc::get_time()
{
    return this->rtc_obj_ptr->GetDateTime();
}

bool rtc::get_write_protection()
{
    return this->rtc_obj_ptr->GetIsWriteProtected();
}

void rtc::set_write_protection(const bool &wp)
{
    return this->rtc_obj_ptr->SetIsWriteProtected(wp);
}

bool rtc::get_running()
{
    return this->rtc_obj_ptr->GetIsRunning();
}

void rtc::set_running(const bool &r)
{
    return this->rtc_obj_ptr->SetIsRunning(r);
}