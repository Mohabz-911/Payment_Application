#include "..\TYPEDEFS.h"
#include "terminal.h"
#include "..\Card\card.h"
#include <time.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	//Function that gets the current date from the computer
	time_t currentTime = time(NULL);
	struct tm d = *localtime(&currentTime);
	
	//Day
	termData->transactionDate[1] = (d.tm_mday % 10) + '0';
	d.tm_mday /= 10;
	termData->transactionDate[0] = (d.tm_mday % 10) + '0';
	termData->transactionDate[2] = '/';
	
	//Month
	d.tm_mon++;
	termData->transactionDate[4] = (d.tm_mon % 10) + '0';
	d.tm_mon /= 10;
	termData->transactionDate[3] = (d.tm_mon % 10) + '0';
	termData->transactionDate[5] = '/';
	
	//Year
	d.tm_year += 1900;
	termData->transactionDate[9] = (d.tm_year % 10) + '0';
	d.tm_year /= 10;
	termData->transactionDate[8] = (d.tm_year % 10) + '0';
	d.tm_year /= 10;
	termData->transactionDate[7] = (d.tm_year % 10) + '0';
	d.tm_year /= 10;
	termData->transactionDate[6] = (d.tm_year % 10) + '0';
	
	return OK_t;
}


EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	uint8_t cardMonth = 0, termMonth = 0;
	uint16_t cardYear = 0, termYear = 0;
	
	//Converting month of expiry date from char to int (stored in uin8_t to save size)
	cardMonth += cardData.cardExpirationDate[0] - '0';
	cardMonth *= 10;
	cardMonth += cardData.cardExpirationDate[1] - '0';
	
	//Converting month of transaction date from char to int
	termMonth += termData.transactionDate[3] - '0';
	termMonth *= 10;
	termMonth += termData.transactionDate[4] - '0';
	
	//Converting year of expiry date from char to int
	cardYear += cardData.cardExpirationDate[3] - '0';
	cardYear *= 10;
	cardYear += cardData.cardExpirationDate[4] - '0';
	
	//Converting year of transaction date from char to int
	termYear += termData.transactionDate[8] - '0';
	termYear *= 10;
	termYear += termData.transactionDate[9] - '0';
	
	//printf("\n%d/%d -- %d/%d\n", cardMonth, cardYear, termMonth, termYear);
	
	if (cardYear < termYear)
		return EXPIRED_CARD;
	
	if (cardYear == termYear && cardMonth < termMonth)
		return EXPIRED_CARD;
	
	return OK_t;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	float amount;
	int32_t test;
	
	printf("\nEnter the desired transaction amount: ");
	scanf("%f", &amount);
	
	test = (int32_t)amount;
	if (test < 0)
		return INVALID_AMOUNT;
	
	//Maybe the integer part is equal to zero, but 0.0006 for example
	if (test == 0)
	{
		test = amount * 10000.0;
		if (test == 0)
			return INVALID_AMOUNT;
	}
	
	termData->transAmount = amount;
	return OK_t;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	int32_t trans, max;
	
	trans = termData->transAmount;
	max = termData->maxTransAmount;
	
	if (trans > max)
		return EXCEED_MAX_AMOUNT;
	
	if (trans < max)
		return OK_t;
	
	//At this point, the integer part of the two amounts is equal
	//We need to check for the digits after the floating point
	trans = (termData->transAmount) * 10000.0;
	max = (termData->maxTransAmount) * 10000.0;
	
	if (trans > max)
		return EXCEED_MAX_AMOUNT;
	
	return OK_t;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
	float amount;
	int32_t test;
	
	printf("\nEnter the max amount for any transaction: ");
	scanf("%f", &amount);
	
	test = (int32_t)amount;
	if (test < 0)
		return INVALID_MAX_AMOUNT;
	
	//Maybe the integer part is equal to zero, but 0.0006 for example
	if (test == 0)
	{
		test = amount * 10000.0;
		if (test == 0)
			return INVALID_MAX_AMOUNT;
	}
	
	termData->maxTransAmount = amount;
	return OK_t;
}

/*
int main(void)
{
	system("cls");
	printf("Test------------------\n");
	
	ST_cardData_t * cardData = (ST_cardData_t *)malloc(sizeof(ST_cardData_t));
	{
	cardData->cardHolderName[0] = 'M'; 
	cardData->cardHolderName[1] = 'o';
	cardData->cardHolderName[2] = 'h';
	cardData->cardHolderName[3] = 'a';
	cardData->cardHolderName[4] = 'b';
	cardData->cardHolderName[5] = ' ';
	cardData->cardHolderName[6] = 'M';
	cardData->cardHolderName[7] = 'o';
	cardData->cardHolderName[8] = 'h';
	cardData->cardHolderName[9] = 'a';
	cardData->cardHolderName[10] = 'm';
	cardData->cardHolderName[11] = 'e';
	cardData->cardHolderName[12] = 'd';
	cardData->cardHolderName[13] = ' '; 
	cardData->cardHolderName[14] = 'Z';
	cardData->cardHolderName[15] = 'a';
	cardData->cardHolderName[16] = 'g';
	cardData->cardHolderName[17] = 'h';
	cardData->cardHolderName[18] = 'l';
	cardData->cardHolderName[19] = 'o';
	cardData->cardHolderName[20] = 'u';
	cardData->cardHolderName[21] = 'l';
	}
	
	{
	cardData->primaryAccountNumber[0] = '1';
	cardData->primaryAccountNumber[1] = '5';
	cardData->primaryAccountNumber[2] = '8';
	cardData->primaryAccountNumber[3] = '9';
	cardData->primaryAccountNumber[4] = '6';
	cardData->primaryAccountNumber[5] = '3';
	cardData->primaryAccountNumber[6] = '2';
	cardData->primaryAccountNumber[7] = '4';
	cardData->primaryAccountNumber[8] = '7';
	cardData->primaryAccountNumber[9] = '5';
	cardData->primaryAccountNumber[10] = '6';
	cardData->primaryAccountNumber[11] = '8';
	cardData->primaryAccountNumber[12] = '9';
	cardData->primaryAccountNumber[13] = '2';
	cardData->primaryAccountNumber[14] = '0';
	cardData->primaryAccountNumber[15] = '1';
	cardData->primaryAccountNumber[16] = '5';
	}
	
	{
	cardData->cardExpirationDate[0] = '1';
	cardData->cardExpirationDate[1] = '0';
	cardData->cardExpirationDate[2] = '/';
	cardData->cardExpirationDate[3] = '2';
	cardData->cardExpirationDate[4] = '2';
	}
	
	ST_terminalData_t * termData = (ST_terminalData_t *)malloc(sizeof(ST_terminalData_t));
	
	EN_terminalError_t transDate, expResult, amountResult1, amountResult2, amountResult3;
	
	transDate = getTransactionDate(termData);
	if(transDate == WRONG_DATE)
		printf("\nError in transaction date !!!!!!\n");
	
	expResult = isCardExpired(*cardData, *termData);
	if(expResult == WRONG_EXP_DATE)
		printf("\nError in expiry date !!!!!!\n");
	
	amountResult1 = getTransactionAmount(termData);
	if(amountResult1 == INVALID_AMOUNT)
		printf("\nINVALID AMOUNT !!!!!!\n");
	
	amountResult2 = setMaxAmount(termData);
	if(amountResult2 == INVALID_MAX_AMOUNT)
		printf("\nINVALID MAX AMOUNT !!!!!!\n");
	
	amountResult3 = isBelowMaxAmount(termData);
	if(amountResult3 == EXCEED_MAX_AMOUNT)
		printf("\nEXCEED MAX AMOUNT !!!!!!\n");
	
	
	
	if (transDate == OK_t && expResult == OK_t)
		printf("\nTransaction Date: %s", termData->transactionDate);
	if (amountResult1 == OK_t && amountResult2 == OK_t && amountResult3 == OK_t)
		printf("\nAmount: %f", termData->transAmount);
	
	
	return 0;
}
*/












