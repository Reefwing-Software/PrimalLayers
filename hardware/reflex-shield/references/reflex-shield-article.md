# Hardwired Instincts: Designing PrimalBot’s Reflex Shield

**Published:** 2026-02-11

[Embedded Ai](/tag/embedded-ai?source=post_page---header_tags--c30957db170d-----------------------------------------)

[Primitive Reflexes](/tag/primitive-reflexes?source=post_page---header_tags--c30957db170d-----------------------------------------)

[Primal Layers](/tag/primal-layers?source=post_page---header_tags--c30957db170d-----------------------------------------)

[Edge Ai](/tag/edge-ai?source=post_page---header_tags--c30957db170d-----------------------------------------)

[Circuit Simulation](/tag/circuit-simulation?source=post_page---header_tags--c30957db170d-----------------------------------------)

# _Hardwired Instincts: Designing PrimalBot’s Reflex Shield_

_The design process of a board that includes hardware pulse width modulation, and real-time reflexes_

Any bio-inspired robot needs hardwired instincts. That means giving PrimalBot the ability to sense its environment, protect itself, and react instantly, without waiting for the processor. To achieve this we had to design a custom shield that provides dedicated reflex circuits. On it, we’ve integrated:

*   **Four hardware reflex loops** that can bypass the controller and activate/stop the motors instantly.
*   **A Pulse Width Modulation (PWM) generator** to enable speed control of the motors.

The shield is designed to act like a robotic brainstem, handling the critical low-level functions that keep the system safe and responsive, while freeing higher-level layers to focus on learning, planning, and decision-making. In this article, we’ll walk through the design and simulation process of the reflex loops.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*Wl_4nrRkrL8djs62hxr81Q.png)

*The PrimalBot concept design (AI generated). All images are by the author unless noted otherwise*

### Hardware Reflexes

A reflex is a fast, closed-loop pathway that turns a sensory signal into an action with minimal processing. Biologically, the pattern is: receptor detects stimulus, local circuit compares it to a threshold (often with a little filtering), and a motor pattern is triggered immediately. The key properties are low latency, deterministic behavior, and independence from higher cognition. You can think of it as hardware that implements:

`_sense → decide → actuate_`

with the _decide_ stage being a small set of fixed rules (often just thresholding plus sign).

For PrimalBot, we can model this in electronics by building a reflex sub-system that sits “below” the ESP32 and can directly influence motor commands without any firmware. The cleanest approach is to treat the reflex circuit as a hardware arbitration layer that can generate PWM and direction signals (or override/scale them) when a stimulus is detected (Figure 1). Conceptually, it mirrors a biological reflex pathway where many afferent nerves feed into a small amount of spinal circuitry, which then drives coordinated muscle activity.

> Afferent nerves are sensory nerves that carry information from the body’s periphery toward the central nervous system, conveying signals such as touch, pain, light, or sound.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*qZWQEvMtM12bdMp8_EtiDA.png)

*Figure 1. Hardware reflex block diagram — single reflex*

### Reflex Trigger Circuits

Each sensor has its own reflex trigger circuit (Figure 2). At the left of the diagram, sensor inputs such as touch/whisker switches or light sensors detect environmental stimuli. These raw signals are first passed through signal conditioning, typically consisting of RC filtering and a Schmitt trigger or comparator with hysteresis. This stage removes noise and contact bounce, ensuring that only meaningful events trigger the reflex.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*613DRQscynossQM-eWYVhA.png)

*Figure 2. A single reflex trigger block diagram*

The conditioned signal feeds a retriggerable one-shot timer, implemented using a 74HC123 or a 555 monostable. This block defines the temporal character of the reflex. Rather than acting only while the sensor is active, the one-shot generates a fixed-duration enable pulse (`REFLEX_EN`) whenever a stimulus is detected. If the stimulus persists or repeats, the timer retriggers, extending the reflex. This ensures the robot has time to move away from obstacles and avoids oscillatory behavior at sensor thresholds. The output, `REFLEX_EN_i`, is a fixed duration 5V pulse, where `i` is the sensor number.

### Touch Sensor Simulation

If you don’t know much about this subject, then have a read of our other article on [electronic circuit simulation](/embedded-ai/electronic-circuit-simulation-on-macos-094744c1f2ad). To test out our ideas for a reflex circuit we wanted to simulate them before buying components, committing to a breadboard, or laying out a PCB. This is because we didn’t know exactly which components or circuit configuration would best achieve the behavior we were aiming for, and simulation gave us the freedom to experiment and iterate before committing to hardware.

[

## Electronic Circuit Simulation on macOS

### If you use a Mac for your development then your circuit simulation options are fewer than for Windows, but there are…

medium.com


](/embedded-ai/electronic-circuit-simulation-on-macos-094744c1f2ad?source=post_page-----c30957db170d-----------------------------------------)

We generally use EasyEDA as our schematic, simulation, and PCB layout tool. In Figure 3 you can see a simulation circuit for the touch reflex. The switch **S1** represents a whisker switch that can be open or closed when the simulation is run. **XMM1** and **XMM2** are multimeters which show the voltage at the points indicated.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*m1910Rb4QDBaUy9yWcnHyw.png)

*Figure 3. Touch reflex trigger simulation circuit*

[Armando Rodrigues](/@armando_rodrigues) recommended the (free and open source) [Falstad java-based simulator](https://www.falstad.com/circuit/) for simple circuits, and we like the animated output it produces so we’ll use that for the remainder of this article. The first thing the simulator highlighted was that driving the 555 trigger from an inverting Schmitt trigger, didn’t produce the expected output. This is removed from the circuit shown in Figure 4. In this applet, green represents positive voltage, gray represents ground, and red represents negative voltage, while the moving yellow dots show the direction of current flow. All the circuits presented in this article are available in the [PrimalLayers GitHub repository](https://github.com/Reefwing-Software/PrimalLayers) under _schematics_. You can upload these into the Falstad simulator and run them yourself.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*EyNdt5DFa3LUR4OVviM-ig.gif)

*Figure 4. The Falstad simulator output for the reflex trigger circuit (reflex_monostable.txt)*

The bottom blue trace in Figure 4 shows the voltage at the push-button switch, which represents the whisker microswitch in this simulation. Each press produces a brief disturbance as the switch makes and breaks contact. The green trace is the output of the 555 monostable, `REFLEX_EN`. Notice that despite multiple switch activations while the output pulse is active, the monostable triggers only once. Once fired, the timing capacitor is charging and the trigger input is effectively ignored until the pulse completes and the circuit resets, preventing retriggering from repeated or noisy switch presses.

### Reflex Classification and Identity

Each reflex trigger only tells you that a nerve has fired. By itself, this has no meaning. We use DIP switches to assign meaning by answering two questions:

1.  **What kind of reflex is this?**
2.  **Where is the nerve located?**

Two DIP switches per nerve are used to encode reflex class (Table 1). Because the DIP switches have pull-up resistors and connect to ground when ON, they are _active low_, `switch OFF = 1` (pulled up to +5 V), and `switch ON = 0` (pulled to GND).

![Image](https://miro.medium.com/v2/resize:fit:908/1*1kyxzN_NLtVmGL2NPK5wQw.png)

*Table 1. Reflex class encoding*

Another two switches encode position (Table 2). At this point we only include left and right in the reflex bus.

![Image](https://miro.medium.com/v2/resize:fit:888/1*m-PXesBIDoRercGIjdEPiw.png)

*Table 2. Spatial identity encoding*

There is a design decision to make, decode nerve information locally or handle it centrally. Biological nervous systems use a local decode. Each nerve encodes what the stimulus is and where it occurred, leaving only arbitration and coordination to the central circuitry. The reflex bus follows the same principle by making each electronic nerve self-describing. Local encoding for each nerve is as follows (`D0` to `D3` are the DIP switches in Tables 1 and 2):

ID\_RIGHT = D1  
ID\_LEFT  = NOT(D1)  
APPROACH = NOT(D3) AND D2  
AVOID    = D3 AND NOT(D2)  
FREEZE   = D3 AND D2

The equivalent classification circuit for an **avoid-front-left sensor** is shown in Figure 5. The top trace is `AVOID` and the lower trace is `ID_LEFT`.

![Image](https://miro.medium.com/v2/resize:fit:1280/1*yDq9AdAp13v9fxs8yJeQ7Q.png)

*Figure 5. The reflex classification circuit (reflex_classification.txt)*

### The Reflex Bus

The _reflex bus_ is the hardware backbone that allows multiple independent electronic nerves to cooperate without a microprocessor. Rather than carrying raw sensor data, the bus carries action requests, each already classified and spatially labeled by the nerve that generated it.

Each electronic nerve connects to the reflex bus using five request lines:

*   `FREEZE_REQ = ENABLE AND FREEZE` — Indicates an urgent stop condition. When asserted, this line immediately overrides all other reflexes and disables motor output.
*   `AVOID_L_REQ = ENABLE AND AVOID AND ID_LEFT` — Requests an avoidance response biased away from the left side of the robot, typically generated by left-side whiskers or proximity sensors.
*   `AVOID_R_REQ = ENABLE AND AVOID AND ID_RIGHT` — Requests an avoidance response biased away from the right side of the robot.
*   `APP_L_REQ = ENABLE AND APPROACH AND ID_LEFT` — Requests an approach behavior biased toward the left side, such as steering toward a light or sound source.
*   `APP_R_REQ = ENABLE AND APPROACH AND ID_RIGHT` — Requests an approach behavior biased toward the right side.

Each request line is asserted only for the duration of the reflex window, defined by the nerve’s local trigger circuit (`ENABLE`). On the bus, corresponding request lines from all nerves are simply ORed together, producing a small set of aggregate signals that feed the arbitration logic. **Priority is enforced by circuit topology rather than software: freeze overrides avoid, and avoid overrides approach.**

The circuit for the reflex bus is provided in Figure 6. We are simulating the monostable output (`ENABLE`) and reflex classification outputs using logic inputs (`H` or `L`). Click on these with the mouse to change the state in the simulator. The circuit can be loaded into the simulator from the Primal Layers repo and is called _reflex\_bus.txt_.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*ZgzhvM-TORCY7_W8RtTBBQ.png)

*Figure 6. The reflex bus circuit (reflex_bus.txt)*

By reducing every reflex to one of five well-defined requests, the reflex bus remains compact, scalable, and deterministic. New sensors can be added by plugging in additional nerves without changing the arbitration or motor-control logic, preserving the biological principle that intelligence emerges from simple, well-structured pathways rather than centralized computation.

### The Pulse Width Modulation Generator

_Pulse-width modulation (PWM)_ is used to control motor speed by rapidly switching the supply fully `ON` and `OFF`. Instead of reducing the supply voltage, the motor is driven at full voltage for a fraction of each cycle, with the proportion of `ON` time, known as the _duty cycle_, determining the motor’s average power and therefore its speed (Figure 7). Most microprocessors include built-in PWM generators, but for this layer we wanted a purely hardware solution. The humble 555 timer returns.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*0kKYtRuMNKlzomjFz-oDeg.png)

*Figure 7. Examples of PWM with varying duty cycle*

With a small flourish and a handful of components, the 555 can be configured to generate a PWM waveform (Figure 8). By varying the potentiometer resistance, you control the motor speed, without any microcontroller, firmware, or analogue voltage regulation. Is there anything this chip can’t do?

![Image](https://miro.medium.com/v2/resize:fit:1400/1*lQauz_QCAT1UXGWser-AGg.gif)

*Figure 8. The 555 PWM generator (pwm_generator.txt)*

### Mapping Reflex Requests to Motor Control

At this point in the signal chain, the system is ready to act. The reflex bus describes what response is required and on which side, and the PWM generator provides a continuous timing signal for speed control. The role of the mapping stage is to combine these inputs, apply reflex priorities, and translate abstract reflex requests into the enable and direction signals needed by the motor drivers.

> To drive a typical DC motor forward and backward you apply a voltage across its two terminals, and you reverse direction by reversing the polarity. This is what a **_H-bridge_** does, it also acts as an electrical boundary that isolates low-voltage logic from the high-current motor path. The motor path in our design needs to be able to handle **2.8 A** of stall current per motor.

For simplicity the motors are grouped. The front-left and rear-left motors form the _left motor group_, while the front-right and rear-right motors form the _right motor group_. Each group shares a common enable and direction signal, allowing the reflex system to command forward motion, reverse motion, or turning.

The SN754410 H-bridge requires only two pieces of information per motor group: whether the motors should be driven and in which direction. Our task (should we choose to accept it), is to take the five reflex request lines and generate four signals: `EN_L`, `DIR_L`, `EN_R`, and `DIR_R`. We use two SN754410 devices, one for each motor group, and Table 3 shows the control connections for the left motor group. The motor outputs are pins 3 (`1Y`) and 6 (`2Y`) for the front, and pins 11 (`3Y`) and 14 (`4Y`) for the rear.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*OMega7HGxHMj5277mbiWcQ.png)

*Table 3. SN754410 control mapping for the left motor group (front-left + rear-left)*

The logic converting the reflex bus into the four drivetrain control signals, and injecting PWM is fairly straight forward. We start by creating interim logic nodes.

AVOID\_ANY = AVOID\_L\_REQ OR AVOID\_R\_REQ  
APP\_ANY   = APP\_L\_REQ OR APP\_R\_REQ  
MOVE\_ANY  = AVOID\_ANY OR APP\_ANY

The enable outputs are PWM-gated and forced off during freeze.

EN\_L = PWM AND (NOT FREEZE\_REQ) AND MOVE\_ANY  
EN\_R = PWM AND (NOT FREEZE\_REQ) AND MOVE\_ANY

The translation enforces priority. A freeze request represents an emergency condition and overrides everything else. If `FREEZE_REQ` is asserted, both motor enables are forced low, immediately stopping all motion regardless of any avoid or approach requests. No direction information is needed in this case because the H-bridges are disabled entirely.

### Direction Mapping

Direction is encoded as a single bit per motor group:

DIR = 1 → Forward  
DIR = 0 → Reverse

Direction is only meaningful when the corresponding enable signal is active; when the motors are disabled, direction can be treated as a don’t-care. This allows us to keep the logic simple.

Avoid reflexes cause the robot to turn away from the side where the stimulus occurred (e.g., _avoid left_ means turn right with left motors in reverse and right motors forward). If avoid left and right are activated together, the robot will reverse both motor groups.

DIR\_L\_avoid = AVOID\_R\_REQ AND NOT(AVOID\_L\_REQ)  
DIR\_R\_avoid = AVOID\_L\_REQ AND NOT(AVOID\_R\_REQ)

Approach reflexes use the same differential-drive strategy, but with the sense reversed so the robot turns toward the stimulus.

DIR\_L\_app = APP\_R\_REQ  
DIR\_R\_app = APP\_L\_REQ

When both approach requests are asserted, both directions evaluate to forward.

### Priority Selection

Avoid reflexes take priority over approach reflexes. If any avoid request is present, the approach mapping is ignored. This priority selection is implemented as:

DIR\_L = DIR\_L\_avoid OR (NOT(AVOID\_ANY) AND APP\_ANY AND DIR\_L\_app)  
DIR\_R = DIR\_R\_avoid OR (NOT(AVOID\_ANY) AND APP\_ANY AND DIR\_R\_app)

Freeze behavior is handled separately by forcing both motor enables low, making the direction outputs irrelevant during a freeze condition.

### Motor Arbitration Simulation

As you can see in Figure 9, we are reaching the limits of the falstad simulator in terms of space. This circuit is saved as _motor\_arbitration.txt_ in the repo. Note that there are some redundant nodes in the circuit that are the archeological remains of the design process. These will be optimised out when we convert everything to NAND gates.

![Image](https://miro.medium.com/v2/resize:fit:1248/1*Vbw9Rkq97jWqLDInXte_fg.png)

*Figure 9. Simulation of the motor control nodes (motor_arbitration.txt)*

The final circuit converts the PWM and motor arbitration nodes into H-bridge inputs, as illustrated in Figure 10 for the left motor group. The top trace is for the motor PWM (speed) and the bottom is for the `DIR_L` signal. The circuit is called _h\_bridge\_driver.txt_ in the Primal Layers repo.

![Image](https://miro.medium.com/v2/resize:fit:1400/1*cbcOw9PmU9uP9qqHUaCAsA.gif)

*Figure 10. Simulation of the left motor group control (h_bridge_driver.txt)*

Because we can’t fit all the motor control logic in one simulation window, we decided to double check the logic using a Python script (_reflex\_arbitration\_validator.py_ in the repo). This script acts as a testbench that exhaustively evaluates the reflex arbitration logic. It iterates over all combinations of reflex bus requests, computes the resulting motor enable and direction signals, and compares them against the expected behavior. By reporting pass or fail for each case, it allows the reflex-to-motor mapping logic to be validated in software before it is implemented in hardware. This was a good sanity check, the first time through we had some `FAIL` results, which required tweaking of the arbitration circuit.

AV\_L AV\_R AP\_L AP\_R | EN\_L DIR\_L EN\_R DIR\_R |             Expected | Result  
\---------------------------------------------------------------------------  
   0    0    0    0 |    0     0    0     0 | (False, 0, False, 0) | PASS  
   0    0    0    1 |    1     1    1     0 |   (True, 1, True, 0) | PASS  
   0    0    1    0 |    1     0    1     1 |   (True, 0, True, 1) | PASS  
   0    0    1    1 |    1     1    1     1 |   (True, 1, True, 1) | PASS  
   0    1    0    0 |    1     1    1     0 |   (True, 1, True, 0) | PASS  
   0    1    0    1 |    1     1    1     0 |   (True, 1, True, 0) | PASS  
   0    1    1    0 |    1     1    1     0 |   (True, 1, True, 0) | PASS  
   0    1    1    1 |    1     1    1     0 |   (True, 1, True, 0) | PASS  
   1    0    0    0 |    1     0    1     1 |   (True, 0, True, 1) | PASS  
   1    0    0    1 |    1     0    1     1 |   (True, 0, True, 1) | PASS  
   1    0    1    0 |    1     0    1     1 |   (True, 0, True, 1) | PASS  
   1    0    1    1 |    1     0    1     1 |   (True, 0, True, 1) | PASS  
   1    1    0    0 |    1     0    1     0 |   (True, 0, True, 0) | PASS  
   1    1    0    1 |    1     0    1     0 |   (True, 0, True, 0) | PASS  
   1    1    1    0 |    1     0    1     0 |   (True, 0, True, 0) | PASS  
   1    1    1    1 |    1     0    1     0 |   (True, 0, True, 0) | PASS

### The NAND gate Version

Any Boolean function can be built using NAND gates alone, this is handy because one IC type can be used for all the digital logic. This optimises the chip count, number of decoupling capacitors, BOM, and routing complexity. If you use different types of gates (`AND`, `OR`, `NOT`) then propagation delays can vary. With NAND-only logic all paths have similar delays. For the PCB version of this circuit we will use only NAND gates.

### The Substrate Non-discrimination Assumption

There is an elephant in the room that we should address. The Primal Layers framework relies on the _Substrate Non-discrimination Assumption_. The assumption is that intelligence and behavior are properties of structure and dynamics, not of the material they are built from. Whether a reflex is implemented in biological neurons, CMOS logic, or a microcontroller running firmware is secondary to the organization of the system and the signals flowing through it. By building reflexes in hardware, we demonstrate that behaviors often attributed to intelligence can emerge from any suitable physical substrate. If consciousness is intrinsically tied to the _neuromodulatory influences of biochemistry_, then a purely digital system may, by its very nature, be incapable of replicating it. The only meaningful way to test this assumption is to build hardware equivalents and observe whether the same behaviors emerge under the same constraints.

_If you enjoyed this article and would like to support my writing, then please show your appreciation by_ [_following me_](https://reefwing.medium.com/)_, or_ [_subscribe_](https://reefwing.medium.com/subscribe) _to get an email whenever I publish a new article._

[

## Primal Layers — Is the Ancient Brain the Future of AI?

### If you are using AI at the moment, then it is probably based on a model of the human neocortex. All of the current…

ai.gopubby.com


](https://ai.gopubby.com/primal-layers-is-the-ancient-brain-the-future-of-ai-5419880b410e?source=post_page-----c30957db170d-----------------------------------------)