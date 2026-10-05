# PCB design lessons — EasyEDA and JLCPCB

Last updated: 5 October 2026.

Shared PCB guidance carried into PrimalLayers from NFC Business Card Rev C testing.
NFC-specific details below are historical case studies, not requirements for the
reflex shield. Do not copy its supply voltage, pinout, antenna or fixture constraints
into this design; use hardware/reflex-shield/DESIGN-BRIEF.md for the new scope.

Original source: Reefwing-Software/NFC-Business-Card, commit ace3b20. This file is versioned with the repository and referenced by the root
`AGENTS.md`, so future agent chats working in this checkout can discover it.
For a chat without access to this repository, attach this file or provide its
GitHub link; chat history alone is not a reliable project record.

## 1. Select parts that can actually be assembled

- Check **JLCPCB's assembly parts catalogue and current stock** before committing
  to a component, and recheck before ordering. LCSC availability alone is not
  confirmation that JLCPCB can assemble that part in the selected service.
- Record the exact manufacturer part number, LCSC/JLCPCB order code, package,
  required quantity, observed stock, check date and source link. Record any
  applicable assembly-service restrictions and Basic/Extended classification.
- Treat stock observations as dated evidence, not reservations. Preserve saved
  library definitions for reproducibility, but do not treat their cached stock
  fields as current availability.
- Review substitutions for pinout, footprint, polarity and electrical behaviour;
  sharing a nominal value or package name is insufficient.

## 2. Prefer supplied symbols and their linked footprints

- Use the supplied EasyEDA/LCSC symbol and its linked footprint for purchased
  parts when available. Do not redraw a standard part merely to make its symbol
  look different or fit a preferred arrangement.
- Preserve supplied pin numbers, names and electrical types. Normal placement,
  rotation, reference designators and instance values are separate from changing
  a component definition.
- Check the library against the manufacturer's datasheet. Library provenance
  reduces transcription risk but does not guarantee correctness.
- Reserve custom definitions for genuinely custom features, such as the printed
  antenna, programming contacts, solder bridge and test pads, or for a part with
  no suitable supplied definition. Document and review any necessary exception.
- Save source definitions, symbol/footprint UUIDs and versions or hashes. Verify
  that the placed geometry actually matches the linked definition; a UUID or
  supplier part number attached to a custom drawing is not proof that it does.

## 3. Verify pin roles independently of netlist agreement

- Trace each critical connection through **datasheet pin function → symbol pin
  number → footprint pad number → PCB net → physical package orientation**.
- For LEDs and diodes, check A/K and the physical polarity mark. For ICs, use the
  exact package variant, distinguish top and bottom views, and check pin 1 and
  any exposed pad requirements. Check polarized capacitors and connectors too.
- A symbol pointing in the expected direction can still have incorrect pin
  numbers. A PCB faithfully following that symbol can therefore be wrong.
- Build validation expectations from manufacturer information, not only from
  the schematic being tested. Comparing two files with the same mistaken
  assumption does not independently validate the circuit.
- Review actual package marks in the assembly preview. Numeric placement
  rotations alone are insufficient because library zero-degree conventions
  can differ.

**Rev C lesson:** our custom C2286 LED symbols assigned pin 1 to K and pin 2 to A,
whereas the saved supplied symbol and manufacturer drawing identify 1=A and
2=K. Earlier connectivity checks repeated the incorrect assumption and passed.
The file-level error is confirmed; physical assembly orientation was not
independently established by those checks. See the
[Rev C polarity review](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/REV-C-LED-POLARITY-REVIEW.md).

## 4. Design for the actual programming and test fixtures

- Add a small number of useful, labelled probe points instead of covering the
  board with speculative test pads. Consider probe access early, since small
  SMD lands are difficult to measure reliably.
- Allow space for the **entire fixture**, including unused contacts and its body,
  not just the pins carrying signals. Check potential contact with other exposed
  copper as well as mechanical interference.
- NFC case study: the six-pin pogo connector uses three programming contacts. The
  two additional **3V/VDD and GND** pads were placed left of J1 to leave its
  right-hand extension area clear. The user chose only those two extra pads.
  Determine the reflex shield's own probe and fixture needs independently.
- PCB contact pads need appropriate mask openings and paste/assembly exclusions.
  Label pin 1 and supply polarity clearly.
- Document external-power isolation and voltage limits. For the NFC card: JP1 open,
  no NFC field, regulated 3 V on VDD for programming; disconnect the programmer
  before closing JP1 for NFC operation. The label “3V” does not make the harvested
  rail a regulated 3 V supply.

## 5. Separate connectivity, manufacturability and operation

- Check symbol/footprint links and the physical copper connections, not only net
  names. Recheck connectivity when a linked footprint changes pad dimensions.
- Run native EasyEDA ERC/DRC and independently inspect exported Gerbers, board
  outline, mask, paste, BOM and placement data before describing a design as
  fabrication-ready. Custom validation scripts are supplementary evidence.
- Review intentional DRC findings individually. For the printed antenna, the
  continuous winding intentionally joins its two terminal nets; this does not
  justify ignoring unrelated clearance errors or disabling global checks.
- Treat DNP parts and copper features explicitly in both schematic and PCB
  exports. A symbol's BOM exclusion may not propagate to the placed PCB instance.
- Check both sides of silkscreen in the correct viewing orientation, including
  logos, QR codes, reference labels and clearance from exposed pads. Do not
  manually mirror bottom artwork again without checking how the editor/export
  represents the bottom layer.
- Rounded corners require a closed contour with connected lines/arcs and an
  edge-clearance review. Verify the exported outline, not just a rounded preview.

## 6. Understand the circuit before diagnosing a missing part

- Determine whether a component is in series or parallel before assuming a DNP
  location breaks the circuit or needs a bridge.
- On the NFC card, **C1 is across the antenna terminals**. The tag remains connected
  with C1 absent. Leave it DNP initially; do not short its pads. Select tuning
  capacitance from measurements rather than an unverified inductance estimate.
- Include GPIO voltage drop, LED forward voltage and harvested-supply sag in the
  current budget. Measure brightness and current; resistor calculations alone
  do not establish useful daylight visibility.
- Distinguish steady-state power from startup/inrush requirements. Adding bulk
  capacitance is not automatically a cure for a weak harvested supply.

## 7. Bring up one function at a time

- Start with the documented external supply and a minimal diagnostic before
  combining harvested power, timing, peripheral configuration and animation.
- A successful firmware upload verifies programming communication, not LED
  polarity, correct assembly or successful application execution.
- The NFC project used the URL-provisioning sketch, simple LED test and NFC-powered animation as
  separate stages. Record the sketch version, board revision, power arrangement,
  measurements and observed behaviour.
- Confirm package-to-GPIO mapping, alternate peripheral pin routing, clock/fuse
  settings and unused-pin treatment against the actual hardware configuration.
- State diagnostic confidence accurately: “confirmed in the design files” and
  “confirmed on the assembled board” are different findings.

## 8. Keep revisions and evidence reproducible

- Preserve manufactured revisions; implement fixes in the intended new revision.
  Keep schematic, PCB, generators, previews and documentation consistent.
- Record meaningful validation results and their limitations. Mark native DRC,
  assembly-preview review and hardware tests pending until actually completed.
- The user prefers **EasyEDA Standard** unless a demonstrated requirement
  needs Pro. Use native project files and retain the supplied library snapshots.
- For these JSON deliverables, ordinary **Import File** opens embedded symbols
  and footprints. **Import File and Extract Libs** additionally creates library
  entries; it is not required just to use the project. Save schematic and PCB in
  the matching revision project.
- Commit intentional project changes and verify the remote revision when
  reporting a push. A local file, cloud EasyEDA project and GitHub repository
  are separate copies; do not assume one automatically updates the others.

## Evidence and current implementation

- [Current project status](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/README.md)
- [Rev D design review and fabrication handoff](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/revisions/rev-d/README.md)
- [Supplier library provenance](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/libraries/README.md)
- [Supplier-symbol and footprint checks](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/revisions/rev-d/supplier-validation.json)
- [Independent Rev D validation](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/revisions/rev-d/validation.json)
- [Rev D change tracker](https://github.com/Reefwing-Software/NFC-Business-Card/blob/ace3b20/REV-D-CHANGE-TRACKER.md)

Update this guide when a new lesson is supported by evidence. Keep project
preferences distinct from manufacturer requirements and avoid treating a
prototype-specific choice as a universal PCB design rule.
