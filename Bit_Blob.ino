
#include <Servo.h>

//define notes and their frequencies that will be used in the song
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_F4  349
#define NOTE_CS4 277
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_C6  1047
#define NOTE_F6  1397
#define REST      0



// defines pins numbers
const int trigPin = 2;
const int echoPin = 3;
const int buzzer = 11;

Servo blob1;
Servo blob2;
Servo blob3;
Servo hands_dance;
int angle = 0;


int tempo = 140;

//cases for each distance required
int newcase = 0;
int cases = 0;
int firstcase = 1;
int secondcase = 2;
int thirdcase = 3;
int fourthcase = 4;

//variables required to work out speed_of_user of the users' movemnet
long duration;
int distance1 = 0;
int distance2 = 0;
int speed_of_user = 0;
int distance = 0;


//function to move a motor by calling the blob number and specifying an angle
void motor_move (int blob_number, int angle) {
  switch(blob_number){
    case 1:
      blob1.write(angle);
      break;
    case 2:
      blob2.write(angle);
      break;
    case 3:
      blob3.write(angle);
      break;
  }
}

//function to reset all the motors back to 0  
void resetmotor () {
  blob1.write(0);
  blob2.write(0);
  blob3.write(0);
}

//function to reset the specific motor to 0 and then move depending on note
void reset_move (int blob_number, int angle) {
  resetmotor();
  motor_move(blob_number, angle);
}

// moving the cams to make the hands dance
void dancing_hands(){
  for(angle = 0; angle < 90; angle++){                                  
    hands_dance.write(angle);               
    delay(1);                   
  }
  for(angle = 90; angle > 0; angle--){                              
    hands_dance.write(angle);           
    delay(1);       
  } 
} 
  



//function that sets a particular motor to move with a particular angle depending on note
//higher notes have a higher angle so it visually looks like it is corresponding with the note value
void dance(int note) {
  switch(note){
    case NOTE_C4: reset_move(1,30); break;
    case NOTE_D4: reset_move(2,30); break;
    case NOTE_DS4: reset_move(3,30); break;
    case NOTE_E4: reset_move(1,50); break;
    case NOTE_FS4: reset_move(2,50); break;
    case NOTE_G4: reset_move(3,150); break;
    case NOTE_GS4: reset_move(1,110); break;
    case NOTE_A4: reset_move(2,150); break;
    case NOTE_AS4: reset_move(3,150); break;
    case NOTE_B4: reset_move(1,90); break;
    case NOTE_C5: reset_move(2,100); break; 
    case NOTE_CS5: reset_move(3,125); break;
    case NOTE_D5: reset_move(1,90); break;
    case NOTE_DS5: reset_move(2,100); break;
    case NOTE_E5: reset_move(3,100); break;
    case NOTE_F5: reset_move(1,90); break;
    case NOTE_FS5: reset_move(2,120); break;
    case NOTE_G5: reset_move(3,120); break;
    case NOTE_A5: reset_move(1,130); break; 
    case NOTE_AS5: reset_move(2,130); break;
    case NOTE_C6: reset_move(3,120); break;
    case NOTE_F6: reset_move(2,100); break;  
    case NOTE_F4: reset_move(3,90); break;
    case NOTE_CS4: reset_move(1,90); break;
  }
}

//The basis of the code was based off code written by Robson Couto, 2019
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
//int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// this calculates the duration of a whole note in ms
//int wholenote = (60000 * 4) / tempo;
// for loop iterates over the notes of the melody and executes the dance function for each note
// The array is twice the number of notes (notes + durations)
// divider calculates the duration of each note


void play_song(int* melody, int notes, int wholenote) {
  int divider = 0, noteDuration = 0; 
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    ultrasonicRead();
      if (newcase != cases) {
        break;
    }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    //calling the dance function
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    dance(melody[thisNote]);
    
    // Wait for the specified duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
 }


//function to move the cams for the dancing hands
void play_max_speed_song(int* melody, int notes, int wholenote) {  
  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    ultrasonicRead();
      if (newcase != cases) {
        break;
    }
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    
    // Wait for the specified duration before playing the next note.
    delay(noteDuration);
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
 }



//Songs list
  // notes of the melody followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!

int starwars [] = {
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, 
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8,NOTE_G5,2,
};

int never_gonna_give_you_up[] = {
  REST,-4, NOTE_A4,8, NOTE_B4,8, NOTE_CS5,8, NOTE_D5,8, NOTE_B4,8,
  REST,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_E5,-8, NOTE_E5,-8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_D5,4, NOTE_E5,8, NOTE_CS5,-8, NOTE_B4,16, NOTE_A4,4, NOTE_A4,8, 

  NOTE_E5,4, NOTE_D5,2, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16, 
  NOTE_FS5,-8, NOTE_FS5,-8, NOTE_E5,-4, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
  NOTE_A5,4, NOTE_CS5,8, NOTE_D5,-8, NOTE_CS5,16, NOTE_B4,8, NOTE_A4,16, NOTE_B4,16, NOTE_D5,16, NOTE_B4,16,
     NOTE_E5,4, NOTE_D5,2,
};


int pinkpanther[] = {
  NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_G4,8, NOTE_B4,-8, NOTE_E5,8,
  NOTE_DS5,1,   
  NOTE_D5,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,-4, REST,4,
};

int supermariobros[] = {
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8,
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4,
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4,
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,
};


//clamps values to get smoother readings
//Required because sensors sometimes output values higher or lower than the limits of the range.
//This code sets the highest end of the range as the maximum possible input height.
//It also sets the lower end of the range as the minimum possible input height.
int clamp(int VAL, int min, int max){
  if(VAL <= min){
    VAL = min; 
    return VAL;
    }
  else if(VAL >= max){
    VAL = max;
    return VAL;
    }
  else{
    return VAL;
   }
}


float ultrasonicRead ()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(8);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(9);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

//calculating distance
//This code works by using the speed of sound.
//It is divided by two to account for the travel to the user's hand and back.
distance = duration*0.034/2;
distance = clamp(distance,0,50);

//making cases for each distance to change the song in the play song function
//Simple cases - these can be switched for any other song.
//You would just need to input the lists for new songs in each case.
  if(distance > 0 && distance < 5){          
    newcase = firstcase;
    }
  else if(distance >= 5 && distance < 10){      
    newcase = secondcase;
    }
  else if (distance > 10 && distance < 15){
    newcase = thirdcase;
    }
  else if (distance > 15 && distance < 20){
   newcase = fourthcase;
   }




// Prints the distance & Speed on the Serial Monitor
Serial.print("Distance in cm : ");
Serial.println(distance);
Serial.print("Speed in cm/s  :"); 
Serial.println(speed_of_user);
return distance;
return newcase;

}


void setup(){
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  blob1.attach(8);
  blob2.attach(9);
  blob3.attach(10);
  hands_dance.attach(13);
   
}


//This loop first checks the distance of the user's hand from the sensor.
//It then checks the speed at which the hand moves. This 
//These two factors decide the song and nature of interaction (i.e. all dancing or just front hands).
void loop() 
{
  distance1 = ultrasonicRead(); //calls ultrasoninicRead() function below
  delay(1000);//giving a time gap of 1 sec
  distance2 = ultrasonicRead();

   //formula change in distance divided by change in time
  int speed_of_user = (distance2 - distance1)/1.0; //as the time gap is 1 sec we divide it by 1.
  
  if(distance > 0 && distance < 5 && speed_of_user < 35){
    int tempo = 190;
    cases = firstcase;
    int wholenote = (60000 * 4) / tempo;
    int starwars_notes = sizeof(starwars) / sizeof(starwars[0]) / 2;
    play_song(starwars, starwars_notes, wholenote);
    }
  else if(distance > 0 && distance < 5 && speed_of_user > 35){
    int tempo = 380;
    cases = firstcase;
    int wholenote = (60000 * 4) / tempo;
    int starwars_notes = sizeof(starwars) / sizeof(starwars[0]) / 2;
    play_song(starwars, starwars_notes, wholenote);
  }
  else if(distance >= 5 && distance < 10 && speed_of_user < 35){      
    int tempo = 120;
    cases = secondcase;
    int wholenote = (60000 * 4) / tempo;
    int pinkpanther_notes = sizeof(pinkpanther) / sizeof(pinkpanther[0]) / 2;
    play_song(pinkpanther,pinkpanther_notes, wholenote);
  }
  else if(distance >=5 && distance < 10 && speed_of_user > 35){
    int tempo = 240;
    cases = secondcase;
    int wholenote = (60000 * 4) / tempo;
    int pinkpanther_notes = sizeof(pinkpanther) / sizeof(pinkpanther[0]) / 2;
    play_max_speed_song(pinkpanther, pinkpanther_notes, wholenote);
  }
  else if (distance >= 10 && distance < 15 && speed_of_user < 35){
    int tempo = 114;
    cases = thirdcase;
    int wholenote = (60000 * 4) / tempo;
    int never_gonna_give_you_up_notes = sizeof(never_gonna_give_you_up) / sizeof(never_gonna_give_you_up[0]) / 2;
    play_song(never_gonna_give_you_up,never_gonna_give_you_up_notes, wholenote);
  }
  else if(distance >=10 && distance < 15 && speed_of_user > 35){
    int tempo = 228;
    cases = thirdcase;
    int wholenote = (60000 * 4) / tempo;
    int never_gonna_give_you_up_notes = sizeof(never_gonna_give_you_up) / sizeof(never_gonna_give_you_up[0]) / 2;
    play_max_speed_song(never_gonna_give_you_up, never_gonna_give_you_up_notes, wholenote);
  }
  else if (distance >= 15 && distance < 20 && speed_of_user < 35){
    int tempo = 200;
    cases = fourthcase;
    int wholenote = (60000 * 4) / tempo;
    int supermariobros_notes = sizeof(supermariobros) / sizeof(supermariobros[0]) / 2;
    play_song(supermariobros,supermariobros_notes, wholenote);
  }
  else if(distance >=15 && distance < 20 && speed_of_user > 35){
    int tempo = 400;
    cases = fourthcase;
    int wholenote = (60000 * 4) / tempo;
    int supermariobros_notes = sizeof(supermariobros) / sizeof(supermariobros[0]) / 2;
    play_max_speed_song(supermariobros, supermariobros_notes, wholenote);
  }

}
