/**
 * @file buzzer.cpp
 * @author Daniel K. Vinther Wolf
 * @brief play melodies using the buzzer
 * @version 0.1
 * @date 2020-10-27
 * 
 * 
 */

#include "Particle.h"
#include "buzzer.hpp"

Buzzer::Buzzer(/* args */)
{
    // Pinsetting are setup in pinsettings.h
}

Buzzer::~Buzzer()
{
}

/**
 * @brief Execute 1kHz beep for x ms
 * 
 * @param ms 
 */
void Buzzer::beep(int ms)
{
    tone(BUZPIN, 1000, ms);
    delay(ms);
    noTone(BUZPIN);
}

/**
 * @brief Class method to play melody
 * 
 * @param melody 
 * @param noteDurations 
 * @param tempo 
 */
void Buzzer::_playMelody(int melody[], int noteDurations[], int tempo)
{
    for (int thisNote = 0; thisNote < this->_nodes; thisNote++)
    {

        // to calculate the note duration, take one second
        // divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 60 * 1000 / tempo / noteDurations[thisNote];
        tone(BUZPIN, (melody[thisNote] != 0 ? (500000 / melody[thisNote]) : 0), noteDuration - 4);

        // blocking delay needed because tone() does not block
        delay(noteDuration * 1.3);
        noTone(BUZPIN);
    }
}

/**
 * @brief Play Angry Birds Theme
 * 
 */
void Buzzer::playAngryBirds()
{
    // https://www.youtube.com/watch?v=B9iPJaDNuO4
    int tempo = 65;
    int melody[] = {
        NOTE_E4, NOTE_FS4, NOTE_G4,
        0,
        NOTE_E4,
        0,
        NOTE_B4,
        0,
        NOTE_E4, NOTE_FS4, NOTE_G4,
        0,
        NOTE_E4,
        0,
        NOTE_B4

    };

    int noteDurations[] = {
        8, 8, 8, // e f# g
        8,
        8, // e
        8,
        4, // b
        8,
        8, 8, 8, // e f# g
        8,
        8, // e
        8,
        4 // b
    };

    this->_nodes = (sizeof(melody) / sizeof(int));
    this->_playMelody(melody, noteDurations, tempo);
}

/**
 * @brief Play super mario theme
 * 
 */
void Buzzer::playSuperMario()
{
    int tempo = 30;
    int melody[] = {NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
                    NOTE_AS3, NOTE_AS4, 0,
                    0,
                    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
                    NOTE_AS3, NOTE_AS4, 0,
                    0,
                    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
                    NOTE_DS3, NOTE_DS4, 0,
                    0,
                    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
                    NOTE_DS3, NOTE_DS4, 0,
                    0, NOTE_DS4, NOTE_CS4, NOTE_D4,
                    NOTE_CS4, NOTE_DS4,
                    NOTE_DS4, NOTE_GS3,
                    NOTE_G3, NOTE_CS4,
                    NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
                    NOTE_GS4, NOTE_DS4, NOTE_B3,
                    NOTE_AS3, NOTE_A3, NOTE_GS3,
                    0, 0, 0};

    int noteDurations[] = {
        12, 12, 12, 12,
        12, 12, 6,
        3,
        12, 12, 12, 12,
        12, 12, 6,
        3,
        12, 12, 12, 12,
        12, 12, 6,
        3,
        12, 12, 12, 12,
        12, 12, 6,
        6, 18, 18, 18,
        6, 6,
        6, 6,
        6, 6,
        18, 18, 18, 18, 18, 18,
        10, 10, 10,
        10, 10, 10,
        3, 3, 3};
    this->_nodes = (sizeof(melody) / sizeof(int));
    this->_playMelody(melody, noteDurations, tempo);
}

/**
 * @brief Play Error signal
 * 
 */
void Buzzer::playError()
{
    int tempo = 80;
    int melody[] = {
        NOTE_F4, NOTE_F3, NOTE_F2, NOTE_F3,
        NOTE_A4, NOTE_A3, NOTE_A2, NOTE_A3,
        NOTE_F4, NOTE_F3, NOTE_F2, NOTE_F3,
        NOTE_A4, NOTE_A3, NOTE_A2, NOTE_A3};

    int noteDurations[] = {
        8, 8, 8, 8,
        8, 8, 8, 8,
        8, 8, 8, 8,
        8, 8, 8, 8

    };
    this->_nodes = (sizeof(melody) / sizeof(int));
    this->_playMelody(melody, noteDurations, tempo);
}

/**
 * @brief Play signal when IoT Device wakes up
 * 
 */
void Buzzer::playWokeUp()
{
    int tempo = 80;
    int melody[] = {

        NOTE_F4, NOTE_A4, NOTE_F5, NOTE_A5,
        NOTE_G5, NOTE_F6, NOTE_G6, NOTE_A6};

    int noteDurations[] = {

        16, 16, 16, 16,
        16, 16, 16, 16

    };
    this->_nodes = (sizeof(melody) / sizeof(int));
    this->_playMelody(melody, noteDurations, tempo);
}