Arduino IDE ok.
 funktiot, muuttujat voi vapaasti jakaa eri tiedostoihin, ei tarvi viittauksia - käännösvaiheessa A-IDEniputtaa kaikki yhteen.

---------------------------------------------

VSC with Arduino extensions:
(c++, programmer: AVR ISP, board: DOIT ESP32 DEVKIT V1,/dev/ttyUSB0 )
Jotain pielessä, tarviiko olla Arduino IDEstä vanhempi versio kuin 1.8.7 ?
warning:
"2021-03-09T17:43:00.311Z WARN p.a.h.BoardCloudResolver:64 [cc.arduino.packages.discoverers.serial.SerialDiscovery] Fail to get the Vid Pid information from the builder response code=404"


Kun terminal = Arduino näkee edistymisen. 

-------------------------------------------------
VSC Platformio
- import ei toimi tällä setupilla, virhettä viittauksista toisista tiedostoista ei tunnista
- ei löydä DHT.h (vaikka import Arduino libs)