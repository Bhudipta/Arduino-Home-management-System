#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_PUSH_BUTTON_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD


#include <OneSheeld.h>

HttpRequest GetControl("WebsitePath/api/GetControl");
HttpRequest SetSensors("WebsitePath/api/SetSensors/Temperature/");  

int LightsPin = 11;
int DoorLockPin = 12;
int WindowsPin = 13;
int DistanceSensorPin = A0;
int PIRSensorPin = 7;
bool Flag = true;
bool Flag2 = false;
bool Flag3=true;
struct ControlParameters
{
  char Lights;
  char DoorLock;
  char Windows;
};

ControlParameters CP;

void setup() 
{

  OneSheeld.begin();

  pinMode(LightsPin,OUTPUT);
  pinMode(DoorLockPin,OUTPUT);
  pinMode(WindowsPin,OUTPUT);
  pinMode(DistanceSensorPin,INPUT);
  pinMode(PIRSensorPin,INPUT); 
  GetControl.setOnSuccess(&onSuccess);
  SetSensors.setOnSuccess(&onSuccess2);
  GetControl.setOnFailure(&onFailure);
  GetControl.setOnStart(&onStart);
  GetControl.setOnFinish(&onFinish);
  GetControl.getResponse().setOnNextResponseBytesUpdate(&onBytesUpdate);
  GetControl.getResponse().setOnError(&onError);
  GetControl.getResponse().setOnJsonResponse(&GetControlResponse);
  
}

void loop()
{
  if(Flag==true)
  {
    Flag=false;
    Internet.performGet(GetControl);
  }
  else if(Flag2==true)
  {
    Flag2=false;
    String URL="server_ip/api/SetSensors/Temperature/"+ReadDistanceSensor();
    SetSensors.setUrl(URL.c_str());
    Internet.performGet(SetSensors);
  }
  else if(Flag3==true)
  {
    Flag3=false;
    String URL="server_ip/api/SetSensors/PIR/"+ReadPIRSensor();
    SetSensors.setUrl(URL.c_str());
    Internet.performGet(SetSensors);
  }
}

void onSuccess(HttpResponse &response)
{
  char *Temp=response.getBytes();
  Terminal.println(Temp);
  CP.Lights=Temp[0];
  CP.DoorLock=Temp[1];
  CP.Windows=Temp[2];
  Control(CP.Lights-'0',CP.DoorLock-'0',CP.Windows-'0');
  Terminal.println("-----------");
  Flag2=true;
}

void onSuccess2(HttpResponse &response)
{
  static int FlagNumber=2;
  if(FlagNumber==2)
  {
    FlagNumber=3;
    Flag3=true;
  }
  else
  {
    FlagNumber=2;
    Flag=true;
  }
}

void GetControlResponse(JsonKeyChain& chain,char value [])
{ 
}

void onFailure(HttpResponse &response)
{
  Terminal.println(response.getStatusCode());
  Terminal.println(response.getBytes());
}

void onStart()
{
  //digitalWrite(13,HIGH);
}

void onFinish()
{
  //digitalWrite(13,LOW);
}

void onBytesUpdate(HttpResponse &response)
{
  
  //Terminal.println(response.getBytes());
  //if(!response.isSentFully())
  //  {       
  //    /* Ask for the next 64 bytes. */
  //    response.getNextBytes();
  //  }
}

void onError(int errorNumber)
{
  Terminal.print("Error:");
  switch(errorNumber)
  {
    case INDEX_OUT_OF_BOUNDS: Terminal.println("INDEX_OUT_OF_BOUNDS");break;
    case RESPONSE_CAN_NOT_BE_FOUND: Terminal.println("RESPONSE_CAN_NOT_BE_FOUND");break;
    case HEADER_CAN_NOT_BE_FOUND: Terminal.println("HEADER_CAN_NOT_BE_FOUND");break;
    case NO_ENOUGH_BYTES: Terminal.println("NO_ENOUGH_BYTES");break;
    case REQUEST_HAS_NO_RESPONSE: Terminal.println("REQUEST_HAS_NO_RESPONSE");break;
    case SIZE_OF_REQUEST_CAN_NOT_BE_ZERO: Terminal.println("SIZE_OF_REQUEST_CAN_NOT_BE_ZERO");break;
    case UNSUPPORTED_HTTP_ENTITY: Terminal.println("UNSUPPORTED_HTTP_ENTITY");break;
    case JSON_KEYCHAIN_IS_WRONG: Terminal.println("JSON_KEYCHAIN_IS_WRONG");break;
  }
}

void Control(int Lights, int DoorLock, int Windows)
{
  int Parameters[3]={Lights,DoorLock,Windows};
  for(int i=11;i<=13;i++)
  {
    if(Parameters[i-11])
    {
      digitalWrite(i,HIGH);
    }
    else
    {
      digitalWrite(i,LOW);
    }
  }
}

String ReadDistanceSensor()
{
  String StrS(6787 / (analogRead(DistanceSensorPin) - 3) - 4);  
  return StrS;
}

String ReadPIRSensor()
{
  if(digitalRead(PIRSensorPin)==HIGH)
  {
    return "1";
  }
  else
  {
    return "0";
  }
}
