/*
 * Tetris_V1.c
 *
 * Created: 06/04/2021 19:41:36
 * Author : dakak
 */ 


#include "Tetris.h"

#define NUM_DEVICES    2	 // Number of cascaded max7219's, or just 1
void initSPI(void)
{
	DDRB |= (1 << PB2);		// Set SS output
	PORTB |= (1 << PB2);      // Begin high (unselected)

	DDRB |= (1 << PB3);       // Output on MOSI
	DDRB |= (1 << PB5);       // Output on SCK

	SPCR |= (1 << MSTR);            // Clockmaster
	SPCR |= (1 << SPE);       // Enable SPI
	
}
//**********************************************************************************************************************************************************
// Send byte through SPI
void writeByte(uint8_t byte)
{
	SPDR = byte;                      // SPI starts sending immediately
	while(!(SPSR & (1 << SPIF)));     // Loop until complete bit set
}



//**********************************************************************************************************************************************************
// Sends word through SPI
void writeWord(uint8_t address, uint8_t data)
{
	writeByte(address);	// Write first byte
	writeByte(data);      // Write Second byte
}


//**********************************************************************************************************************************************************
// Initializes all cascaded devices

void initMatrix()
{
	uint8_t i;	// Var used in for loops

	// Set display brighness
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)	// Loop through number of cascaded devices
	{
		writeByte(0x0E); // Select Intensity register
		writeByte(0x07); // Set brightness
	}
	SLAVE_DESELECT;

	
	// Set display refresh
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0B); // Select Scan-Limit register
		writeByte(0x07); // Select columns 0-7
	}
	SLAVE_DESELECT;

	
	// Turn on the display
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0C); // Select Shutdown register
		writeByte(0x01); // Select Normal Operation mode
	}
	SLAVE_DESELECT;

	
	// Disable Display-Test
	SLAVE_SELECT;
	for(i = 0; i < NUM_DEVICES; i++)
	{
		writeByte(0x0F); // Select Display-Test register
		writeByte(0x00); // Disable Display-Test
	}
	SLAVE_DESELECT;
}
//**********************************************************************************************************************************************************
void clearMatrix(void)
{
	for(uint8_t x = 1; x < 9; x++) // for all columns
	{
		SLAVE_SELECT;
		for(uint8_t i = 0; i < NUM_DEVICES; i++)
		{
			writeByte(x);    // Select column x
			writeByte(0x00); // Set column to 0
		}
		SLAVE_DESELECT;
	}
}
//**********************************************************************************************************************************************************
void initTaster(void)
{
	DDRD = DDRD & 0xF0;             // Port B auf Eingabe schalten
	PORTD |= 0x0F;                  // Pullup-Rs eingeschaltet
	_delay_us(10);                  // Wartezeit Umstellung Hardware-Signal
}
//**********************************************************************************************************************************************************
int readButton(void)
{
	// Einlesen der 4 Tastensignale
	sw1_neu = (PIND & (1 << PD0));
	sw2_neu = (PIND & (1 << PD1));
	sw3_neu = (PIND & (1 << PD2));
	sw4_neu = (PIND & (1 << PD3));
	
	// Auswerten der Flanken beim Druecken
	
	if ((sw1_neu==0)&(sw1_alt==1))  // wenn Taste 1 soeben gedrueckt wurde:
	sw1_slope = 1;              //    Flankenbit Taste 1 setzen
	
	if ((sw2_neu==0)&(sw2_alt==1))  // wenn Taste 2 eben gedrueckt wurde:
	sw2_slope = 1;              //    Flankenbit Taste 2 setzen
	
	if ((sw3_neu==0)&(sw3_alt==1))  // wenn Taste 3 eben gedrueckt wurde:
	sw3_slope = 1;              //    Flankenbit Taste 3 setzen
	
	if ((sw4_neu==0)&(sw4_alt==1))  // wenn Taste 4 eben gedrueckt wurde:
	sw4_slope = 1;              //    Flankenbit Taste 4 setzen
	
	// Zwischenspeichern aktuelle Tastenwerte
	
	sw1_alt = sw1_neu;              // aktuelle Tastenwerte speichern
	sw2_alt = sw2_neu;              //    in Variable fuer alte Werte
	sw3_alt = sw3_neu;
	sw4_alt = sw4_neu;
}
//**********************************************************************************************************************************************************
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
//**********************************************************************************************************************************************************
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
//**********************************************************************************************************************************************************
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

//**********************************************************************************************************************************************************
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
void gameover()
{
  int i;
  int j;
  

  gameoverFlag = true;
  startTime = millis();       
       
  _delay_us(300);       
            
  while(true)      //To re-play if any buttons depressed again
  {      
    int button = readButton();
    
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
}



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
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



//**********************************************************************************************************************************************************  
// ISR(TIMER1_COMPA_vect){  //change the 0 to 1 for timer1 and 2 for timer2
//     LEDRefresh();
// }



//**********************************************************************************************************************************************************  
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
          
//           if (cnt > 80)
//           {
//             TriggerSound();
//             TriggerSound();
//             cnt = 0;
//           }
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

      
//         To alternate color of new block between RED and GREEN
//                 if (selectColor == RED)
//                 {
//                   bi_maxTransferSingle(RED, 1, i, lower);
//                   bi_maxTransferSingle(RED, 2, i, upper);
//                 }
//                 else
//                 {
//                   bi_maxTransferSingle(GREEN, 1, i, lower);
//                   bi_maxTransferSingle(GREEN, 2, i, upper);    
//                 }  

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

// 
//  * Transfers data to a MAX7219/MAX7221 register of a particular Bi-color LED Matrix module.
//  *
//  * @param whichMax The Max72xx to load data and value into
//  * @param address The register to load data into
//  * @param value Value to store in the register

 
//**********************************************************************************************************************************************************  

void maxTransferAll(uint8_t address, uint8_t value) {
	/*stopISR();*/
	SLAVE_SELECT;

	for ( int c=1; c<= maxInUse*2;c++) {
		writeByte(address);  // specify register
		writeByte(value);  // put data
	}

	SLAVE_DESELECT;
/*	startISR();*/
}
//**********************************************************************************************************************************************************  
void maxTransferOne(uint8_t whichMax, uint8_t address, uint8_t value) {

  uint8_t noop_reg = 0x00;    //max7219 No op register
  uint8_t noop_value = 0x00;  //value

 /* stopISR();*/
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
 /* startISR();*/
}



//**********************************************************************************************************************************************************  
void maxTransferSingle( uint8_t whichMax, uint8_t address, uint8_t value) {  //whichColor = 1 for RED, 2 for GREEN

 uint8_t noop_reg = 0x00;    //max7219 No op register
  uint8_t noop_value = 0x00;  //value

 /* stopISR();*/
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
 /* startISR();*/
}



//**********************************************************************************************************************************************************  
void maxShutdown(uint8_t cmd) 
{
  uint8_t noop_reg = 0x00;      //max7219_reg_no_op
   uint8_t shutdown_reg = 0x0c;  //max7219_reg_shutdown
   uint8_t col = 0x01;  //shutdown false
   uint8_t col2 = 0x00;  //shutdown true



    
    /*stopISR();*/
   SLAVE_SELECT;

    for (int c =1; c<= maxInUse; c++)
    {
      writeByte(shutdown_reg);  // Send the register address
      writeByte(col2);          // Send the value

      writeByte(shutdown_reg);  // Send the register address
     writeByte(col2);          // Send the value
    }

    SLAVE_DESELECT;
    /*startISR();*/
 
 


  //No ops register to shift out instructions   
  /*stopISR();*/
 SLAVE_SELECT;

  for (int c =1; c<= maxInUse; c++) 
  {      
   writeByte(noop_reg);  // Send the register address
  writeByte(0x00);      // Send the value

   writeByte(noop_reg);  // Send the register address
   writeByte(0x00);      // Send the value
  }

 SLAVE_DESELECT;
 /* startISR();*/
}

//**********************************************************************************************************************************************************
/////////////////////////////ISR Timer Functions using Timer2///////////////////////////
// ISR(TIMER2_COMPA_vect) {  //This ISR toggles shutdown between the 2MAX7221's
// 
//   if (colorMode == '3')    // ORANGE
//   {
//     if(maxInShutdown==RED){
//       bi_maxShutdown(onREDoffGREEN);
//       maxInShutdown=GREEN;
//     } 
//     else 
//     { 
//       bi_maxShutdown(offREDonGREEN);
//       maxInShutdown=RED;
//     }
//   }
//   else if (colorMode == '2')   // GREEN
//   {
//     bi_maxShutdown(offREDonGREEN);
//     maxInShutdown=RED;
//   }
//   else if (colorMode == '1')   // RED
//   {
//     bi_maxShutdown(onREDoffGREEN);
//     maxInShutdown=GREEN;
//   }
//   else if (colorMode == '4')  //Blank Display
//   {
//     bi_maxShutdown(offREDoffGREEN);       
//     maxInShutdown=GREEN;
//   }
// } 



//**********************************************************************************************************************************************************
// void setISRtimer() // setup ISR timer controling toggleing
// { 
//   TCCR2A = 0x02;                        // WGM22=0 + WGM21=1 + WGM20=0 = Mode2 (CTC)
//   TCCR2B = 0x05;                // CS22=1 + CS21=0 + CS20=1 = /128 prescaler (125kHz)
//   TCNT2 = 0;                            // clear counter
//   OCR2A = ISR_FREQ;                     // set TOP (divisor) - see #define
// }
//      
//  
//      
// //**********************************************************************************************************************************************************
// void startISR()    // Starts the ISR
// {
//   TCNT2 = 0;                            // clear counter (needed here also)
//   TIMSK2|=(1<<OCIE2A);                  // set interrupts=enabled (calls ISR(TIMER2_COMPA_vect)
// }
// 
//   
//      
// //**********************************************************************************************************************************************************
// void stopISR()    // Stops the ISR
// {
//   TIMSK2&=~(1<<OCIE2A);                  // disable interrupts
// }



//**********************************************************************************************************************************************************



int main(void)
{
	initSPI();
	initMatrix();
	clearMatrix();
	

	
    /* Replace with your application code */
    while (1) 
    {
    }
}

