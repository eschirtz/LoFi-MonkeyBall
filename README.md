# LoFi-MonkeyBall
#### [Eric Schirtzinger](https://github.com/eschirtz) & [Carly Krebs](https://github.com/krebcarl)

Taking inspiration from the classic video game series "Monkey Ball", we wrote this game for the [Tiva Launch Pad (ARM Cortex-M)](http://processors.wiki.ti.com/index.php/Tiva_C_Series_LaunchPads).

>This application was written as part of an open ended final project for ECE353 Introduction to Microprocessor Systems, taught by Eric Hoffman at University of Wisconsin Madison in 2018.

### Watch the Demo

<a href="http://www.youtube.com/watch?feature=player_embedded&v=pqWpO7V8EQA
" target="_blank"><img src="http://img.youtube.com/vi/pqWpO7V8EQA/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

## Features
+ [Simulated Inertia](#simulated-inertia)
+ [Fast Collision Detection](#fast-collision-detection)
+ [Easy Level Construction](#easy-level-construction)
+ [Animated Sprites](#animated-sprites)
+ [High Scores](#high-scores)

### Simulated Inertia
It was very important for us to make sure that this game felt real while playing. One of our main focuses was on simulating inertia, to give the ball some weight. I came up with this little "hack" while writing another program, and thought it would work perfectly here.
The main idea is to have user input only update a "target" speed, rather than the true speed. In this case, it was the raw accellerometer data that set our target speeds. Then each time the update function is called, we increment the real speed towards the target speed by some fraction of the difference. This results in the real speed smoothly chasing a more hectic input speed.
```C
ball->y_speed_target = board_tilt.y / SPEED_FACTOR; // Scale down the input data
ball->x_speed_target = -1 * (board_tilt.x / SPEED_FACTOR); 
ball->x_speed += (ball->x_speed_target - ball->x_speed) / EASING; // Update "real" speed
ball->y_speed += (ball->y_speed_target - ball->y_speed) / EASING;
```
### Fast Collision Detection
A more detailed description and implementation examples
```C
int main()
{
    char a = 30;
    char b = 40;
    char c = 10;
    char d = (a * b) / c;
    printf ("%d ", d);
 
    return 0;
}
```
### Easy Level Construction
A more detailed description and implementation examples
```C
int main()
{
    char a = 30;
    char b = 40;
    char c = 10;
    char d = (a * b) / c;
    printf ("%d ", d);
 
    return 0;
}
```
### Animated Sprites
A more detailed description and implementation examples
```C
int main()
{
    char a = 30;
    char b = 40;
    char c = 10;
    char d = (a * b) / c;
    printf ("%d ", d);
 
    return 0;
}
```
### High Scores
A more detailed description and implementation examples
```C
int main()
{
    char a = 30;
    char b = 40;
    char c = 10;
    char d = (a * b) / c;
    printf ("%d ", d);
 
    return 0;
}
```
___

#### Copyright (c) 2015-16, Joe Krachey
#### All rights reserved.
Redistribution and use in source or binary form, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions in source form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
