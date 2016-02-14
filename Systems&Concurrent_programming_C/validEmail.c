#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

/* find the number of full stops in 
   a given email address. */
int findNumberOfStops(char * address)
{	
	int number = 0;
	char nullPointer = '\0';
	for(int i = 0; *(address+i) != nullPointer; i++)
	{
		if(*(address+i) == '.')
		{
			number++;
		}
	}
	return number;
}

/* determines whether a given email address is appropriately 
   terminated after it's last full stop by comparing the sub string
   after the last full stop to an accepted list of terminators. */
int isTerminated(char * address, char ** terminators, int num_terminators)
{
	int result = 0;
	int match = 0;
	char nullPointer = '\0';
	for(int i = 0; i < num_terminators; i++)
	{
		for(int j = 0; *(address+j) != nullPointer; j++)
		{
			if(*(*(terminators+i)+j) == *(address+j))
			{
				match = 1;				
			}
			else
			{
				match = 0;
			}
		}
		if(match == 1)
		{
			result = 1;
			return result;
		}
	}
	return result;
}

/* determines whether a given email address is valid. */
int isValidEmailAddress(char * address, char ** terminators,
                        int num_terminators)
{
	int identifier1 = 0;
	int atSymbol = 0;
	int identifier2 = 0;
	int terminated = 0;
	int numberOfStops = findNumberOfStops(address);
	int stopNumber = 0;
	int result = 0;
	char nullPointer = '\0';
	int alphaCheck = 0;

	for(int i = 0; *(address+i) != nullPointer; i++)
	{
		if(!isalpha(*(address+i)) && !isdigit(*(address+i)) 
			&& *(address+i) != '.' && *(address+i) != '@')
		{
			result = 0;
			return result; 
		}	
		if(isalpha(*(address+i)))
		{
			alphaCheck = 1;
			if(identifier1 == 0)
			{
				identifier1 = 1;
			}
			else
			{
				identifier2 = 1;	
			}
		}
		if(isdigit(*(address+i)))
		{
			if(identifier1 == 0)
			{
				identifier1 = 1;
			}
			else
			{
				identifier2 = 1;	
			}
			if(alphaCheck != 1)
			{
				return 0;
			}

		}
		if(*(address+i) == '.')
		{
			alphaCheck = 0;
			stopNumber++;
			if(stopNumber == numberOfStops)
			{
				if(atSymbol == 0)
				{
					result = 0;
					return result;
				}
				if(isTerminated(address + i + 1, terminators, num_terminators)) // == 1 ?
				{
					terminated = 1;
				}
				else
				{
					result = 0;
					return result;
				}
			}
			if(identifier1 == 0)
			{
				result = 0;
				return result;
			}
			if(atSymbol == 0)
			{
				identifier1 = 0;
			}
			else
			{
				if(identifier2 == 0)
				{
					result = 0;
					return result;
				}
				else
				{
					identifier2 = 0;
				}
			}
		}
		if(*(address+i) == '@')
		{
			if(identifier1 == 0)
			{
				result = 0;
				return result;
			}
			if(atSymbol == 1)
			{
				result = 0;
				return result;
			}
			else
			{
				atSymbol = 1;
			}
		}
	}
	if(identifier1 != 1 || atSymbol != 1 || identifier2 != 1
			|| terminated != 1)
	{
		result = 0;
		return result;
	}
	result = 1;
	return result;
}

/* determines the validity of a given email addresses,
   writing the result out to the screen. */
void print_validity(char * address, char** terminators,
                    int length)
{
  printf("The string %s is ", address);
  if (isValidEmailAddress(address, terminators, length) == 0) {
    printf("not ");
  }
  printf("a valid email address\n");
}

/* determines the validity of a series of email addresses,
   writing the results out to the screen. */
int main()
{
  const int length = 5;
  char * terminators[length];
  char * address1, * address2, * address3, * address4;

  terminators[0] = "com";
  terminators[1] = "net";
  terminators[2] = "edu";
  terminators[3] = "ie";
  terminators[4] = "tv";

  address1 = "santa.claus@north.1pole.com";
  address2 = "I..am@fish.com";
  address3 = "lorcanreidy@tcd@google.ie";
  address4 = "a@cat.nett";

  print_validity(address1, terminators, length);
  print_validity(address2, terminators, length);
  print_validity(address3, terminators, length);
  print_validity(address4, terminators, length);

  return 0;
}




