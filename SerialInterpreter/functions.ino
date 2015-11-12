



void configStepper () {
  Serial.println("Input the step:");
  wait();
  int spd = Serial.parseInt();
  if (spd <= 10) {
    Serial.println("Bad speed.");
    return;
  }
  Serial.println("Input four input pins:");
  wait();
  int pin1 = Serial.parseInt();
  wait();
  int pin2 = Serial.parseInt();
  wait();
  int pin3 = Serial.parseInt();
  wait();
  int pin4 = Serial.parseInt();
  stp = new Stepper(spd, pin1, pin2, pin3, pin4);
}

void rotateStepper () {
  if (NULL == stp) {
    Serial.println("Not configed yet.");
    return;
  }
  Serial.println("Input the speed:");
  wait();
  int spd = Serial.parseInt();
  if (spd <= 0) {
    Serial.println("Bad value.");
    return;
  }
  stp->setSpeed(spd);
  Serial.println("Input the steps:");
  wait();
  spd = Serial.parseInt();
  

  stp->step(spd);
}

void rollWheel() {
  Serial.println("Speed?");
  wait();
  int spd = Serial.parseInt();
  pinMode(3, OUTPUT);
  analogWrite(3, spd);
  wait();
  analogWrite(3, 0);
}

/**
 * Car drive control
 *
 */
#define STEP 10
static int wheels[] = {
  5  /*left forward*/,
  3  /*left backward*/,
  6  /*right forward*/,
  9  /*right backward*/};
static int speeds[] = { /*Value range of analog write:0~255*/
  0, /*Left speed*/
  0  /*Right speed*/
};
void applySpeeds() {
  Serial.println("Speeds:");
  Serial.println(speeds[0]);
  Serial.println(speeds[1]);
  if (speeds[0] >= 0) {
    analogWrite(wheels[0], speeds[0]);
    analogWrite(wheels[1], 0);
  } else {
    analogWrite(wheels[0], 0);
    analogWrite(wheels[1], 0 - speeds[0]);
  }
  if (speeds[1] >= 0) {
    analogWrite(wheels[2], speeds[1]);
    analogWrite(wheels[3], 0);
  } else {
    analogWrite(wheels[2], 0);
    analogWrite(wheels[3], 0 - speeds[1]);
  }
}
void forward() {
  //Serial.println("In forward");
  //Serial.println(speeds[0]);
  //Serial.println(speeds[1]);
  speeds[0] += STEP;
  speeds[1] += STEP;
  if (speeds[0] > 255) {
    speeds[0] = 255;
  }
  if (speeds[1] > 255) {
    speeds[1] = 255;
  }
  applySpeeds();
}

void fullSpeed() {
  speeds[0] = 255;
  speeds[1] = 255;
  applySpeeds();
}

void backward() {
  speeds[0] -= STEP;
  speeds[1] -= STEP;
  if (speeds[0] < -255) {
    speeds[0] = -255;
  }
  if (speeds[1] < -255) {
    speeds[1] = -255;
  }
  applySpeeds();
}

void turnLeft() {
  speeds[0] -= STEP;
  speeds[1] += STEP;
  if (speeds[0] < -255) {
    speeds[0] = -255;
  }
  if (speeds[1] > 255) {
    speeds[1] = 255;
  }
  applySpeeds();
}

void turnRight() {
  speeds[0] += STEP;
  speeds[1] -= STEP;
  if (speeds[0] > 255) {
    speeds[0] = 255;
  }
  if (speeds[1] < -255) {
    speeds[1] = -255;
  }
  applySpeeds();
}

