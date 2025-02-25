# The concept / goal
A central board is controling an RGB LED, cycling it through eight states. The eight states are:

0. ...
1. ..B
2. .G.
3. .GB
4. R..
5. R.B
6. RG.
7. RGB

Two buttons can be used to cycle through the states (iterating the state up or down by 1). At the same time, an infrared remote can set the state directly to one of the eight values (without needing to cycle through).

# Stretch goals
## Display
A display is used to display the current state number as well as which operation was the last to change the state.
## Bluetooth
Add bluetooth as an additional input. 
# Steps:
1. Learn/test/verify how buttons work. 
<newline> **DONE** -> test_button_function.ino
2. Use buttons only to control LED state.
<newline> **DONE** -> buttons_control_led.ino
3. Learn/test/verify how IR receiver works. Record remote signals for buttons 0-7.
<newline> **DONE** -> (online research)
4. Implement buttons + IR remote setting LED state.
<newline> **DONE** -> buttons_and_ir_control_led.ino
5. Document pin diagram.

# Hardware used
- Kuman uno R3 starter kit
