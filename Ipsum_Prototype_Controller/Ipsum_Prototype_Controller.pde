import processing.serial.*;

Serial myPort;
String port = "COM5"; //Adjust port accordingly before running code

void setup() {
  size(400, 400);
  myPort = new Serial(this, port, 9600);
}

void draw() {
  // Nothing
}

//Use arrow keys to send number that activates specific scenario on Ipsum prototype
void keyPressed() {
  if (keyCode == LEFT) {
    myPort.write('1');
  } else if (keyCode == UP) {
    myPort.write('2');
  } else if (keyCode == RIGHT) {
    myPort.write('3');
  } else if (keyCode == DOWN) {
    myPort.write('4');
  } else {
    // Nothing
  }
  
  delay(500); //Fat finger corrector
}
