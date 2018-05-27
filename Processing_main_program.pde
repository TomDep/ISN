/* -----------------------------------------------------------
 *                Processing main program
 *                     Made by
 *      Belloncle Nicolas, Bernard Martin, De Pasquale Tom
 *                     2017/2018
 * -----------------------------------------------------------
 */

//Import of the library
import processing.serial.*;
//Create the Serial Object for the link between Arduino & Processing
Serial arduinoPort;

boolean initialized = false;

//Array to stock values
String[] data;

//Values
String totalAverageSpeed = "00,00";
String totalPedalingTime = "00,00";

String averageSpeed = "00,00";
String pedalingTime = "00,00";

String maxSpeed = "00,00";
String maxTime = "00,00";

String speed = "00,00";

void setup()
{
  size(840, 582);  //Size of the window
  background(255);  //Backround color
  
  text("Chargement de l'application ...", 20, 50);
  
  frame.setTitle("Compteur vélo - Projet d'ISN");  
}

void draw()
{
  if (serialInitialized())
  {
    getData();
    
    display();
    
    if (data != null)
    {
      printArray(data);
      
      totalAverageSpeed = data[0];
      totalPedalingTime = data[1];
    
      averageSpeed = data[2];
      pedalingTime = data[3];
      
      maxSpeed = data[4];
      maxTime = data[5];
      
      speed = data[6];
    }
  }
  else if (!serialInitialized())
  {
    textSize(20);
    fill(255, 0, 0);
    background(200);
    println("L'ordinateur n'est pas connecté à Arduino");
    text("L'ordinateur n'est pas connecté à Arduino. \nVérifiez le câblage puis relancez l'application.", 20, 50);
  }
}

void getData()
{
  String fullData;

  if (arduinoPort.available() > 0)
  {
    fullData = arduinoPort.readStringUntil('\n');
    fullData = trim(fullData);
    if (fullData != null)
    {
      data = split(fullData, ';');
    }
  }
}


boolean serialInitialized()
{
  if (initialized == false)
  {
    int i = 0;
    int len = Serial.list().length;  //get number of ports available 
    printArray(Serial.list());   //print list of ports to screen
    println("Serial Port Count = " + len); //print count of ports to screen
    if (len > 0)  //it must be at least one port connected
    {
      for (i = 0; i < len; i++) //testing all the ports
      { 
        println("Testing port " + Serial.list()[i]); 
        arduinoPort = new Serial(this, Serial.list()[i], 9600);      // Open port i in list 

        long millisStart = millis();
        while ((millis() - millisStart) < 2000) ; //wait for USB port reset (Guessed at 2 secs) 

        //Maybe replace it by delay() because it is in draw function

        arduinoPort.clear();    // empty buffer (incase of trash) 
        arduinoPort.bufferUntil('T'); //buffer until there is a 'T'
        millisStart = millis(); 
        while ((millis() - millisStart) < 100) ; //collect some chars
        //Waiting 100ms

        if (arduinoPort.available() > 0)   //if we have a character 
        { 
          char c = arduinoPort.readChar(); //get the character 
          println("Char available = " + c); 

          if (c == 'T')    //if we got a 'T' 
          { 
            arduinoPort.write('A');  //send a 'A' 

            initialized = true;
            println("Serial Port used = " + Serial.list()[i]);

            return true;
            //leave for loop
          }
        } else 
        arduinoPort.stop();   //if no 'T', stop port
      }
      return false;
    } else
    {
      return false;
    }
  } 
  else
  {
    return true;
  }
}

void display()
{
  pushMatrix();
    translate(10, 10);
    fill(180);
    rect(0, 0, 810, 562, 0, 13, 0, 13);  //Main rectangle
    fill(255);
    rect(20, 10, 243, 452, 0, 13, 0, 13);  //secondary rectangle -> Total
    fill(0);
    textSize(25);
    text("Compteur Vélo 3000", 21, 492);
    pushMatrix();
      translate(20,510);
      fill(255);
      rect(0,0,320,28, 13, 13, 13, 13);  //Speed
      fill(0);
      textSize(18);
      text("Vitesse instantannée :  "+ speed +" km/h", 10, 20);
      textSize(12);
      text("Réalisé dans le cadre du projet ISN par BELLONCLE Nicolas, BERNARD Martin et DE PASQUALE Tom   2017/2018",135,45);
      fill(255);
      rect(640, 0, 130, 30,13,13,13,13);  //Time display
      int x=hour();  //Formula to get the time
      int y=minute();
      int z=second();
      fill(0);
      textSize(18);
      text(x+":",670,20);  //Displaying time variables
      text(y+":",700,20);
      text(z,730,20);
    popMatrix();
    pushMatrix();
      translate(20, 10);
      fill(0);
      textSize(20);
      text("Total :", 10, 20);
      pushMatrix();
        translate(10,50);
        fill(230);
        rect(0,0,220,176,0,13,0,13);  //Average Speed
        fill(0);
        textSize(18);
        text("Vitesse Moyenne :", 10, 20);
        pushMatrix();
          translate(130,146);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(totalAverageSpeed, 12, 22);
        popMatrix();
        fill(230);
        rect(0,206,220,176,0,13,0,13);  //Pedaling Time
        fill(0);
        textSize(18);
        text("Durée de pédalage :" , 10, 226);
        pushMatrix();
          translate(130,352);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(totalPedalingTime, 12, 22);
        popMatrix();
      popMatrix();
    popMatrix();
    fill(255);
    rect(283, 10, 243, 452, 0, 13, 0, 13);  //Secondary Rectangle
    pushMatrix();
      translate(283, 10);
      fill(0);
      textSize(18);
      text("Depuis le dernier reset :", 10, 20);
            pushMatrix();
        translate(10,50);
        fill(230);
        rect(0,0,220,176,0,13,0,13);  //Average Speed
        fill(0);
        textSize(18);
        text("Vitesse Moyenne :", 10, 20);
        pushMatrix();
          translate(130,146);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(averageSpeed, 12, 22);
        popMatrix();
        fill(230);
        rect(0,206,220,176,0,13,0,13);  //Pedaling Time
        fill(0);
        textSize(18);
        text("Durée de pédalage :" , 10, 226);
        pushMatrix();
          translate(130,352);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(pedalingTime, 12, 22);
        popMatrix();
      popMatrix();
    popMatrix();
    fill(255);
    rect(546, 10, 243, 452, 0, 13, 0, 13);  //Secondary rectangle
    pushMatrix();
      translate(546,10);
      fill(0);
      textSize(20);
      text("Les maximums :", 10, 20);
            pushMatrix();
        translate(10,50);
        fill(230);
        rect(0,0,220,176,0,13,0,13);  //maximum speed
        fill(0);
        textSize(18);
        text("Vitesse maximum :", 10, 20);
        pushMatrix();
          translate(130,146);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(maxSpeed, 12, 22);
        popMatrix();
        fill(230);
        rect(0,206,220,176,0,13,0,13);  //maximum distance
        fill(0);
        textSize(18);
        text("Durée de pédalage :" , 10, 226);
        pushMatrix();
          translate(130,352);
          fill(255);
          rect(0,0,90,30,0,13,0,13);
          fill(0);
          textSize(22);
          text(maxTime, 12, 22);
        popMatrix();
      popMatrix();
    popMatrix();
  popMatrix();
}
