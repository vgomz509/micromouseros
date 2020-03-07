import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayDeque;

public class FloodFill {
	static int[] maze=new int[256];
	static String  fichero_input="maze.txt" ;
	static int max_n_nodes=0;
	static String[] txt=new String[21600];

	public static void main(String[] args) {
		int labs=readFile(fichero_input);//lee el laberinto conocido en maze[16*y+x]
		for (int n=0;n<labs;n++) {
		paredes(n);	
		pintaLab(n);
		puntua();
		}
		
	}

static boolean paredN(int x, int y) {
	if ((maze[y*16+x]&1)>0) {return true;}
	return false;
}
static boolean paredE(int x, int y) {
	if ((maze[y*16+x]&2)>0) {return true;}
	return false;
}
static boolean paredS(int x, int y) {
	if ((maze[y*16+x]&4)>0) {return true;}
	return false;
}
static boolean paredO(int x, int y) {
	if ((maze[y*16+x]&8)>0) {return true;}
	return false;
}
static int readFile(String fichero_input) {
	int labs=0;
	try {
        BufferedReader in = new BufferedReader(new FileReader(fichero_input));
        while (in.readLine()!=null) {
        in.readLine();in.readLine();
        for (int n=0;n<33;n++) {
        	txt[labs*33+n]=in.readLine();
        	//System.out.println(txt[n]);
        }
        labs++;

        }
    in.close();
	System.out.println("Leidos "+labs+" laberintos.");
    
	}catch (Exception e) {
		e.printStackTrace();
	}
	
	return labs;
	}	

static void paredes(int lab) {
	for (int y=0;y<16;y++) {	
	for (int x=0;x<16;x++){
		byte b=0;
		if (txt[33*lab+y*2].charAt(x*4+1)==45) {b+=1;}//Si hay un - en (x,y-1) es que hay pared N 0001 (1)
		if (txt[33*lab+y*2+1].charAt(x*4+4)==124) {b+=2;}//Si hay un | en (x+1,y) es que hay pared E 0010 (2)
		if (txt[33*lab+y*2+2].charAt(x*4+2)==45) {b+=4;}//Si hay un - en (x,y+1) es que hay pared S 0100 (4)
		if (txt[33*lab+y*2+1].charAt(x*4)==124) {b+=8;}// Si hay un | en (x-1,y) es que hay pared O 1000 (8)
		maze[y*16+x]=(char)b;//guarda paredes en maze
		//System.out.format("X=%d Y=%d Byte=%d",x,y,b);
		//System.out.println();
	}
	}
}

static void puntua() {
	int[][] valor=new int[16][16];
	int cx=7;
	int cy=7;
	for (int y=0;y<16;y++)for (int x=0;x<16;x++) {valor[x][y]=255;}
	for (int y=7;y<9;y++)for (int x=7;x<9;x++) {valor[x][y]=0;}
	int v=0;
	ArrayDeque<Integer> queue =new  ArrayDeque<Integer>();
	queue.push(16*cy+cx);
	queue.push(16*cy+cx+1);
	queue.push(16*(cy+1)+cx);
	queue.push(16*(cy+1)+cx+1);

	while (!queue.isEmpty()){
		int nodo=queue.pollLast();
		cx=nodo&15;
		cy=(nodo&240)/16;
		v=(nodo&65280)/256;
		
		if (valor[cx][cy]>250) {
			v++;
			valor[cx][cy]=v;
		}

		
		if ((cy>0)&&(valor[cx][cy-1]==255)&&!paredN(cx,cy)) {
			queue.push(cx+(cy-1)*16+v*256);
			valor[cx][cy-1]--;//marca como nodo ya subido a la cola
		} 
		
		if ((cy<15)&&(valor[cx][cy+1]==255)&&!paredS(cx,cy)) {
			queue.push(cx+(cy+1)*16+v*256);
			valor[cx][cy+1]--;
		}
		
		if ((cx>0)&&(valor[cx-1][cy]==255)&&!paredO(cx,cy)) {
				queue.push((cx-1)+cy*16+v*256);
				valor[cx-1][cy]--;
		}
		
		if ((cx<15)&&(valor[cx+1][cy]==255)&&!paredE(cx,cy)) {
					queue.push(cx+1+cy*16+v*256);
					valor[cx+1][cy]--;
		} 
		
		if (queue.size()>max_n_nodes) {max_n_nodes=queue.size();}
	}
	pintaValor(valor);
}

static void pintaLab(int lab) {
	for (int n=0;n<33;n++) {
		System.out.println(txt[lab*33+n]);
	}
}
static void pintaValor(int[][] valor) {
	
	for (int y=0;y<16;y++) {
		
		for (int x=0;x<16;x++) {
			System.out.print((paredN(x,y)?"-----":"     "));
		}
		System.out.println();
		for (int x=0;x<16;x++) {
			System.out.print((paredO(x,y)?"|":" ")+iToS(valor[x][y])+ (paredE(x,y)?"|":" "));
		}
		System.out.println();
		for (int x=0;x<16;x++) {
			System.out.print((paredS(x,y)?"-----":"     "));
			}
		System.out.println();	
	}
	System.out.println();		
}

static String iToS(int n) {
	return String.format ("%3d", n);
}


}


