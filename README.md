## pwm_lib v1.3

This is pwm_lib library for the Arduino DUE electronic prototyping platform. 

Copyright (C) 2015,2020 Antonio C. Domínguez Brito (<antonio.dominguez@ulpgc.es>). División de Robótica y Oceanografía Computacional (<http://www.roc.siani.es>) and Departamento de Informática y Sistemas (<http://www.dis.ulpgc.es>). Universidad de Las Palmas de Gran  Canaria (ULPGC) (<http://www.ulpgc.es>).
  
### 0. License 

The pwm_lib library is an open source project which is openly available under the GNU General Public License (GPL) license.

### 1. Introduction

This is a C++ library to abstract the use of the eight hardware PWM channels available on Arduino DUE's Atmel ATSAM3X8E microcontroller. Each channel can be used to generate a completely independent PWM signal directly on hardware. The motivation to develop this library was two fold. First, the current limitation of the Arduino standard library where it is not possible to change the PWM output period of the PWM signals generated using function *analogWrite()*. And second, to have a library for servos directly supported by the hardware, therefore, "lighter", in terms of code generated and computational load, than the Servo library available in the Arduino standard library.

The library provides two kind of objects associated with each PWM channel: pwm and servo objects. As those objects abstract the PWM channels available on the micro controller, using pwm_lib you can use, at most, eight independent pwm_lib objects in your application, each one with its own PWM characteristics (PWM signal period and pulse duration). In its current version, the maximum period for PWM signals you can get using pwm_lib is a period of 102.261126095238 seconds (minimum frequency of 0.009778887033462533 Hz).

### 2. PWM objects

PWM objects abstract the use of  ATSAM3X8E's PWM channels for generating PWM signals on hardware. There are eight independent hardware PWM channels in Atmel 's ATSAM3X8E, and using pwm_lib you can use one PWM object associated which each channel. Mind that, according to ATSAM3X8E's data sheet, for correct operation, at the same time it is possible to use only one object associated with a given hardware PWM channel.

To use pwm_lib's pwm objects you have to declare it using a C++ template, here a snippet of code extracted from *basic_test.ino*:

```
// defining pwm object using pin 35, pin PC3 mapped to pin 35 on the DUE
// this object uses PWM channel 0
pwm<pwm_pin::PWMH0_PC3> pwm_pin35;
```

The template argument provided indicates which PWM channel and which pin is used to generate the PWM output. Take into account that each channel can only be associated with a set of pins, according to the data sheet of the micro controller. The set of pins available for each channel is "hardwired", and is listed in ATSAM3X8E data sheet's Table 38-2 (the data sheet is available in Atmel's web site at [http://www.atmel.com/devices/SAM3X8E.aspx](http://www.atmel.com/devices/SAM3X8E.aspx)). In fact, in pwm_lib, the correspondence between PWM channels and pins is mapped into a set of identifiers which you must use as arguments to declare pwm objects. This set of identifiers is the enumeration *pwm_pin*  defined in pwm_defs.h file. The whole enumeration is reproduced next:

```
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
```

You have to put special emphasis on what PWM channel you are using (specified by the first part of the identifier) and which pin out of the set of pins available you will utilize for the channel. In order to know which Arduino DUE's pin corresponds to the one you have selected, you must consult the mapping of ATSM38XE's on the DUE consulting [https://www.arduino.cc/en/Hacking/PinMappingSAM3X](https://www.arduino.cc/en/Hacking/PinMappingSAM3X). In the example provided previously, the PWM channel used is channel 0, ATSAM3X8E's pin is pin PC3, and its corresponding pin on the DUE is pin 35 (the code is reproduced here for clarification):

```
// defining pwm object using pin 35, pin PC3 mapped to pin 35 on the DUE
// this object uses PWM channel 0
pwm<pwm_pin::PWMH0_PC3> pwm_pin35;
```

Once the pwm object has been defined you start using it providing its PWM period and its initial duty value (pulse duration). You specify both period and duty in hundredths of microseconds (1e-8 seconds). Here a snippet from *basic_test.ino*:

```
pwm_pin35.start(PWM_PERIOD_PIN_35,PWM_DUTY_PIN_35);
```

Then, you can change the duty at any moment (the duty is specified equally in hundredths of microseconds):

```
pwm_pin35.set_duty(duty_value);
```
And the period and the duty (specified both also in hundredths of microseconds):

```
if(pwm_pin35.set_period_and_duty(period,duty))
{
  // period and duty were changed 
}
else
{
  // either the period is too large, or duty>period
}

```
Mind that for changing the period you must change the duty, and the duty you provide must be less than the period.

And you can stop PWM generation:

```
pwm_pin35.stop();
```
### 3. Servo objects

Servo objects allows us to use any of the PWM channels available in ATSAM3X8E micro controller to generate a PWM signal for a typical servo, where the pulse duration of the PWM corresponds to angle positions of the servo. Thus, the main difference with the PWM objects introduced in the previous section is that, once declared a servo object, when started, we have to specify not only the period of the signal (typically 20 msecs), but also the correspondence between pulse duration and angle intervals.

Servo object in pwm_lib are also defined using a template. Here we include a snippet from *servo_test.ino*:

```
servo<pwm_pin::PWML0_PB16> servo_pwm_pinDAC1; // PB16 is DUE's pin DAC1
```

As you can observe from the previous code fragment, equally to PWM objects, the argument of the template is the same identifier we use for PWM objects. In the example the PWM channel used is channel 0, and the pin is PB16, which in turns, corresponds to pin DAC1 on the DUE. In fact, a servo object is a PWM object with a different mapping for duty values, instead of specifying them using time units (hundredths of microseconds), in servo objects the duty is specified in angle degrees.

So, once a servo object is declared we start using it, providing its PWM period, the interval of pulse duration and its correspondence to an interval of angles, and its initial duty specified as an angle degree. The next portion of code extracted from *servo_test.ino* illustrates it:

```
servo_pwm_pinDAC1.start(
  PWM_PERIOD, // pwm servo period
  100000, // 1e-8 secs. (1 msecs.), minimum duty value
  200000, // 1e-8 secs. (2 msecs.), maximum duty value
  0, // minimum angle, corresponding minimum servo angle
  180, // maximum angle, corresponding minimum servo angle
  duty_angle // initial servo angle 
);
``` 

Then, you can change the duty angle:

```
servo_pwm_pinDAC1.set_angle(duty_angle);
```

And finally, you may stop servo output:

```
servo_pwm_pinDAC1.stop();
```

### 4. Download & installation

The  library  is  available  through  an  open	git  repository  available   at:

* <https://github.com/antodom/pwm_lib>

and also at our own mirror at:

* <http://bizet.dis.ulpgc.es/antodom/pwm_lib>

For using it you just have to copy the library on the libraries folder used by your Arduino IDE, the folder should be named "pwm_lib".

In addition you must add the flag -std=gnu++11 for compiling. For doing that add -std=gnu++11 to the platform.txt file, concretely to compiler.cpp.flags. In Arduino IDE 1.6.6 and greater versions this flag is already set.

### 5. Examples

On directory *examples* you have available three examples, namely: *basic_test.ino*, *changing_period_test.ino* and *servo_test.ino*, who illustrate respectively the use of pwm and servo objects.

Example *basic_test.ino* uses two PWM objects for generating two independent PWM outputs with different PWM characteristics (period and duty). Example *wrapper_basic_test.ino* is equal to *basic_test.ino*, but illustrates the use of wrapper objects for using pwm objects as pointers. This is useful, for example, to make an array of pointer to pwm objects. Example *changing_period_test.ino* utilizes a PWM object to generate a PWM signal with different periods. And example *servo_test.ino* uses a servo object to generated a PWM output for a typical servo. Finally, *wrapper_servo_test.ino* has the same functionality that *servo_test.ino* but using wrapper servo objects.

For all examples we use tc_lib's capture objects as "oscilloscopes" probes for checking the PWM outputs generated. This library is available at [https://github.com/antodom/tc_lib](https://github.com/antodom/tc_lib), and it is necessary for compiling both examples.

### 6. Incompatibilities

This library is potentially incompatible with analogWrite(), if you use analogWrite() on the output pin associated with a pwm_lib object.

### 7. Compiling with CMake

For compiling in command line using CMake, just proceed in the following manner:

1. Set the following environment variables (a good place to put them is on .bashrc):
  * Set `ARDUINO_DUE_ROOT_PATH` to `~/.arduino15/packages/arduino/`.
  * Set `ARDUINO_DUE_VERSION` to `1.6.17`.
  * Set `ARDUINO_UNO_ROOT_PATH` to the path where you have installed the Arduino IDE, for example, `~/installations/arduino-1.6.5`.
  * Set `ARDUINO_IDE_LIBRARY_PATH` to the path for the Arduino IDE projects you have in preferences. For example, `~/arduino_projects`.

2. Go to `pwm_lib` directory (the one you have cloned with the project).
3. Create directory `build` (if not already created).
4. Execute `cmake ..`.
5. Set the following flags and variables (if not done before), you should execute `ccmake ..`:
  * Set `PORT` to the serial port you will use for uploading.
  * Set `IS_NATIVE_PORT` to `true` (if using DUE's native port) or `false` (if using DUE's programming port).
6. Be sure the changes were applied, usually running `cmake ..`.
7. Compile executing `make`.
8. The previous step has generated the examples available with the library. You can upload the code executing:
  * `make upload_basic_test`, 
  * `make upload_wrapper_basic_test`, 
  * `make upload_changing_period_test`, 
  * `make upload_servo_test`, 
  * `make upload_wrapper_servo_test` 

### 8. Version changes

#### 7.1 v1.3

With respect to version v1.2 the library has been modifyed for admitting longer PWM periods, now the maximum acceptable PWM period is 102.261126095238 seconds.

### 9. Library users

In this section we would like to enumerate users using the library in their own projects and developments. So please, if you are using the library, drop us an email indicating in what project or development you are using it.

The list of users/projects goes now:

1. **Project:** Autonomous sailboat A-Tirma (<http://velerorobot.blogspot.com.es>). **User**: División de Robótica y Oceanografía Computacional (<http://www.roc.siani.es>). **Description**: The library was a specific development for this project. The sailboat onboard system is based on an Arduino DUE. 

### 10. Feedback & Suggestions

Please be free to send me any comment, doubt of use, or suggestion in relation to pwm_lib.
