
//Create trigger button for 'someone walking by', and toggle button to open/close doors and windows (as requested by Kota)
void GUIsetup() {

  PImage[] activeButtons = {loadImage("Active_Button.png"), loadImage("Active_Button_Hover.png"), loadImage("Active_Button.png")};
  activeB = cp5.addButton("active")
    .setPosition(width-270, height-100)
    .setImages(activeButtons)
    .updateSize();

  PImage[] windoorToggle = {loadImage("Windoors_Closed.png"), loadImage("Windoors_Opened.png")};
  windoorT = cp5.addToggle("windoor")
    .setValue(false)
    .setPosition(50, height-100)
    .setImages(windoorToggle)
    .updateSize();
}

//Called when trigger is pressed. Sends value to other groups which then automatically send their values back.
public void active() {
  println("Ipsum Active!");
  oocsi.channel("Ipsum").data("ips_active", true).send();

  //Reset duration of each scenario. If multiple scenarios are activated in draw(), these times are corrected
  hazardDuration = millis() + 10000;
  energyDuration = millis() + 10000;
  washingDuration = millis() + 10000;
  
  delay(100);
}

//Called when toggle is pressed. Sends arrays with names of doors/windows if open, sends array of 'none' when closed (as requested by Kota)
public void windoor(boolean value) {

  if (value == false) {
    oocsi.channel("Ipsum").data("ips_windoors", false).data("ips_windoors_name", windoors_false).send();
    println("False");
  }

  if (value == true) {
    oocsi.channel("Ipsum").data("ips_windoors", true).data("ips_windoors_name", windoors_true).send();
    println("True");
  }
}
