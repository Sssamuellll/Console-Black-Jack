#include <iostream>
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

//Initialises the card structure.
struct cardObject
{
	char m_suit[4][9] = { "Clubs", "Diamonds", "Hearts", "Spades", };
	char m_val[13][6] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "JACK", "QUEEN", "KING" };
};

//Initialises all the functions.
void Menu();
int Play(bool debug, int _pot);
void HowTo();
int DeckCreate(int _deck[], bool _debug);
int ShuffleDeck(int _deck[], bool _debug);
int Deal(int _pHand[], int _dHand[], int _deck[], int _round, int _cardsInPlay[], char _player, char _whoWon[], char _choice, bool _debug, int _cardsDrawn);
int CardCheck(int _count, int _array1[], int _array2[], int _doesCardExist[]);
int CardDisplay(cardObject&, int _deck[], int _hand[], char _player, char _whoWon[], int _round, bool _debug);
int WinState(char _player, char _whoWon[], int  _pHand[], int _dHand[], char _choice, bool _debug, int _pot, int _betNum);
int CardCount(int _cards[]);
int CardValues(int _hand[], int _pos);

//The main function
int main()
{
	srand(time(0));//Initialises the random seed using time to allow for any random integer generation.
	Menu();//Calls the menu function.
	std::cout << "End of Program";//Once the menu function has complete this will output.

	return 0;
}

//The menu function initalised using void as there is nothing that needs to be returned.
void Menu()
{
	//Initalising variables.
	int pot = 1000;
	bool debug = false;
	char user[80];
	//This do while loop asks the user for a valid single charater input.
	//If the user does not provide a valid input, it will loop back around and re-ask the user for a valid input.
	//The loop checks if the user has provided a valid input by using the strcmp() function to the desired input 
	//which will output 0 if both the user's input meets the desired input.
	do
	{
		std::cout << "\n1. Play\n2. Instructions\n3. Debug\n4. Quit" << std::endl;
		std::cin >> user;
		if (strcmp(user, "1") == 0)
		{
			debug = false;
			pot = Play(debug, pot);//Calls the play() function passing debug and pot by value.
		}
		else if (strcmp(user, "2") == 0)
		{
			HowTo();//Call
		}
		else if (strcmp(user, "3") == 0)
		{
			debug = true;
			pot = Play(debug, pot);
		}
		for (int i = 0; i > strlen(user); i++)
		{
			user[i] = 0;
		}
	} while (strcmp(user, "4") != 0 && pot > 0);//Checks if the condition, user != "4" and pot > 0, is met.
}

//The main play function.
int Play(bool _debug, int _pot)
{
	std::cout << "\n\n-{BLACK JACK}-" << std::endl;
	//Initalises many variables that will be used and manipulated by different functions.
	int deck[53] = {};
	int cardsInPlay[53] = {};
	char userInput[80] = "0";
	char choice = '0';
	int cardsDrawn = 1;
	DeckCreate(deck, _debug);//Calls the deckCreate() function and passes the variables deck and _debug by value.
	ShuffleDeck(deck, _debug);//Calls shuffleDeck() function and pass the variables deck and _debug by value
	do
	{
		int pHand[27] = {};
		int dHand[27] = {};
		int round = 1;
		char player = '0';
		char whoWon[2] = { 0 };
		std::cout << whoWon[0] << whoWon[1] << std::endl;
		char bet[80] = "0";
		int betNum = 0;
		choice = '0';
		std::cout << "=====================================\nROUND " << round << "\n=====================================";
		std::cout << "\nMoney Pot: " << _pot << std::endl;
		//If the game is at the beginning round, it asks the user for a valid bet input.
		//The program checks if the user's input is valid by using the atoi() function it converts the cstring to an integer.
		//If the atoi function returns an output of 0 this means that the user has entered either a character or 0 which forces the
		//user to bet aleast £1. The bet cstring also acts as an input buffer.
		if (round == 1)
		{
			do
			{
				std::cout << "How Much Would you like to bet: ";
				std::cin >> bet;
				if (strlen(bet) <= 80)
				{
					betNum = atoi(bet);
				}
			} while (betNum <= 0 || betNum >= _pot);
			//Returns user feedback acknowledging the input using std::cout.
			std::cout << "You Have bet: " << betNum << std::endl;
			std::cout << "=====================================" << std::endl;
			_pot = _pot - betNum;
		}
		//This is where the main game of Blackjack takes place.
		Deal(pHand, dHand, deck, round, cardsInPlay, player, whoWon, choice, _debug, cardsDrawn);
		_pot = WinState(player, whoWon, pHand, dHand, choice, _debug, _pot, betNum);
		//passing all necessary variables and arrays by value.
		//The whoWon[] array holds the winners of the game. The while loop checks if anyone has won the current game.
		//While no one has won, the round is incremented by 1.
		while ((whoWon[0] != 'p' && whoWon[0] != 'd') && choice != 'Q')
		{
			round = round + 1;
			cardsDrawn = cardsDrawn + 1;
			if (whoWon[0] != 'p' && whoWon[0] != 'd')//still check if 'p' or 'd' has been stored in whoWon.
			{
				//Asks the user for a vaild single character input and uses the strcmp() function to compare if the user's input is valid.
				//If the input is invalid then it loops back around and re-asks the user to enter a valid input. The userInput cstring also acts as an input buffer.
				std::cout << "=====================================\nWould you like to (S)tand or (H)it: ";
				std::cin >> userInput;
				if (strlen(userInput) == 1)
				{
					if (strcmp(userInput, "s") == 0 || strcmp(userInput, "S") == 0)
					{
						choice = 'S';//choice is assigned the current input when the if statement is true so that the input can be pass by value through functions more easily.
						Deal(pHand, dHand, deck, round, cardsInPlay, player, whoWon, choice, _debug, cardsDrawn);
						_pot = WinState(player, whoWon, pHand, dHand, choice, _debug, _pot, betNum);
					}
					if (strcmp(userInput, "h") == 0 || strcmp(userInput, "H") == 0)
					{
						choice = 'H';
						std::cout << "\n=====================================\nROUND " << round << "\n=====================================\n";
						Deal(pHand, dHand, deck, round, cardsInPlay, player, whoWon, choice, _debug, cardsDrawn);
						_pot = WinState(player, whoWon, pHand, dHand, choice, _debug, _pot, betNum);
					}
				}
			}
		}
		do
		{
			std::cout << "=====================================\n(P)lay another hand or (Q)uit to main menu: ";
			std::cin >> userInput;
			if (((strcmp(userInput, "q") == 0) || (strcmp(userInput, "Q") == 0)))
			{
				choice = 'Q';
			}
		} while (((strcmp(userInput, "p") != 0) && (strcmp(userInput, "P") != 0)) && choice != 'Q');
	} while (choice != 'Q' && _pot > 0);
	return _pot;//The pot variable is returned so it can be updated beyond this function and prevents it from resetting to its initailised state.
}

//This function generates the deck of cards by...
int DeckCreate(int _deck[], bool _debug)
{
	if (_debug == true)
	{
		std::cout << "Deck Array:" << std::endl;
	}
	//..looping through the deck array, it assigns the position of "i" in deck and assigns it as "i" creating a deck of numbers 1-52.
	for (int i = 1; i < 53; i++)
	{
		_deck[i] = i;
		if (_debug == true)//if the debug option is selected then this deck of numbers will be shown to the user by looping through the deck and using std::cout to output every position.
		{
			std::cout << _deck[i] << " ";
		}
	}
	std::cout << std::endl;
	return 0;
}

//This function shuffles the deck of cards that have been previously created.
int ShuffleDeck(int _deck[], bool _debug)
{
	//creates a copy of the already created deck by looping through the deck array by assigning each position of the array to the tempDeck array.
	int tempDeck[53] = {};
	for (int i = 0; i < 53; i++)
	{
		tempDeck[i] = _deck[i];
	}
	//shuffles the deck array by looping and calling the cardCheck() function which passes the previously initalised variables and arrays including "i", position of the loop.
	int shuffled[53] = {};
	for (int i = 0; i < 52; i++)
	{
		CardCheck(i, _deck, tempDeck, shuffled);
	}

	if (_debug == true)//if the debug option is selected then this shuffled deck of numbers will be shown to the user by looping through the deck and using std::cout to output every position.
	{
		std::cout << "Shuffled Deck Array:" << std::endl;
		for (int i = 0; i < 52; i++)
		{
			std::cout << _deck[i] << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
//This function deals cards to both the player and dealer once called.
int Deal(int _pHand[], int _dHand[], int _deck[], int _round, int _cardsInPlay[], char _player, char _whoWon[], char _choice, bool _debug, int _cardsDrawn)
{
	cardObject card{};//This function ultilises the cardObject structure.
	//Initalising variables
	int cardNum = _cardsDrawn + 1;
	int dTotal = 0;
	int cards[27] = {};
	//This is again retreiving the total card values, but of the dealer's hand this time.
	for (int pos = 0; pos < 27; pos++)
	{
		cards[pos] = CardValues(_dHand, pos);
	}
	dTotal = CardCount(cards);
	//If its the first round of the game, the function will deal two cards each to the player and dealer by looping the calling of the cardCheck() function twice for both.
	if (_round == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			CardCheck(i, _pHand, _deck, _cardsInPlay);
			CardCheck(i, _dHand, _deck, _cardsInPlay);
			//Once the cards are dealt into the player's and dealer's hands they are displayed by calling the cardDisplay() function and passing the necessary variables and arrays by value.
		}
		_player = 'p';//The player character variable is overwritten and assigned as the character 'p' to indicate to the cardDisplay function who's hand is being displayed.
		CardDisplay(card, _deck, _pHand, _player, _whoWon, _round, _debug);
		_player = 'd';//The same is done with the dealer as the player variable is again overwritten as the character 'd'.
		CardDisplay(card, _deck, _dHand, _player, _whoWon, _round, _debug);
	}
	//However, if the round is not the first, then depending on what the player's choice is, which has previously been inputted,...
	if (_round > 1)
	{
		if (_choice == 'h' || _choice == 'H')
		{
			//If the player has chosen to "Hit" then the function will deal the player another card...
			CardCheck(cardNum, _pHand, _deck, _cardsInPlay);

		}
		// and depending if the dealer's total card value, the function will either deal another card to the dealer or skip ahead to displaying both hands again.
		//This simulates the dealer choosing to either "Hit" or "Stand".
		if (dTotal < 17)
		{
			CardCheck(cardNum, _dHand, _deck, _cardsInPlay);
		}
		_player = 'p';
		CardDisplay(card, _deck, _pHand, _player, _whoWon, _round, _debug);
		_player = 'd';
		CardDisplay(card, _deck, _dHand, _player, _whoWon, _round, _debug);
	}
	return 0;
}

//This function chooses a random number from the shuffled deck of cards and is assigned to the certain array that is pass by value into the function.
int CardCheck(int _count, int _array1[], int _array2[], int _doesCardExist[])
{
	//Initalises variables
	bool found = false;
	bool checked = false;
	int pos = 0;
	//This loops while the card that has been chosen all ready exists in an array that is pass by value via the parameter _doesCardExist[].
	do
	{
		//generates a random number using the seed initalised in the play() game function and is assigned to a variable.
		pos = (rand() % 53);
		//This variable is then used to assign the position of the array that includes all the cards to the array that wants to add random card to specific position using the _count parameter.
		_array1[_count] = _array2[pos];
		found = false;
		checked = false;
		//checks through the array that has been passed by value into the function to see if the card that has been chosen already exist in the part of the game the card is needed.
		for (int i = 0; i < 53; i++)
		{
			if (_doesCardExist[i] == 0)
			{
				//if the array that has been pass by value via a parmeter includes a 0 at any position the boolean checked variable will become true. This is used to determine the end of the array.
				//This works as the cards are number 1-52 not 0-51.
				checked == true;
			}
			//If the boolean checked is still false and the card is found in the array then the boolean found will become true and continue the while loop.
			if (_doesCardExist[i] == _array1[_count] && checked == false)
			{
				found = true;
			}
		}
	} while (found == true);//Once the found boolean stays false the while loop will end meaning a unique card has been found.
	if (found == false)//Once a unique card has been found, the card will be assigned as a position in the _deosCardExist[] array parameter so it can't be used again, 
		//but only if the specificed position if avaliable...
	{
		if (_doesCardExist[_count] == 0)
		{
			_doesCardExist[_count] = _array1[_count];
		}
		else//otherwise it will be saved in the next avaliable position which is done by utilising a while loop to find it determined by the found boolean variable.
		{
			int pos = 0;
			while (found == false)
			{
				if (_doesCardExist[pos] == 0)
				{
					_doesCardExist[pos] = _array1[_count];
					found = true;
				}
				pos = pos + 1;
			}
		}
	}
	return 0;
}

//This function loops through the specified array paremeter _hand[] and displays it using std::cout for the user to see.
int CardDisplay(cardObject&, int _deck[], int _hand[], char _player, char _whoWon[], int _round, bool _debug)
{
	int cardsDisplayed = 0;
	//Depending on what the _player parameter is equal these if statments display the title for the player's hand and dealer's hand before displaying their cards.
	if (_player == 'p')
	{
		std::cout << "\nYour Hand\n-------------------------------------\n";
	}
	else
	{
		std::cout << "=====================================\nDealer's Hand\n-------------------------------------\n";
	}
	if (_debug == true)//If the debug option is selected then the numebers representing the cards are also displayed.
	{
		std::cout << "Hand Array: ";
		//This is done using a for loop to loop through what's in the _hand[] array parameter and displaying it with std::cout.
		for (int i = 0; i < 27; i++)
		{
			if (_hand[i] != 0)
			{
				std::cout << _hand[i] << " ";
			}
		}
		std::cout << std::endl;
	}
	cardObject card{};//This function utilises the cardObject structure.
	if (_debug == true)//If the debug option is selected, this std::cout display just sperates the card numbers from the english display of the cards.
	{
		std::cout << "=====================================\nCards:" << std::endl;
	}
	//This if statement checks whether the _player parameter is representing the dealer and if it's the first round then the first card dealt to the dealer will be hidden for the player...
	//but reveal after the first round.
	if ((_player == 'd' && _round == 1) && _debug == false)
	{
		if (cardsDisplayed == 0)//The cardDisplayed variable just keeps track of how many cards are being displayed for the sole purpose of hiding the dealer's first card.
		{
			std::cout << "[HIDDEN]" << std::endl;
			cardsDisplayed = 1;
		}
	}
	//This for loop, loops through what is in the _hand[] parameter and determines its card value base on the number that's in the _hand parameter at a certain position.
	//These are then displayed in with there suits and values by refering to the card sturcture members m_val and m_suit which are both cstrings.
	//The deck array of cards are setup 1-52 meaning, 1-13 is clubs values A-KING and etc for all four suits.
	for (int pos = cardsDisplayed; pos < 27; pos++)
	{
		if (_hand[pos] < 14 && _hand[pos] > 0)
		{
			std::cout << card.m_val[_hand[pos] - 1] << " of " << card.m_suit[0] << std::endl;
		}
		if (_hand[pos] > 13 && _hand[pos] < 27)
		{
			std::cout << card.m_val[_hand[pos] - 14] << " of " << card.m_suit[1] << std::endl;
		}
		if (_hand[pos] > 26 && _hand[pos] < 40)
		{
			std::cout << card.m_val[_hand[pos] - 27] << " of " << card.m_suit[2] << std::endl;
		}
		if (_hand[pos] > 39 && _hand[pos] < 53)
		{
			std::cout << card.m_val[_hand[pos] - 40] << " of " << card.m_suit[3] << std::endl;
		}
	}
	return 0;
}

//This function determines who wins the game of Blackjack by obtain both player and the dealer's card values and comparing them aswell as the choices the player makes.
int WinState(char _player, char _whoWon[], int  _pHand[], int _dHand[], char _choice, bool _debug, int _pot, int _betNum)
{
	//Initalising variables.
	int cardTotal = 0;
	int pTotal = 0;
	int dTotal = 0;
	bool pWon = false;
	bool dWon = false;
	int cards[27] = {};
	//These for loops, loop through both hands 1 at a time and call the CardValues() function that passes the parameter _pHand and variable pos by value...
	//and assign the return value to the cards integer array at the specific position.
	for (int pos = 0; pos < 27; pos++)
	{
		cards[pos] = CardValues(_pHand, pos);
	}
	pTotal = CardCount(cards);//The cardCount() function is called which passes the cards integer array by value and the returned value is assigned to pTotal short for PlayerTotal.
	for (int pos = 0; pos < 27; pos++)
	{
		cards[pos] = CardValues(_dHand, pos);
	}
	dTotal = CardCount(cards);//The same is done for the dealer's total card values.

	//Displays the players total card values using std::cout.
	std::cout << "=====================================\nYour Card Total: " << pTotal << std::endl;
	if (_debug == true)//if the debug option is selected then the dealer's total card values will also be displayed.
	{
		std::cout << "Dealer's Card Total: " << dTotal << std::endl;
	}
	std::cout << "=====================================" << std::endl;
	//This next section of the function determines the winner of the Blackjack game depending on the player's and dealer's total cards.

	//If the player has chosen to "Stand" then both the player's card total and dealer's card total will be compared using logical operators...
	//which also determine whether to assign 'p' to the parameter array _whoWon or 'd'.
	if (_choice == 'S')
	{
		if ((pTotal > dTotal && dTotal < 21) || (pTotal < dTotal && dTotal > 21) || pTotal == dTotal)//if something in the if statment is true, then the character 'p' representing player is assigned to a position in the parameter array _whoWon[].
		{
			if (_whoWon[0] != 'd' && _whoWon[0] != 'p')//This checks if there is already a position being occupied by a character so it does not overwrite it.
			{
				_whoWon[0] = 'p';
			}
			else
			{
				_whoWon[1] = 'p';
			}
		}
		//The same is determined for the dealer.
		if ((dTotal > pTotal && pTotal < 21) || (dTotal < pTotal && pTotal > 21) || pTotal == dTotal)
		{
			if (_whoWon[0] != 'd' && _whoWon[0] != 'p')
			{
				_whoWon[0] = 'd';
			}
			else
			{
				_whoWon[1] = 'd';
			}
		}
	}
	//This if statment determines who wins once someone has reach Blackjack, 21, or once either the dealer or player have exceeded 21.
	if (pTotal == 21 || ((pTotal > 21 && dTotal > 21) && pTotal > dTotal) || (pTotal < 21 && dTotal > 21))
	{
		if (_whoWon[0] != 'd' && _whoWon[0] != 'p')
		{
			_whoWon[0] = 'p';
		}
		else
		{
			_whoWon[1] = 'p';
		}
	}
	if (dTotal == 21 || ((dTotal > 21 && pTotal > 21) && dTotal > pTotal) || (dTotal < 21 && pTotal > 21))
	{
		if (_whoWon[0] != 'd' && _whoWon[0] != 'p')
		{
			_whoWon[0] = 'd';
		}
		else
		{
			_whoWon[1] = 'd';
		}
	}
	//This for loop, loops through the _whoWon array parameter checking for either 'p' or 'd' representing the player or dealer.
	for (int i = 0; i < 2; i++)
	{
		if (_whoWon[i] == 'p')
		{
			pWon = true;//Once found 'p' in the _whoWon array parameter true is assigned to boolean variable pWon which stands for playerWon.
		}
		if (_whoWon[i] == 'd')
		{
			dWon = true;//Once found 'd' in the _whoWon array parameter true is assigned to boolean variable dWon which stands for dealerrWon.
		}
	}
	//These if statements check for a combination of true and false between boolean variables pWon and dWon.
	//These if statments determine what is displayed to the player indicating who won the game.
	if (pWon == true && dWon == false)
	{
		std::cout << "\n=====================================\nYou Win!\n=====================================" << std::endl;
	}
	if (pWon == false && dWon == true)
	{
		std::cout << "\n=====================================\nDealer Wins!\n=====================================" << std::endl;
	}
	if ((pWon == true && dWon == true) && (pWon == false && dWon == false))
	{
		std::cout << "\n=====================================\nDraw!\n=====================================" << std::endl;
	}
	//if pWon is true then these series of if statements determines what the player's winnings depending on their card values.
	if (pWon == true)
	{
		std::cout << "\n=====================================\nMoney Pot: " << _pot;
		//if both the player and dealer have drawn this will award the player double their bet bonus
		if ((pWon == true && dWon == true) || (pWon == false && dWon == false))
		{
			_pot = _pot + (_betNum * 2);
			//which is output using std::cout to inform the player of there winnings.
			std::cout << " + " << _betNum << " + Draw Bonus: " << (_betNum) << std::endl;
		}
		//if the player won by a cardTotal of 21 they get 50% extra of there bet as a Blackjack bonus.
		else if (pTotal == 21)
		{
			_pot = (_pot + _betNum) + _betNum / 2;
			//which is output using std::cout to inform the player of there winnings.
			std::cout << " + " << _betNum << " + Blackjack Bonus: " << (_betNum / 2) << std::endl;
		}
		else//Else if they have won by anyother means there bet is returned to there money pot.
		{
			_pot = _pot + _betNum;
			std::cout << " + " << _betNum << std::endl;
		}
	}
	//_pot parameter is returned to the play function.
	return _pot;
}
//This Function totals up all the card values and returns them.
int CardCount(int _cards[])
{
	//Initialising variables.
	int cardTotal = 0;
	int aceCount = 0;
	//This for loop, loops through the _cards[] array parameter which contain the acurate card values due to functions calling this function after they have called the CardValues() function.
	for (int pos = 0; pos < 27; pos++)
	{
		if (_cards[pos] > 10)//checks if the card value is > 10, if it is it defaults the value back to 10 as this is the highest value a card can have in Blackjack apart from the Ace.
		{
			_cards[pos] = 10;
		}
		cardTotal = cardTotal + _cards[pos];//This totals all the values up and is assigned to one integer variable.
		if (_cards[pos] == 1)//checks for any aces in the card values which are represented as a value of 1.
		{
			aceCount = aceCount + 1;//if they're are aces the aceCount integer variable will be incremented by 1.
		}
	}
	if (cardTotal <= 11)//checks if the card value is less than or equal to 11.
	{
		if (aceCount == 1 || (cardTotal == 2 && aceCount == 2))//if they is only 1 ace in the array or if the total card values equals 2 and they're 2 aces...
		{
			cardTotal = cardTotal + 10;//increment cardTotal by 10 as this is changing one ace's value to 11 rather than 1.
		}
	}
	return cardTotal;//The final total card values is return.
}

//This function converts the cards in a player's hand to their acruate values.
int CardValues(int _hand[], int _pos)
{
	//Initialising variables
	int cards[27] = {};
	//This function is designed to be used in a loop and requires a pass by value integer parameter _pos to determine the position of the card it's evaluating.
	if (_hand[_pos] < 14 && _hand[_pos] > 0)//This works similar to the cardDisplay() function e.g. The deck array of cards are setup 1-52 meaning, 1-13 is clubs values A-KING and etc for all four suits.
	{
		cards[_pos] = _hand[_pos];
	}
	if (_hand[_pos] > 13 && _hand[_pos] < 27)
	{
		cards[_pos] = _hand[_pos] - 13;//It does this by subtracting the number representing the card by the last card in the previous suit.
		//The idea of this is that when evaluated in cardDisplay() its easy to pare values to its cstring equivilent...
		//and evaluates a total card value in cardCount() thats easy to iterate and determine a winner in winState().
	}
	if (_hand[_pos] > 26 && _hand[_pos] < 40)
	{
		cards[_pos] = _hand[_pos] - 26;
	}
	if (_hand[_pos] > 39 && _hand[_pos] < 53)
	{
		cards[_pos] = _hand[_pos] - 39;
	}
	return cards[_pos];//the cards integer array is returned.
}

void HowTo()
{
	//This line of code uses std::cout and "\n" to display the rules of Blackjack to the player.
	std::cout << "\nInstructions - How To Play:\n\nThe aim of Blackjack is to accumulate a higher point total, determined by the\n indiviual card numbers, than the dealer without going over 21.\ncards 2 through 10 are valued using their face values, cards J, 1 & K are worth\n 10 points each, and the Ace is worth 1 or 11 points determined by you.\n\nDealing:\n\nWhen the game begins both you and the dealer will receive 2 cards each.\nYour cards are dealt face up, while the dealers is dealt one facing down and one facing up.\nA Blackjack is when the player's opening cards include an Ace and any 10 point card.\n This means you automatically win, however if the dealer receives this you lose.\n\nYour Turn:\n\nOnce cards have been dealt, the game starts.\nYou will be given the choice to either [stand] or [hit].\n[stand] will allow you keep your current hand and compare against the\n dealers and determine whether you win or lose.\n[hit] will allow you to be dealt another card from the deck until you decide to\n [stand] or the total card value exceeds 21 and imediatley lose, this is called a bust.\n\nThe Dealers Turn:\n\nOnce you have made your decision the dealer will reveal his hidden hole card.\nIf the dealer has 21 you lose or if you also have 21 you draw, this is called a [push], and your bet is returned.\nYour bet is returned if you win or draw.\nIf not he then chooses to [stand] or [hit] depending on his total card value.\n The dealer's turn is dictated by the rules as if their total card value\n is lower than 17 they must [hit] otherwise they [stand].\n\nShowdown:\n\nIf the dealer goes bust and your still in the game you win.\nOtherwise if your point total is higher than the dealer you win.\nPlayers with a Blackjack win a bet plus a bonus amount.\nThis is equal to half your original wager.\nA Blackjack hand beats any other hand aswell as the dealer's 21 \nif they have more cards than you.\n" << std::endl;
}

