# Reflex shield design brief

Status: initial scope extracted from the user-provided article; 5 October 2026.
The user wants to design and build a similar Arduino reflex shield in the
existing `Reefwing-Software/PrimalLayers` repository. This is not a commitment
to every component or equation in the article.

## Intended functions

- Four independent sensor/reflex channels with input conditioning.
- A defined reflex duration per channel, with retrigger/held-input behaviour to
  be chosen explicitly.
- Per-channel classification and side identity, using the article's DIP-switch
  approach as the starting point.
- Five aggregate requests: FREEZE, AVOID_L, AVOID_R, APP_L and APP_R.
- Hardware priority: freeze above avoid, avoid above approach.
- Hardware PWM, with a 555-based circuit as a candidate rather than a selected
  production part.
- Left/right motor-group enable and direction outputs, with a documented host
  interface and motor-driver boundary.
- EasyEDA Standard design and a JLCPCB-compatible assembly BOM, based on current
  sourcing checks and supplied symbols/footprints.

## Decisions needed before pinout and layout

| Decision | Current information | Needed input / verification |
| --- | --- | --- |
| Host board | User says Arduino shield; article mentions ESP32 | Exact board/model, header geometry, logic voltage and pins to reserve |
| Motors and battery | Article states four motors, 2.8 A stall each, grouped left/right | Actual motor model, voltage, stall/run current, battery limits and wiring |
| Power stage location | Article includes H-bridges | On-shield drivers versus separate motor-driver board |
| Host interaction | Reflexes operate without firmware | Monitoring only, host command pass-through, override rules, host absent/reset behaviour |
| Sensors | Touch/whisker and light sensing are examples | Four initial sensor types, interface levels, cable lengths and connectors |
| Trigger window | Article gives conflicting behaviour | Duration range, retriggering, continuously asserted sensor and release behaviour |
| Mechanical envelope | No Arduino form factor confirmed | Board size, mounting, stacking height, connector and fixture clearances |

Do not guess these decisions from the Arduino name or from the earlier NFC card.

## Issues to resolve from the reference

### Motor-driver current

The article's SN754410 choice must not be carried forward without correction.
TI specifies 1 A output-current capability per driver; its datasheet lists 2 A
peak as an absolute maximum, not a general 2.8 A motor rating. This conflicts with
the article's stated 2.8 A stall per motor. Select a suitable driver after the
actual motor voltage/current and power-stage arrangement are known, including
thermal and transient behaviour. Do not assume two grouped motors share the
current rating of one motor.

Source: [TI SN754410 datasheet](https://www.ti.com/lit/ds/symlink/sn754410.pdf),
reviewed 5 October 2026. This brief does not select an alternative part yet.

### Reflex timing

The trigger description calls for a retriggerable one-shot, but the later 555
simulation description explicitly says presses during the pulse do not retrigger.
Choose the desired behaviour first, then validate the chosen device and circuit
for pulse overlap, contact bounce and a continuously held input.

### Steering signs and independent expected behaviour

The article and existing validator give the same output pair for a left-only
avoid request and a left-only approach request, despite describing opposite
steering intentions. For example both yield DIR_L=0 and DIR_R=1. Establish a
physical forward convention for each wired motor group and derive a corrected
behaviour table before changing equations. A test that repeats an equation's
assumptions cannot establish that the robot turns in the intended direction.

### Freeze and electrical stop mode

Forcing an enable low is an electrical command, not proof of an immediate
mechanical stop. Specify coast versus active braking, startup/reset defaults,
and the response to brownout or missing host commands for the selected driver.
Do not describe a disabled motor output as a verified emergency-stop system.

### Bus and logic implementation

- Choose an explicit implementation for combining requests; do not interpret
  “ORed together” as permission to connect push-pull outputs directly together.
- Verify all host, sensor, timer, gate and driver logic thresholds and supply
  compatibility. Do not assume a 5 V reflex signal is safe for an ESP32 input.
- Review unused inputs, pull-ups, hysteresis and connector fault behaviour.
- Treat NAND-only logic as the article's preference to evaluate. Chip count,
  propagation delay, glitches and layout complexity need actual comparison;
  using one gate type does not by itself guarantee equal path delays.
- Verify the DIP code table against its image and equations, including the role
  of D0 and unsupported codes. Do not invent values from an unread image table.

## Planned validation

Retain the existing simulations and validator as historical inputs. Add an
independent expected-behaviour model covering all request combinations, FREEZE,
PWM high/low, simultaneous requests and host arbitration once its rules are set.
Add timing checks for sustained sensors, retriggering and direction changes.
Bench-test one channel and the driver interface before repeating four channels.
Record native ERC/DRC, Gerber/assembly review and hardware results separately.

No PCB has been designed, no assembly parts have been selected or stock-checked,
and no manufacturing files are ready as part of this setup.
