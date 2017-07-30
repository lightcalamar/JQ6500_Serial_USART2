/** Full demo of MP3 Controls, after uploading, open your Serial Monitor and enter commands.
 * 
 * Allows you to test all the various controls of the JQ6500 module.
 *
 * @author James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
#include <JQ6500_Serial_USART2.h>

// Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 9 is connected to one end of a  1k resistor, 
//     the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor
JQ6500_Serial_USART2 mp3(SerialUART2);

void setup() {

  SerialUART1.begin(115200);
  SerialUART1.print("Tomas");
  SerialUART2.begin(9600);//inicializamos UART2 antes de usar las funciones de JQ6500
  mp3.reset();
  mp3.setVolume(10);
  statusAndHelpOutput();
}

void loop() {
  
    byte b;
    if(SerialUART1.available())
    {
      b = SerialUART1.read();

      switch(b)
      {
        case 'p': SerialUART1.println("Play");   mp3.play();     return;
        case 'r': SerialUART1.println("Restart"); mp3.restart(); return;
        case ' ': SerialUART1.println("Pause"); mp3.pause();     return;
        case '>': SerialUART1.println("Next");  mp3.next();      return;
        case '<': SerialUART1.println("Prev");  mp3.prev();      return;
        
        case ']': SerialUART1.println("Next Folder");  mp3.nextFolder(); return;
        case '[': SerialUART1.println("Prev Folder");  mp3.prevFolder(); return;
    
        case '+': SerialUART1.println("Vol +"); mp3.volumeUp(); return;
        case '-': SerialUART1.println("Vol -"); mp3.volumeDn(); return;
        case 'm': SerialUART1.println("Vol 0"); mp3.setVolume(0); return;
        
        case 'v': 
        {
          char volBuff[10]; 
          memset(volBuff, 0, sizeof(volBuff));          
          SerialUART1.readBytesUntil('\n',volBuff, sizeof(volBuff)-1);
          mp3.setVolume(max(0,min(30, atoi(volBuff))));
          
          SerialUART1.print("Vol ");
          SerialUART1.println(max(0,min(30, atoi(volBuff))));                    
        }
        return;

        case 'e':
        {
          do
          {
            while(!SerialUART1.available()); // Wait
            b = SerialUART1.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          SerialUART1.print("Equalizer ");
          switch(b)
          {
            case 'N': SerialUART1.println("Normal");  mp3.setEqualizer(MP3_EQ_NORMAL);  break;
            case 'P': SerialUART1.println("Pop");     mp3.setEqualizer(MP3_EQ_POP);     break;
            case 'R': SerialUART1.println("Rock");    mp3.setEqualizer(MP3_EQ_ROCK);    break;
            case 'J': SerialUART1.println("Jazz");    mp3.setEqualizer(MP3_EQ_JAZZ);    break;
            case 'C': SerialUART1.println("Classic"); mp3.setEqualizer(MP3_EQ_CLASSIC); break;
            case 'B': SerialUART1.println("Bass");    mp3.setEqualizer(MP3_EQ_BASS);    break;
          }
        }
        return;

        case 'l':
        {
          do
          {
            while(!SerialUART1.available()); // Wait
            b = SerialUART1.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          SerialUART1.print("Loop ");
          switch(b)
          {
            case 'A': SerialUART1.println("All"); mp3.setLoopMode(MP3_LOOP_ALL);      break;  // Plays the tracks one after another and repeats 
            case 'F': SerialUART1.println("Folder"); mp3.setLoopMode(MP3_LOOP_FOLDER);       break; // Loop within folder
            case 'O': SerialUART1.println("One (repeat playing same file)"); mp3.setLoopMode(MP3_LOOP_ONE);       break; // | These seem to do the same, repeat the same track over and over
            case 'R': SerialUART1.println("??? - Don't know what it means exactly, in the datasheet it is \"RAM\""); mp3.setLoopMode(MP3_LOOP_RAM);       break; //- 
            case 'N': 
            case 'S': SerialUART1.println("None (play file and stop)"); mp3.setLoopMode(MP3_LOOP_ONE_STOP); break;  // Default, plays track and stops
          }
        }
        return;

        case 's':
        {
          do
          {
            while(!SerialUART1.available()); // Wait
            b = SerialUART1.read();
            if(b != ' ') break; // Allow "e N" or "eN" etc...
          } while(1);
          
          SerialUART1.print("Source ");
          switch(b)
          {
            case 'S': SerialUART1.println("SD Card (if available)."); mp3.setSource(MP3_SRC_SDCARD);      break;
            case 'B': SerialUART1.println("on board memory.");mp3.setSource(MP3_SRC_BUILTIN);   break;            
          }
        }
        return;

        case 'f':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          SerialUART1.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);

          SerialUART1.println();
          SerialUART1.print("Play file #");
          SerialUART1.print(fnum);
          SerialUART1.println(F(" (if it exists)."));
          mp3.playFileByIndexNumber(fnum); // 48 == ord('0')   
          return;
          
        }
        return;
        
        case 'F':
        {
          char fnumBuff[10]; 
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          SerialUART1.readBytesUntil('/',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int folnum = strtoul(fnumBuff, NULL, 10);
          
          memset(fnumBuff, 0, sizeof(fnumBuff));          
          SerialUART1.readBytesUntil('\n',fnumBuff, sizeof(fnumBuff)-1);
          unsigned int fnum = strtoul(fnumBuff, NULL, 10);
          
          fnum   = max(1,min(fnum, 999));
          folnum = max(1,min(folnum, 99));
          
          SerialUART1.print("Play "); 
            if(folnum < 10) SerialUART1.print('0');
          SerialUART1.print(folnum);
          SerialUART1.print('/');
            if(fnum < 10) SerialUART1.print("00");
            else if(fnum < 10) SerialUART1.print('0');
          SerialUART1.print(fnum);
          SerialUART1.println(".mp3 (if it exists).");
          mp3.playFileNumberInFolderNumber(folnum, fnum); // 48 == ord('0')   
        }
        return;
                  
        case '?': statusAndHelpOutput(); return;
        
        case 'S': SerialUART1.println("Sleep"); mp3.sleep(); return;
        case 'z': SerialUART1.println("Reset"); mp3.reset(); return;
        
      }
      
    }
    
    static unsigned long m = millis();
    
    if(millis() > 1000 && m < (millis() - 1000))
    {
      if((mp3.getStatus() == MP3_STATUS_PLAYING))
      {
        SerialUART1.print(F("Playing, Current Position: "));
        SerialUART1.print(mp3.currentFilePositionInSeconds());
        SerialUART1.print(F("s / "));
        SerialUART1.print(mp3.currentFileLengthInSeconds());       
        SerialUART1.println('s');
      }
      m = millis();
    }  
}

void statusAndHelpOutput()
{
   SerialUART1.println();
   SerialUART1.println(F("JQ6500 MP3 Player Demo"));
   SerialUART1.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   
   SerialUART1.print(F("Status           : "));
   switch(mp3.getStatus())
   {
     case MP3_STATUS_STOPPED: SerialUART1.println(F("Stopped")); break;
     case MP3_STATUS_PLAYING: SerialUART1.println(F("Playing")); break;
     case MP3_STATUS_PAUSED:  SerialUART1.println(F("Paused"));  break;
   }
   
   SerialUART1.print(F("Volume (0-30)    : "));
   SerialUART1.println(mp3.getVolume());
  
   SerialUART1.print(F("Equalizer        : "));
   switch(mp3.getEqualizer())
   {
     case MP3_EQ_NORMAL:     SerialUART1.println(F("Normal"));  break;
     case MP3_EQ_POP:        SerialUART1.println(F("Pop"));     break;
     case MP3_EQ_ROCK:       SerialUART1.println(F("Rock"));    break;
     case MP3_EQ_JAZZ:       SerialUART1.println(F("Jazz"));    break;
     case MP3_EQ_CLASSIC:    SerialUART1.println(F("Classic")); break;
     case MP3_EQ_BASS:       SerialUART1.println(F("Bass"));    break;     
   }
   
   SerialUART1.print(F("Loop Mode        : "));
   switch(mp3.getLoopMode())
   {
     case MP3_LOOP_ALL:      SerialUART1.println(F("Play all tracks, then repeat."));  break;
     case MP3_LOOP_FOLDER:   SerialUART1.println(F("Play all tracks in folder, then repeat."));     break;
     case MP3_LOOP_ONE:      SerialUART1.println(F("Play one track then repeat (loop track)."));    break;
     case MP3_LOOP_RAM:      SerialUART1.println(F("Unknown function exactly, seems to play one track then repeat?"));    break;
     case MP3_LOOP_ONE_STOP: SerialUART1.println(F("Play one track then stop."));    break;          
   }
   SerialUART1.println();
    
   SerialUART1.print(F("# of On Board Memory Files    : "));
   SerialUART1.println(mp3.countFiles(MP3_SRC_BUILTIN));
       
   SerialUART1.print(F("\"Current\" On Board Memory File Index: "));
   SerialUART1.println(mp3.currentFileIndexNumber(MP3_SRC_BUILTIN));
   SerialUART1.println();
   
   SerialUART1.print(F("# of SD Card Files    : "));
   SerialUART1.println(mp3.countFiles(MP3_SRC_SDCARD));
   
   SerialUART1.print(F("# of SD Card Folders  : "));
   SerialUART1.println(mp3.countFolders(MP3_SRC_SDCARD));
   
   SerialUART1.print(F("\"Current\" SD Card File Index: "));
   SerialUART1.println(mp3.currentFileIndexNumber(MP3_SRC_SDCARD));
   
   SerialUART1.print(F("\"Current\" SD Card File Name : "));   
   char buff[120];
   mp3.currentFileName(buff, sizeof(buff));
   SerialUART1.println(buff);
   
   SerialUART1.println();
   SerialUART1.println(F("Controls (type in serial monitor and hit send): "));
   SerialUART1.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
   SerialUART1.println(F("? Display this menu.\n"));
   
   SerialUART1.println(F("p Play\t\t> Next\t\t< Prev\n[space] Pause\tr Restart from start of file\n] Next folder\t[ Prev folder\n"));
   
   SerialUART1.println(F("f[1-65534]      Play file by (FAT table) index number\nF[01-99]/[001-999].mp3 Play [001-999].mp3 in folder [01-99]\n"));
   
   SerialUART1.println(F("+ Vol up\t- Vol down\tm Mute\nv[0-30] Set volume\n\ne[N/P/R/J/C/B] Equalizer (N)ormal, (P)op, (R)ock, (J)azz, (C)lassic, (B)ass\nl[A/F/O/R/N]   Loop (A)ll, (F)older, (O)ne, (R)???, (N)o Loop\ns[S/B]         Switch to (S)D Card/(B)uilt In Memory\n\n"));
}


