uint8_t WIDTH;
uint8_t HEIGHT;
uint8_t PANELS;

const uint8_t PANEL_LEFT = 0xFE;
const uint8_t TXTZEICHEN_ZEILE = 23;
uint16_t DISPLAYDELAY = 1000;           // 800 bei LED, 15 nur Dots (200 besser) aber man kann einfach 2 x schreiben

void flipdot_pixel (uint8_t, uint8_t, uint8_t);
void send_data (uint8_t, uint8_t);
void flipdot_writepixel (uint8_t, uint8_t, uint8_t);
void flipdot_cls (uint8_t);
void writechar (unsigned char, uint8_t, uint8_t);

/*
   @brief   dreht die Reihenfolge der Bits in einem Byte um
   @param   Byte
   @return  Byte in LITTLE Endian
*/
uint8_t rev_bits(uint8_t input)
{
  uint8_t output = 0;
  uint8_t n = sizeof(input) << 3;
  uint8_t i = 0;

  for (i = 0; i < n; i++)
    if ((input >> i) & 0x1)
      output |=  (0x1 << (n - 1 - i));

  return output;
}

void flipdot_setup(uint8_t width, uint8_t height, uint8_t panels)
{
  WIDTH = width;
  HEIGHT = height;
  PANELS = panels;
}
/*
   @brief   schreibt einen Pixel auf das Display. Rechnet Koordinaten in physikalische Adressen um
            Ursprung oben/links = 0/0
   @param   Spalte, Zeile, set = farbe: 0=schwarz, 1=gelb
   @return  none
*/
void flipdot_pixel (uint8_t spalte, uint8_t reihe, uint8_t set)
{
  uint8_t panel, spalte_offset, offset8 = 0;

  panel = uint8_t(spalte / WIDTH);            // welches Panel (ganzzahl. Anteil). 0=links
  spalte = (spalte - (WIDTH * panel)) + 1;    // Spalte auf einem Panel (starten physisch mit 1)
  panel = PANEL_LEFT - panel;                       // Paneladresse

  reihe = reihe+2;
  
  switch (reihe)    // Fuer jede Reihe die physische Adresse ermitteln und die physische Adresse der (nicht vorhandenen) 0. Spalte
  { // Adressen zum setzen:
    case (0)  : reihe = 0x32; spalte_offset = 0x00; break;
    case (1)  : reihe = 0x31; spalte_offset = 0x00; break;
    case (2)  : reihe = 0x33; spalte_offset = 0x00; break;
    
    case (3)  : reihe = 0x30; spalte_offset = 0x80; break;
    case (4)  : reihe = 0x32; spalte_offset = 0x80; break;
    case (5)  : reihe = 0x31; spalte_offset = 0x80; break;
    case (6)  : reihe = 0x33; spalte_offset = 0x80; break;
    
    case (7)  : reihe = 0x36; spalte_offset = 0x00; break;
    case (8)  : reihe = 0x35; spalte_offset = 0x00; break;
    case (9)  : reihe = 0x37; spalte_offset = 0x00; break;
    
    case (10) : reihe = 0x34; spalte_offset = 0x80; break;
    case (11) : reihe = 0x36; spalte_offset = 0x80; break;
    case (12) : reihe = 0x35; spalte_offset = 0x80; break;
    case (13) : reihe = 0x37; spalte_offset = 0x80; break;
    
    case (14) : reihe = 0xC2; spalte_offset = 0x00; break;
    case (15) : reihe = 0xC1; spalte_offset = 0x00; break;
    case (16) : reihe = 0xC3; spalte_offset = 0x00; break;
    
    case (17) : reihe = 0xC0; spalte_offset = 0x80; break;
    case (18) : reihe = 0xC2; spalte_offset = 0x80; break;
    case (19) : reihe = 0xC1; spalte_offset = 0x80; break;
    case (20) : reihe = 0xC3; spalte_offset = 0x80; break;
    
    case (21) : reihe = 0xC6; spalte_offset = 0x00; break;
    case (22) : reihe = 0xC5; spalte_offset = 0x00; break;
    case (23) : reihe = 0xC7; spalte_offset = 0x00; break;

    case (24) : reihe = 0xC4; spalte_offset = 0x80; break;
    case (25) : reihe = 0xC6; spalte_offset = 0x80; break;
    case (26) : reihe = 0xC5; spalte_offset = 0x80; break;
    case (27) : reihe = 0xC7; spalte_offset = 0x80; break;
  }

  if (!set)     // Pixel schwarz (loeschen) => Adresse korrigieren
  {
    reihe -= 8;
    spalte_offset = spalte_offset | 0x40;
  }

  // alle x Pixel eine Adresse ueberspringen (weil EP2800A 7-Segment-Treiber):
  if ((spalte >= 8) && (spalte <= 14))
    offset8 = 1;
  if ((spalte >= 15) && (spalte <= 21))
    offset8 = 2;
  if ((spalte >= 22))
    offset8 = 3;

  spalte += offset8;

  // Bits umsortieren
  spalte = rev_bits (spalte);       // Spalte rueckwaerts
  spalte = spalte >> 2;             // zwei nach rechts schiften
  spalte = spalte | spalte_offset;  // mit physischer Spaltenadresse verodern

  flipdot_writepixel (spalte, reihe, panel);
}

void send_data (uint8_t address, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(value);
  Wire.endTransmission();
}

void flipdot_writepixel (uint8_t spalte, uint8_t reihe, uint8_t panel)
{
  send_data (0x20, panel);    // Panel        // urspuenglich Adresse 0x20, wenn PCF8574. Aufgrund Defekt aber jetzt ein PCF8574A (0x38). Im Artikel wird Adresse 0x20 genannt!
  send_data (0x21, spalte);   // Column
  send_data (0x22, reihe);    // Row
  delayMicroseconds(DISPLAYDELAY);
  send_data (0x22, 0x0F);     // END?
  //send_data (0x38, 0xFF);     // LED follows?         // urspuenglich 0x20
}

void flipdot_cls (uint8_t set)
{
  uint8_t x, y;

  for (x = 0; x < (WIDTH * PANELS); x++)
  {
    for (y = 0; y < HEIGHT; y++)
      flipdot_pixel (x, y, set);
  }
}

