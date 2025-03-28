# >1MBaud on 54L15
`NCS SDK Ver 2.9.0`

Port2 for uart00, uart00 only one that can get higher peripheral CLK (128MHz)
## [INSTANCES](https://docs.nordicsemi.com/bundle/ps_nrf54L15/page/uarte.html#d1900e528)

![image](https://github.com/user-attachments/assets/9f6fe3b9-8f5e-4da3-83df-7e0aa43ba19a) ![image](https://github.com/user-attachments/assets/1b8501d7-988d-4ac8-8fbf-0564410ed734)


## [BAUDRATE](https://docs.nordicsemi.com/bundle/ps_nrf54L15/page/uarte.html#ariaid-title75)
![image](https://github.com/user-attachments/assets/03f3f2e9-7a3e-4dec-a954-4ce9bfe3bfbb)

## [PINS](https://docs.nordicsemi.com/bundle/ps_nrf54L15/page/chapters/pin.html#ariaid-title4)
> Note: These are all for the following instances: `spim00/20`, `uarte00/20`, `FLPR/(QSPI)`, or `uarte21`. Check the above link. `uarte00` ties to all.

port.pin | desc
---|---
2.00|General purpose I/O,SPIM DCX,**UARTE RXD**,QSPI D3
2.01|General purpose I/O,SPIM SCK,SPIS SCK,QSPI SCK
2.02|General purpose I/O,SPIM SDO,SPIS SDO,**UARTE TXD**,QSPI D0,Serial wire output (SWO)
2.03|General purpose I/O,QSPI D2
2.04|General purpose I/O,SPIM SDI,SPIS SDI,**UARTE CTS**,QSPI D1
2.05|General purpose I/O,SPIM CS,**UARTE RTS**,QSPI CS
2.06|General purpose I/O,SPIM SCK,SPIS SCK,Trace clock
2.07|General purpose I/O,Trace data,Serial wire output (SWO),SPIM DCX,**UARTE RXD**
2.08|General purpose I/O,Trace data,SPIM SDO,SPIS SDO,**UARTE TXD**
2.09|General purpose I/O,Trace data,SPIM SDI,SPIS SDI,UARTE CTS
2.10|General purpose I/O,Trace data,SPIM CS,UARTE RTS

# [DK](https://docs.nordicsemi.com/bundle/ug_nrf54l15_dk/page/UG/nRF54L15_DK/hw_desription/connector_if.html)
![image](https://github.com/user-attachments/assets/36c3992d-b794-4613-87c2-f0e8aedd3405)

> If you want to use 00, cut [SB16](https://docs.nordicsemi.com/bundle/ug_nrf54l15_dk/page/UG/nRF54L15_DK/hw_desription/solder_bridge.html), but you lose flash access.

# Testing
Use the board configurator to disable the LED power from PMIC if using `2.07`
change the `TRANSMITTER` flag to be false on one DK that acts as receiver.
conn 2.08 of `TRANSMITTER` DK to 2.07 or `RECEIVER` dk. Probe `2.08` on the `RECEIVER` dk to see an echo of what it's receiving.

You will see `0x0BADC0DE` at the baud rate specified, it is *8 whatever you put in the .dts (3.686 Mbaud by default) every 200 ms.

![image](https://github.com/user-attachments/assets/720715ce-9acd-43e5-a8f2-87240ef7a35b)

![image](https://github.com/user-attachments/assets/ddec710c-ddc1-4203-bdd3-fe30b4f07455)

![image](https://github.com/user-attachments/assets/89c5e9d4-4fa1-42ba-a042-93bbf5856d46)

# Notes
The binding enum is in `nordic,nrf-uart-common.yaml` in `sdk/zephyr/dts/bindings/serial`
```
current-speed:
    required: true
    description: |
      Initial baud rate setting for UART. Only a fixed set of baud
      rates are selectable on these devices.
    enum:
      - 1200
      - 2400
      - 4800
      - 9600
      - 14400
      - 19200
      - 28800
      - 31250
      - 38400
      - 56000
      - 57600
      - 76800
      - 115200
      - 230400
      - 250000
      - 460800
      - 921600
      - 1000000```

Anything more granular will require [`deferred-init`](https://docs.zephyrproject.org/latest/kernel/drivers/index.html#deferred-initialization) and manually adjusting the `BAUDRATE` register, until that file is changed or something else is offered.
