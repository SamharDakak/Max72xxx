/*
 * Tetris.c
 *
 * Created: 18.04.2021 18:05:21
 *  Author: rmkod
 */ 

#include <Tetris.h>

int SetbrightnessValue = 15;
bool sw1_neu = 1;               // Bitspeicher fuer Taste 1
bool sw2_neu = 1;               // Bitspeicher fuer Taste 2
bool sw3_neu = 1;               // Bitspeicher fuer Taste 3
bool sw4_neu = 1;               // Bitspeicher fuer Taste 4

bool sw1_alt = 1;               // alter Wert von Taste 1
bool sw2_alt = 1;               // alter Wert von Taste 2
bool sw3_alt = 1;               // alter Wert von Taste 3
bool sw4_alt = 1;               // alter Wert von Taste 4

bool sw1_slope = 0;             // Flankenspeicher fuer Taste 1
bool sw2_slope = 0;             // Flankenspeicher fuer Taste 2
bool sw3_slope = 0;             // Flankenspeicher fuer Taste 3
bool sw4_slope = 0;             // Flankenspeicher fuer Taste 4

long delays = 0;
short delay_ = 500;
long bdelay = 0;
short buttondelay = 150;
short btdowndelay = 30;
short btsidedelay = 80;
unsigned char blocktype;
unsigned char blockrotation;
unsigned long startTime;
unsigned long elapsedTime;
bool block[8][18]; //2 extra for rotation
bool pile[8][16];
bool disp[8][16];

bool gameoverFlag = false;
int cnt = 0;





bool moveleft()
{
		
	if (space_left())
	{
		int i;
		int j;
		for (i=0;i<7;i++)
		{
			for (j=0;j<16;j++)
			{
				block[i][j]=block[i+1][j];
			}
		}
		
		for (j=0;j<16;j++)
		{
			block[7][j]=0;
		}

		updateLED();
		return 1;
	}

	return 0;
}
//***********************************************************

bool moveright()
{
	if (space_right())
	{
		int i;
		int j;
		for (i=7;i>0;i--)
		{
			for (j=0;j<16;j++)
			{
				block[i][j]=block[i-1][j];
			}
		}

		for (j=0;j<16;j++)
		{
			block[0][j]=0;
		}
		
		updateLED();
		return 1;
		
	}
	return 0;
}
//***********************************************************

void initTaster(void)
{
	DDRD = DDRD & 0xF0;             // Port B auf Eingabe schalten
	PORTD |= 0x0F;                  // Pullup-Rs eingeschaltet
	_delay_us(10);                  // Wartezeit Umstellung Hardware-Signal
}

int readBut()
{
	if (bdelay > millis())
	{
		return 0;
	}
		// Einlesen der 4 Tastensignale
		sw1_neu = (PIND & (1 << PD0));
		sw2_neu = (PIND & (1 << PD1));
		sw3_neu = (PIND & (1 << PD2));
		sw4_neu = (PIND & (1 << PD3));
		
		// Auswerten der Flanken beim Druecken
		
		if ((sw1_neu==0)&(sw1_alt==1))  // wenn Taste 1 soeben gedrueckt wurde:
		moveleft();						// Links
		
		if ((sw2_neu==0)&(sw2_alt==1))  // wenn Taste 2 eben gedrueckt wurde:
		moveright();					// Rechts
		
		if ((sw3_neu==0)&(sw3_alt==1))  // wenn Taste 3 eben gedrueckt wurde:
		rotate();						// Drehen
		
		if ((sw4_neu==0)&(sw4_alt==1))  // wenn Taste 4 eben gedrueckt wurde:
		movedown();					    // Fallen lassen
		
		// Zwischenspeichern aktuelle Tastenwerte
		
		sw1_alt = sw1_neu;              // aktuelle Tastenwerte speichern
		sw2_alt = sw2_neu;              //    in Variable fuer alte Werte
		sw3_alt = sw3_neu;
		sw4_alt = sw4_neu;
		
		
		
		
	}
//***********************************************************
void init_millis(unsigned long F_CPU)
{
	unsigned long ctc_match_overflow;
	
	ctc_match_overflow = ((F_CPU / 1000) / 8); //when timer1 is this value, 1ms has passed
	
	// (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 8)
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	
	// high byte first, then low byte
	OCR1AH = (ctc_match_overflow >> 8);
	OCR1AL = ctc_match_overflow;
	
	// Enable the compare match interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	//REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}

unsigned long millis ()
{
	unsigned long millis_return;
	
	// Ensure this cannot be disrupted
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer1_millis;
	}
	return millis_return;
}

//***********************************************************
void updateLED()
{
	int i;
	int j;
	for (i=0;i<8;i++)
	{
		for (j=0;j<16;j++)
		{
			disp[i][j] = block[i][j] | pile[i][j];
		}
	}
}

//***********************************************************
void rotate()
{
	//skip for square block(3)
	if (blocktype == 3) return;
	
	int xi;
	int yi;
	int i;
	int j;
	//detect left
	for (i=7;i>=0;i--)
	{
		for (j=0;j<16;j++)
		{
			if (block[i][j])
			{
				xi = i;
			}
		}
	}
	
	//detect up
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				yi = i;
			}
		}
	}
	
	if (blocktype == 0)
	{
		if (blockrotation == 0)
		{
			
			
			if (!space_left())
			{
				if (space_right3())
				{
					if (!moveright())
					return;
					xi++;
				}
				else return;
			}
			else if (!space_right())
			{
				if (space_left3())
				{
					if (!moveleft())
					return;
					if (!moveleft())
					return;
					xi--;
					xi--;
				}
				else
				return;
			}
			else if (!space_right2())
			{
				if (space_left2())
				{
					if (!moveleft())
					return;
					xi--;
				}
				else
				return;
			}
			
			
			block[xi][yi]=0;
			block[xi][yi+2]=0;
			block[xi][yi+3]=0;
			
			block[xi-1][yi+1]=1;
			block[xi+1][yi+1]=1;
			block[xi+2][yi+1]=1;

			blockrotation = 1;
		}
		else
		{
			block[xi][yi]=0;
			block[xi+2][yi]=0;
			block[xi+3][yi]=0;
			
			block[xi+1][yi-1]=1;
			block[xi+1][yi+1]=1;
			block[xi+1][yi+2]=1;

			blockrotation = 0;
		}
	}
	
	//offset to mid
	xi ++;
	yi ++;
	
	if (blocktype == 1)
	{
		if (blockrotation == 0)
		{
			block[xi-1][yi-1] = 0;
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;

			block[xi][yi-1] = 1;
			block[xi+1][yi-1] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 1;
		}
		else if (blockrotation == 1)
		{
			if (!space_left())
			{
				if (!moveright())
				return;
				xi++;
			}
			xi--;
			
			block[xi][yi-1] = 0;
			block[xi+1][yi-1] = 0;
			block[xi][yi+1] = 0;
			
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			block[xi+1][yi+1] = 1;
			
			blockrotation = 2;
		}
		else if (blockrotation == 2)
		{
			yi --;
			
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;
			block[xi+1][yi+1] = 0;
			
			block[xi][yi-1] = 1;
			block[xi][yi+1] = 1;
			block[xi-1][yi+1] = 1;
			
			blockrotation = 3;
		}
		else
		{
			if (!space_right())
			{
				if (!moveleft())
				return;
				xi--;
			}
			block[xi][yi-1] = 0;
			block[xi][yi+1] = 0;
			block[xi-1][yi+1] = 0;

			block[xi-1][yi-1] = 1;
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			
			blockrotation = 0;
		}
	}



	if (blocktype == 2)
	{
		if (blockrotation == 0)
		{
			block[xi+1][yi-1] = 0;
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;

			block[xi][yi-1] = 1;
			block[xi+1][yi+1] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 1;
		}
		else if (blockrotation == 1)
		{
			if (!space_left())
			{
				if (!moveright())
				return;
				xi++;
			}
			xi--;
			
			block[xi][yi-1] = 0;
			block[xi+1][yi+1] = 0;
			block[xi][yi+1] = 0;
			
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			block[xi-1][yi+1] = 1;
			
			blockrotation = 2;
		}
		else if (blockrotation == 2)
		{
			yi --;
			
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;
			block[xi-1][yi+1] = 0;
			
			block[xi][yi-1] = 1;
			block[xi][yi+1] = 1;
			block[xi-1][yi-1] = 1;
			
			blockrotation = 3;
		}
		else
		{
			if (!space_right())
			{
				if (!moveleft())
				return;
				xi--;
			}
			block[xi][yi-1] = 0;
			block[xi][yi+1] = 0;
			block[xi-1][yi-1] = 0;

			block[xi+1][yi-1] = 1;
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			
			blockrotation = 0;
		}
	}
	
	if (blocktype == 4)
	{
		if (blockrotation == 0)
		{
			block[xi+1][yi-1] = 0;
			block[xi-1][yi] = 0;

			block[xi+1][yi] = 1;
			block[xi+1][yi+1] = 1;
			
			blockrotation = 1;
		}
		else
		{
			if (!space_left())
			{
				if (!moveright())
				return;
				xi++;
			}
			xi--;
			
			block[xi+1][yi] = 0;
			block[xi+1][yi+1] = 0;
			
			block[xi-1][yi] = 1;
			block[xi+1][yi-1] = 1;
			
			blockrotation = 0;
		}
	}


	if (blocktype == 5)
	{
		if (blockrotation == 0)
		{
			block[xi][yi-1] = 0;
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;

			block[xi][yi-1] = 1;
			block[xi+1][yi] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 1;
		}
		else if (blockrotation == 1)
		{
			if (!space_left())
			{
				if (!moveright())
				return;
				xi++;
			}
			xi--;
			
			block[xi][yi-1] = 0;
			block[xi+1][yi] = 0;
			block[xi][yi+1] = 0;
			
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 2;
		}
		else if (blockrotation == 2)
		{
			yi --;
			
			block[xi-1][yi] = 0;
			block[xi+1][yi] = 0;
			block[xi][yi+1] = 0;
			
			block[xi][yi-1] = 1;
			block[xi-1][yi] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 3;
		}
		else
		{
			if (!space_right())
			{
				if (!moveleft())
				return;
				xi--;
			}
			block[xi][yi-1] = 0;
			block[xi-1][yi] = 0;
			block[xi][yi+1] = 0;
			
			block[xi][yi-1] = 1;
			block[xi-1][yi] = 1;
			block[xi+1][yi] = 1;
			
			blockrotation = 0;
		}
	}
	
	if (blocktype == 6)
	{
		if (blockrotation == 0)
		{
			block[xi-1][yi-1] = 0;
			block[xi][yi-1] = 0;

			block[xi+1][yi-1] = 1;
			block[xi][yi+1] = 1;
			
			blockrotation = 1;
		}
		else
		{
			if (!space_left())
			{
				if (!moveright())
				return;
				xi++;
			}
			xi--;
			
			block[xi+1][yi-1] = 0;
			block[xi][yi+1] = 0;
			
			block[xi-1][yi-1] = 1;
			block[xi][yi-1] = 1;
			
			blockrotation = 0;
		}
	}

	//if rotating made block and pile overlap, push rows up
	while (!check_overlap())
	{
		for (i=0;i<18;i++)
		{
			for (j=0;j<8;j++)
			{
				block[j][i] = block[j][i+1];
			}
		}
		delays = millis() + delay_;
	}
	
	
	updateLED();
}
//***********************************************************

void movedown()
{
	if (space_below())
	{
		//move down
		int i;
		for (i=15;i>=0;i--)
		{
			int j;
			for (j=0;j<8;j++)
			{
				block[j][i] = block[j][i-1];
			}
		}
		for (i=0;i<7;i++)
		{
			block[i][0] = 0;
		}
	}
	else
	{
		//merge and new block
		int i;
		int j;
		for (i=0;i<8;i++)
		{
			for(j=0;j<16;j++)
			{
				if (block[i][j])
				{
					pile[i][j]=1;
					block[i][j]=0;
				}
			}
		}
		newBlock();
	}
	updateLED();
}
//***********************************************************

bool check_overlap()
{
	int i;
	int j;
	for (i=0;i<16;i++)
	{
		for (j=0;j<7;j++)
		{
			if (block[j][i])
			{
				if (pile[j][i])
				return false;
			}
		}
	}
	for (i=16;i<18;i++)
	{
		for (j=0;j<7;j++)
		{
			if (block[j][i])
			{
				return false;
			}
		}
	}
	return true;
}
//***********************************************************

void check_gameover()
{
	int i;
	int j;
	int cnt=0;;
	
	for(i=15;i>=0;i--)
	{
		cnt=0;
		for (j=0;j<8;j++)
		{
			if (pile[j][i])
			{
				cnt ++;
			}
		}
		if (cnt == 8)
		{
			for (j=0;j<8;j++)
			{
				pile[j][i]=0;
			}
			updateLED();
			_delay_us(50);
			
			int k;
			for(k=i;k>0;k--)
			{
				for (j=0;j<8;j++)
				{
					pile[j][k] = pile[j][k-1];
				}
			}
			for (j=0;j<8;j++)
			{
				pile[j][0] = 0;
			}
			updateLED();
			_delay_us(50);
			i++;
			
			
			
		}
	}
	
	
	for(i=0;i<8;i++)
	{
		if (pile[i][0])
		gameover();
	}
	return;
}


//***************************************************************
  
void gameover()
{
  int i;
  int j;
  

  gameoverFlag = true;
  startTime = millis();       
       
  _delay_us(300);       
            
  while(true)      //To re-play if any buttons depressed again
  {      
    int button = readBut();
    
    if ((button < 5) && (button > 0))
    {
      gameoverFlag = false;    
    
      for(i=15;i>=0;i--)
      {
        for (j=0;j<8;j++)
        {
          pile[j][i]=0;
        }             
      }
    
      break;
    }  
  }  
}//***********************************************************

void newBlock()
{
  check_gameover();
  
  blocktype = rand()%7+1;

  
  if (blocktype == 0)
  // 0
  // 0
  // 0
  // 0
  {
    block[3][0]=1;
    block[3][1]=1;
    block[3][2]=1;
    block[3][3]=1;      
  }

  if (blocktype == 1)
  // 0
  // 0 0 0
  {
    block[2][0]=1;
    block[2][1]=1;
    block[3][1]=1;
    block[4][1]=1;        
  }
  
  if (blocktype == 2)
  //     0
  // 0 0 0
  {
    block[4][0]=1;
    block[2][1]=1;
    block[3][1]=1;
    block[4][1]=1;         
  }

  if (blocktype == 3)
  // 0 0
  // 0 0
  {
    block[3][0]=1;
    block[3][1]=1;
    block[4][0]=1;
    block[4][1]=1;          
  }    

  if (blocktype == 4)
  //   0 0
  // 0 0
  {
    block[4][0]=1;
    block[5][0]=1;
    block[3][1]=1;
    block[4][1]=1;         
  }    
  
  if (blocktype == 5)
  //   0
  // 0 0 0
  {
    block[4][0]=1;
    block[3][1]=1;
    block[4][1]=1;
    block[5][1]=1;       
  }        

  if (blocktype == 6)
  // 0 0
  //   0 0
  {
    block[3][0]=1;
    block[4][0]=1;
    block[4][1]=1;
    block[5][1]=1;         
  }    

  blockrotation = 0;
}



//***************************************************************

bool space_below()
{ 
  int i;
  int j;  
  for (i=15;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (i == 15)
           return false;
         if (pile[j][i+1])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}
//**********************************************************

bool space_left2()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 0 || j == 1)
				return false;
				if (pile[j-1][i] | pile[j-2][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}
//**********************************************************

bool space_left3()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 0 || j == 1 ||j == 2 )
				return false;
				if (pile[j-1][i] | pile[j-2][i]|pile[j-3][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}
//**********************************************************

bool space_left()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 0)
				return false;
				if (pile[j-1][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}

//**********************************************************

bool space_right()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 7)
				return false;
				if (pile[j+1][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}
//**********************************************************

bool space_right3()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 7||j == 6||j == 5)
				return false;
				if (pile[j+1][i] |pile[j+2][i] | pile[j+3][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}


//**********************************************************

bool space_right2()
{
	int i;
	int j;
	for (i=15;i>=0;i--)
	{
		for (j=0;j<8;j++)
		{
			if (block[j][i])
			{
				if (j == 7 || j == 6)
				return false;
				if (pile[j+1][i] |pile[j+2][i])
				{
					return false;
				}
			}
		}
	}
	return true;
}
//**********************************************************

void LEDRefresh()
{
	int i;
	int k;

	bool tmpdispUpper[8][8];
	bool tmpdispLower[8][8];
	
	bool tmppileUpper[8][8];
	bool tmppileLower[8][8];
	
	
	//rotate 90 degrees for upper Bicolor LED matrix
	for (k=0;k<8;k++)
	{
		for(i=0;i<8;i++)
		{
			tmpdispUpper[k][i]=disp[i][k];
		}
	}
	
	
	//rotate 90 degrees for lower Bicolor LED matrix
	for (k=8;k<16;k++)
	{
		for(i=0;i<8;i++)
		{
			tmpdispLower[k-8][i]=disp[i][k];
		}
	}



	//For pile
	//rotate 90 degrees for upper Bicolor LED matrix
	for (k=0;k<8;k++)
	{
		for(i=0;i<8;i++)
		{
			tmppileUpper[k][i]=pile[i][k];
		}
	}
	
	
	//rotate 90 degrees for lower Bicolor LED matrix
	for (k=8;k<16;k++)
	{
		for(i=0;i<8;i++)
		{
			tmppileLower[k-8][i]=pile[i][k];
		}
	}

	

	for(i=0;i<8;i++)
	{
		uint8_t upper = 0;
		int b;
		for(b = 0;b<8;b++)
		{
			upper <<= 1;
			if (tmpdispUpper[b][i]) upper |= 1;
		}
		
		
		uint8_t lower = 0;
		for(b = 0;b<8;b++)
		{
			lower <<= 1;
			if (tmpdispLower[b][i]) lower |= 1;
		}

		
		if (gameoverFlag == true)
		{
			elapsedTime = millis() - startTime;

			// Display random pattern for pre-defined period before blanking display
			if (elapsedTime < 2000)
			{
				maxTransferSingle( 1, i,  ((rand()%256)));
				maxTransferSingle( 2, i, ((rand()%256)));
				
				maxTransferSingle( 1, i, ((rand()%256)));
				maxTransferSingle( 2, i, ((rand()%256)));
				
				cnt = cnt + 1;
				
				
			}
			else
			{
				maxTransferSingle( 1, i, 0x00);  // clear
				maxTransferSingle( 2, i, 0x00);  // clear

				maxTransferSingle( 1, i, 0x00);  // clear
				maxTransferSingle( 2, i, 0x00);  // clear
			}
			
		}
		else
		{
			
			
			maxTransferSingle( 1, i, lower);
			maxTransferSingle( 2, i, upper);
			
			
			
		}
	}
	
	
	
	if (gameoverFlag == false)
	{
		// For pile - to display orange
		for(i=0;i<8;i++)
		{
			unsigned char  upper = 0;
			int b;
			for(b = 0;b<8;b++)
			{
				upper <<= 1;
				if (tmppileUpper[b][i]) upper |= 1;
			}
			
			
			unsigned char lower = 0;
			for(b = 0;b<8;b++)
			{
				lower <<= 1;
				if (tmppileLower[b][i]) lower |= 1;
			}

		}
	}
}
//**********************************************************************************************************************************************************  

// Change Max72xx brightness
void setBrightness()
{      
    maxTransferAll(0x0A, SetbrightnessValue);      //Set Brightness
    maxTransferAll(0x00, 0x00);  //No-op commands
}


//**********************************************************************************************************************************************************  

void maxTransferAll(uint8_t address, uint8_t value) {
	
	SLAVE_SELECT;

	for ( int c=1; c<= maxInUse*2;c++) {
		writeByte(address);  // specify register
		writeByte(value);  // put data
	}

	SLAVE_DESELECT;

}
//**********************************************************************************************************************************************************  
void maxTransferOne(uint8_t whichMax, uint8_t address, uint8_t value) {

  uint8_t noop_reg = 0x00;    //max7219 No op register
  uint8_t noop_value = 0x00;  //value

 
  SLAVE_SELECT; 

  for (int i=maxInUse; i>0; i--)   // Loop through our number of Bi-color LED Matrices 
  {
    if (i==whichMax)
    {
      writeByte(address);  // Send the register address
      writeByte(value);    // Send the value

      writeByte(address);  // Send the register address
      writeByte(value);    // Send the value

    }
    else
    {
      writeByte(noop_reg);    // Send the register address
      writeByte(noop_value);  // Send the value

      writeByte(noop_reg);    // Send the register address
      writeByte(noop_value);  // Send the value
    }
  }

  SLAVE_DESELECT;
 
}



//**********************************************************************************************************************************************************  
void maxTransferSingle( uint8_t whichMax, uint8_t address, uint8_t value) {  //whichColor = 1 for RED, 2 for GREEN

 uint8_t noop_reg = 0x00;    //max7219 No op register
  uint8_t noop_value = 0x00;  //value

 
  SLAVE_SELECT;



  for (int i=maxInUse; i>0; i--)   // Loop through our number of Bi-color LED Matrices 
  {
    if (i==whichMax)
    {
      writeByte(address+1);   // Send the register address
      writeByte(value);       // Send the value

      writeByte(noop_reg);    // Send the register address
      writeByte(noop_value);  // Send the value

    }
    else
    {
      writeByte(noop_reg);    // Send the register address
      writeByte(noop_value);  // Send the value

      writeByte(noop_reg);    // Send the register address
      writeByte(noop_value);  // Send the value
    }
  }


 SLAVE_DESELECT;
 
}



//**********************************************************************************************************************************************************  
void maxShutdown(uint8_t cmd) 
{
  uint8_t noop_reg = 0x00;      //max7219_reg_no_op
   uint8_t shutdown_reg = 0x0c;  //max7219_reg_shutdown
   uint8_t col = 0x01;  //shutdown false
   uint8_t col2 = 0x00;  //shutdown true



    
    
   SLAVE_SELECT;

    for (int c =1; c<= maxInUse; c++)
    {
      writeByte(shutdown_reg);  // Send the register address
      writeByte(col2);          // Send the value

      writeByte(shutdown_reg);  // Send the register address
     writeByte(col2);          // Send the value
    }

    SLAVE_DESELECT;
   
 
 


  //No ops register to shift out instructions   
  
 SLAVE_SELECT;

  for (int c =1; c<= maxInUse; c++) 
  {      
   writeByte(noop_reg);  // Send the register address
  writeByte(0x00);      // Send the value

   writeByte(noop_reg);  // Send the register address
   writeByte(0x00);      // Send the value
  }

 SLAVE_DESELECT;
}


//**********************************************************
//**********************************************************
//**********************************************************