//Create controlP5 objects
import controlP5.*;
ControlP5 cp5;
controlP5.Button activeB;
controlP5.Toggle windoorT;

//Create OOCSI object
import nl.tue.id.oocsi.*;
OOCSI oocsi = new OOCSI(this, "ips_wall", "oocsi.id.tue.nl");

//Images
PImage background;
PImage energy;
PImage hazard;
PImage washing;
PImage tiles;

//Arrays that simulate which doors/windors are open, depending on being true or false
String[] windoors_false = {"None", "None", "None"};
String[] windoors_true = {"living_door_garden", "ashna_window_1", "kitchen_window_2"};

//Keep track of state of the house
boolean ovenActive = false;
boolean washingNearDone = false;
boolean houseEmpty = false;
boolean houseHeating = false;

//Booleans that keep track which scenarios is active, and their duration
boolean hazardAlert = false;
int hazardDuration;
boolean energyAlert = false;
int energyDuration;
boolean washingAlert = false;
int washingDuration;

//Keeps track of how long ago values were last checked (determines next request)
long checkDelay;


void setup() {
  size (1920, 1080);
  background = loadImage("Background.png");
  energy = loadImage("Energy.png");
  hazard = loadImage("Hazard.png");
  washing = loadImage("Washing.png");
  tiles = loadImage("Tiles.png");

  cp5 = new ControlP5(this);
  GUIsetup(); //Create buttons for control

  //Subscribe to oocsi channels of groups we collaborate with
  oocsi.subscribe("heating");
  oocsi.subscribe("Tala");
  oocsi.subscribe("laundry");
}


void draw() {
  image(background, 0, 0, 1920, 1080);

  if (millis() > checkDelay) { //Check state of house every 2000 ms
    checkAlert();
  }

  //If statements to check if certain scenarios are active. If so, the tiles 'light up' accordingly.
  if (hazardAlert == true && millis() < hazardDuration) {
    image(hazard, 0, 0, 1920, 1080);
    image(tiles, 0, 0, 1920, 1080);
    if (hazardDuration - millis() < 500) { //When current animation almost finished
      energyDuration = millis() + 10000; //In case of multiple scenarios being activated, makes sure the next scenario also plays for 10 seconds
    }
  } else if (energyAlert == true && millis() < energyDuration) {
    image(energy, 0, 0, 1920, 1080);
    image(tiles, 0, 0, 1920, 1080);
    if (hazardDuration - millis() < 500) { //When current animation almost finished
      washingDuration = millis() + 10000; //In case of multiple scenarios being activated, makes sure the next scenario also plays for 10 seconds
    }
  } else if (washingAlert == true && millis() < washingDuration) {
    image(washing, 0, 0, 1920, 1080);
    image(tiles, 0, 0, 1920, 1080);
  }
}


//Uses state of the house to determine if certain scenarios should become active
void checkAlert() {
  //Useless heating
  if (houseEmpty == true && houseHeating == true) {
    energyAlert = true;
  } else {
    energyAlert = false;
  }

  //Hazardous equipment
  if (houseEmpty == true && ovenActive == true) {
    hazardAlert = true;
  } else {
    hazardAlert = false;
  }
  
  //Washing machine almost done
  if (washingNearDone == true) {
    washingAlert = true;
  } else {
    washingAlert = false;
  }

  checkDelay = millis() + 2000;
}

//When Kota (heating system) sends values that we need, use them to determine state of the house
void heating(OOCSIEvent event) {
  println("Heating -----------------------------------------------------------");
  if (event.has("RoomOccupancy")) {
    boolean[] RoomOccupancy = event.getBooleanArray("RoomOccupancy", null);
    if (RoomOccupancy != null) {
      for (boolean i : RoomOccupancy) { //If just one room is occupied, the house is not empty
        if (i == true) {
          houseEmpty = false;
          break;
        } else {
          houseEmpty = true;
        }
      }
    }
    printArray(RoomOccupancy); //Debugging
  }

  if (event.has("consumptions")) {
    float[] RoomHeatingUsage = event.getFloatArray("consumptions", null);
    if (RoomHeatingUsage != null) {
      for (int i = 0; i < RoomHeatingUsage.length; i++) {
        if (RoomHeatingUsage[i] > 0) { //If there is just one room heating, it means that heating is on
          houseHeating = true;
          break;
        } else {
          houseHeating = false;
        }
      }
    }
    printArray(RoomHeatingUsage); //Debugging
  }
  
  println();
}

//When Tala (energy management system) sends values that we need, use them to determine state of the house
void Tala(OOCSIEvent event) {
  if (event.has("Oven_On")) {
    println("Tala -----------------------------------------------------------");
    ovenActive = event.getBoolean("Oven_On", false);
    println(ovenActive);
    println();
  }
}

//When Lavo (energy management system) sends values that we need, use them to determine state of the house
void laundry(OOCSIEvent event) {
  if (event.has("lavo_almost_done")) {
    println("Lavo");
    washingNearDone = event.getBoolean("lavo_almost_done", false);
    println(washingNearDone);
    println();
  }
}
