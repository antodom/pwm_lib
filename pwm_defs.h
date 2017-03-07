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
 * File: pwm_defs.h
 * Description: This file includes definitions for using Arduino DUE's
 * ATMEL ATSAM3X8E microcontroller PWM modules. 
 * Date: December 17th, 2015
 * Author: Antonio C. Dominguez-Brito <adominguez@iusiani.ulpgc.es>
 * ROC-SIANI - Universidad de Las Palmas de Gran Canaria - Spain
 */

#ifndef PWM_DEFS_H
#define PWM_DEFS_H

#include "Arduino.h"

namespace arduino_due
{

  namespace pwm_lib
  {

    enum class pwm_pin: uint32_t
    {
      // PWM pins listed on Table 38-2 for Atmel ATSAM3X8E 
      // datasheet available in www.atmel.com
      // NOTE: only it is possible to use one pin for each
      // PWM channel 
      PWMH0_PA8 , // PWM_CH0
      PWMH0_PB12, // PWM_CH0
      PWMH0_PC3 , // PWM_CH0
      PWML0_PA21, // PWM_CH0
      PWML0_PB16, // PWM_CH0
      PWML0_PC2 , // PWM_CH0
      
      PWMH1_PA19, // PWM_CH1
      PWMH1_PB13, // PWM_CH1
      PWMH1_PC5 , // PWM_CH1
      PWML1_PA12, // PWM_CH1
      PWML1_PB17, // PWM_CH1
      PWML1_PC4 , // PWM_CH1
      
      PWMH2_PA13, // PWM_CH2
      PWMH2_PB14, // PWM_CH2
      PWMH2_PC7 , // PWM_CH2
      PWML2_PA20, // PWM_CH2
      PWML2_PB18, // PWM_CH2
      PWML2_PC6 , // PWM_CH2
      
      PWMH3_PA9 , // PWM_CH3
      PWMH3_PB15, // PWM_CH3
      PWMH3_PC9 , // PWM_CH3
      PWML3_PA0 , // PWM_CH3
      PWML3_PB19, // PWM_CH3
      PWML3_PC8 , // PWM_CH3
      
      PWMH4_PC20, // PWM_CH4
      PWML4_PC21, // PWM_CH4
      
      PWMH5_PC19, // PWM_CH5
      PWML5_PC22, // PWM_CH5
      
      PWMH6_PC18, // PWM_CH6
      PWML6_PC23, // PWM_CH6
      
      PWML7_PC24  // PWM_CH7
    };

    template<pwm_pin PIN> struct pin_traits {};

    #define pin_traits_specialization(the_pwm_pin,pio,pio_pin,pio_id,pio_type,pio_conf,pwm_channel,pwm_inverted) \
    template<> struct pin_traits< \
      the_pwm_pin \
    > \
    { \
      static constexpr Pio* pio_p = pio; \
      static constexpr const uint32_t pin = pio_pin; \
      static constexpr const uint32_t id = pio_id; \
      static constexpr const EPioType type= pio_type; \
      static constexpr const uint32_t conf = pio_conf; \
      static constexpr const EPWMChannel channel = pwm_channel; \
      static constexpr const bool inverted = pwm_inverted; \
    };

    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH0-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH0_PA8 , PIOA, PIO_PA8B_PWMH0 , ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH0_PB12, PIOB, PIO_PB12B_PWMH0, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH0_PC3 , PIOC, PIO_PC3B_PWMH0 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   false  ); 
    pin_traits_specialization(pwm_pin::PWML0_PA21, PIOA, PIO_PA21B_PWML0, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   true   ); 
    pin_traits_specialization(pwm_pin::PWML0_PB16, PIOB, PIO_PB16B_PWML0, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   true   ); 
    pin_traits_specialization(pwm_pin::PWML0_PC2 , PIOC, PIO_PC2B_PWML0 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH0,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH1-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH1_PA19, PIOA, PIO_PA19B_PWMH1, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH1_PB13, PIOB, PIO_PB13B_PWMH1, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH1_PC5 , PIOC, PIO_PC5B_PWMH1 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   false  ); 
    pin_traits_specialization(pwm_pin::PWML1_PA12, PIOA, PIO_PA12B_PWML1, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   true   ); 
    pin_traits_specialization(pwm_pin::PWML1_PB17, PIOB, PIO_PB17B_PWML1, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   true   ); 
    pin_traits_specialization(pwm_pin::PWML1_PC4 , PIOC, PIO_PC4B_PWML1 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH1,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH2-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH2_PA13, PIOA, PIO_PA13B_PWMH2, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH2_PB14, PIOB, PIO_PB14B_PWMH2, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH2_PC7 , PIOC, PIO_PC7B_PWMH2 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   false  ); 
    pin_traits_specialization(pwm_pin::PWML2_PA20, PIOA, PIO_PA20B_PWML2, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   true   ); 
    pin_traits_specialization(pwm_pin::PWML2_PB18, PIOB, PIO_PB18B_PWML2, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   true   ); 
    pin_traits_specialization(pwm_pin::PWML2_PC6 , PIOC, PIO_PC6B_PWML2 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH2,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH3-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH3_PA9 , PIOA, PIO_PA9B_PWMH3 , ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH3_PB15, PIOB, PIO_PB15B_PWMH3, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   false  ); 
    pin_traits_specialization(pwm_pin::PWMH3_PC9 , PIOC, PIO_PC9B_PWMH3 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   false  ); 
    pin_traits_specialization(pwm_pin::PWML3_PA0 , PIOA, PIO_PA0B_PWML3 , ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   true   ); 
    pin_traits_specialization(pwm_pin::PWML3_PB19, PIOB, PIO_PB19B_PWML3, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   true   ); 
    pin_traits_specialization(pwm_pin::PWML3_PC8 , PIOC, PIO_PC8B_PWML3 , ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH3,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH4-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH4_PC20, PIOC, PIO_PC20B_PWMH4, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH4,   false  ); 
    pin_traits_specialization(pwm_pin::PWML4_PC21, PIOC, PIO_PC21B_PWML4, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH4,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH5-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH5_PC19, PIOC, PIO_PC19B_PWMH5, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH5,   false  ); 
    pin_traits_specialization(pwm_pin::PWML5_PC22, PIOC, PIO_PC22B_PWML5, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH5,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH6-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWMH6_PC18, PIOC, PIO_PC18B_PWMH6, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH6,   false  ); 
    pin_traits_specialization(pwm_pin::PWML6_PC23, PIOC, PIO_PC23B_PWML6, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH6,   true   ); 
    // ---------------------------------------------------------------------------------------------------------------------------
    // --------PWM_CH7-------|    the_pwm_pin    | pio |     pio_pin    | per_id |  pio_type   |    conf    | channel | inverted |
    pin_traits_specialization(pwm_pin::PWML7_PC24, PIOC, PIO_PC24B_PWML7, ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PWM_CH7,   true   ); 

    namespace pwm_core
    {
  
      constexpr const uint32_t max_two_power=10;
      extern const double max_periods[max_two_power+1];
      extern const double tick_times[max_two_power+1];
      extern const uint32_t clock_masks[max_two_power+1];
  
      constexpr inline double tick_time(uint32_t two_power) noexcept
      {
        return static_cast<double>(
          ( (0<=two_power) && (two_power<=max_two_power) )? 
            static_cast<uint32_t>(1<<two_power)/static_cast<double>(VARIANT_MCK):
            -1
        );
      }

      constexpr inline double max_period(uint32_t two_power) noexcept
      {
        return static_cast<double>(
          ( (0<=two_power) && (two_power<=max_two_power) )? 
            static_cast<uint64_t>(1<<(16+two_power))/static_cast<double>(VARIANT_MCK):
            -1
        );
      }

      constexpr inline static double max_period() noexcept
      { return max_period(max_two_power); }

      extern bool find_clock(
        uint32_t period, // hundredths of usecs (1e-8 secs)
        uint32_t& clock
      ) noexcept;

      template<typename T>
      inline bool is_inside(const T& v_min, const T& v_max, const T& v)
      { return ((v_min<=v) && (v<=v_max)); }


      // WARNING: this is a sustituton of PWMC_SetDutyCycle() function
      // in libsam, to avoid the assert.
      void pwmc_setdutycycle(Pwm* pPwm,uint32_t ul_channel,uint16_t duty);
    }

  }

}

#endif // PWM_DEFS_H                                                                           
