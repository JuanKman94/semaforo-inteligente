#define RED_1 7
#define YELLOW_1 8
#define GREEN_1 9
#define RED_2 10
#define YELLOW_2 11
#define GREEN_2 12

#define RED 0
#define YELLOW 1
#define GREEN 2

#define YELLOW_TIMEOUT 2000
#define INACTIVITY_TIMEOUT 1800
#define TRAFFIC_PASSING_MIN 100 // in centimeters

// Ultra-sound sensors:
// trigger sends signal for sensor to start measuring
// echo pin retrieves data measured
#define USENSOR_TRIG_1 3
#define USENSOR_ECHO_1 4
#define USENSOR_TRIG_2 5
#define USENSOR_ECH0_2 6

#define TRIGGER 0
#define ECHO 1

#define LIGHT_SENSOR 0

#define NIGHT 0
#define DAY 1

// Time intervals for night and day, respectively
const int interval[2] = { 20000, 30000 }; // in milliseconds

// We map the semaphore LEDs
const int semaphore[2][3] = { { RED_1, YELLOW_1, GREEN_1 }, { RED_2, YELLOW_2, GREEN_2 } };

// And the ultra-sound sensors
const int usensors[2][2] = { { USENSOR_TRIG_1, USENSOR_ECHO_1 }, { USENSOR_TRIG_2, USENSOR_ECH0_2 } };

// Also the distances - but this WILL change!
int distances[2] = { 0, 0 };

unsigned short turn = 0,
               prev = 0,
               day_time = 0;

unsigned short day_or_night()
{  
  if (analogRead(LIGHT_SENSOR) > 511) // We split by half
    return DAY;
  else
    return NIGHT;
}

/**
 * Get distance (in cm) of the ultrasound
 * sensor given by the id
 *
 * @param int id
 */
int distance_sensor(int id)
{
  long time = 0;

  digitalWrite(usensors[id][TRIGGER], LOW);
  delayMicroseconds(5); // To stabilize
  
  digitalWrite(usensors[id][TRIGGER], HIGH);
  delayMicroseconds(10); // Stabilize some more
  
  time = pulseIn(usensors[id][ECHO],  HIGH);
    
  return int(0.017 * time);
}

/**
 * Deactivate the semaphore given by the id
 *
 * @param int id
 */
void deactivate(int id)
{
  // Turn on yellow LED
  digitalWrite(semaphore[id][GREEN], LOW);
  digitalWrite(semaphore[id][YELLOW], HIGH);
  delay(YELLOW_TIMEOUT);

  // Turn on red LED
  digitalWrite(semaphore[id][YELLOW], LOW);
  digitalWrite(semaphore[id][RED], HIGH);
}

/**
 * Activate the semaphore given by the id
 *
 * @param int id
 */
void activate(int id)
{
  digitalWrite(semaphore[id][RED], LOW);
  digitalWrite(semaphore[id][GREEN], HIGH);
}

void setup() {
  Serial.begin(9600);
  int i = 0;
  
  // Initialize lights pins
  for (i = RED; i <= GREEN; i++) {
      pinMode(semaphore[0][i], OUTPUT);
      pinMode(semaphore[1][i], OUTPUT);
  }
  
  // And ultra-sound pins
  for (i = 0; i <= 1; i++) {
    pinMode(usensors[i][TRIGGER], OUTPUT);
    pinMode(usensors[i][ECHO], INPUT);
  }

  digitalWrite(semaphore[0][RED], HIGH);
  digitalWrite(semaphore[1][RED], HIGH);
}

unsigned long time_start = millis(),
              time_unused = 0,        // Time passed without vehicles crossing
              now = 0;

void loop() {
  now = millis(); // current time
  day_time = day_or_night();
  
  // Get distances of each sensor
  distances[turn] = distance_sensor(turn);
  distances[!turn] = distance_sensor(!turn);
  
  // If current light doesn't have traffic and waiting light does
  if ( distances[turn] > TRAFFIC_PASSING_MIN && distances[!turn] < TRAFFIC_PASSING_MIN) {
    time_unused = now - time_start;
  } else {
    // A car passed, ignore unused time
    time_unused = 0;
  }
  
  //String stat = String(turn) + "] distance => " + String(distances[turn]) + "cm";
  //Serial.println(stat);
  
  // TODO: Don't Repeat Yourself
  if ( time_unused >= INACTIVITY_TIMEOUT ) {
    // People on the other line have been waiting enough for nothing
    time_start = now;
    time_unused = 0;
    
    deactivate(!turn);
    activate(turn);
    
    turn = !turn;
  } else if ( now - time_start >= interval[day_time] ) {
    // Default timeout
    time_start = now;
    time_unused = 0;
    
    deactivate(!turn);
    activate(turn);
    
    turn = !turn;
  }
}
