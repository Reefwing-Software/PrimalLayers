# PrimalLayers project guidance

For PCB/reflex-shield work, read [PCB-LESSONS-LEARNED.md](PCB-LESSONS-LEARNED.md)
and [the reflex-shield design brief](hardware/reflex-shield/DESIGN-BRIEF.md) first.
The lessons contain historical NFC-card examples; their voltages, connectors and
part choices are not requirements for this shield.

Use EasyEDA Standard unless a demonstrated requirement needs Pro. Prefer
supplied symbols and their linked footprints, independently check manufacturer
pin roles, and verify current JLCPCB assembly availability before selecting parts.

The existing article and Falstad circuits are reference material, not a validated
manufacturing design. Resolve the design brief's open issues before freezing the
pinout, BOM or PCB layout. Keep original simulations and test code traceable.
Do not infer the Arduino form factor or logic voltage from the word “Arduino”.

Preserve existing library code, examples and user files. Scope shield deliverables
to hardware/reflex-shield and link them from the root README. Record design
choices and validation evidence there. Distinguish simulation/script results,
native ERC/DRC, manufacturing review and actual hardware measurements.

These instructions do not require additional approval for ordinary authorized
work. Ask only for missing decisions that materially affect the design, and
continue independent work while awaiting them.
