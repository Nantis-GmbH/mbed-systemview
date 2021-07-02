# Segger SystemView Integration in Mbed OS

## Prerequisites

* Sleep is not allowed. It is disabled by removing `DEVICE_SLEEP` in CMake.
* RTX events are enabled. Can be done by setting `"rtos.enable-all-rtx-events": true` in `mbed_app.json`. (PR pending)