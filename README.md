# BachelorThesis-Astrochronograph

This repository contains files created for a Bachelor’s Thesis on the topic of ‘Development of a device for receiving and recording coordinated time for astronomical measurement purposes’. It includes the source code of the application and the LaTeX code of the thesis.

**Srtucture of repository:**
<pre>
├── README.md                                  # Documentation
│   ├── src                                    # Source code folder
│   │   ├── Arduino                            # Arduino codes
│   │   │   ├── Components                     # Codes for individual components
│   │   │   │   ├── BLE
│   │   │   │   ├── Bluetooth
│   │   │   │   ├── DCF77_time
│   │   │   │   ├── Display
│   │   │   │   ├── GNSS_time
│   │   │   │   ├── NTP
│   │   │   │   ├── RGB_LED
│   │   │   │   ├── Rotarry_encoder
│   │   │   │   ├── RTC
│   │   │   │   ├── SD
│   │   │   │   ├── Sound
│   │   │   │   └── TS_Reader
│   │   │   ├── Complet
│   │   │   │   ├── Astro-Chronograph_R3
│   │   │   │   └── Astro-Chronograph_R4
│   │   │   └── Test                             # Arduino codes for testing time synchronization
│   │   ├── LaTeX                                # LaTeX source code of the bachelor thesis
│   │   └── R                                    # Processing of testing data
├── Eagle                                        # Circuit schematics and PCB design
└── Text - Text                                  # Text of the Thesis
    ├── Thesis.pdf -                             # Thesis document in PDF format
    └── Presentation.pdf                         # Presentation document in PDF format
</pre>
**Circuit schematics:**
![Popis obrázku](https://github.com/kovarmi9/BachelorThesis-Astrochronograph/blob/main/src/LaTex/schema_zapojeni_2.png)
