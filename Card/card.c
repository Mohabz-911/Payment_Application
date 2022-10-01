#include "..\TYPEDEFS.h"
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	printf("\nPlease enter your name: ");
	uint8_t * temp = cardData->cardHolderName;
	gets(temp);
	if (temp == NULL || strlen(temp) < 20 || strlen(temp) > 24)
		return WRONG_NAME;
	
	return OK_c;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	printf("\nEnter the card expiry date: ");
	uint8_t * temp = cardData->cardExpirationDate;
	gets(temp);
	if (temp == NULL || strlen(temp) > 5 || wrongDateFormat(temp))
		return WRONG_EXP_DATE;
	
	return OK_c;
}

uint8_t wrongDateFormat(uint8_t * str)
{
	if (str[2] != '/' 
		|| str[0] < '0' 
		|| str[0] > '1'
		|| str[1] < '0'
		|| str[1] > '9'
		|| (str[0] == '1' && str[1] > '2')
		|| str[3] < '0'
		|| str[3] > '9'
		|| str[4] < '0'
		|| str[4] > '9')	return 1;
	
	return 0;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	printf("\nEnter your Primary Account Number: ");
	uint8_t * temp = cardData->primaryAccountNumber;
	gets(temp);
	if (temp == NULL || strlen(temp) < 16 || strlen(temp) > 19)
		return WRONG_PAN;
	
	return OK_c;
}

/*
int main(void)
{
	system("cls");
	printf("Test------------------\n");
	
	ST_cardData_t * cardData = (ST_cardData_t *)malloc(sizeof(ST_cardData_t));
	
	EN_cardError_t nameResult, expResult, panResult;
	
	nameResult = getCardHolderName(cardData);
	if(nameResult == WRONG_NAME)
		printf("\nError in name !!!!!!\n");
	
	expResult = getCardExpiryDate(cardData);
	if(expResult == WRONG_EXP_DATE)
		printf("\nError in expiry date !!!!!!\n");
	
	panResult = getCardPAN(cardData);
	if(panResult == WRONG_PAN)
		printf("\nError in PAN !!!!!!\n");
	
	
	if (nameResult == OK_c)
		printf("\nName: %s", cardData->cardHolderName);
	if (expResult == OK_c)
		printf("\nExpiry Date: %s", cardData->cardExpirationDate);
	if (panResult == OK_c)
		printf("\nPAN: %s", cardData->primaryAccountNumber);
	
	
	return 0;
}
*/



