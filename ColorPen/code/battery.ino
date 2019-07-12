#define GNDPIN 13
void batteryInit()
{
  pinMode(GNDPIN, OUTPUT);
  digitalWrite(GNDPIN, LOW);

}

boolean batteryRead() {
  int batteryRead = analogRead(A0);
  if (batteryRead < 1500) {
    return true;
  } else {
    return false;
  }
}
