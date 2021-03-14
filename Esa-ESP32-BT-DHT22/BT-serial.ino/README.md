Arduino IDE ok.
 funktiot, muuttujat voi vapaasti jakaa eri tiedostoihin, ei tarvi viittauksia - käännösvaiheessa A-IDEniputtaa kaikki yhteen.

---------------------------------------------

VSC with Arduino extensions:
(c++, programmer: AVR ISP, board: DOIT ESP32 DEVKIT V1,/dev/ttyUSB0 )

[Warning] Output path is not specified. Unable to reuse previously compiled files. Verify could be slow.
-> vaikka laitoin .vscode/arduino.json output parametriksi "../build" ei toimi



-------------------------------------------------
VSC Platformio
- import ei toimi tällä setupilla, virhettä viittauksista toisista tiedostoista ei tunnista
- ei löydä DHT.h (vaikka import Arduino libs)