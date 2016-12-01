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
 * File: pwm_defs.cpp
 * Description: This file includes definitions for using Arduino DUE's
 * ATMEL ATSAM3X8E microcontroller PWM modules. 
 * Date: December 20th, 2015
 * Author: Antonio C. Dominguez-Brito <adominguez@iusiani.ulpgc.es>
 * ROC-SIANI - Universidad de Las Palmas de Gran Canaria - Spain
 */

#include "pwm_defs.h"

namespace arduino_due
{

  namespace pwm_lib
  {

    namespace pwm_core
    {

      const double max_periods[max_two_power+1]=
      {
        max_period(0), // no clock divisor
        max_period(1), // clock divisor 2
        max_period(2), // clock divisor 4
        max_period(3), // clock divisor 8
        max_period(4), // clock divisor 16
        max_period(5), // clock divisor 32
        max_period(6), // clock divisor 64 
        max_period(7), // clock divisor 128 
        max_period(8), // clock divisor 256 
        max_period(9), // clock divisor 512 
        max_period(10) // clock divisor 1024 
      };
 
      const uint32_t clock_masks[max_two_power+1]=
      {
        PWM_CMR_CPRE_MCK,
        PWM_CMR_CPRE_MCK_DIV_2,
        PWM_CMR_CPRE_MCK_DIV_4,
        PWM_CMR_CPRE_MCK_DIV_8,
        PWM_CMR_CPRE_MCK_DIV_16,
        PWM_CMR_CPRE_MCK_DIV_32,
        PWM_CMR_CPRE_MCK_DIV_64,
        PWM_CMR_CPRE_MCK_DIV_128,
        PWM_CMR_CPRE_MCK_DIV_256,
        PWM_CMR_CPRE_MCK_DIV_512,
        PWM_CMR_CPRE_MCK_DIV_1024
      };

      const double tick_times[max_two_power+1]=
      {
        tick_time(0), // no clock divisor
        tick_time(1), // clock divisor 2
        tick_time(2), // clock divisor 4
        tick_time(3), // clock divisor 8
        tick_time(4), // clock divisor 16
        tick_time(5), // clock divisor 32
        tick_time(6), // clock divisor 64 
        tick_time(7), // clock divisor 128 
        tick_time(8), // clock divisor 256 
        tick_time(9), // clock divisor 512 
        tick_time(10) // clock divisor 1024 
      };
 
      bool find_clock(
        uint32_t period, // usecs.
        uint32_t& clock
      ) noexcept
      {
        for( 
          clock=0; 
          (clock<=max_two_power) && 
          (static_cast<double>(period)/100000000>max_periods[clock]);
          clock++
        ) { /* nothing */ }

        if(clock>max_two_power) return false;
        return true;
      }

      // WARNING: this is a sustituton of PWMC_SetDutyCycle() function
      // in libsam, to avoid the assert.
      void pwmc_setdutycycle(Pwm* pPwm,uint32_t ul_channel,uint16_t duty)
      {
        // WARNING: assert has been commented, the caller should
	// guarantee that duty<=period when calling this function,
	// in order to generate a good PWM signal
	//assert(duty <= pPwm->PWM_CH_NUM[ul_channel].PWM_CPRD);

        /* If ul_channel is disabled, write to CDTY */
        if ((pPwm->PWM_SR & (1 << ul_channel)) == 0) {

            pPwm->PWM_CH_NUM[ul_channel].PWM_CDTY = duty;
        }
        /* Otherwise use update register */
        else {

            pPwm->PWM_CH_NUM[ul_channel].PWM_CDTYUPD = duty;
        }
      }

    }

  }

}


