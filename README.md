# MiotyWaffles IoT Project
Make a Waffle Iron control a Sonos speaker system, and play music based on the weather report, and the state of the Waffle Iron.

## Process Description
A waffle iron is controlled from a Particle Argon IoT device. 

1. The process is initiated from a digital input signal (GPIO).
2. The IoT device shall try to send an API call to retrieve a local weather report, and set the mains relay for the waffle iron ON.
3. Then it shall send an API call to Sonos and request to play "guns N’ Roses – Patience", or alike, while the waffle iron is warming up. 
4. When the waffle iron signals that it is ready, then the IoT device shall perform another API call to sonos and request to change the music to "AC/DC Let Me Put My Love Into You", or alike. 
5. The user puts the waffle batter into the waffle iron and closes the lid.
6. When the lid is closed, then the IoT device sends a new event that will make sonos change the music. However this time it changes the music based on the local weather report that summarizes the current weather conditions. 
    - Weather is nice: The music will be upbeat eg. Mark Ronson ft. Bruno Mars - Uptown funk
    - Weather is "bad": The music will be more soft eg. Jack johson - banana pancakes. 
7. When a timer run out, then the relay is set Off, and a beep is sounded by the buzzer to signal that the waffles are done. (The timer settings are to be established)

![MiotyWaffels](https://github.com/VintherWolf/miotywaffles/blob/master/doc/Bilag/09_Media/MiotyWaffles_TechnicalRichPicture.jpg?raw=true)


## Sequence Diagram

![PlantUML SequenceDiagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/VintherWolf/miotywaffles/master/doc/Bilag/03_Diagrammer/sd-miotywaffles.iuml)