Signed ArduinoOTA example for Platformio
========
Example repository with a small wrapper ("espota-signed"), which allows you to build & deploy signed OTA updates directly from Platform.io build system. 
ArduinoOTA library documentation covers almost all you need to know, so [check it out](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)!

Currently, it works only on Unix, so this time Windows is not invited to the party.

## I want to create my project with this wrapper, what I need to do?
I assume that you have already read [ArduinoOTA documentation](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html), 
so you have a keyring and necessary configuration in code. If not — please read a related article and check my example in `src/main.ino`.

Copy "espota-signed" script into your project root, then add new environment into platformio.ini:
```ini
[env:example-signed-ota]
platform = espressif8266
framework = arduino
board = nodemcu
upload_protocol = espota
upload_port = 172.16.0.15
upload_command = ./espota-signed --ota-sign-private private.key --upload-built-binary $SOURCE -i $UPLOAD_PORT $UPLOAD_FLAGS 
```

Ensure that the path to your private key is correct and adjust other configuration parameters. Signing wrapper requires only specific `upload_command`, as in the example above. 
You can change any other options, including the `upload_flags` parameter.

Never store your private key in a repository — it'll be a massive pain in the ass in case of the leak. In production usage, you should keep your private key on the isolated machine or HSM.
