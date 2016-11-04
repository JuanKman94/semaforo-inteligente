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

#define SENSOR 0
#define NIGHT 0
#define DAY 1

// Time intervals for night and day, respectively
const int interval[2] = { 10000, 20000 }; // in milliseconds

// We map the semaphore LEDs
const int semaphore[2][3] = { { RED_1, YELLOW_1, GREEN_1 }, { RED_2, YELLOW_2, GREEN_2 } };

unsigned short turn = 0,
               prev = 0,
               day_time = 0;

unsigned short day_or_night()
{  
  if (analogRead(SENSOR) > 511) // We split by half
    return DAY;
  else
    return NIGHT;
}

/**
 * Deactivate the semaphore given by the id
 *
 * @var id int
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
 * @var id int
 */
void activate(int id)
{
  digitalWrite(semaphore[id][RED], LOW);
  digitalWrite(semaphore[id][GREEN], HIGH);
}

void setup() {
  Serial.begin(9600);
  for (int i = RED; i <= GREEN; i++) {
      pinMode(semaphore[0][i], OUTPUT);
      pinMode(semaphore[1][i], OUTPUT);
  }

  digitalWrite(semaphore[0][RED], HIGH);
  digitalWrite(semaphore[1][RED], HIGH);
}

unsigned long time_start = millis(),
              now = 0;

void loop() {
  now = millis(); // current time
  day_time = day_or_night();
  
  if ( now - time_start >= interval[day_time] ) {
    time_start = now;
    
    deactivate(!turn);
    activate(turn);
    
    turn = !turn;
  }
}
