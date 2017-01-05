/*
 * Ball_On_Plate_Software_1_0_0
 *
 * File:   Ball_On_Plate_Software_1_0_0.ino
 *
 * Description:
 *
 * This file is software part of the CSE395 Undergraduate Project I Ball On Plate
 *
 * @author CSE_395_Undergraduate_Project_I_Group_7
 * @since Friday 25 November 2016
 */

/**************************** ABOUT INCLUDE LIBRARYS *******************************************/
 #include <Servo.h>  /* Include for servos */
 #include <string.h> /*Include for strtok  */
 #include <PID_v1.h> /*Include for PID     */
 #include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
 
/*
 * Struct for Ball.
 *
 * Description:
 *
 * Ball have:
 *      - Current X
 *      - Current Y
 *      - Previous X
 *      - Previous Y
 */
struct Ball{

  double currentX;              /* Current Ball X Position      */
  double currentY;              /* Current Ball Y Position      */
  double previousX;             /* Previous Ball X Position     */
  double previousY;             /* Previous Ball Y Position     */
  
}ballStruct;

/*
 * Struct for Plate.
 *
 * Description:
 *
 * Plate have:
 *      - Angle X
 *      - Angle Y
 */
struct Plate{

  int AngleX;              /* Current Plate X Angle      */
  int AngleY;              /* Current Plate Y Angle      */
  
}plateStruct;

/*
 * Struct for Servos.
 *
 * Description:
 *
 * Servos have:
 *      - Angle X
 *      - Angle Y
 */
struct Servos{

  Servo firstServo;            /* Create servo object to control a first servo  */
  Servo secondServo;           /* Create servo object to control a second servo */
  double AngleX;              /* Current Servos X Angle                        */
  double AngleY;              /* Current Servos Y Angle                        */
  double previousAngleX; 
  double previousAngleY; 
}servosStruct;

/*
 * Struct for Outputs to Visual and Inputs from Visual.
 *
 * Description:
 *
 * Outputs to Visual and Inputs from Visual have:
 *      - String Output
 *      - String Input
 */
struct Visual{

  String outputsToVisual;           /* Outputs To Visual      */
  char inputsFromVisual[15];        /* Inputs To Visual       */
  const char end_char = '$';        /* End character         */
  const char connect_end = '!';        /* End character         */
  
}visualStruct;

/*
 * Struct for Operation.
 *
 * Description:
 *
 * Operation have:
 *      - operation
 *      - equilibriumX
 *      - equilibriumY
 */
struct Operation{

  int operation;             /* Operation */
  double equilibriumX;          /* Current Equilibrium X Position  */
  double equilibriumY;          /* Current Equilibrium Y Position  */
  
}operationStruct;

int noBall = 1;

/**************************** ABOUT PID **************************************************************************************************************************************************************************/

double XKp = 0.25;
double XKi = 0.008;
double XKd = 0.16;

double YKp = 0.25;
double YKi = 0.008;
double YKd = 0.14;

PID myPID(&ballStruct.currentX, &servosStruct.AngleX, &operationStruct.equilibriumX, XKp, XKi, XKd, REVERSE);
PID myPIDY(&ballStruct.currentY, &servosStruct.AngleY, &operationStruct.equilibriumY, YKp, YKi, YKd, DIRECT);


double AndroidXKp = 0.28;
double AndroidXKi = 0.13;
double AndroidXKd = 0.13;

double AndroidYKp = 0.27;
double AndroidYKi = 0.13;
double AndroidYKd = 0.13;
        
PID AndroidXPID(&ballStruct.currentX, &servosStruct.AngleX, &operationStruct.equilibriumX, AndroidXKp, AndroidXKi, AndroidXKd, REVERSE);
PID AndroidYPID(&ballStruct.currentY, &servosStruct.AngleY, &operationStruct.equilibriumY, AndroidYKp, AndroidYKi, AndroidYKd, DIRECT);


/**************************** ABOUT PINS **************************************************/

// Touch panel output wiring. These define the pins 
// used to apply voltages across the touch screen
#define leftOutput A0    // LEFT to digital output 2 
#define bottomOutput A1  // BOTTOM to digital output 3 
#define rightOutput A2   // RIGHT to digital output 4 
#define topOutput A3     // TOP to Digital output 5 


// Touch panel input wiring. These define the analog 
// input pins used to read the voltage from the screen
#define topInput A1      // TOP also to analog input 3 
#define rightInput A2    // RIGHT also to analog input 4 

#define RED 2
#define GREEN 3
#define YELLOW 4

#define SERVO_1 8
#define SERVO_2 7

#define ANDROID_VISUAL_TRANSITION_LED_PIN 6
#define ANDROID_VISUAL_TRANSITION_BUTTON_PIN 5
#define ANDROID_VISUAL_TRANSITION_BUTTON_PUSH_TIME  10
#define VISUAL_MODE  1
#define ANDROID_MODE  2


int countAndroidVisualTransition= 0;
int addNumForCountAndroidVisualTransition = 1;
int AndroidVisualMode = VISUAL_MODE;

/**************************** ABOUT LEDS **************************************************/
/**
 * Set Red Led
 */
void setRedLed(){
    pinMode(RED, OUTPUT); 
}

/**
 * Make Red Led High
 */
void errorForSerialCommunication(){
    digitalWrite (RED, HIGH);
}

/**
 * Set Green Led
 */
void setGreenLed(){
    pinMode(GREEN, OUTPUT); 
}

/**
 * Make Green Led High
 */
void errorForServos(){
    digitalWrite (GREEN, HIGH);
}

/**
 * Set Yellow Led
 */
void setYellowLed(){
    pinMode(YELLOW, OUTPUT); 
}

/**
 * Make Yellow Led High
 */
void errorForTouchScreen(){
    digitalWrite (YELLOW, HIGH);
}

/**************************** ABOUT VISUAL **************************************************/

int checkReaded = 0;


/**
 * Create Connect For Serial Communication
 */
void createConnectForSerialCommunicationToVisual(){
  Serial.begin(115200);                  /* Create Connect For Serial Communication               */ 
  if (!Serial)                          /* Check The Connection                                 */ 
      errorForSerialCommunication();   /* If The Connection not valid, Then call error function */
}

/**
 * Take Inputs From Visual
 * 
 * Take: visualStruct.inputsFromVisual
 */
void takeInputsFromVisual(){

  if(AndroidVisualMode == VISUAL_MODE){
  
    int i = 0;
    
    char c = readChar();
    bool check = true;


    
    if(c == visualStruct.end_char)
        check = false;
    if(c == visualStruct.connect_end)
        check = false;
    
    while(check){
         
      visualStruct.inputsFromVisual[i] = c; 
      i++;
      
      c = readChar();
  
      if(c == visualStruct.end_char)
        check = false;
      if(c == visualStruct.connect_end)
        check = false;
    }
  
    if(c == visualStruct.connect_end){
      visualStruct.inputsFromVisual[0] ='\0' ;
    }
      
    visualStruct.inputsFromVisual[i] = '\0';

    checkReaded = 1;

  }

  
}

/**
 * Wait Until Serial is available
 */
char readChar(){
  while(!Serial.available());
  return Serial.read();
}

/**
 * Send Outputs To Visual
 * 
 * Send: visualStruct.outputsToVisual
 */
void sendOutputsToVisual(){

  if(AndroidVisualMode == VISUAL_MODE && noBall == 0){

    Serial.print(visualStruct.outputsToVisual);
       
  }
  else if(AndroidVisualMode == VISUAL_MODE){

    Serial.print("+999_+999_+000_+000$");
        
  }
  else if (noBall == 1) {
   
    Serial.print("!+999_+999_+000_+000$");
      
  }else{
    String newString = "!" + visualStruct.outputsToVisual;
    Serial.print(newString);
  }

  
  //  if(AndroidVisualMode == VISUAL_MODE && checkReaded == 1){
      
  //  }
  //  else if (checkReaded == 1){
  //    Serial.print("!");
  //    checkReaded == 0;
  //  }
 
}

/*
 * Make A String For Send Outputs To Visual
 *
 * Outputs To Visual String like: +ballPositionX_+ballPositionY_platePositionX_platePositionY$
 */
void makeAStringForSendOutputsToVisual(){

    int number[4];
    int i, j, len;

    number[0] = ballStruct.currentX ;
    number[1] = ballStruct.currentY ;
    number[2] = servosStruct.AngleX;
    number[3] = servosStruct.AngleY;
  /*  
    - operationStruct.operation
     - operationStruct.equilibriumX
    - operationStruct.equilibriumY
    


    number[0] = 100 + operationStruct.operation;
    number[1] = operationStruct.equilibriumX ;
    number[2] = operationStruct.equilibriumY;
    number[3] = servosStruct.AngleX;*/
 
    visualStruct.outputsToVisual = "";

    for(i=0; i<4; ++i){
          
        if(number[i] >= 0){
            visualStruct.outputsToVisual += '+';
        }
        else{
            visualStruct.outputsToVisual += '-';
            number[i] = -number[i];
        }
            
        String skip;
        skip += number[i];
        len = skip.length();

        for(j=0; j<3-len; ++j){
          visualStruct.outputsToVisual += '0';
        }
        visualStruct.outputsToVisual += skip;
        if(i != 3)
            visualStruct.outputsToVisual += '_';
    }
    visualStruct.outputsToVisual += visualStruct.end_char; 
}

int previousOperation= 1;
double previousEqulibrumX = 0;
double previousEqulibrumY = 0;
int count = 0;

/*
 * Split The Inputs String For Inputs From Visual
 *
 * Inputs From Visual String like: operation_+equilibriumPositionX_+equilibriumPositionY
 * 
 *  Split The Inputs String and Set: 
 *      - operationStruct.operation
 *      - operationStruct.equilibriumX
 *      - operationStruct.equilibriumY
 */
void splitTheinputsFromVisualString(){

    if(visualStruct.inputsFromVisual[0] != '\0'){

  
      int number[3];
      int i=0;
      char *str = visualStruct.inputsFromVisual;
      char *temp;
    
      while ((temp = strtok_r(str, "_", &str)) != NULL) {
          number[i] = atoi(temp);
          ++i;
      }
    
      operationStruct.operation = number[0];
      operationStruct.equilibriumX = number[1] ;
      operationStruct.equilibriumY = number[2] ; 
  
   /*   Serial.print("O: ");
      Serial.print(operationStruct.operation);
          Serial.print(" X: ");
          Serial.print(operationStruct.equilibriumX);
              Serial.print(" Y: ");
              Serial.println(operationStruct.equilibriumY);*/
  
       if( operationStruct.operation == 1){
  
          if(previousEqulibrumX != operationStruct.equilibriumX || previousEqulibrumY != operationStruct.equilibriumY){
              servosStruct.firstServo.attach(SERVO_1);   /* Attaches the servo on pin SERVO_1 to the servo object */
              servosStruct.secondServo.attach(SERVO_2);
                            count = 0;
          }
       }else if(previousOperation != operationStruct.operation){
           servosStruct.firstServo.attach(SERVO_1);   /* Attaches the servo on pin SERVO_1 to the servo object */
           servosStruct.secondServo.attach(SERVO_2);
                         count = 0;
       }
  
     previousEqulibrumX = operationStruct.equilibriumX;
     previousEqulibrumY = operationStruct.equilibriumY;
         
   }
}



/**************************** ABOUT TOUCH SCREEN **************************************************/

/**
 * Create Connect For Touch Screen
 */
void createConnectForTouchScreen(){}


int countActive = 0;


/**
 * Take Inputs From Touch Screen
 * 
 * Before Taking Set: 
 *      - ballStruct.previousX = ball.currentX;
 *      - ballStruct.previousY = ball.currentY;
 * 
 * Take Inputs For: 
 *      - ballStruct.currentX
 *      - ballStruct.currentY
 */
void takeInputsFromTouchScreen(){

    int touchX;
    int touchY;
    int mapX,mapY;
    double mapTouchX;
    double mapTouchY;
    
     // Horizontal routine
     // 1: apply voltages to left & right
     // 2: turn top & bottom digital outputs off
     // 3: read top via analog inputs
     pinMode(leftOutput, OUTPUT); 
     digitalWrite(leftOutput, LOW);     // set Left = ground 
     pinMode(rightOutput, OUTPUT); 
     digitalWrite(rightOutput, HIGH);   // set Right = Vcc 
     
     pinMode(topOutput, INPUT);         // Sets Top and Bottom to inputs
     pinMode(bottomOutput, INPUT);      // to give them high impedence
     
     delay(20);  
     touchX = analogRead(topInput);   // wait a bit, then read from Top 
     touchX = touchX-540;
     mapX = map(touchX, 0, 1024, 0, 255);
     
     // Vertical routine
     // 1: apply voltages to top & bottom
     // 2: turn left & right digital outputs off
     // 3: read right via analog inputs
     pinMode(bottomOutput, OUTPUT); 
     digitalWrite(bottomOutput, LOW); // Set Bottom to 0v
     pinMode(topOutput, OUTPUT); 
     digitalWrite(topOutput, HIGH);  // set Top to +Vcc
    
     pinMode(rightOutput, INPUT); 
     pinMode(leftOutput, INPUT); 
     delay(10);
     touchY = analogRead(rightInput); // wait a bit, then read from Right
     touchY = touchY-500;
     mapY = map(touchY, 0, 1024, 0, 255);

     if(mapX == -28   || mapY == 2 || mapY == 4 || mapX > 100 || mapX < -100 || mapY > 80 || mapY < -80){
        // no ball
        ++count;
        countActive = 0; 
        
        if(count > 40){
              noBall = 1;
              servosStruct.firstServo.write(96);
              servosStruct.secondServo.write(86);
              delay(200);
              servosStruct.firstServo.detach();   /* Attaches the servo on pin SERVO_1 to the servo object */
              servosStruct.secondServo.detach();   
        }
          
     }else{

            ++countActive;
            if(countActive > 3){
              noBall = 0;
              servosStruct.firstServo.attach(SERVO_1);   /* Attaches the servo on pin SERVO_1 to the servo object */
              servosStruct.secondServo.attach(SERVO_2);
              count = 0;
              ballStruct.currentX = mapX ;
              ballStruct.currentY = mapY ;
              if( operationStruct.operation == 1)
                calculateServosAngles();                       /* Calculate Servos Angles                   */
            }
            
     }

}

/**************************** ABOUT SERVOS **************************************************/

/**
 * Create Connect For Servos
 */
void createConnectForServos(){
  servosStruct.firstServo.attach(SERVO_1);   /* Attaches the servo on pin SERVO_1 to the servo object */
  servosStruct.secondServo.attach(SERVO_2);  /* Attaches the servo on pin SERVO_2 to the servo object */

  if (servosStruct.firstServo.attached()==false || servosStruct.secondServo.attached()==false)  /* Check The Connection          */ 
      errorForServos();                                                                         /* If The Connection not valid, Then call error function */
}

/**
 * SendAngle
 */
void sendAngle(){

    if(ballStruct.currentX - operationStruct.equilibriumX > 4 || ballStruct.currentX - operationStruct.equilibriumX < -4)
      servosStruct.firstServo.write(96 + servosStruct.AngleX);
      
    if(ballStruct.currentY - operationStruct.equilibriumY > 4 || ballStruct.currentY - operationStruct.equilibriumY < -4)
      servosStruct.secondServo.write(86 - servosStruct.AngleY);
}


/**
 * Send Outputs To Servos
 * 
 * Send: servosStruct.AngleX for Servo 1
 * Send: servosStruct.AngleY for Servo 2
 */
void sendAngleToServos(){

    /* if operation is Equilibrium then make Rectangle */
    if( operationStruct.operation == 1){
      sendAngle();
    }

    makeRectangle(); /* if operation is Rectangle then make Rectangle */
    makeCirle();     /* if operation is Circle then make Rectangle */
    makeInfinity();  /* if operation is Infinity then make Rectangle */

    previousOperation = operationStruct.operation; /* Update previousOperation */
}


/**
 * About Infinity Movement
 */
int infinityStep = 0;
int infinityCount = 0;
int infinityTimeLimit = 15;

/**
 * Make Infinity Movement
 */
void makeInfinity(){

  if(previousOperation != operationStruct.operation && operationStruct.operation  == 2){   
      infinityCount = 0; 
      infinityStep = 0;  
  }

  // Infinity
    if(operationStruct.operation == 2){
      
        if(infinityStep == 0){
          //70 65
          operationStruct.equilibriumX = -17;
          operationStruct.equilibriumY = -2;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
            
          ++infinityCount;
          if(infinityCount > infinityTimeLimit)
             infinityStep = 1; 
        }
        else if(infinityStep == 1){
          //70 -65
          operationStruct.equilibriumX = -7;
          operationStruct.equilibriumY = 16;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*2))
             infinityStep = 2; 
        }
        else if(infinityStep == 2){
          //-70 -65
          operationStruct.equilibriumX = 16;
          operationStruct.equilibriumY = 25;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*3))
             infinityStep = 3; 
        }
        else if(infinityStep == 3){
          //-70 -65
          operationStruct.equilibriumX = 34;
          operationStruct.equilibriumY = 15;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*4))
             infinityStep = 4; 
        }
        else if(infinityStep == 4){
          //-70 -65
          operationStruct.equilibriumX = 41;
          operationStruct.equilibriumY = -20;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*5))
             infinityStep = 5; 
        }
        else if(infinityStep == 5){
          //-70 -65
          operationStruct.equilibriumX = 40;
          operationStruct.equilibriumY = -14;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*6))
             infinityStep = 6; 
        }
        else if(infinityStep == 6){
          //-70 -65
          operationStruct.equilibriumX = 28;
          operationStruct.equilibriumY = -30;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*7))
             infinityStep = 7; 
        }
        else if(infinityStep == 7){
          //-70 65
          operationStruct.equilibriumX = 10;
          operationStruct.equilibriumY = -24;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*8)){
            infinityStep = 8; 
          }             
        }
        else if(infinityStep == 8){
          //-70 65
          operationStruct.equilibriumX = -16;
          operationStruct.equilibriumY = -3;


          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*9)){
            infinityStep = 9;
          }             
        }
        else if(infinityStep == 9){
          //-70 65
          operationStruct.equilibriumX = -36;
          operationStruct.equilibriumY = 28;


          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*10)){
            infinityStep = 10;
          }             
        }
        else if(infinityStep == 10){
          //-70 65
          operationStruct.equilibriumX = -48;
          operationStruct.equilibriumY = 20;


          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*11)){
           infinityStep = 11;
          }             
        }
        else if(infinityStep == 11){
          //-70 65
          operationStruct.equilibriumX = -60;
          operationStruct.equilibriumY = 0;


          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*12)){
           infinityStep = 12;
          }             
        }
        else if(infinityStep == 12){
          //-70 65
          operationStruct.equilibriumX = -48;
          operationStruct.equilibriumY = -22;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*13)){
           infinityStep = 13;
          }             
        }
        else{
          //-70 65
          operationStruct.equilibriumX = -30;
          operationStruct.equilibriumY = -30;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++infinityCount;
          if(infinityCount > (infinityTimeLimit*14)){
            infinityStep = 0; 
            infinityCount = 0;
          }             
        }
    }
}

/**
 * About Rectangle Movement
 */
int rectangleStep = 0;
int rectangleCount = 0;
int rectangleTimeLimit = 60;

/**
 * Make Rectangle Movement
 */
void makeRectangle(){

   if(previousOperation != operationStruct.operation && operationStruct.operation  == 3){   
      rectangleCount = 0; 
      rectangleStep = 0;  
    }

    // Rectangle
    if(operationStruct.operation == 3){
      
        if(rectangleStep == 0){
          //70 65
          operationStruct.equilibriumX = 40;
          operationStruct.equilibriumY = 35;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++rectangleCount;
          if(rectangleCount > rectangleTimeLimit)
             rectangleStep = 1; 
        }
        else if(rectangleStep == 1){
          //70 -65
          operationStruct.equilibriumX = 40;
          operationStruct.equilibriumY = -35;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++rectangleCount;
          if(rectangleCount > rectangleTimeLimit+rectangleTimeLimit)
             rectangleStep = 2; 
        }
        else if(rectangleStep == 2){
          //-70 -65
          operationStruct.equilibriumX = -40;
          operationStruct.equilibriumY = -35;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++rectangleCount;
          if(rectangleCount > rectangleTimeLimit+rectangleTimeLimit+rectangleTimeLimit)
             rectangleStep = 3; 
        }
        else{
          //-70 65
          operationStruct.equilibriumX = -40;
          operationStruct.equilibriumY = 35;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

         sendAngle();
          
          ++rectangleCount;
          if(rectangleCount > rectangleTimeLimit+rectangleTimeLimit+rectangleTimeLimit+rectangleTimeLimit){
            rectangleStep = 0; 
            rectangleCount = 0;
          }             
        }
    }
  
}

/**
 * About Circle Movement
 */
int cirleStep = 0;
int cirleCount = 0;
int cirleTimeLimit = 30;

/**
 * Make Circle Movement
 */
void makeCirle(){
  
    if(previousOperation != operationStruct.operation && operationStruct.operation  == 4){   
      cirleCount = 0; 
      cirleStep = 0;  
    }

    // Circle
    if(operationStruct.operation == 4){
      
        if(cirleStep == 0){
          //70 65
          operationStruct.equilibriumX = 40;
          operationStruct.equilibriumY = 8;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
            
          ++cirleCount;
          if(cirleCount > cirleTimeLimit)
             cirleStep = 1; 
        }
        else if(cirleStep == 1){
          //70 -65
          operationStruct.equilibriumX = 30;
          operationStruct.equilibriumY = -40;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit)
             cirleStep = 2; 
        }
        else if(cirleStep == 2){
          //-70 -65
          operationStruct.equilibriumX = -2;
          operationStruct.equilibriumY = -51;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit)
             cirleStep = 3; 
        }
        else if(cirleStep == 3){
          //-70 -65
          operationStruct.equilibriumX = -35;
          operationStruct.equilibriumY = -41;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit)
             cirleStep = 4; 
        }
        else if(cirleStep == 4){
          //-70 -65
          operationStruct.equilibriumX = -40;
          operationStruct.equilibriumY = 8;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit)
             cirleStep = 5; 
        }
        else if(cirleStep == 5){
          //-70 -65
          operationStruct.equilibriumX = -35;
          operationStruct.equilibriumY = 29;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit)
             cirleStep = 6; 
        }
        else if(cirleStep == 6){
          //-70 -65
          operationStruct.equilibriumX = -7;
          operationStruct.equilibriumY = 54;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit)
             cirleStep = 7; 
        }
        else{
          //-70 65
          operationStruct.equilibriumX = 30;
          operationStruct.equilibriumY = 30;

          calculateServosAngles();                       /* Calculate Servos Angles                   */

          sendAngle();
          
          ++cirleCount;
          if(cirleCount > cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit+cirleTimeLimit){
            cirleStep = 0; 
            cirleCount = 0;
          }             
        }
    }
}


/**************************** ABOUT PID **************************************************/

/**
 * Calculate Servos Angles
 * 
 * Check The Operation and Calculate and Set: 
 *      - servosStruct.AngleX
 *      - servosStruct.AngleY
 */
void calculateServosAngles(){

  if(AndroidVisualMode == VISUAL_MODE){
     myPID.Compute();
     myPIDY.Compute();
  }
    
  else{
    AndroidXPID.Compute();
    AndroidYPID.Compute();
  }


     
}


/**************************** ABOUT BLUETOOTH **************************************************/


void takeInputsFromBluetooth(){

  if(AndroidVisualMode == ANDROID_MODE){
    
    int i = 0;
  
    char c = readCharBlue();
    bool check = true;

      if(c == visualStruct.end_char)
          check = false;
      if(c == visualStruct.connect_end)
          check = false;
      
      while(check){
           
        visualStruct.inputsFromVisual[i] = c; 
        i++;
        
        c = readCharBlue();
    
        if(c == visualStruct.end_char)
          check = false;
        if(c == visualStruct.connect_end)
          check = false;
      }
    
      if(c == visualStruct.connect_end){
        visualStruct.inputsFromVisual[0] ='\0' ;
      }
        
      visualStruct.inputsFromVisual[i] = '\0';
  
  }
}

/**
 * Wait Until Serial is available
 */
char readCharBlue(){
  while(!mySerial.available());
  return mySerial.read();
}


/**************************** ABOUT Android Visual Transition **************************************************/

void setupForAndroidVisualTransition(){
  pinMode(ANDROID_VISUAL_TRANSITION_LED_PIN, OUTPUT);
  pinMode(ANDROID_VISUAL_TRANSITION_BUTTON_PIN, INPUT);
}

void checkTheAndroidVisualTransition(){
  
  if ( digitalRead(ANDROID_VISUAL_TRANSITION_BUTTON_PIN) == HIGH)
  {

    countAndroidVisualTransition = countAndroidVisualTransition +  addNumForCountAndroidVisualTransition;
     
    if (countAndroidVisualTransition>ANDROID_VISUAL_TRANSITION_BUTTON_PUSH_TIME)
    {     
       digitalWrite(ANDROID_VISUAL_TRANSITION_LED_PIN, HIGH);
       addNumForCountAndroidVisualTransition = -1;
       AndroidVisualMode = ANDROID_MODE;
    }
    
    if (countAndroidVisualTransition<0)
    {
        
       digitalWrite(ANDROID_VISUAL_TRANSITION_LED_PIN, LOW);
       addNumForCountAndroidVisualTransition = +1;
       AndroidVisualMode = VISUAL_MODE;
    }
 }
   
}

/**************************** SETUP **************************************************/

void setup (){

   mySerial.begin(9600);
   operationStruct.operation = 1;
  operationStruct.equilibriumX = 0;
  operationStruct.equilibriumY = 0;
  setRedLed();                                    /* Set Red Led                                       */
  setGreenLed();                                  /* Set Green Led                                     */
  setYellowLed();                                 /* Set Yellow Led                                    */
  createConnectForTouchScreen();                  /* Create Connect For Touch Screen                   */
  createConnectForServos();                       /* Create Connect For Servos                         */
  createConnectForSerialCommunicationToVisual();  /* Create Connect For Serial Communication To Visual */
  setupForAndroidVisualTransition();

  servosStruct.firstServo.write(96);
  servosStruct.secondServo.write(86);
  servosStruct.previousAngleX = 96;
  servosStruct.previousAngleY = 86;

  /* PID Settings */
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-60,60);
  myPID.SetSampleTime(30);
  myPIDY.SetMode(AUTOMATIC);
  myPIDY.SetOutputLimits(-60,60);
  myPIDY.SetSampleTime(30);

  /* Android PID Settings */
  AndroidXPID.SetMode(AUTOMATIC);
  AndroidXPID.SetOutputLimits(-60,60);
  AndroidXPID.SetSampleTime(20);
  AndroidYPID.SetMode(AUTOMATIC);
  AndroidYPID.SetOutputLimits(-60,60);
  AndroidYPID.SetSampleTime(20);
}

/**************************** LOOP **************************************************/

void loop (){
  
  takeInputsFromVisual();                        /* Take Inputs From Visual                   */
  if(mySerial.available() > 0)
    takeInputsFromBluetooth();                   /* Take Inputs From Bluetooth                */
  
  checkTheAndroidVisualTransition();             /* Check The Android Visual Transition       */
  splitTheinputsFromVisualString();              /* Split The inputs From Visual String       */
  takeInputsFromTouchScreen();                   /* Take Inputs From Touch Screen             */
  sendAngleToServos();                           /* Send Angle To Servos                      */
  makeAStringForSendOutputsToVisual();           /* Make A String For Send Outputs To Visual  */
  sendOutputsToVisual();                         /* Send Outputs To Visual                    */
}
