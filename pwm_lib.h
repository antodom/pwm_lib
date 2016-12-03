/**
 ** pwm_lib library
 ** Copyright (C) 2015, 2016
 **
 **   Antonio C. Domínguez Brito <adominguez@iusiani.ulpgc.es>
 **     División de Robótica y Oceanografía Computacional <www.roc.siani.es>
 **     and Departamento de Informática y Sistemas <www.dis.ulpgc.es>
 **     Universidad de Las Palmas de Gran  Canaria (ULPGC) <www.ulpgc.es>
 **  
 ** This file is part of the pwm_lib library.
 ** The pwm_lib library is free software: you can redistribute it and/or modify
 ** it under  the  terms of  the GNU  General  Public  License  as  published  by
 ** the  Free Software Foundation, either  version  3  of  the  License,  or  any
 ** later version.
 ** 
 ** The  pwm_lib library is distributed in the hope that  it  will  be  useful,
 ** but   WITHOUT   ANY WARRANTY;   without   even   the  implied   warranty   of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE.  See  the  GNU  General
 ** Public License for more details.
 ** 
 ** You should have received a copy  (COPYING file) of  the  GNU  General  Public
 ** License along with the pwm_lib library.
 ** If not, see: <http://www.gnu.org/licenses/>.
 **/
/* 
 * File: pwm_lib.h
 * Description: This file includes classes and objects for using Arduino DUE's
 * ATMEL ATSAM3X8E microcontroller PWM modules. 
 * Date: December 18th, 2015
 * Author: Antonio C. Dominguez-Brito <adominguez@iusiani.ulpgc.es>
 * ROC-SIANI - Universidad de Las Palmas de Gran Canaria - Spain
 */

#ifndef PWM_LIB_H
#define PWM_LIB_H

#include "pwm_defs.h"

namespace arduino_due
{
 
 namespace pwm_lib
 {

   template<pwm_pin PIN>
   class pwm
   {
     public:

       using pin_info = pin_traits<PIN>;

       pwm(): _started_(false) {} 

       pwm(const pwm& the_pwm) = delete;
       pwm(pwm&& the_pwm) = delete;
       pwm& operator=(const pwm& the_pwm) = delete;
       pwm& operator=(pwm&& the_pwm) = delete;

       bool start(
	 uint32_t period, // hundredths of usecs (1e-8 secs)
	 uint32_t duty // // hundredths of usecs (1e-8 secs)
       )
       {
         uint32_t clock;
	 if(
	   _started_ 
	   || !pwm_core::find_clock(period,clock) 
	   || (duty>period)
	 ) return false;

	 _start_(period,duty,clock);

	 return true;
       }

       void stop() { if(_started_) _stop_(); }

       uint32_t get_duty() { return _duty_; }

       bool set_duty(uint32_t duty /* 1e-8 secs. */)
       {
	 if(!_started_ || (duty>_period_)) return false;

	 _duty_=duty;
	 //PWMC_SetDutyCycle(
	 pwm_core::pwmc_setdutycycle(
	   PWM_INTERFACE,
	   pin_info::channel,
	   static_cast<uint32_t>(
	     (static_cast<double>(duty)/100000000)/
	     pwm_core::tick_times[_clock_]
	   )
	 );
       
	 return true;
       }

       bool set_period_and_duty(
         uint32_t period, /* 1e-8 secs. */
	 uint32_t duty /* 1e-8 secs. */
       )
       {
	 uint32_t clock;
	 
	 if(
	   !_started_
	   || !pwm_core::find_clock(period,clock) 
	   || (duty>period)
	 ) return false;

	 if(clock==_clock_)
	 {
	   _period_=period;
	   PWMC_SetPeriod(
	     PWM_INTERFACE,
	     pin_info::channel,
	     static_cast<uint32_t>(
	       (static_cast<double>(period)/100000000)/
	       pwm_core::tick_times[_clock_]
	     )
	   );

	   set_duty(duty); 
	 }
	 else
	 { _stop_(); _start_(period,duty,clock); }

	 return true;
       }

       uint32_t get_period() { return _period_; }

       uint32_t get_clock() { return _clock_; }

     private:

       uint32_t _clock_;
       uint32_t _period_; // hundredths of usecs (1e-8 secs.)
       uint32_t _duty_; // hundredths of usecs (1e-8 secs.)
       bool _started_;

       void _start_(
	 uint32_t period, // hundredths of usecs (1e-8 secs.)
	 uint32_t duty, // hundredths of usecs (1e-8 secs.)
	 uint32_t clock
       );
       
       void _stop_()
       {
         PWMC_DisableChannel(
	   PWM_INTERFACE,
	   pin_info::channel
	 ); 

	 while(
	   (PWM->PWM_SR & (1<<pin_info::channel)) 
	   != 0
	 ) { /* nothing */ } 

	 _started_=false;
       }
   };

   template<pwm_pin PIN>
   void pwm<PIN>::_start_(
     uint32_t period, // hundredths of usecs (1e-8 secs.)
     uint32_t duty, // hundredths of usecs (1e-8 secs.)
     uint32_t clock
   )
   {
     _clock_=clock;

     pmc_enable_periph_clk(PWM_INTERFACE_ID);

     // we are not using clkA and clkB
     //PWMC_ConfigureClocks(0,0,VARIANT_MCK);

     // configuring the pwm pin
     PIO_Configure(
       pin_info::pio_p,
       pin_info::type,
       pin_info::pin,
       pin_info::conf
     );

     PWMC_ConfigureChannelExt(
       PWM_INTERFACE,
       pin_info::channel,
       pwm_core::clock_masks[_clock_],
       0, // left aligned
       (pin_info::inverted)? 0: (1<<9), // polarity
       0, // interrupt on counter event at end's period
       0, // dead-time disabled
       0, // non inverted dead-time high output
       0 // non inverted dead-time low output
     );

     _period_=period;
     PWMC_SetPeriod(
       PWM_INTERFACE,
       pin_info::channel,
       static_cast<uint32_t>(
	 (static_cast<double>(period)/100000000)/
	 pwm_core::tick_times[_clock_]
       )
     );
    
     PWMC_EnableChannel(PWM_INTERFACE,pin_info::channel);

     _duty_=duty;
     //PWMC_SetDutyCycle(
     pwm_core::pwmc_setdutycycle(
       PWM_INTERFACE,
       pin_info::channel,
       static_cast<uint32_t>(
	 (static_cast<double>(duty)/100000000)/
	 pwm_core::tick_times[_clock_]
       )
     );

     _started_=true;
   }

   class pwm_base
   {
     public:
       virtual ~pwm_base() {}

       virtual bool start(uint32_t period, uint32_t duty) = 0;
       virtual void stop() = 0;
       virtual uint32_t get_duty() = 0;
       virtual bool set_duty(uint32_t duty) = 0;
       virtual bool set_period_and_duty(uint32_t period, uint32_t duty) = 0; 
       virtual uint32_t get_period() = 0; 

       virtual uint32_t get_clock() = 0; 
   };

   template<typename PWM_TYPE>
   class pwm_wrapper: public pwm_base
   {
     public:

       pwm_wrapper(PWM_TYPE& pwm_obj): _pwm_obj_(pwm_obj) {}
       
       pwm_wrapper(const pwm_wrapper& the_pwm) = delete;
       pwm_wrapper(pwm_wrapper&& the_pwm) = delete;
       pwm_wrapper& operator=(const pwm_wrapper& the_pwm) = delete;
       pwm_wrapper& operator=(pwm_wrapper&& the_pwm) = delete;
       
       ~pwm_wrapper() override {}

       bool start(uint32_t period, uint32_t duty) override
       { return _pwm_obj_.start(period,duty); }

       void stop() override
       { _pwm_obj_.stop(); }

       uint32_t get_duty() override
       { return _pwm_obj_.get_duty(); }

       bool set_duty(uint32_t duty) override 
       { return _pwm_obj_.set_duty(duty); }

       bool set_period_and_duty(uint32_t period, uint32_t duty) override
       { return _pwm_obj_.set_period_and_duty(period,duty); }

       uint32_t get_period() override
       { return _pwm_obj_.get_period(); }

       uint32_t get_clock() override
       { return _pwm_obj_.get_clock(); }

     private:
       
       PWM_TYPE& _pwm_obj_; 
   };
   
   template<pwm_pin PIN>
   class servo
   {
     public:

       using pin_info = pin_traits<PIN>;
       using pwm_t = pwm<PIN>;

       servo() = default;

       servo(const servo& the_servo) = delete;
       servo(servo&& the_servo) = delete;
       servo& operator=(const servo& the_servo) = delete;
       servo& operator=(servo&& the_servo) = delete;

       bool start(
	 uint32_t period, // hundredths of usecs (1e-8 secs)
	 uint32_t time_min, // hundredths of usecs (1e-8 secs)
	 uint32_t time_max, // hundredths of usecs (1e-8 secs)
	 uint32_t angle_min, // degrees
	 uint32_t angle_max, // degress
	 uint32_t duty // degress
       );

       void stop() { _pwm_obj_.stop(); }

       uint32_t get_angle() { return _angle_; }

       bool set_angle(uint32_t angle /* degrees */)
       {
	 if(
	   !pwm_core::is_inside(_a_min_,_a_max_,angle)
	 ) return false;

	 _angle_=angle;

         uint32_t duty=
           (
             static_cast<double>(angle-_a_min_)/
             static_cast<double>(_a_max_-_a_min_)
           )*(_t_max_-_t_min_) + _t_min_;

	 if(_pwm_obj_.set_duty(duty)) { _angle_=angle; return true; }
       
	 return false;
       }

       uint32_t get_period() { return _pwm_obj_.get_period(); }
       uint32_t get_t_min() { return _t_min_; }
       uint32_t get_t_max() { return _t_max_; }
       uint32_t get_a_min() { return _a_min_; }
       uint32_t get_a_max() { return _a_max_; }

     private:

       pwm_t _pwm_obj_;

       uint32_t _t_min_; // hundredth of usecs (1e-8 secs)
       uint32_t _t_max_; // hundredth of usecs (1e-8 secs)

       uint32_t _a_min_; // degrees
       uint32_t _a_max_; // degrees

       uint32_t _angle_; // degrees
   };

   template<pwm_pin PIN>
   bool servo<PIN>::start(
     uint32_t period, // hundredths of usecs (1e-8 secs)
     uint32_t time_min, // hundredths of usecs (1e-8 secs)
     uint32_t time_max, // hundredths of usecs (1e-8 secs)
     uint32_t angle_min, // degrees
     uint32_t angle_max, // degress
     uint32_t duty_angle // degress
   )
   {
     if(
       (angle_min>angle_max) || (time_min>time_max) ||
       (period<time_min) || (period<time_max) ||
       !pwm_core::is_inside<uint32_t>(0,360,angle_min) || 
       !pwm_core::is_inside<uint32_t>(0,360,angle_max) || 
       !pwm_core::is_inside<uint32_t>(angle_min,angle_max,duty_angle)
     ) return false;

     uint32_t duty=
       (
         static_cast<double>(duty_angle-angle_min)/
         static_cast<double>(angle_max-angle_min)
       )*(time_max-time_min) + time_min;

     if(_pwm_obj_.start(period,duty))
     {
       _t_min_=time_min; _t_max_=time_max;
       _a_min_=angle_min; _a_max_=angle_max;
       _angle_=duty_angle;
       return true;
     }

     return false;
   }

   class servo_base
   {
     public:

       virtual ~servo_base() {}

       virtual bool start(
	 uint32_t period, // hundredths of usecs (1e-8 secs)
	 uint32_t time_min, // hundredths of usecs (1e-8 secs)
	 uint32_t time_max, // hundredths of usecs (1e-8 secs)
	 uint32_t angle_min, // degrees
	 uint32_t angle_max, // degress
	 uint32_t duty // degress
       ) = 0;
       virtual void stop() = 0;
       virtual uint32_t get_angle() = 0;
       virtual bool set_angle(uint32_t angle /* degrees */) = 0;
       virtual uint32_t get_period() = 0;
       virtual uint32_t get_t_min() = 0;
       virtual uint32_t get_t_max() = 0;
       virtual uint32_t get_a_min() = 0;
       virtual uint32_t get_a_max() = 0;
   };

   template<typename SERVO_TYPE>
   class servo_wrapper: public servo_base
   {
     public:
        
       servo_wrapper(SERVO_TYPE& servo_obj): _servo_obj_(servo_obj) {}
       ~servo_wrapper() override {}

       bool start(
	 uint32_t period, // hundredths of usecs (1e-8 secs)
	 uint32_t time_min, // hundredths of usecs (1e-8 secs)
	 uint32_t time_max, // hundredths of usecs (1e-8 secs)
	 uint32_t angle_min, // degrees
	 uint32_t angle_max, // degress
	 uint32_t duty // degress
       ) override 
       { 
         return _servo_obj_.start(
	   period,
	   time_min,
	   time_max,
	   angle_min,
	   angle_max,
	   duty
	 ); 
       }

       void stop() override { _servo_obj_.stop(); }

       uint32_t get_angle() override 
       { return _servo_obj_.get_angle(); }
       
       bool set_angle(uint32_t angle) override 
       { return _servo_obj_.set_angle(angle); }
       
       uint32_t get_period() override 
       { return _servo_obj_.get_period(); }
       
       uint32_t get_t_min() override 
       { return _servo_obj_.get_t_min(); }

       uint32_t get_t_max() override 
       { return _servo_obj_.get_t_max(); }

       uint32_t get_a_min() override 
       { return _servo_obj_.get_a_min(); }

       uint32_t get_a_max() override 
       { return _servo_obj_.get_a_max(); }

     private:

       SERVO_TYPE& _servo_obj_;
   };

 }

}

#endif // PWM_LIB_H




