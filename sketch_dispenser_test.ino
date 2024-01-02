#include <WiFi.h>
#include <time.h>
#include <SolarCalculator.h>

// Declareer globale variabelen
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
double latitude = 51.7515, longitude = 5.8836; //Mook
int utc_offset = 1, utc_offset_minutes = 60;
int sunrise_minutes, sunset_minutes;
int test_minutes = 996;
double transit, sunrise, sunset; // nodig voor sunrise en sunset calc

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT); //Test pin
  pinMode(32, OUTPUT); //De dispenser motor is verbonden op pin32.
  initWiFi(); //Verbind met de WIFI
}

void initWiFi() {
  const char* ssid = "RinWireless";
  const char* passphrase = "DonderOpMa89n!";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void connectToNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void getTime(int* time_minutes, int* day, int* month, int* year) { //pointers to make multiple return values possible
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){ //als NTP het doet is de uitkomst van getLocalTime(&timeinfo) 1 (true)
    Serial.println("Failed to obtain time");
  }
  *time_minutes = ((60*timeinfo.tm_hour) + timeinfo.tm_min);
  *day = timeinfo.tm_mday;
  *month = (timeinfo.tm_mon + 1);
  *year = (timeinfo.tm_year + 1900);
}

void calcSunriseSunsetMinutes(double sunrise, double sunset, int* sunrise_minutes, int* sunset_minutes) {
  *sunrise_minutes =  int(round(sunrise * 60) + utc_offset_minutes);
  *sunset_minutes = int(round(sunset * 60) + utc_offset_minutes);
}

void dispenser(int sunrise_minutes, int sunset_minutes, int time_minutes) {
  if ((sunrise_minutes == time_minutes) || sunset_minutes == time_minutes ) {
    digitalWrite(32, HIGH); // Open de dispenser
    Serial.println("opening dispenser");
    Serial.println("dispensing");
    delay(410); // Pas aan zodat de dispenser een halve cirkel heeft gedraaid.
    digitalWrite(32, LOW);
    delay(2000); //aanpassen naar hoeveelheid voer
    digitalWrite(32, HIGH); // Sluit de dispenser
    Serial.println("closing dispenser");
    delay(410); // Pas aan zodat de dispenser een halve cirkel heeft gedraaid.
    digitalWrite(32, LOW);
    delay(2000);
    delay(300000); // Ga 5 minuten uit, zodat de dispenser nooit twee keer in dezelfde minuut aan kan gaan.
  }
  Serial.println("niks gedaan");
}

// Rounded HH:mm format
char * hoursToString(double h, char *str)
{
  int m = int(round(h * 60));
  int hr = (m / 60) % 24;
  int mn = m % 60;

  str[0] = (hr / 10) % 10 + '0';
  str[1] = (hr % 10) + '0';
  str[2] = ':';
  str[3] = (mn / 10) % 10 + '0';
  str[4] = (mn % 10) + '0';
  str[5] = '\0';
  return str;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);

  //haal de huidige tijd op. Internet connectie vereist.
  connectToNTP();

  // initialiseer de variabelen die door de functie gevuld gaan worden.
  int time_minutes, day, month, year; 

  //refer to the addresses to get multiple returns from the function.
  getTime(&time_minutes, &day, &month, &year);

  //haal de waarden voor o.a sunset en sunrise op.
  calcSunriseSunset(year, month, day, latitude, longitude, transit, sunrise, sunset); 
  char str[6];
  Serial.println(hoursToString(sunrise + utc_offset, str));

  //bereken de minuutwaarden om te kunnen vergelijken.
  calcSunriseSunsetMinutes(sunrise, sunset, &sunrise_minutes, &sunset_minutes); 
  Serial.println(sunrise_minutes);
  Serial.println(sunset_minutes);
  Serial.println(time_minutes);
  Serial.println(test_minutes);

  //check of dispenser open moet
  //dispenser(sunrise_minutes, sunset_minutes, time_minutes); 
  //alle tijdswaarden in minuten om te kunnen vergelijken (sunrise, sunset en actuele tijd).
  dispenser(sunrise_minutes, sunset_minutes, test_minutes);
  delay(5000);
  test_minutes++;
}

