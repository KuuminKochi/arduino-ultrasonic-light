const int LIGHT_p = 53;
const int ECHO_p = 52;
const int TRIG_p = 51;
bool currentState = false;
bool previousState = false;

const int mode[4] = {0, 1, 2, 3};
int currentMode = 0;

const int THRESHOLD = 300;
int shutTicker = 0;

void setup() {
        Serial.begin(9600);
        pinMode(LIGHT_p, OUTPUT);
        pinMode(ECHO_p, INPUT);
        pinMode(TRIG_p, OUTPUT);
}

void loop() {

        if (currentMode > 3) {
                currentMode = 0;
        }

        while (Serial.available() == 0) {

                digitalWrite(TRIG_p, LOW);
                delayMicroseconds(10);
                digitalWrite(TRIG_p, HIGH);
                delayMicroseconds(10);
                digitalWrite(TRIG_p, LOW);

                long duration = pulseIn(ECHO_p, HIGH);
                long distance = (duration * 0.034) / 2; // Calculate distance in cm

                if (distance < 60.0 && distance != 0 && currentMode == 0) {
                        nextMode();
                        currentMode += 1;
                }

                if (distance < 60.0 && distance != 0) {
                        shutTicker = 0;
                }

                if (shutTicker > THRESHOLD) {
                        while (currentMode != 0) {
                                currentMode += 1;
                                if (currentMode > 3) {
                                        currentMode = 0 ;
                                }
                        }
                        shutTicker = 0;
                }

                if (currentMode != 0) {
                        shutTicker += 1;
                }
                Serial.println(shutTicker);
        }


        int menuChoice = Serial.parseInt();

        switch (menuChoice) {
        case 1:
                nextMode();
                currentMode += 1;
                if (currentMode > 3) {
                        currentMode = 0;
                }

                shutTicker = 0;
                break;
        case 2:
                changeIntensity();

                shutTicker = 0;
                break;
        case 8:
                Serial.println("Set current mode {0, 1, 2, 3}:");
                delay(5000);
                int changeMode = Serial.parseInt();
                currentMode = changeMode;
                shutTicker = 0;

                break;
        case 9:
                Serial.println("Sleeping");
                delay(6000000);

                break;
        default:
                break;
        
        delay(1000);
        }
}


void nextMode() {
        digitalWrite(LIGHT_p, HIGH);
        delay(200);
        digitalWrite(LIGHT_p, LOW);
        delay(200);
}

void changeIntensity() {
        digitalWrite(LIGHT_p, HIGH);
        delay(1500);
        digitalWrite(LIGHT_p, LOW);
        delay(1500);
}
