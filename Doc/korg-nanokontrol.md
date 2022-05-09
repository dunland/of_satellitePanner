# KORG nanokontrol to Pd:

## on Lenovo W530:

1. connect device
2. start `qjackctl`
    - settings:
        - driver: alsa
        - MIDI driver: none
        - Options → Execute script on startup: `/usr/bin/a2jmidid &` (must be installed via `sudo apt-get install a2jmidid` (https://unix.stackexchange.com/questions/155214/jack-not-recognizing-midi-device)
3. start Pd
    - Edit → Preferences → MIDI midi-api: alsa
4. open patch `korg-nanokontrol.pd` to receive data from KORG nanoKONTROL2

## on Lenovo P14s:

JACK recongized nanoKONTROL2 automatically as MIDI device..