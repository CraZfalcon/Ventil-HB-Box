# Ventil-HB-Box



# Possible issues:

The following libraries might not be required: 

#include <SPI.h> & #include <Ticker.h>

- The code functioned just fine without them.
In case of problems, try adding them.

- No need to install the libraries, they should be included in the Arduino IDE and can be imported without downloading anything.

- The fram script has not been tested yet.
fram.write32(addr, (fram.read32(addr)) + 1); might not work.
If so, try replacing it with the code from the original file.

# Stekker pinout:

Pin | Def | Kleur
--- | --- | ---
1 | out1 | Groen
2 | out2 | Roze
3 | out3 | Grijs
4 | out4 | Geel
5 | - | -
6 | - | -
7 | - | -
8 | - | -
9 | 24vdc | Bruin
10 | 0v gnd | Wit