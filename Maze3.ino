uint8_t x;
uint8_t y;
uint8_t o; //default

uint8_t labFloo[16][16];
String path;
int iteration = 0;
uint8_t blind_steps = 0;
uint8_t bif[20];
uint8_t ori_bif[20];
boolean backwards = false;

void setup() {
  Serial.begin (115200);
  bif[0] = 0;
  x = 0;
  y = 0;
  o = 0;
  path = "";
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      labFloo[x][y] = 0;
    }
  }
  labFloo[0][0] = 1; //weight to first cell
}

char inkey() {
  while (Serial.available() == 0) {}
  
  char receivedChar;
  do{
    receivedChar=Serial.read();
    } while ((receivedChar>127)||(receivedChar<32));
  
  
  Serial.println();Serial.print("ascii:");Serial.println((uint8_t)receivedChar);
  return receivedChar;
}

uint8_t floorDir(int x, int y, int dir_rel) {
  uint8_t d_abs = (dir_rel + o) % 4;
  if (d_abs == 0) {
    if (y > 0)return labFloo[x][y - 1];
  }
  else if (d_abs == 2) {
    if (y < 15)return labFloo[x][y + 1];
  }
  else if (d_abs == 1) {
    if (x < 15)return labFloo[x + 1][y];
  }
  else if (d_abs == 3) {
    if (x > 0)return labFloo[x - 1][y];
  }
  return 5;
}

boolean canIGo(uint8_t rel_dir) { //0Front, 1Right, 2Backwards, 3Left
  if (rel_dir == 2) {
    return true; //You can always go backwards
  }
  String d = "";
  switch (rel_dir) {
    case 0: d = "Front"; break;
    case 1: d = "Right"; break;
    case 2: d = "Backwards"; break;
    case 3: d = "Left"; break;
  }
  Serial.print("Can I Go " + d + " (y/n)?"); Serial.print(rel_dir);
  char response = inkey();
  Serial.println();
  return (response == 'y' ? true : false);
}

void blindGo() {
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una celda");
  path += "0";
  blind_steps++;
}

double distanceToCenterDirRel(int x, int y, int dir_rel) {
  if (x > 7)x++;
  if (y > 7)y++;
  int dir2_abs = (dir_rel + o) % 4;
  if (dir2_abs == 0)y--; //N
  if (dir2_abs == 1)x++; //E
  if (dir2_abs == 2)y++; //S
  if (dir2_abs == 3)x--; //O
  return sqrt((8 - x) * (8 - x) + (8 - y) * (8 - y));
}

void go(uint8_t dir_rel) {
  
  switch (dir_rel) {
    case 0: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una...");
      break;
    case 1: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la derecha...");
      break;
    case 2: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Date la vuelta y atrás...");
      break;
    case 3: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la izquierda...");
      break;
  }

  o = (dir_rel + o) % 4;

  switch (o) {
    case 0: y--;
      break;
    case 1: x++;
      break;
    case 2: y++;
      break;
    case 3: x--;
      break;
  }


  if (!backwards) {
    path = path + (char)(48 + dir_rel);
    if (bif[0] > 0) {
      bif[bif[0]]++;
    }
  } else {
    bif[bif[0]]--;
    if (bif[bif[0]] == 0) {
      o = ori_bif[bif[0]];
      bif[0]--;
      backwards = false;
    } else {
      labFloo[x][y] += 6;
    }
    path = path.substring(0, path.length() - 1);
  }
  labFloo[x][y]++;
}


String solve() {
  while (canIGo(0)) { //go forward until first turn CUIDADO:Puede que encuentre salida a un lado sin bloqueo frontal
    blindGo();
    summary();
  }
  if (canIGo(1)) {
    o = 1;
  } else {
    o = 2;
  }
  
  while (!center(x, y)) {
    iteration++;
    if (blind_steps <= 0) {
      double score[] = {0, 0, 0, 0};
      double max_score = 0;
      uint8_t max_dir_rel = 0;
      uint8_t exits = 0;
      boolean canIGoV[4];
      char key=' ';
      do{
      for (int i = 0; i < 4; i++) {
        canIGoV[i]=canIGo(i);      
      }
        Serial.print("F:");Serial.print (canIGoV[0]);Serial.print(", R:");Serial.print (canIGoV[1]);Serial.print(", L:");Serial.print (canIGoV[3]);Serial.println(": ¿(o)k?");
        key=inkey();
      } while (key=='e');
      for (int i_rel = 0; i_rel < 4; i_rel++) {
        //Choose the better path based on 4 scores
        //SCORE A: is it possible?
        int scoreA = 0;
        if (canIGoV[i_rel]) {
          scoreA = 1000;
          exits++;
        }
        //SCORE B: Next cell weight?
        int scoreB = (10 - floorDir(x, y, i_rel)) * 5; 
        
        if ((scoreA == 1000) && (floorDir(x, y, i_rel) > 5)) {
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
          max_dir_rel = i_rel;
        }
      }
      if ((exits >= 3) && (!backwards)) {
        bif[0]++;
        bif[bif[0]] = 0;
        ori_bif[bif[0]] = o;
        Serial.print("Añado bif: Num:");Serial.print(bif[0]);Serial.print(", Ori:");Serial.println(ori_bif[bif[0]]);
      }
      if (exits == 1) {
        backwards = true;
        labFloo[x][y] += 6;
      }
      go(max_dir_rel);
    } else {
      blind_steps--;
      if (o == 1) {
        x++;
      }
      if (o == 2) {
        y++;
      }
      labFloo[x][y]++;
    }
    summary();
  }
}

void loop() {
  Serial.println("Hola...");
  
  solve();
  goHome();
  goStraight();
}

boolean center(uint8_t x, uint8_t y) {
  return ((x > 6) && (x < 9) && (y > 6) && (y < 9));
}

void goStraight() {
  x = 0;
  y = 0;
  for (int i = 0; i < path.length(); i++) {
    inkey();
    String dir_rel = path.substring(i, i + 1);
    switch (dir_rel.toInt()) {
      case 0: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Avanza una...");
        break;
      case 1: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la derecha...");
        break;
      case 2: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Date la vuelta y atrás...");
        break;
      case 3: Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Vete a la izquierda...");
        break;
    }
  }
Serial.println("He llegado!!!");
Serial.println(path);
  
}

void goHome(){
  Serial.println("Vuelvo a casa!!!");
  int l=path.length();
  path=path+"2";
  Serial.println(path);

  for(int i=0;i<l;i++){
   inkey();
    String dir_rel = path.substring(l-i, l-i+1 );
    Serial.println(dir_rel);
    switch (dir_rel.toInt()) {
      case 0: Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Avanza una...");
        break;
      case 1: Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Vete a la izquierda...");
        break;
      case 2: Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Date la vuelta y atrás...");
        break;
      case 3: Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Vete a la derecha...");
        break;
    }    }
  }

void paint(){
  for (int yy=0;yy<16;yy++){
    for (int xx=0;xx<16;xx++){
      String c="";
      switch (o){
      case 0: c="^";break;
      case 1: c=">";break;
      case 2: c="v";break;
      case 3: c="<";break;
      }
      Serial.print(labFloo[xx][yy]);Serial.print((xx==x)&&(yy==y)?c:" ");
    }
    Serial.println();
  }
}

void summary() {
  Serial.println("---------------");
  Serial.print("Path:"); Serial.println(path);
  Serial.print("Iteration:"); Serial.println(iteration);
  Serial.print("Blind Steps:"); Serial.println(blind_steps);
  Serial.print("x:"); Serial.println(x);
  Serial.print("y:"); Serial.println(y);
  Serial.print("o:"); Serial.println(o);
  Serial.print("Backwards Mode:"); Serial.println(backwards);
  Serial.print("Bifurcations:"); Serial.println(bif[0]);
  Serial.print("Latest Bif Dist:"); Serial.println(bif[bif[0]]);
  for (int z=0;z<bif[0];z++){
  Serial.print("Latest Bif Orie[");Serial.print(z);Serial.print("]:");Serial.println(ori_bif[z]);
  }
  
  paint();
  Serial.println("---------------");
}
