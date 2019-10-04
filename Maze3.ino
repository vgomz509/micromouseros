const uint8_t labirynthSizeX = 16;
const uint8_t labirynthSizeY = 16;

uint8_t x; //mouse x
uint8_t y; //mouse y 
uint8_t o; //mouse orientation
uint8_t orientation_correction=0; //needed for backwards correction 
uint8_t FloorWeight[16][16]; //weight of cell
String path; //path to center
int iteration = 0; //number of iterations
uint8_t blind_steps = 0;
uint8_t bifurcation[20];
uint8_t bifurcationOrientation[20];
boolean goingBackwards = false; //when a blockage is found activate backwards mode

void setup() {
  Serial.begin(115200);
  bifurcation[0] = 0; //number of pending bifurcations 
  x = 0;
  y = 0;
  o = 0;
  path = "";
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      FloorWeight[x][y] = 0;
    }
  }
  FloorWeight[0][0] = 1; //weight to first cell
}

void loop() {
  solve();
  goHome();
  goStraight();
}

void goStraight() {//Go to straight to center. Path should be setted.
  x = 0;
  y = 0;
  for (int i = 0; i < path.length(); i++) {
    inkey();
    String relativeDirection = path.substring(i, i + 1);
    switch (relativeDirection.toInt()) {
    case 0:
      Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una...");
      break;
    case 1:
      Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la derecha...");
      break;
    case 2:
      Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Date la vuelta y atrás...");
      break;
    case 3:
      Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la izquierda...");
      break;
    }
  }
  Serial.println("He llegado!!!");
  Serial.println(path);

}

void goHome() { //go Home from center. Path should be setted.
  Serial.println("Vuelvo a casa!!!");
  int l = path.length();
  path = path + "2";//go bacwards (rotate 180º and forward 1)  
  Serial.println(path);

  for (int i = 0; i < l; i++) {
    inkey();
    String relativeDirection = path.substring(l - i, l - i + 1);
    Serial.println(relativeDirection);
    switch (relativeDirection.toInt()) {
    case 0:
      Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Avanza una...");
      break;
    case 1:
      Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Vete a la izquierda...");
      break;
    case 2:
      Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Date la vuelta y atrás...");
      break;
    case 3:
      Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Vete a la derecha...");
      break;
    }
  }
}

uint8_t NextCellFloorWeight(int x, int y, int relativeDirection) { //weight of next cell based on current X,Y and direction
  uint8_t absoluteDirection = (relativeDirection + o) % 4;
  if (absoluteDirection == 0) {
    if (y > 0) return FloorWeight[x][y - 1];
  } else if (absoluteDirection == 2) {
    if (y < 15) return FloorWeight[x][y + 1];
  } else if (absoluteDirection == 1) {
    if (x < 15) return FloorWeight[x + 1][y];
  } else if (absoluteDirection == 3) {
    if (x > 0) return FloorWeight[x - 1][y];
  }
  return 5;
}

void blindGo() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una celda");
  path += "0";
  blind_steps++;
}

void go(uint8_t relativeDirection) {

  switch (relativeDirection) {
  case 0:
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una...");
    break;
  case 1:
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la derecha...");
    break;
  case 2:
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Date la vuelta y atrás...");
    break;
  case 3:
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la izquierda...");
    break;
  }

  o = (relativeDirection + o) % 4;

  switch (o) {
  case 0:
    y--;
    break;
  case 1:
    x++;
    break;
  case 2:
    y++;
    break;
  case 3:
    x--;
    break;
  }

  if (!goingBackwards) {
    path = path + (char)(48 + (relativeDirection + orientation_correction)%4);
    orientation_correction=0;
    if (bifurcation[0] > 0) {
      bifurcation[bifurcation[0]]++;
    }
  } else {
    bifurcation[bifurcation[0]]--;
    if (bifurcation[bifurcation[0]] == 0) {
      //o = bifurcationOrientation[bifurcation[0]];
        orientation_correction=((o+(4-bifurcationOrientation[bifurcation[0]]))%4);
      bifurcation[0]--;
      goingBackwards = false;
    } else {
      FloorWeight[x][y] += 6;
    }
    path = path.substring(0, path.length() - 1);
  }
  FloorWeight[x][y]++;
}




String solve() {
  while (canIGo(0)) { //go forward until first turn 
    blindGo();
    summary();
  }
  if (canIGo(1)) {
    o = 1; //labirynth is clockwise oriented
  } else {
    o = 2; //labirynth is counterclockwise oriented
  }

  while (!center(x, y)) {
    iteration++;
    if (blind_steps <= 0) {
      double score[] = {0,0,0,0};//FR,CW,BCK,CCW
      double max_score = 0;
      uint8_t max_relativeDirection = 0;
      uint8_t exits = 0;

      //Sensor simulation---------------------------------
      boolean canIGoV[4];
      char key = ' ';
      do {
        for (int i = 0; i < 4; i++) {
          canIGoV[i] = canIGo(i);
        }
        Serial.print("F:");
        Serial.print(canIGoV[0]);
        Serial.print(", R:");
        Serial.print(canIGoV[1]);
        Serial.print(", L:");
        Serial.print(canIGoV[3]);
        Serial.println(": ¿(o)k?");
        key = inkey();
      } while (key == 'e');
      //---------------------------------------------------

      for (uint8_t i_rel = 0; i_rel < 4; i_rel++) {
        //Choose the better path based on 4 scores
        //SCORE A: is it possible?
        int scoreA = 0;
        if (canIGoV[i_rel]) {
          scoreA = 1000;
          exits++;
        }
        //SCORE B: Next cell weight?
        int scoreB = (10 - NextCellFloorWeight(x, y, i_rel)) * 5;

        if ((scoreA == 1000) && (NextCellFloorWeight(x, y, i_rel) > 5)) {
          scoreB = -2000;
          exits--;
        }
        //SCORE C: Distance to center?
        double scoreC = (20 - distanceToCenterDirRel(x, y, i_rel)) * 2; //distance to center
        //SCORE D: In case of tie Backwards is the lastest option
        int scoreD = (i_rel == 2 ? -10 : 0);

        //SUM all scores
        double this_score = scoreA + scoreB + scoreC + scoreD;
        score[i_rel] = this_score;
        if (score[i_rel] > max_score) {
          max_score = score[i_rel];
          max_relativeDirection = i_rel;
        }
      }
      if ((exits >= 3) && (!goingBackwards)) { //Add new bifurcation
        bifurcation[0]++; //cont
        bifurcation[bifurcation[0]] = 0;
        bifurcationOrientation[bifurcation[0]] = o;
        Serial.print("Añado bif: Num:");
        Serial.print(bifurcation[0]);
        Serial.print(", Ori:");
        Serial.println(bifurcationOrientation[bifurcation[0]]);
      }
      if (exits == 1) { //Activate Backwards mode
        goingBackwards = true;
        FloorWeight[x][y] += 6;
      }
      go(max_relativeDirection);

    } else {
      blind_steps--;
      if (o == 1) {
        x++;
      }
      if (o == 2) {
        y++;
      }
      FloorWeight[x][y]++;
    }
    summary();
  }
}

//Utils-----------------------------------------------------------------------------------------------
double distanceToCenterDirRel(int x, int y, int relativeDirection) {
  uint8_t halfX = labirynthSizeX / 2;
  uint8_t halfY = labirynthSizeY / 2;

  if (x > (halfX - 1)) x++;
  if (y > (halfY - 1)) y++;
  int dir2_abs = (relativeDirection + o) % 4;
  if (dir2_abs == 0) y--; //N
  if (dir2_abs == 1) x++; //E
  if (dir2_abs == 2) y++; //S
  if (dir2_abs == 3) x--; //O
  return sqrt((halfX - x) * (halfX - x) + (halfY - y) * (halfY - y));
}

boolean center(uint8_t x, uint8_t y) {
  uint8_t halfX = labirynthSizeX / 2;
  uint8_t halfY = labirynthSizeY / 2;
  return ((x > halfX - 2) && (x < halfX + 1) && (y > halfY - 2) && (y < halfY + 1));
}

void summary() {
  Serial.println("---------------");
  Serial.print("Path:"); Serial.println(path);
  Serial.print("Iteration:"); Serial.println(iteration);
  Serial.print("Blind Steps:"); Serial.println(blind_steps);
  Serial.print("x:"); Serial.println(x);
  Serial.print("y:"); Serial.println(y);
  Serial.print("o:"); Serial.println(o);
  Serial.print("Backwards Mode:");Serial.println(goingBackwards);
  Serial.print("Bifurcations:");Serial.println(bifurcation[0]);
  paintLabirynth();
  Serial.println("---------------");
}

void paintLabirynth() {
  for (int yy = 0; yy < 16; yy++) {
    for (int xx = 0; xx < 16; xx++) {
      String c = "";
      switch (o) {
      case 0:
        c = "^";
        break;
      case 1:
        c = ">";
        break;
      case 2:
        c = "v";
        break;
      case 3:
        c = "<";
        break;
      }
      Serial.print((FloorWeight[xx][yy]>9)?9:FloorWeight[xx][yy]);
      Serial.print((xx == x) && (yy == y) ? c : " ");
    }
    Serial.println();
  }
}

//sensors simulation-----------------------------------------------------------------------------------------------

boolean canIGo(uint8_t relativeDirection) { //0Front, 1Right, 2Backwards, 3Left
  if (relativeDirection == 2) {
    return true; //You can always go backwards
  }
  String d = "";
  switch (relativeDirection) {
  case 0:
    d = "Front";
    break;
  case 1:
    d = "Right";
    break;
  case 2:
    d = "Backwards";
    break;
  case 3:
    d = "Left";
    break;
  }
  Serial.print("Can I Go " + d + " (y/n)?");
  Serial.print(relativeDirection);
  char response = inkey();
  Serial.println();
  return (response == 'y' ? true : false);
}

char inkey() {
  while (Serial.available() == 0) {}

  char receivedChar;
  do {
    receivedChar = Serial.read();
  } while ((receivedChar > 127) || (receivedChar < 32));

  Serial.println();
  Serial.print("ascii:");
  Serial.println((uint8_t) receivedChar);
  return receivedChar;
}
