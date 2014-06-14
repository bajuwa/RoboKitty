TuneManager
===========

A C++ library for Arduino that uses ABC Notation to read music information from an SD Card and play it to a piezo.<br>

This library does not fully support ABC Notation, it only supports what is needed from the notation to play a basic (one note at a time) 8bit song.<br>
To be more specific, this library currently supports:<br>
- Four continuous octaves (starting at the octave below Middle C)<br>
- Flats, Sharps, Normalized Notes (no doubles) (examples: ^A is A sharp, _A is A flat, =A is normal A)<br>
- Modifying note durations (examples: A2 is the A note for twice the default duration, A/2 and A/ are half default duration, A3/4 is 75% of default duration)<br>
- The following header fields (both in the header and in mid-tune when surrounded by [] tags) : <br>
        - M - Meter (default: 4/4)<br>
        - L - Note Length (default: 1/8)<br>
        - Q - Tempo (default: 120)<br>
This library IGNORES:<br>
- The K - Key Header, currently this library defaults to C Major Key<br>
- Repeat symbols ( :| )<br>
- Doubled up flats/sharps, these will default to <br>
- Anything inside double quotes (")<br>
- Any whitespace or decorative characters (such as bars |, parenthesis (), newlines, etc)<br>
This library will break/error or display undefined behaviour if:<br>
- The K - Key Header is not the last header in the initial header list (mid-tune header changes do not follow this rule)<br>