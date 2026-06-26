# CG_Projekt

## Mitglieder
Philipp Gierenz,
Christoph Kohnen,
Bennet Kreth,
Karl Kunze

## Bedienung
Das Programm lässt sich durch einen einfachen Aufruf an `make` kompilieren.

Die exportierte Binary heißt `demo`.

Mit `make run` lässt sich das Programm auch direkt kompilieren und neu ausführen.

Eine Testsuite lässt sich mit `make test` ausführen.

Im laufenden Programm kann man mit den linken und rechten Pfeiltasten zwischen den 
Kamerapositionen wechseln. Die Bildschirmeffekte (Alte Videokamera auf einem CRT wiedergegeben)
lassen sich mit der Leertaste an und ausschalten.

## Scene

Die Scene zeigt einen 80-er Jahre Raum, der durch die Linse einer sehr alten Kamera
auf einem Röhrenbildmonitor angeschaut wird. Dabei wird der Gourand-Lichtalgorithmus zur 
Beleuchtung verwendet. Zur Simulation der alten Kamera auf einem CRT wird die Scene zunächst
auf einen off-screen Framebuffer gerendert, welcher anschließend mit einem post-processing shader
auf den Bildschirm gerendert wird.

## Quellen von den Objekten und Texturen

### Objekte
- Television 01; https://polyhaven.com/a/Television_01
- Rockingchair 01; https://polyhaven.com/a/Rockingchair_01
- Sofa 03; https://polyhaven.com/a/sofa_03
- Boombox; https://polyhaven.com/a/boombox
- Vintage Cabinet 01; https://polyhaven.com/a/vintage_cabinet_01
- Wooden Table 02; https://polyhaven.com/a/wooden_table_02
- Chandelier 02; https://polyhaven.com/a/Chandelier_02
- Hanging Picture Frame 02; https://polyhaven.com/a/hanging_picture_frame_02
- Fancy Picture Frame 01; https://polyhaven.com/a/fancy_picture_frame_01
- Gamepad; https://polyhaven.com/a/gamepad
- Modern Wooden Cabinet; https://polyhaven.com/a/modern_wooden_cabinet

### Texturen
- Bi Stretch; https://polyhaven.com/a/bi_stretch
- Wood Floor; https://polyhaven.com/a/wood_floor
- Crépe Satin; https://polyhaven.com/a/crepe_satin
