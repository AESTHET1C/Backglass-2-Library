# How to C: The Basics of Programming Pinball

## Getting started

### Some notes on terminology

The *Backglass 2* is the purple board on your pinball machine that contains all the circuitry that interacts with sensors and can drive all the LEDs, display, and speaker.

The *Backglass 2 User Library* is a collection of code that makes it easier to program for the Backglass 2 board. This is what the `#include <backglass.h>` line at the top of your code is referencing.

The *Backglass 2 Firmware* is some behind-the-scenes code that is already on the Backglass 2 board, and is not affected when you upload your code. For all intents and purposes, you can ignore the Firmware.

The term *Arduino* is, by itself, somewhat vague. It may refer to the company, Arduino LLC, their software, any of the many boards they make, or even the entire ecosystem of these things. Most of the time, the intended meaning can be determined through context.

The *Arduino Uno* is a physical device, similar to the Backglass 2 board. The Backglass 2 board is compatible with the UNO, meaning you upload code to it in exactly the same way you would for an Uno board.

The *Arduino IDE* is the software that you write and upload code in.

### Setting up the Arduino IDE

1. Download and install the Arduino IDE from [here.]<https://www.arduino.cc/en/Main/Software>
2. Download and install the Backglass 2 User Library from [here.]<https://github.com/AESTHET1C/Backglass-2-Library> Scroll down a small amount for installation instructions.
3. Verify the Arduino Uno board is selected. `Tools > Board: "Arduino/Genduino Uno"`
4. Plug in the Backglass 2 board and install any drivers (if needed).
5. Verify the Arduino IDE can talk with the Backglass 2. `Tools > Port` and select the port that appears.

### Uploading code

Plug in the Backglass 2 board and verify the Arduino IDE is set up as explained above. Click the upload button and make sure it completes uploading before unplugging the USB cable.

### Program Template

Every program you write needs to have the following:

```c
#include <backglass.h>

void setup() {
  initBackglass();

}

void loop() {

}
```

## The C programming language

The Backglass 2 board is intended to be programmed in the language *C*. While C is far from the most complicated programming language, you'll still need to learn the basic syntax. The following section is intended to be a brief overview, but a better understanding can be gained by looking at example programs or writing your own code.

**Semicolons** (`;`) are used to show when a line of code ends, similar to how a period is used in English. Every line of code you write, with only a few exceptions, must have a semicolon at the end of it.

**Comments** are an extremely useful tool to leave notes for your future self, or explain complicated bits of code. They can be added at the end of any line by using two slashes (`//`) and then writing your comment afterwards.

```c
// A comment
```
```c
// The following code handles if the left flipper is pressed
[code that does something involving the left flipper]
```

**Variables** can store values to be used later, and can be modified throughout your program. This can be useful if you want to use a value many times in your code (such as a delay time) or if you want to keep track of a value for later (such as a score).

In general, variables will be one of the following *datatypes* (or *types*):

* `boolean` or `bool` – *true* or *false*. May also be used as *HIGH* and *LOW* or *1* and *0*.
* `byte` – small, non-negative numbers (0 to 255).
* `int` – medium sized numbers (-32768 to +32767). Most common.
* `long` – large numbers (-2147483648 to +2147483647). Used for timing, but overkill for most things.
* `float` – decimals like 0.5 or 17.625. Unused by the Backglass 2 User Library.

```c
type Variable_Name = value;
…
Variable_Name = new_value;
```
```c
int Score = 0;
…
Score = (Score + 1);
```

**Functions** are shortcuts to run chunks of code from a single line. Run them by writing their name followed by parenthases:

```c
function();
```
```c
clearDisplay();
```

Some functions take extra information (or require it) in the form of one or more *arguments*:

```c
function(argument1, argument2);
```
```c
displayNumber(numberToDisplay);
```

Some functions *return* a value, which can be treated as a variable (or saved to one):

```c
variable = function();
```
```c
long Start_Time = 0;
long Time_Passed = 0;
…
Start_Time = getMillis();  // Returns the number of milliseconds passed since the program started
…
Time_Passed = (getMillis() - Start_Time);
```

**Conditionals** allow you to run sections of code only in certain cases. They also allow you to select which section of code to run, depending on the situation.

```c
if (condition1) {
	// This code runs if condition1 is true
}
else if (condition 2) {
	// This code runs if condition1 is false and condition2 is true
}
else if (condition3) {
	// This code runs if condition1 and condition2 are false, but condition3 is true
}
else {
	// This code runs if condition1, condition2, and condition3 are all false
}
```

You can have any number of `else if` blocks (including zero). The `else` block is optional, but must be last.

**Loops** allow you to run a section of code some number of times. There are two types of loops:

`For` loops are used to repeat a section of code some number of times:

```c
// Count to 100, starting at 0, and increasing by 1 every time
for (int Number = 0; Number <= 100; Number = (Number + 1)) {
	// This is the code that will run repeatedly
	displayNumber(Number);
}
```

`While` loops are used for repeating code until some condition becomes true:

```c
while (Score < 100) {
	// This code runs forever, until Score is greater than or equal to 100
	[insert code here]
}
```

**Math** works just as it does outside of programming. Parenthases can be used to neaten things up.

* `a + b` – addition
* `a - b` – subtraction
* `a * b` – multiplication
* `a / b` – division
* `a % b` – modulo (remainder from division)

**Boolean expressions**, like the boolean datatype, are ultimately either true or false. They can come in many forms:
* A simple `true` or `false`
* A boolean variable (which is either `true` or `false`)
* A function that returns a boolean value, such as `readSensor()`
* Two boolean values or variables with a *boolean operator*:
	* `a && b` (and) – are both `a` and `b` true?
	* `a || b` (or) – is at least one of `a` or `b` true?
	* `!a` (not) – is `a` false?
* Two non-boolean values or variables with a *relationship operator*:
	* `a == b` – is `a` equal to `b`?
	* `a != b` – is `a` different from `b`?
	* `a > b` – is `a` greater than `b`?
	* `a >= b` – is `a` greater than or equal to `b`?
	* `a < b` – is `a` less than `b`?
	* `a <= b` – is `a` less than or equal to `b`?

Just like in math operations, parenthases can be used combine and clean up expressions.

```c
if ((Score > 100) || ((Cheat_Mode) && (Score > 50))) {
	// This code will run only if:
	// - The variable 'Score' is greater than 100 OR
	// - The boolean 'Cheat_Mode' is true AND 'Score' is greater than 50
	[insert code now]
}
```

## The Backglass 2 User Library

To make coding for the Backglass 2 board easier, we've written a large amount of code that you can use in your program. This collection of code (called a "library") automates some tedious, complicated, and often-used tasks that a standard pinball game might require. This section covers some of the functions provided by this library.

Additional functions and details can be found in the documentation for the Library, found [here.]<TODO>

`initBackglass()` sets up a lot of behind-the-scenes code for the pinball machine and the Library. This function must be the first line of your `setup()` function.

### Sensors and LEDs

`readSensor(sensor)` reads whether or not a sensor is engaged or not. It returns either `true` or `false`. The `sensor` argument can be one of the following:

* `RAMP`
* `LOSE`
* `FLIP_L`
* `FLIP_R`
* `BUMP_0`
* `BUMP_1`
* `BUMP_2`

`setLED(LED, state)` sets an LED to on or off (`true` or `false`). The `LED` argument can be one of the following:

* `BUMP_0`
* `BUMP_1`
* `BUMP_2`

### Motor

`setMotor(state)` turns the motor to either on or off (`true` or `false`)

### Speaker

Audio on the Backglass 2 is fairly advanced. A brief overview will be given here, but for a list of available notes, durations, clips, and additional arguments, check the full documentation.

`playTone(note, duration)` plays a single tone for a given duration.

`playAudio(clip)` plays an audio clip once.

`loopAudio(clip)` plays an audio clip repeatedly until stopped.

`stopAudio()` stops all audio from playing.

`setVolume(volume)` sets the volume of the speaker to some value between 0 and 255.

### Display

`displayNumber(number)` displays a number between -999 and +9999.

`displayText("text")` displays a short bit of text.

`scrollNumber(number, speed)` scrolls a number across the display. The `speed` argument can be any number between 1 and 255, but it is suggested to use a preset value:

* `SLOW_SCROLL`
* `NORM_SCROLL`
* `FAST_SCROLL`

`scrollText("text", speed)` scrolls text across the display, using the same `speed` argument as above.

`showNumberOfBalls(number)` lights up `number` of the decimal points on the display.

`clearDisplay()` clears the display.

### Timing

`getMillis()` returns the number of milliseconds elapsed since the program started.

`waitMillis(number)` stops your program from running for `number` milliseconds.
