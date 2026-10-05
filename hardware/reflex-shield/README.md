# PrimalBot Arduino Reflex Shield

Project setup: 5 October 2026. **Requirements and architecture review; no PCB or
manufacturing release yet.**

Build an Arduino-compatible shield inspired by the supplied article, with four
hardware reflex channels, hardware PWM and deterministic motor-command
arbitration independent of the host processor.

- [Design brief and unresolved decisions](DESIGN-BRIEF.md)
- [Shared PCB lessons](../../PCB-LESSONS-LEARNED.md)
- [Reference article](references/reflex-shield-article.md)
- [Reference provenance](references/README.md)
- [Existing Falstad simulations](../../schematics/reflex%20circuits/)
- [Existing arbitration validator](../../test/reflex_motor_arbitration/reflex_arbitration_validator.py)

## Existing baseline

The existing validator was run during setup: all 16 printed avoid/approach
combinations reported PASS. That confirms agreement between its current
implementation and expected table only. It does not test FREEZE, PWM gating,
physical steering direction, startup, brownout, timing or power electronics.
The script prints failures rather than making its exit status a reliable CI gate.
No existing simulation, library code or test implementation was changed.

## Next work

1. Identify the exact host board, motor/supply specifications and whether motor
   power drivers belong on the shield or a separate board.
2. Resolve trigger semantics, steering signs, stopping mode and host/reflex
   arbitration; write independent truth tables and timing requirements.
3. Select and verify JLCPCB-available parts and manufacturer pinouts. Use supplied
   EasyEDA Standard symbols and their linked footprints.
4. Simulate and bench-check the critical channels, then produce Rev A schematic
   and layout, validation reports and manufacturing outputs.

Keep future native EasyEDA designs and their versioned library sources here.
Create manufacturing exports only from a reviewed design revision.
