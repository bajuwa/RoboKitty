TuneManager
===========

A C++ library for Arduino that uses ABC Notation to read music information from an SD Card and play it to a piezo.<br>
For more information on the ABC Notation: http://abcnotation.com/<br>
You can search their large database of ABC Notated songs here: http://abcnotation.com/search<br>
Many more songs can be found outside of this website, just google phrases such as "[song name] abc notation"!<br>

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
- Any stemmed notes, except for the first note in the bracketed stem (ex: in the simple stem [ABC], only A would play, B and C would be ignored)<br>
- Doubled up flats/sharps, these will default to single flats/sharps respectively (ex: ^^A will be interpretted as ^A)<br>
- Anything inside double quotes (")<br>
- Any whitespace or decorative characters (such as bars |, parenthesis (), newlines, etc)<br>

This library will break/error or display undefined behaviour if:<br>
- The K - Key Header is not the last header (either in the initial header set or the mid-tune header changes)<br>

Note: This library is still in development and is potentially unstable.  Some known bugs include potentially incorrect tempo parsing and some undefined behaviours with certain extra characters.
