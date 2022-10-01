#include "TYPEDEFS.h"
#include "Card\card.h"
#include "Terminal\terminal.h"
#include "Server\server.h"
#include "Application\app.h"


int main(void)
{
	system("cls");
	while (1)
	{
		appStart();
		fflush(stdin);
	}
	return 0;
}
