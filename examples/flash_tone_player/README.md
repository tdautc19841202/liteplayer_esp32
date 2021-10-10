## Make tone bin and download

- Please use mk_audio_bin.py and prompt sound file in the same directory(the mk_audio_bin.py in tools folder), then run mk_audio_bin.py file, and eventually generate audio_tone.bin file and a components folder in upper level directory, and the components folder include the url of tone.

``` bash
python $ADF_PATH/tools/audio_tone/mk_audio_tone.py -f ./tone -r ./tone
```

- The default audio_tone.bin include tone as follows, and we will judge the tone based on the number

```
"flash://tone/0_dingdong.mp3",
"flash://tone/1_network_connected.mp3",
"flash://tone/2_system_ready.mp3",
"flash://tone/3_wakeword.mp3",
```

- Download audio_tone.bin

``` bash
python $ADF_PATH/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x210000 ./tone/audio_tone.bin
```
