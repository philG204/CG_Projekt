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

## Code responsibility

### src/main.c:
Startpunkt des Programms. Initialisiert das Fenster, lädt die Szene und enthält die Hauptschleife. In der Hauptschleife werden Eingaben verarbeitet, die Szene aktualisiert und anschließend gerendert.

## src/core/

Enthält grundlegende Programmfunktionen, die nicht direkt zur Szene gehören, sondern den allgemeinen Ablauf des Programms unterstützen.

### window.c / window.h

Kapselt die Fensterverwaltung mit GLFW. Dazu gehören das Initialisieren von GLFW, das Erstellen des Fensters, das Erzeugen des OpenGL-Kontexts, das Tauschen der Buffer und das Aufräumen am Programmende.

### input.c / input.h

Kapselt die Eingabeverarbeitung. Hier werden Tastatur- und gegebenenfalls Mauseingaben abgefragt und für die Kamera- oder Objektsteuerung bereitgestellt.

### loadObj.c / loadObj.h

Lädt Geometriedaten aus separaten .obj-Dateien. Die geladenen Daten bestehen zum Beispiel aus Positionen, Normalen, Texturkoordinaten und Indizes. Diese Daten werden anschließend an den Renderer weitergegeben.

### loadShader.c / loadShader.h

Lädt Shaderprogramme aus separaten .glsl-Dateien. Die geladenen Shader können von den Objekten in der Scene angewendet werwden

## src/renderer/

Enthält den OpenGL-spezifischen Rendering-Code. Dieser Ordner kümmert sich darum, Daten an die Grafikkarte zu übertragen und Objekte zu zeichnen.

### shader.c / shader.h

Lädt GLSL-Shaderdateien aus dem shaders/-Ordner, kompiliert Vertex- und Fragment-Shader, linkt sie zu einem Shader-Programm und stellt Funktionen zum Setzen von Uniforms bereit.

### texture.c / texture.h

Lädt Bilddateien als Texturen und überträgt sie an OpenGL. Außerdem enthält dieser Code Funktionen zum Binden und Freigeben von Texturen.

### mesh.c / mesh.h

Erstellt und verwaltet OpenGL-Meshes. Hier werden VAO, VBO und gegebenenfalls EBO erzeugt. Der eigentliche Draw Call, zum Beispiel glDrawElements, findet ebenfalls hier statt.

## src/scene/

Enthält den konkreten Aufbau der 3D-Szene. Hier wird festgelegt, welche Objekte existieren, wo sie stehen, welche Kamera verwendet wird und welche Lichtquellen aktiv sind.

### scene.c / scene.h

Verwaltet die gesamte Szene. Hier werden Objekte, Kamera, Lichtquellen, Shader, Texturen und Meshes initialisiert. Außerdem enthält die Datei Funktionen zum Aktualisieren und Rendern der Szene.

### object.c / object.h

Beschreibt ein einzelnes Objekt in der Szene. Ein Objekt besitzt typischerweise ein Mesh(vertecies,VBO,VAO), ein Material(Shader, Texture, Farbwerte, Lichtparameter) sowie Transformationsdaten wie Position, Rotation und Skalierung. Aus diesen Daten wird die Model-Matrix berechnet.

### camera.c / camera.h

Verwaltet die Kamera. Dazu gehören Position, Blickrichtung, Bewegung und die Berechnung der View- und Projection-Matrix.

### light.c / light.h

Beschreibt Lichtquellen in der Szene. Dazu können zum Beispiel Richtungslicht, Punktlicht oder Spotlicht gehören. Die Lichtdaten werden beim Rendern an die Shader übergeben.

## assets/Shaders/

Enthält die GLSL-Shaderprogramme, die auf der Grafikkarte ausgeführt werden.

## assets/Shaders/NormaleBeleuchtung

Enthält das Shader Programm für normale Beleuchtung mit mehreren möglichen Lichtquellen

## assets/

Enthält externe Dateien, die vom Programm zur Laufzeit geladen werden. Dazu gehören Modelle, Texturen und eventuell weitere Ressourcen.

### assets/Meshes/

Enthält 3D-Modelle, zum Beispiel im .obj-Format.

### assets/Textures/

Enthält Bilddateien für Texturen, zum Beispiel .png oder .jpg.

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
